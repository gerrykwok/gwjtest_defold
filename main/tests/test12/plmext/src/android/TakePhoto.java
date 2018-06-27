package com.xishanju.plm.plmext;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.util.Locale;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.content.Intent;
import android.content.ContentResolver;
import android.util.Log;
import android.net.Uri;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.provider.MediaStore;
import android.provider.MediaStore.Images.ImageColumns;

public class TakePhoto extends Activity
{
	private int avatarCallback = -1;
	private String avatarLocalPath;
	private String camarafilepath;
	private int m_imageWidth;
	private int m_imageHeight;

	private static final String PHOTO_FILE_NAME = "myMahjongAvatar.png";
	private static final int ACT_RES_LOCAL_PHOTOS = 1;
	private static final int ACT_RES_CAMERA_PHOTOS = 2;
	private static final int ACT_CROP_PHOTOS = 3;

	private static final int AVATAR_GET_RES_SUCCES = 1;
	private static final int AVATAR_GET_RES_FAIL = 0;

	private boolean m_fromCamera;
	private Handler m_handler = new Handler();

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);

		Intent iii = getIntent();
		m_fromCamera = iii.getBooleanExtra("fromCamera", false);
		avatarLocalPath = iii.getStringExtra("localPath");
		m_imageWidth = iii.getIntExtra("imageWidth", 64);
		m_imageHeight = iii.getIntExtra("imageHeight", 64);
		avatarCallback = iii.getIntExtra("luaCallback", -1);
		camarafilepath = getAvatorPath();

		Log.d(plmext.TAG, "TakePhoto1.onCreate,fromCamera="+m_fromCamera);

		if(!m_fromCamera)
		{
			Intent intent = new Intent();
			intent.setType("image/*");
			intent.setAction(Intent.ACTION_GET_CONTENT);
			//intent.putExtra("crop", "true");
			//intent.putExtra("output", Uri.fromFile(new File(camarafilepath)));
			//intent.putExtra("outputX", 160);
			//intent.putExtra("outputY", 160);
			//intent.putExtra("outputFormat", "PNG");
			//intent.putExtra("aspectX", 1);
			//intent.putExtra("aspectY", 1);
			startActivityForResult(intent, ACT_RES_LOCAL_PHOTOS);
		}
		else
		{
			Intent i = new Intent(android.provider.MediaStore.ACTION_IMAGE_CAPTURE);
			i.putExtra(android.provider.MediaStore.EXTRA_OUTPUT, Uri.fromFile(new File(camarafilepath)));
			startActivityForResult(i, ACT_RES_CAMERA_PHOTOS);
		}
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		Log.d(plmext.TAG, "onActivityResult_0, requestCode:"+requestCode+", resultCode:"+resultCode);
		super.onActivityResult(requestCode, resultCode, data);
		if (requestCode == ACT_RES_CAMERA_PHOTOS || requestCode == ACT_RES_LOCAL_PHOTOS)
		{
			if (resultCode != RESULT_OK) {
				notifyAvatarGetResult(AVATAR_GET_RES_FAIL);
				finish();
				return;
			}

			if(data != null && data.getData() != null)
			{
				//从相册选择会执行这里
				Log.d(plmext.TAG, "data is "+data.toString());
				this.cropImageFromUri(data.getData());
			}
			else
			{
				//从相机选择会执行这里
				Log.d(plmext.TAG, "data is null");
				this.cropImageFromUri(Uri.fromFile(new File(camarafilepath)));

			}
			// boolean mirrorX = (requestCode == ACT_RES_CAMERA_PHOTOS);//
		}
		else if(requestCode == ACT_CROP_PHOTOS)
		{
			if(data != null && data.getData() != null)
			{
				//在模拟器andy中，通过文件管理在DCIM目录下，会出现裁剪结果无法保存到camarafilepath,这个时候就move过去。目前只在这个模拟器遇到过
				Log.d(plmext.TAG, data.getData().toString());
				String realFilePath = getRealFilePath(data.getData());
				if(realFilePath.endsWith(camarafilepath))
				{
					Log.d(plmext.TAG, "need not to move");
				}
				else
				{
					File srcFile = new File(realFilePath);
					File destFile = new File(camarafilepath);
					srcFile.renameTo(destFile);
				}

			}
			else
			{
				Log.d(plmext.TAG, "crop no uri");
			}
			Log.i(plmext.TAG, "camarafilepath:" + camarafilepath+", avatarLocalPath:"+avatarLocalPath);

			testFile(camarafilepath);

			boolean mirrorX = false;
			boolean res = saveAsAvatar(camarafilepath, avatarLocalPath, mirrorX);
			notifyAvatarGetResult(res ? AVATAR_GET_RES_SUCCES : AVATAR_GET_RES_FAIL);
			finish();
		}
	}

	private void testFile(String filepath)
	{
		try
		{
			FileInputStream is;
			int filesize;
			byte[] buffer;
			is = new FileInputStream(filepath);
			filesize = is.available();
			Log.d(plmext.TAG, "filesize="+filesize);
			buffer = new byte[filesize];
			is.read(buffer);
			is.close();
		} catch (Exception e)
		{
			Log.e(plmext.TAG, "gwjgwj,error:" + e.getMessage());
			e.printStackTrace();
		}
	}

	private void cropImageFromUri(Uri uri)
	{
		if(camarafilepath == null)
			return;
		Intent intent = new Intent("com.android.camera.action.CROP");
		intent.setDataAndType(uri, "image/*");
		intent.putExtra("crop", "true");
		intent.putExtra("aspectX", m_imageWidth);
		intent.putExtra("aspectY", m_imageHeight);
		intent.putExtra("outputX", m_imageWidth);
		intent.putExtra("outputY", m_imageHeight);
		intent.putExtra("scale", true);
		Log.d(plmext.TAG, "path=file://"+camarafilepath);
		intent.putExtra(MediaStore.EXTRA_OUTPUT, Uri.fromFile(new File(camarafilepath)));
		intent.putExtra("return-data", false);
		intent.putExtra("outputFormat", Bitmap.CompressFormat.PNG.toString());
		intent.putExtra("noFaceDetection", true); // no face detection
		startActivityForResult(intent, ACT_CROP_PHOTOS);
	}

	public String getRealFilePath(final Uri uri)
	{
		if ( null == uri ) return null;
		final String scheme = uri.getScheme();
		String data = null;
		if ( scheme == null )
			data = uri.getPath();
		else if ( ContentResolver.SCHEME_FILE.equals( scheme ) ) {
			data = uri.getPath();
		} else if ( ContentResolver.SCHEME_CONTENT.equals( scheme ) ) {
			Cursor cursor = getContentResolver().query( uri, new String[] { ImageColumns.DATA }, null, null, null );
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

	/** get path for avatar */
	private static String getAvatorPath() {
		return Environment.getExternalStorageDirectory().getAbsolutePath() + "/" + PHOTO_FILE_NAME;
	}

	/**Save an image as an avatar 
	 * @param originPath
	 * @param avatarPath
	 * @param mirrorX
	 * @return
	 */
	public boolean saveAsAvatar(String originPath, String avatarPath, boolean mirrorX) {
		if(originPath==null || avatarPath==null){
			Log.e(plmext.TAG, "originPath or avatarPath is null");
			return false;
		}
		Bitmap bgimage = BitmapFactory.decodeFile(originPath);
		if (bgimage == null) {
			Log.e(plmext.TAG, "bgimage is null");
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
			Log.e(plmext.TAG, "bmp or path is null");
			return false;
		}
		boolean res=false;
		File f = new File(path);
//		//
//		if (f.isFile() && f.exists()) {  
//	        f.delete();
//	    }
		try {
			f.createNewFile();
		} catch (IOException e) {
			Log.e(plmext.TAG, "create new file exception");
			e.printStackTrace();
		}
		FileOutputStream fOut = null;
		try {
			fOut = new FileOutputStream(f);
		} catch (FileNotFoundException e) {
			Log.e(plmext.TAG, "file not found:" + f.toString());
			e.printStackTrace();
		}
		bmp.compress(Bitmap.CompressFormat.PNG, 100, fOut);
		try {
			fOut.flush();
			res=true;
		} catch (IOException e) {
			Log.e(plmext.TAG, "io exception");
			e.printStackTrace();
		}
		try {
			fOut.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return res;
	}

	private void notifyAvatarGetResult(final int res) {
		Log.d(plmext.TAG, String.format(Locale.US, "notifyAvatarGetResult(),avatarCallback=%d,res=%d", avatarCallback, res));
		if (avatarCallback >= 0) {
			LuaJavaBridge.callLuaFunctionWithString(avatarCallback, "" + res);
			LuaJavaBridge.releaseLuaFunction(avatarCallback);
			avatarCallback = -1;
		}
	}
}
