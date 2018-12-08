package com.xishanju.plm.crasheye;

import org.json.JSONObject;
import java.util.Iterator;

import android.content.Context;
import android.util.Log;

import com.xsj.crasheye.Crasheye;

class CrasheyeUtil
{
	public static final String TAG = "crasyeyeutil";
	public static String init(Context ctx, String appkey)
	{
//		Crasheye.initWithNativeHandle(ctx.getApplicationContext(), appkey);
		Crasheye.init(ctx.getApplicationContext(), appkey);
		Log.d(TAG, "init crasheye with " + appkey);
		return "success";
	}

	public static String sendScriptError(Context ctx, String data)
	{
		String title = "";
		String content = "";
		try
		{
			JSONObject json = new JSONObject(data);
			if(json.has("title")) title = json.getString("title");
			if(json.has("content")) content = json.getString("content");
		} catch(Exception e)
		{
			e.printStackTrace();
		}

		Crasheye.sendScriptException(title, content);
		return "success";
	}

	public static String setUserId(Context ctx, String identifier)
	{
		Crasheye.setUserIdentifier(identifier);
		return "success";
	}

	public static String addExtraData(Context ctx, String data)
	{
		Log.d(TAG, "gwjgwj,addExtraData,data=" + data);
		try
		{
			JSONObject json = new JSONObject(data);
			Iterator<String> it = json.keys();
			while(it.hasNext())
			{
				String key = it.next();
				String value = json.getString(key);
				Log.d(TAG, "gwjgwj,addExtraData,key="+key+",value="+value);
				Crasheye.addExtraData(key, value);
			}
		} catch(Exception e)
		{
			e.printStackTrace();
		}
		return "success";
	}
}
