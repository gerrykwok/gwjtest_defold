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

	public static String init(Context ctx, String wechatAppId)
	{
		s_context = ctx;
		s_wxApi = WXAPIFactory.createWXAPI(ctx, null);
		s_wxApi.registerApp(wechatAppId);
		return "";
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
		notifyLua(str);
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
