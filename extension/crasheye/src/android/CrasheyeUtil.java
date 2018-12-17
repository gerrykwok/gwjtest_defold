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
//		Log.d(TAG, "init crasheye with " + appkey);
//		Crasheye.initWithNativeHandle(ctx.getApplicationContext(), appkey);
		Crasheye.init(ctx.getApplicationContext(), appkey);
		return "success";
	}

	public static String sendScriptError(Context ctx, JSONObject json)
	{
		String title = "";
		String content = "";
		try
		{
			if(json.has("title")) title = json.getString("title");
			if(json.has("content")) content = json.getString("content");
		} catch(Exception e)
		{
			e.printStackTrace();
		}

		Crasheye.sendScriptException(title, content);
		return "success";
	}

	public static String setUserId(Context ctx, JSONObject json)
	{
		String userId = "";
		try
		{
			if(json.has("userId")) userId = json.getString("userId");
		} catch(Exception e)
		{
			e.printStackTrace();
		}
		Crasheye.setUserIdentifier(userId);
		return "success";
	}

	public static String addExtraData(Context ctx, JSONObject json)
	{
		try
		{
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

	public static String removeExtraData(Context ctx, JSONObject json)
	{
		String key = "";
		try
		{
			if(json.has("key")) key = json.getString("key");
		} catch(Exception e)
		{
			e.printStackTrace();
		}
		if(key.equals(""))
			return "key invalid";
//		Log.d(TAG, "crasheye:remove extra of key "+key);
		Crasheye.removeExtraData(key);
		return "success";
	}

	public static String clearExtraData(Context ctx, JSONObject json)
	{
//		Log.d(TAG, "crasheye:clear all extra data");
		Crasheye.clearExtraData();
		return "success";
	}
}
