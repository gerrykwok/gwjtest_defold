package com.xishanju.plm.takephoto;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.util.Locale;

import org.json.JSONObject;

import android.app.Activity;
import android.os.Build;
import android.os.Environment;
import android.content.Context;
import android.content.Intent;
import android.content.ContentResolver;
import android.util.Log;
import android.media.MediaScannerConnection;
import android.net.Uri;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.provider.MediaStore;
import android.provider.MediaStore.Images.ImageColumns;

public class TakePhoto
{
	private static final String TAG = "takephoto";
	private static int m_avatarCallback = -1;
	private static String m_avatarLocalPath;
	private static String m_camerafilepath;
	private static int m_imageWidth;
	private static int m_imageHeight;

	private static final int ACT_RES_LOCAL_PHOTOS = 1;
	private static final int ACT_RES_CAMERA_PHOTOS = 2;
	private static final int ACT_CROP_PHOTOS = 3;

	private static final int AVATAR_GET_RES_SUCCES = 1;
	private static final int AVATAR_GET_RES_FAIL = 0;

	private static Context m_ctx;

	public static String getPhoto(Context ctx, JSONObject params)
	{
		m_ctx = ctx;
		boolean fromCamera = false;
		m_camerafilepath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/plmhd_from_camera.png";
		try
		{
			if(params.has("fromCamera")) fromCamera = params.getBoolean("fromCamera");
			if(params.has("localPath")) m_avatarLocalPath = params.getString("localPath");
			if(params.has("imageWidth")) m_imageWidth = params.getInt("imageWidth");
			if(params.has("imageHeight")) m_imageHeight = params.getInt("imageHeight");
			if(params.has("callback")) m_avatarCallback = params.getInt("callback");
		} catch(Exception e)
		{
			e.printStackTrace();
		}

		Activity act = (Activity)ctx;
		if(fromCamera)
		{
			Intent i = new Intent(android.provider.MediaStore.ACTION_IMAGE_CAPTURE);
			File f = new File(m_camerafilepath);
			Uri uri = Uri.fromFile(f);
			Log.i(TAG, "uri1="+uri);
			if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT)
			{
				uri = UriUtil.getImageContentUri(ctx, f);
			}
			Log.i(TAG, "uri2="+uri);
			i.putExtra(android.provider.MediaStore.EXTRA_OUTPUT, uri);
			act.startActivityForResult(i, ACT_RES_CAMERA_PHOTOS);
		}
		else
		{
			Intent intent = new Intent();
			intent.setType("image/*");
			intent.setAction(Intent.ACTION_GET_CONTENT);
			act.startActivityForResult(intent, ACT_RES_LOCAL_PHOTOS);
		}

		return "";
	}
	public static void onActivityResult(int requestCode, int resultCode, Intent data)
	{
		Log.i(TAG, "gwjgwj,onActivityResult, requestCode:"+requestCode+", resultCode:"+resultCode+",data="+data);
		if(resultCode != Activity.RESULT_OK)
		{
			notifyAvatarGetResult(AVATAR_GET_RES_FAIL);
			return;
		}
		if(requestCode == ACT_RES_LOCAL_PHOTOS)
		{
			Uri uri = data.getData();
			Log.i(TAG, "album uri="+uri);
			//https://blog.csdn.net/cryssdut/article/details/50174519
			//https://www.jianshu.com/p/2275bb552327
			if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.N)
			{//在android7.0上，data.getData()返回的uri应该是content://开头的
			}
			else
			{
				if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT)
				{
					String str = UriUtil.getPath(m_ctx, uri);
					uri = Uri.fromFile(new File(str));
				}
			}
			Log.i(TAG, "album uri2="+uri);
			cropImageFromUri(uri);
			Log.i(TAG, "album end");
		}
		else if(requestCode == ACT_RES_CAMERA_PHOTOS)
		{
			updatePhotoMedia(m_ctx, m_camerafilepath);
			File f = new File(m_camerafilepath);
			Uri uri = Uri.fromFile(f);
			Log.i(TAG, "camera uri1="+uri);
			if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.N)
			{
				uri = UriUtil.getImageContentUri(m_ctx, f);
			}
			Log.i(TAG, "camera uri2="+uri);
			cropImageFromUri(uri);
		}
		else if(requestCode == ACT_CROP_PHOTOS)
		{
			if(data != null && data.getData() != null)
			{
				//在模拟器andy中，通过文件管理在DCIM目录下，会出现裁剪结果无法保存到camerafilepath,这个时候就move过去。目前只在这个模拟器遇到过
//				Log.i(TAG, data.getData().toString());
				String realFilePath = getRealFilePath(data.getData());
				if(realFilePath.endsWith(m_camerafilepath))
				{
					Log.i(TAG, "need not to move");
				}
				else
				{
					File srcFile = new File(realFilePath);
					File destFile = new File(m_camerafilepath);
					srcFile.renameTo(destFile);
				}

			}
			else
			{
				Log.i(TAG, "crop no uri");
			}
//			Log.i(TAG, "camarafilepath:" + m_camerafilepath+", avatarLocalPath:"+m_avatarLocalPath);

			boolean mirrorX = false;
			boolean res = saveAsAvatar(m_camerafilepath, m_avatarLocalPath, mirrorX);
			updatePhotoMedia(m_ctx, m_camerafilepath);
			notifyAvatarGetResult(res ? AVATAR_GET_RES_SUCCES : AVATAR_GET_RES_FAIL);
		}
	}

	private static void cropImageFromUri(Uri uri)
	{
		if(m_camerafilepath == null)
			return;
		Intent intent = new Intent("com.android.camera.action.CROP");
		intent.setDataAndType(uri, "image/*");
		intent.putExtra("crop", "true");
		intent.putExtra("aspectX", m_imageWidth);
		intent.putExtra("aspectY", m_imageHeight);
		intent.putExtra("outputX", m_imageWidth);
		intent.putExtra("outputY", m_imageHeight);
		intent.putExtra("scale", true);
//		Log.i(TAG, "path=file://"+m_camerafilepath);
		intent.putExtra(MediaStore.EXTRA_OUTPUT, Uri.fromFile(new File(m_camerafilepath)));
		intent.putExtra("return-data", false);
		intent.putExtra("outputFormat", Bitmap.CompressFormat.PNG.toString());
		intent.putExtra("noFaceDetection", true); // no face detection
		Activity act = (Activity)m_ctx;
		act.startActivityForResult(intent, ACT_CROP_PHOTOS);
	}

	public static String getRealFilePath(final Uri uri)
	{
		if ( null == uri ) return null;
		final String scheme = uri.getScheme();
		String data = null;
		if ( scheme == null )
			data = uri.getPath();
		else if ( ContentResolver.SCHEME_FILE.equals( scheme ) ) {
			data = uri.getPath();
		} else if ( ContentResolver.SCHEME_CONTENT.equals( scheme ) ) {
			Cursor cursor = m_ctx.getContentResolver().query( uri, new String[] { ImageColumns.DATA }, null, null, null );
			if ( null != cursor ) {
				if ( cursor.moveToFirst() ) {
					int index = cursor.getColumnIndex( ImageColumns.DATA );
					if ( index > -1 ) {
						data = cursor.getString( index );
					}
				}
				cursor.close();
			}
		}
		return data;
	}

	public static boolean saveAsAvatar(String originPath, String avatarPath, boolean mirrorX) {
		if(originPath==null || avatarPath==null){
			Log.e(TAG, "originPath or avatarPath is null");
			return false;
		}
		Bitmap bgimage = BitmapFactory.decodeFile(originPath);
		if (bgimage == null) {
			Log.e(TAG, "bgimage is null");
			return false;
		}
		bgimage = Bitmap.createScaledBitmap(bgimage, m_imageWidth, m_imageHeight, false);
		if(mirrorX){
			Matrix matrix = new Matrix();
			matrix.setScale(-1.0f, 1.0f);
			bgimage = Bitmap.createBitmap(bgimage, 0, 0, bgimage.getWidth(), bgimage.getHeight(), matrix, false);
		}

		return saveBitmap(bgimage, avatarPath);
	}

	public static boolean saveBitmap(Bitmap bmp, String path) {
		if(bmp==null || path==null){
			Log.e(TAG, "bmp or path is null");
			return false;
		}
		boolean res=false;
		File f = new File(path);
		try {
			f.createNewFile();
		} catch (IOException e) {
			Log.e(TAG, "create new file exception");
			e.printStackTrace();
		}
		FileOutputStream fOut = null;
		try {
			fOut = new FileOutputStream(f);
		} catch (FileNotFoundException e) {
			Log.e(TAG, "file not found:" + f.toString());
			e.printStackTrace();
		}
		bmp.compress(Bitmap.CompressFormat.PNG, 100, fOut);
		try {
			fOut.flush();
			res=true;
		} catch (IOException e) {
			Log.e(TAG, "io exception");
			e.printStackTrace();
		}
		try {
			fOut.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return res;
	}

	private static native void nativeNotifyGetResult(int callback, String res);
	private static void notifyAvatarGetResult(int res) {
//		Log.i(TAG, String.format(Locale.US, "gwjgwj,notifyAvatarGetResult,callback=%d,res=%d", m_avatarCallback, res));
		if (m_avatarCallback > 0) {
			String str = String.format(Locale.US, "{\"result\":%d}", res);
			nativeNotifyGetResult(m_avatarCallback, str);
			m_avatarCallback = 0;
		}
	}
	
	private static void updatePhotoMedia(Context context, String path)
	{
		MediaScannerConnection.scanFile(m_ctx, new String[]{path}, null, new MediaScannerConnection.OnScanCompletedListener()
		{
			@Override
			public void onScanCompleted(String path, Uri uri)
			{
				Log.i(TAG, "scan " + path + " completed");
			}
		});

//		Intent intent = new Intent();
//		intent.setAction(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE);
//		intent.setData(Uri.fromFile(new File(path)));
//		context.sendBroadcast(intent);

//		File f = new File(path);
//		String filename = f.getName();
//		try
//		{
//			MediaStore.Images.Media.insertImage(context.getContentResolver(), path, filename, null);
//		} catch (FileNotFoundException e)
//		{
//			e.printStackTrace();
//		}
	}
}
