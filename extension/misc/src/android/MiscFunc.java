package com.xishanju.plm.misc;

import java.io.File;
import org.json.JSONObject;

import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.Uri;
import android.util.Log;

class MiscFunc
{
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
			Log.e("misc", "exception while installing apk");
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
}
