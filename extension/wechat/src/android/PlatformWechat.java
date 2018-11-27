package com.xishanju.plm.wechat;

import java.util.Locale;

import android.content.Context;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;

import com.tencent.mm.opensdk.modelmsg.SendAuth;
import com.tencent.mm.opensdk.openapi.IWXAPI;
import com.tencent.mm.opensdk.openapi.WXAPIFactory;

public class PlatformWechat
{
	public static final String TAG = "wechat";
	public static Context s_context;
	public static IWXAPI s_wxApi;

	public static void init(Context ctx)
	{
		String wechatAppId = "wxc0acf2068b5dd7a3";

		s_context = ctx;
		s_wxApi = WXAPIFactory.createWXAPI(ctx, null);
		s_wxApi.registerApp(wechatAppId);
	}
	/*
	* 微信登录
	*/
	public static void login()
	{
		SendAuth.Req req = new SendAuth.Req();
		req.scope = "snsapi_userinfo";
		req.state = "wechat_login_plm";
		s_wxApi.sendReq(req);
	}

	public static void logout()
	{
	}

	public static void notifyLoginResult(int errCode, String errStr, String code, String lang, String country)
	{
		String str;
		if(errCode == 0)
		{
			str = String.format(Locale.US, "{\"errCode\":%d, \"errStr\":\"%s\", \"code\":\"%s\", \"lang\":\"%s\", \"country\":\"%s\"}", errCode, "", code, lang, country);
		}
		else
		{
			str = String.format(Locale.US, "{\"errCode\":%d, \"errStr\":\"%s\"}", errCode, errStr);
		}
		final String res = str;
//		LuaJavaBridge.runOnGLThread(new Runnable() {
//			@Override
//			public void run() {
//				if(s_loginCallback >= 0)
//				{
//					LuaJavaBridge.callLuaFunctionWithString(s_loginCallback, res);
//					LuaJavaBridge.releaseLuaFunction(s_loginCallback);
//					s_loginCallback = -1;
//				}
//			}
//		});
		notifyLua(res);
	}

	public static native void notifyLua(String value);

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
