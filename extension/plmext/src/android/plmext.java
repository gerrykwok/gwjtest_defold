package com.xishanju.plm.plmext;

import java.util.HashMap;

import android.util.Log;
import android.content.Context;
import android.content.Intent;
import android.content.ComponentName;
import android.os.Environment;
import android.app.Activity;

import com.mob.MobSDK;
import cn.sharesdk.framework.ShareSDK;
import cn.sharesdk.wechat.friends.Wechat;
import cn.sharesdk.wechat.moments.WechatMoments;

public class plmext
{
	public static final String TAG = "plmext";
	public static Context s_context;

	public static void init(Context ctx)
	{
		String mob_appId = "1299624a6383f";
		String wechatAppId = "wxc0acf2068b5dd7a3";
		String wechatAppSecret = "20aa71045fee2ea0f82c1b4ed643af8b";

		s_context = ctx;
		MobSDK.init(ctx, mob_appId);

		HashMap<String, Object> info = new HashMap<String, Object>();
		info.put("Id", "4");
		info.put("SortId", "4");
		info.put("AppId", wechatAppId);
		info.put("AppSecret", wechatAppSecret);
		info.put("BypassApproval", "false");
		info.put("Enable", "true");
		ShareSDK.setPlatformDevInfo(Wechat.NAME, info);
		HashMap<String, Object> info2 = new HashMap<String, Object>();
		info2.put("Id", "5");
		info2.put("SortId", "5");
		info2.put("AppId", wechatAppId);
		info2.put("AppSecret", wechatAppSecret);
		info2.put("BypassApproval", "false");
		info2.put("Enable", "true");
		ShareSDK.setPlatformDevInfo(WechatMoments.NAME, info2);
	}

	public static void getPhoto(int fromCamera, String localPath, int width, int height, int luaCallbackFunction)
	{
		Context ctx = s_context;
		//Log.i(TAG, "localPath:" + localPath + ", luaCallbackFunction:" + luaCallbackFunction);

		Intent iii = new Intent(ctx, TakePhoto.class);
		iii.putExtra("fromCamera", fromCamera==1);
		iii.putExtra("localPath", localPath);
		iii.putExtra("imageWidth", width);
		iii.putExtra("imageHeight", height);
		iii.putExtra("luaCallback", luaCallbackFunction);
		Activity act = (Activity)ctx;
		act.startActivity(iii);
	}
}
