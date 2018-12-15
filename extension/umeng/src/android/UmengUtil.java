package com.xishanju.plm.umeng;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Locale;
import java.util.Map;

import org.json.JSONObject;

import android.content.Context;
import android.util.Log;

import com.umeng.analytics.MobclickAgent;
import com.umeng.analytics.MobclickAgent.EScenarioType;
import com.umeng.analytics.game.UMGameAgent;
import com.umeng.commonsdk.UMConfigure;

public class UmengUtil
{
	public static final String TAG = "umengutil";
	public static String init(Context ctx, String params)
	{
		String appKey = "";
		String channel = "";
		try
		{
			JSONObject json = new JSONObject(params);
			if(json.has("appkey")) appKey = json.getString("appkey");
			if(json.has("channel")) channel = json.getString("channel");
		} catch(Exception e)
		{
			e.printStackTrace();
		}
		
		MobclickAgent.setScenarioType(ctx, EScenarioType.E_DUM_GAME);
		UMConfigure.init(ctx, appKey, channel, UMConfigure.DEVICE_TYPE_PHONE, null);
		UMConfigure.setLogEnabled(true);
		UMGameAgent.init(ctx);
		return "success";
	}
	public static String onActivityEvent(Context ctx, String event)
	{
		if(event.equals("pause"))
			UMGameAgent.onPause(ctx);
		else if(event.equals("resume"))
			UMGameAgent.onResume(ctx);
		return "";
	}
	public static String sendEvent(Context ctx, JSONObject json)
	{
		String eventName = "";
		Map<String, String> args = new HashMap<String, String>();
		JSONObject argsJson;
		try
		{
			if(json.has("name")) eventName = json.getString("name");
			if(json.has("args"))
			{
				argsJson = json.getJSONObject("args");
				Iterator<String> keys = argsJson.keys();
				String key, value;
				while(keys.hasNext())
				{
					key = keys.next();
					value = argsJson.getString(key);
					args.put(key, value);
				}
			}
		} catch(Exception e)
		{
			e.printStackTrace();
		}
		if(eventName == "")
			return "must specify name";
		Log.d(TAG, "gwjgwj,send umeng event:" + eventName + ", args=" + args);
		MobclickAgent.onEvent(ctx, eventName, args);
		return "success";
	}
}
