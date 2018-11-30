package com.xishanju.plm.wechat;

import java.io.File;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Locale;

import org.json.JSONObject;
import android.util.Log;
import android.content.Intent;
import android.content.ComponentName;
import android.net.Uri;

public class ShareUtil
{
	public static final String TAG = "shareutil";
	public static String shareWithIntent(String data)
	{
		Log.d(TAG, "gwjgwj,shareWithIntent:" + data);
		String pkgName = "";
		String clsName = "";
		String mimeType = "";
		String streamPath = "";
		HashMap<String, String> extras = new HashMap<String, String>();

		try
		{
			JSONObject json = new JSONObject(data);
			if(json.has("pkgName")) pkgName = json.getString("pkgName");
			if(json.has("clsName")) clsName = json.getString("clsName");
			if(json.has("mimeType")) mimeType = json.getString("mimeType");
			if(json.has("streamPath")) streamPath = json.getString("streamPath");
			if(json.has("extras"))
			{
				JSONObject extrasJson = json.getJSONObject("extras");
				Iterator<String> it = extrasJson.keys();
				while(it.hasNext())
				{
					String key = it.next();
					String value = extrasJson.getString(key);
					extras.put(key, value);
				}
			}
		} catch(Exception e)
		{
			e.printStackTrace();
		}

		if(pkgName.equals("")) return "package empty";
		if(clsName.equals("")) return "class empty";
		
//		Log.d("gwjgwj", String.format(Locale.US, "gwjgwj,pkgName=%s,clsName=%s,streamPath=%s,mimeType=%s", pkgName, clsName, streamPath, mimeType));

		boolean success = false;
		final Intent intent = new Intent(Intent.ACTION_SEND);
		intent.setComponent(new ComponentName(pkgName, clsName));
		intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		//mime type
		if(mimeType != null && !mimeType.equals(""))
		{
			intent.setType(mimeType);
		}
		//stream
		if(streamPath != null && !streamPath.equals(""))
		{
			intent.putExtra(Intent.EXTRA_STREAM, Uri.fromFile(new File(streamPath)));
		}
		//extra
		for(String key : extras.keySet())
		{
			String value = extras.get(key);
			Log.d(TAG, "gwjgwj,extra,"+key+"="+value);
			intent.putExtra(key, value);
		}
		try
		{
			PlatformWechat.s_context.startActivity(Intent.createChooser(intent, "分享"));
			success = true;
		} catch(Exception e)
		{
			e.printStackTrace();
		}
		return success ? "success" : "failed";
	}
}
