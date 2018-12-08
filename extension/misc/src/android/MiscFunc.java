package com.xishanju.plm.misc;

import java.io.File;

import android.content.Context;
import android.content.Intent;
import android.net.Uri;

class MiscFunc
{
	public static String startInstallApk(Context ctx, String apkFullPath)
	{
		File f = new File(apkFullPath);
		if(!f.exists())
		{
			return "apk not exist";
		}
		Intent intent = new Intent(Intent.ACTION_VIEW);
		intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		Uri uri = Uri.fromFile(f);
		intent.setDataAndType(uri, "application/vnd.android.package-archive");
		ctx.startActivity(intent);
		return "success";
	}
}
