package com.xishanju.plm.plmext;

import android.util.Log;
import android.content.Context;
import android.content.Intent;
import android.content.ComponentName;
import android.os.Environment;
import android.app.Activity;

class plmext
{
	public static final String TAG = "plmext";

	public static void getPhoto(Context ctx, int fromCamera, String localPath, int width, int height, int luaCallbackFunction)
	{
		//Log.i(TAG, "localPath:" + localPath + ", luaCallbackFunction:" + luaCallbackFunction);

		Intent iii = new Intent(ctx, TakePhoto.class);
		iii.putExtra("fromCamera", fromCamera==1);
		iii.putExtra("localPath", localPath);
		iii.putExtra("imageWidth", width);
		iii.putExtra("imageHeight", height);
		iii.putExtra("luaCallback", luaCallbackFunction);
		Activity act = (Activity)ctx;
		act.startActivity(iii);
	}
}
