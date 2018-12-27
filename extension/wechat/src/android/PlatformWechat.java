package com.xishanju.plm.wechat;

import java.util.Locale;
import org.json.JSONObject;

import android.content.Context;

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
		try
		{
			if(json.has("callback")) s_loginCallback = json.getInt("callback");
		} catch(Exception e)
		{
			e.printStackTrace();
		}
		boolean success;
		SendAuth.Req req = new SendAuth.Req();
		req.scope = "snsapi_userinfo";
		req.state = "wechat_login_plm";
		success = s_wxApi.sendReq(req);
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
}
