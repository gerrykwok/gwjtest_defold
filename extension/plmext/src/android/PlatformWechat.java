package com.xishanju.plm.plmext;

import java.util.HashMap;

import android.util.Log;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.text.TextUtils;
import cn.sharesdk.framework.Platform;
import cn.sharesdk.wechat.friends.Wechat;
import cn.sharesdk.framework.PlatformActionListener;

public class PlatformWechat implements PlatformActionListener
{
	public static final String WEICHAT_UID_PREFIX = "wechat_";
	public static final String TAG = "wechat";
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
		boolean valid = plat.isAuthValid();
		Log.d(TAG, "auth valid:" + valid);

		if (valid)
		{
			String userId = plat.getDb().getUserId();
			if (!TextUtils.isEmpty(userId)) {
				final String nickname = plat.getDb().getUserName();
				final String profileImage = plat.getDb().getUserIcon();
				final String uid = WEICHAT_UID_PREFIX + userId;
				
				final String gender = plat.getDb().getUserGender();
				final String exportData=plat.getDb().exportData();
//				AppActivity.print("wechat_login2===, exportData:"+exportData);

				LuaJavaBridge.runOnGLThread(new Runnable() {
					@Override
					public void run()
					{
						if (s_loginCallback  >= 0) 
						{	
							Log.d(TAG, "wxlogin2");

							String str = "{\"result\":0, \"userInfo\":{\"uid\":"+"\""+uid+"\""+", \"nickname\":"+"\""+nickname+"\""+", \"profileImage\":"+"\""+profileImage+"\""+", \"gender\":"+"\""+gender+"\""+", \"exportData\":"+exportData+"}}";

							LuaJavaBridge.callLuaFunctionWithString(s_loginCallback , str);
							LuaJavaBridge.releaseLuaFunction(s_loginCallback);

							s_loginCallback  = -1;
						}
					}
				});
				return;
			}
		}

		plat.setPlatformActionListener(new PlatformWechat());
		plat.SSOSetting(false);
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
	public void onComplete(Platform platform, int action, HashMap<String, Object> res)
	{
		Log.d(TAG, "on complete,action="+action);
		if (action == Platform.ACTION_AUTHORIZING || action == Platform.ACTION_USER_INFOR)
		{
			final String nickname = platform.getDb().getUserName();
			final String profileImage = platform.getDb().getUserIcon();

//			System.out.println("getUserIcon:"+platform.getDb().getUserIcon());
//			System.out.println("getUserGender:"+platform.getDb().getUserGender());

			String prefix = "";
			if (platform.getId() == 1)
			{
//				prefix = PlatformSinaWeibo.SINA_WEIBO_UID_PREFIX;
			}
			else if(platform.getId() == 2)
			{
//				prefix = PlatformTencentWeibo.TENCENT_WEIBO_UID_PREFIX;
			}
			else if(platform.getId() == 4)
			{
				prefix = PlatformWechat.WEICHAT_UID_PREFIX;
			}
			final String uid = prefix +  platform.getDb().getUserId();

			final String gender = platform.getDb().getUserGender();
			final String exportData=platform.getDb().exportData();
//			print("exportData1:"+exportData);

			LuaJavaBridge.runOnGLThread(new Runnable() {
				@Override
				public void run()
				{
					Log.d(TAG, "gwjgwj,complete,call lua:"+s_loginCallback);
					if (s_loginCallback >= 0)
					{
		//				String str = "{\"result\":0, \"userInfo\":{\"uid\":"+"\""+uid+"\""+", \"nickname\":"+"\""+nickname+"\""+", \"profileImage\":"+"\""+profileImage+"\""+"}}";
						String str = "{\"result\":0, \"userInfo\":{\"uid\":"+"\""+uid+"\""+", \"nickname\":"+"\""+nickname+"\""+", \"profileImage\":"+"\""+profileImage+"\""+", \"gender\":"+"\""+gender+"\""+", \"exportData\":"+exportData+"}}";
						LuaJavaBridge.callLuaFunctionWithString(s_loginCallback, str);
						LuaJavaBridge.releaseLuaFunction(s_loginCallback);
						s_loginCallback = -1;
					}
				}
			});
		}
	}

	@Override
	public void onError(Platform platform, int action, Throwable throwable)
	{
		Log.d(TAG, "onError:"+throwable.getMessage());
		throwable.printStackTrace();
		System.out.println("onError action:"+action);

		if (action == Platform.ACTION_AUTHORIZING)
		{
			LuaJavaBridge.runOnGLThread(new Runnable() {
				@Override
				public void run()
				{
					if (s_loginCallback >= 0)
					{
						String str = "{\"result\":1}";
						LuaJavaBridge.callLuaFunctionWithString(s_loginCallback, str);
						LuaJavaBridge.releaseLuaFunction(s_loginCallback);
						s_loginCallback = -1;
					}
				}
			});
		}
	}

	@Override
	public void onCancel(Platform platform, int action)
	{
		Log.d(TAG, "onCancel action:"+action);

		if (action == Platform.ACTION_AUTHORIZING)
		{
			LuaJavaBridge.runOnGLThread(new Runnable() {
				@Override
				public void run()
				{
					if (s_loginCallback >= 0)
					{
						String str = "{\"result\":1}";
						LuaJavaBridge.callLuaFunctionWithString(s_loginCallback, str);
						LuaJavaBridge.releaseLuaFunction(s_loginCallback);
						s_loginCallback = -1;
					}
				}
			});
		}
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
