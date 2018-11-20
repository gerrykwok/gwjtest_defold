package com.xishanju.plm.plmext;

import java.util.HashMap;

import android.util.Log;
import android.content.Context;
import android.content.Intent;
import android.content.ComponentName;
import android.os.Environment;
import android.app.Activity;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;

import com.tencent.mm.opensdk.openapi.IWXAPI;
import com.tencent.mm.opensdk.openapi.WXAPIFactory;

public class plmext
{
	public static final String TAG = "plmext";
	public static Context s_context;
	public static IWXAPI s_wxApi;

	public static void init(Context ctx)
	{
		String wechatAppId = "wxc0acf2068b5dd7a3";

		s_context = ctx;
		s_wxApi = WXAPIFactory.createWXAPI(ctx, null);
		s_wxApi.registerApp(wechatAppId);
	}

	public static void getPhoto(int fromCamera, String localPath, int width, int height, int luaCallbackFunction)
	{
		Context ctx = s_context;
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

	public static boolean isAppInstalled(String pkgName)
	{
		PackageManager pm =  s_context.getPackageManager();
		try {
			pm.getPackageInfo(pkgName, 0);
			return true;
		} catch (NameNotFoundException e) {
			return false;
		} catch (Exception e)
		{
			return false;
		}
	}
}
