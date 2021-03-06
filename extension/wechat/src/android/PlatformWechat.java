package com.xishanju.plm.wechat;

import org.json.JSONObject;

import android.content.Context;
import android.util.Log;

import com.tencent.mm.opensdk.modelmsg.SendAuth;
import com.tencent.mm.opensdk.openapi.IWXAPI;
import com.tencent.mm.opensdk.openapi.WXAPIFactory;

public class PlatformWechat
{
	public static final String TAG = "wechat";
	public static IWXAPI s_wxApi = null;
	private static int s_loginCallback = 0;

	public static String init(Context ctx, String wechatAppId)
	{
		s_wxApi = WXAPIFactory.createWXAPI(ctx, null);
		s_wxApi.registerApp(wechatAppId);
		return "success";
	}
	/*
	* 微信登录
	*/
	public static String login(Context ctx, JSONObject json)
	{
		double timeout = 0;
		try
		{
			if(json.has("callback")) s_loginCallback = json.getInt("callback");
			if(json.has("timeout")) timeout = json.getDouble("timeout");
		} catch(Exception e)
		{
			e.printStackTrace();
		}
		Log.d(TAG, "login with timeout " + timeout);
		boolean success;
		SendAuth.Req req = new SendAuth.Req();
		req.scope = "snsapi_userinfo";
		req.state = "wechat_login_plm";
		success = s_wxApi.sendReq(req);
		if(timeout != 0)
			delayNotifyLoginResult(s_loginCallback, timeout, "{\"errCode\":-2, \"errStr\":\"cancel login\"}");
		return success ? "success" : "failed";
	}

	public static void logout(Context ctx, JSONObject json)
	{
	}

	public static native void notifyLua(int callback, String value);
	public static void notifyLoginResult(String value)
	{
		if(s_loginCallback > 0)
		{
			notifyLua(s_loginCallback, value);
			s_loginCallback = 0;
		}
	}
	public static native void delayNotifyLoginResult(int callback, double timeout, String value);
}
