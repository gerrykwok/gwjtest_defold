package com.xishanju.plm.plmext;

import java.util.HashMap;

//import org.cocos2dx.lib.Cocos2dxLuaJavaBridge;
//import org.cocos2dx.lua.AppActivity;

import android.util.Log;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.text.TextUtils;
import cn.sharesdk.framework.Platform;
import cn.sharesdk.wechat.friends.Wechat;
import cn.sharesdk.framework.PlatformActionListener;

import com.xishanju.defold.cocosext.*;

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
//				12-10 21:25:07.447: I/mah-jong(22476): wechat_login2===, exportData:{"unionid":"ofxUkwa1UjnUyPbMgEjt6giQXKt0","icon":"","expiresTime":1481376284444,"nickname":"林家斌","token":"dVSSD5IP4DFWicpBWYbMD7KM4-6XEiMoNAtWY4NCnACqR5dumlrvGb0Uzjpkl96TUMDemIELEfBHTmCT2OCdd-hAgC6Y83PzqPEmjVJWP2c","city":"","gender":"2","province":"","weibo":"oEFSXvzr8EWAbydkpTdQWDNJ9MJQ","refresh_token":"ta2XszhmiRdWrgwo3G0zuH7GrcYw7lfvA7QPEmSttDOGfpTMgMrgn6ZrBP7K91ipm9m-xP51BsBwz6Olxcffrn7MK1nYFHsFrFjNqknucPo","openid":"oEFSXvzr8EWAbydkpTdQWDNJ9MJQ","country":"CN","expiresIn":7200}

            	if (s_loginCallback  >= 0) 
            	{	
            		Log.d("wechat", "wxlogin2");
//		    		12-10 20:15:11.913: I/mah-jong(4829): {"result":0, "userInfo":{"uid":"wechat_oEFSXvzr8EWAbydkpTdQWDNJ9MJQ", "nickname":"林家斌", "profileImage":"", "gender":"null"}}

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
