package com.xishanju.plm.misc;

import java.io.File;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Locale;

import org.json.JSONObject;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.Uri;
import android.util.Log;
import android.os.Build;
import android.provider.Settings;

class MiscFunc
{
	public static final String TAG = "misc";
	
	public static String startInstallApk(Context ctx, JSONObject json)
	{
		String apkFullPath = "";
		try
		{
			if(json.has("path")) apkFullPath = json.getString("path");
		} catch(Exception e)
		{
			e.printStackTrace();
		}
		if(apkFullPath.equals(""))
		{
			return "failed";
		}
		File f = new File(apkFullPath);
		if(!f.exists())
		{
			return "apk not exist";
		}
		try
		{
			Intent intent = new Intent(Intent.ACTION_VIEW);
			intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			Uri uri = Uri.fromFile(f);
			intent.setDataAndType(uri, "application/vnd.android.package-archive");
			ctx.startActivity(intent);
		} catch(Exception e)
		{
			Log.e(TAG, "exception while installing apk");
			e.printStackTrace();
			return "exception";
		}
		return "success";
	}

	public static String isAppInstalled(Context ctx, JSONObject json)
	{
		String pkgName = "";
		try
		{
			if(json.has("package")) pkgName = json.getString("package");
		} catch(Exception e)
		{
			e.printStackTrace();
		}
		if(pkgName.equals(""))
		{
			return "package invalid";
		}

		PackageManager pm =  ctx.getPackageManager();
		try
		{
			pm.getPackageInfo(pkgName, 0);
			return "true";
		} catch (NameNotFoundException e)
		{
			return "false";
		} catch (Exception e)
		{
			return "false";
		}
	}

	/////////////////////////////////////////////////////////////////////////////////
	//权限管理:仅对安卓6.0有效
	//https://blog.csdn.net/u011084603/article/details/78120301
	//https://blog.csdn.net/jiang877864109/article/details/78919404
	
	private static int s_callbackPermissions = 0;
	
	@SuppressLint("NewApi")
	public static String checkPermission(Context ctx, String permission)
	{
		if(Build.VERSION.SDK_INT < Build.VERSION_CODES.M)
			return "granted";
		int result = ctx.checkSelfPermission(permission);
		switch(result)
		{
		case PackageManager.PERMISSION_GRANTED: return "granted";
		case PackageManager.PERMISSION_DENIED: return "denied";
		default: return "denied";
		}
	}

	@SuppressLint("NewApi")
	public static String requestPermissions(Context ctx, JSONObject json)
	{
		int callback = 0;
		ArrayList<String> permissions = new ArrayList<String>();
		try
		{
			if(json.has("callback")) callback = json.getInt("callback");
			if(json.has("permissions"))
			{
				JSONObject permissionsJson = json.getJSONObject("permissions");
				Iterator<String> it = permissionsJson.keys();
				while(it.hasNext())
				{
					String key = it.next();
					String value = permissionsJson.getString(key);
//					Log.d(TAG, String.format(Locale.US, "key=%s,value=%s", key, value));
					permissions.add(value);
				}
			}
		} catch(Exception e)
		{
			e.printStackTrace();
		}
		s_callbackPermissions = callback;
		String[] permissionArray = new String[permissions.size()];
		permissions.toArray(permissionArray);
		if(Build.VERSION.SDK_INT < Build.VERSION_CODES.M)
		{
			int i;
			int[] results = new int[permissions.size()];
			for(i = 0; i < permissions.size(); ++i)
			{
				results[i] = PackageManager.PERMISSION_GRANTED;
			}
			MiscFunc.notifyLuaOfRequestPermissions(permissionArray, results);
			return "low_android_version";
		}
		Intent iii = new Intent(ctx, RequestPermissionActivity.class);
		iii.putExtra("permissions", permissionArray);
		ctx.startActivity(iii);
//		Activity act = (Activity)ctx;
//		act.requestPermissions(permissionArray, 123);
		return "success";
	}
	public static native void nativeNotifyRequestPermissionResult(int callback, String res);
	public static void notifyLuaOfRequestPermissions(String[] permissions, int[] grantResults)
	{
		if(s_callbackPermissions <= 0) return;
		JSONObject json = new JSONObject();
		try
		{
			for(int i = 0; i < permissions.length; ++i)
			{
				String permission = permissions[i];
				int grantResult = grantResults[i];
				if(grantResult == PackageManager.PERMISSION_GRANTED)
					json.put(permission, "granted");
				else
					json.put(permission, "denied");
			}
		} catch (Exception e)
		{
			e.printStackTrace();
		}

		nativeNotifyRequestPermissionResult(s_callbackPermissions, json.toString());
		s_callbackPermissions = 0;
	}

	public static String getTargetSdkVersion(Context ctx, String params)
	{
		int ver = ctx.getApplicationInfo().targetSdkVersion;
		return String.format(Locale.US, "%d", ver);
	}

	public static String gotoAppSetting(Context ctx, String packageName)
	{
		if(packageName.length() <= 0)
			packageName = ctx.getApplicationContext().getPackageName();
		Uri packageURI = Uri.parse("package:" + packageName);
		Intent intent =  new Intent(Settings.ACTION_APPLICATION_DETAILS_SETTINGS, packageURI);
		try
		{
			ctx.startActivity(intent);
		} catch (Exception e)
		{
			e.printStackTrace();
			return "exception";
		}
		return "success";
	}
}
