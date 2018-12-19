package com.xishanju.plm.agora;

import android.content.Context;
import android.util.Log;

import io.agora.rtc.RtcEngine;

public class AgoraUtil
{
	public static final String TAG = "agorautil";
	public static native void nativeInit();
	public static String init(Context ctx, String params)
	{
		Log.d(TAG, "gwjgwj,load agora library");
		System.loadLibrary("agora-rtc-sdk-jni");
		Log.d(TAG, "gwjgwj,load agora library finish");

		String version = RtcEngine.getSdkVersion();
		Log.d(TAG, "gwjgwj,agora sdk version:" + version);

		Thread thr = new Thread(new Runnable()
		{
			@Override
			public void run()
			{
				nativeInit();
			}
		});
		thr.start();

		return "success";
	}
}
