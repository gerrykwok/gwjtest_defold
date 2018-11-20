package com.xishanju.plm.plmext;

import java.util.Locale;

import com.tencent.mm.opensdk.modelmsg.SendAuth;

public class PlatformWechat
{
	private static int s_loginCallback = -1;
	/*
	* 微信登录
	*/
	public static void login(final int luaCallbackFunction)
	{
		s_loginCallback = luaCallbackFunction;

		SendAuth.Req req = new SendAuth.Req();
		req.scope = "snsapi_userinfo";
		req.state = "wechat_login_plm";
		plmext.s_wxApi.sendReq(req);
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
		LuaJavaBridge.runOnGLThread(new Runnable() {
			@Override
			public void run() {
				if(s_loginCallback >= 0)
				{
					LuaJavaBridge.callLuaFunctionWithString(s_loginCallback, res);
					LuaJavaBridge.releaseLuaFunction(s_loginCallback);
					s_loginCallback = -1;
				}
			}
		});
	}
}
