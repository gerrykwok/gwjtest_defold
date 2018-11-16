package com.xishanju.plm.plmext;

import java.util.HashMap;

import android.util.Log;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.text.TextUtils;
import cn.sharesdk.framework.Platform;
import cn.sharesdk.wechat.friends.Wechat;
import cn.sharesdk.framework.PlatformActionListener;

//import com.xishanju.defold.cocosext.*;

public class PlatformWechat implements PlatformActionListener
{
	public static final String WEICHAT_UID_PREFIX = "wechat_";
	private static int s_loginCallback = -1;
	
	/**
	 * 微信登录
	 * 
	 * @param platform
	 * @param luaCallbackFunction
	 */
	public static void login(int platform, final int luaCallbackFunction) {
		s_loginCallback = luaCallbackFunction;
		
		Platform plat = new Wechat();

		if (plat.isAuthValid()) {

			String userId = plat.getDb().getUserId();
			if (!TextUtils.isEmpty(userId)) {
				final String nickname = plat.getDb().getUserName();
				final String profileImage = plat.getDb().getUserIcon();
				final String uid = WEICHAT_UID_PREFIX + userId;
				
				final String gender = plat.getDb().getUserGender();
				final String exportData=plat.getDb().exportData();
//				AppActivity.print("wechat_login2===, exportData:"+exportData);

				if (s_loginCallback  >= 0) 
				{	
					Log.d("wechat", "wxlogin2");

					String str = "{\"result\":0, \"userInfo\":{\"uid\":"+"\""+uid+"\""+", \"nickname\":"+"\""+nickname+"\""+", \"profileImage\":"+"\""+profileImage+"\""+", \"gender\":"+"\""+gender+"\""+", \"exportData\":"+exportData+"}}";

					LuaJavaBridge.callLuaFunctionWithString(s_loginCallback , str);
					LuaJavaBridge.releaseLuaFunction(s_loginCallback);

					s_loginCallback  = -1;
				}
				return;
			}
		}

		plat.setPlatformActionListener(new PlatformWechat());
		plat.SSOSetting(false);
//		plat.authorize();
		plat.showUser(null);
	}

	/**
	 * 登出
	 */
	public static void logout() {
		Platform plat = new Wechat();
		plat.removeAccount(true);
	}

	@Override
	public void onComplete(Platform paramPlatform, int paramInt, HashMap<String, Object> paramHashMap)
	{
	}

	@Override
	public void onError(Platform paramPlatform, int paramInt, Throwable paramThrowable)
	{
	}

	@Override
	public void onCancel(Platform paramPlatform, int paramInt)
	{
	}
	
	/**
	 * 判断是否安装微信
	 * 
	 * @return
	 */
	/*
	public static boolean isClientInstalled()
	{
		PackageManager pm =  AppActivity.appActivity.getPackageManager();
		try {
			pm.getPackageInfo("com.tencent.mm", 0);
			return true;
		} catch (NameNotFoundException e) {
			return false;
		} catch (Exception e)
		{
			return false;
		}
	}
	*/
}
