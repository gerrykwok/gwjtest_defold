package com.xishanju.plm.plmext;

import android.util.Log;
import android.content.Context;
import android.content.Intent;
import android.os.Environment;
import android.app.Activity;

class plmext
{
	public static final String TAG = "plmext";
	private static final int ACT_RES_LOCAL_PHOTOS = 1;
	private static final int ACT_RES_CAMERA_PHOTOS = 2;
	private static final int ACT_CROP_PHOTOS = 3;
	private static final String PHOTO_FILE_NAME = "myMahjongAvatar.png";

	public static void getLocalPicture(Context ctx, String localPath, int luaCallbackFunction) {
		Log.i(TAG, "localPath:" + localPath + ", luaCallbackFunction:" + luaCallbackFunction);
//		AppActivity.appActivity.avatarCallback = luaCallbackFunction;
//		AppActivity.appActivity.avatarLocalPath = localPath;
//		AppActivity.appActivity.camarafilepath = getAvatorPath();
		String camerafilePath = getAvatorPath();
		Intent intent = new Intent();
		intent.setType("image/*");
		intent.setAction(Intent.ACTION_GET_CONTENT);
		//intent.putExtra("crop", "true");
		//intent.putExtra("output", Uri.fromFile(new File(camerafilePath)));
		//intent.putExtra("outputX", 160);
		//intent.putExtra("outputY", 160);
		//intent.putExtra("outputFormat", "PNG");
		//intent.putExtra("aspectX", 1);
		//intent.putExtra("aspectY", 1);
		Activity act = (Activity)ctx;
		act.startActivityForResult(intent, ACT_RES_LOCAL_PHOTOS);
	}

	/** get path for avatar */
	private static String getAvatorPath() {
		return Environment.getExternalStorageDirectory().getAbsolutePath() + "/" + PHOTO_FILE_NAME;
	}
}
