package com.xishanju.plm.gaode;

import java.util.Locale;

import org.json.JSONException;
import org.json.JSONObject;

import android.content.Context;
import android.content.Intent;
import android.location.LocationManager;
import android.provider.Settings;
import android.util.Log;

import com.amap.api.location.AMapLocation;
import com.amap.api.location.AMapLocationClient;
import com.amap.api.location.AMapLocationClientOption;
import com.amap.api.location.AMapLocationClientOption.AMapLocationMode;
import com.amap.api.location.AMapLocationListener;

public class GaodeUtil
{
	public static final String TAG = "gaodeUtil";
	//高德定位
	static AMapLocationClient m_locationClient = null;
	static int m_locationLuaCallback = 0;
	static int m_locationAuthorizationStatus = 1;//0=没有 1=没有询问状态  2=已经获得授权
	static AMapLocationListener m_locationListener = new AMapLocationListener()
	{
		@Override
		public void onLocationChanged(AMapLocation location)
		{
			JSONObject obj = new JSONObject();
			if(location != null)
			{
				int code = location.getErrorCode();
//				Log.d("location", "location err code="+code);
				if(code == 0)
				{
					m_locationAuthorizationStatus = 2;
					final double longitude = location.getLongitude();
					final double latitude = location.getLatitude();
					final String city = location.getCity();
					final String province = location.getProvince();
					try
					{
						obj.put("result", "success");
						obj.put("longitude", longitude);
						obj.put("latitude", latitude);
						obj.put("province", province);
						obj.put("city", city);
					} catch (JSONException e)
					{
						e.printStackTrace();
					}
				}
				else
				{
					if(code == AMapLocation.ERROR_CODE_FAILURE_LOCATION_PERMISSION)
					{
						m_locationAuthorizationStatus = 0;
					}
					else
					{
						m_locationAuthorizationStatus = 0;
					}
					String errInfo = location.getErrorInfo();
					Log.w(TAG, String.format(Locale.US, "location failed,errCode=%d,errInfo=%s", code, errInfo));
					try
					{
						obj.put("result", "failed");
						obj.put("errCode", code);
						obj.put("errInfo", errInfo);
					} catch (Exception e)
					{
						e.printStackTrace();
					}
				}
			}
			else
			{
				m_locationAuthorizationStatus = 0;
				try
				{
					obj.put("result", "location null");
				} catch (Exception e)
				{
					e.printStackTrace();
				}
			}
			notifyLocation(obj.toString());
		}
	};
	public static String locationInit(Context ctx, String apiKey)
	{
		if(m_locationClient == null)
		{
			if(apiKey == null || apiKey.equals(""))
			{
				Log.e(TAG, "must specify gaode apiKey");
				return "failed";
			}
			AMapLocationClient.setApiKey(apiKey);
			m_locationClient = new AMapLocationClient(ctx.getApplicationContext());
			m_locationClient.setLocationListener(m_locationListener);

			AMapLocationClientOption option = new AMapLocationClientOption();
			option.setLocationMode(AMapLocationMode.Hight_Accuracy);
			option.setInterval(1000);//设置定位间隔,单位毫秒,默认为2000ms，最低1000ms。
			option.setNeedAddress(true);//设置是否返回地址信息（默认返回地址信息）
			option.setMockEnable(true);//设置是否允许模拟位置,默认为true，允许模拟位置
			option.setHttpTimeOut(30000);//单位是毫秒，默认30000毫秒，建议超时时间不要低于8000毫秒。
			option.setLocationCacheEnable(false);//关闭缓存机制
			m_locationClient.setLocationOption(option);
		}
		return "success";
	}
	public static String locationStart(Context ctx, JSONObject params)
	{
		int callback = 0;
		if(m_locationClient == null) return "client null";
		try
		{
			if(params.has("callback")) callback = params.getInt("callback");
		} catch (Exception e)
		{
			e.printStackTrace();
		}
		m_locationLuaCallback = callback;
		m_locationClient.startLocation();
		return "success";
	}
	public static String locationStop(Context ctx, JSONObject params)
	{
		if(m_locationClient == null) return "client null";
		nativeReleaseLuaCallback(m_locationLuaCallback);
		m_locationLuaCallback = 0;
		m_locationClient.stopLocation();
		return "success";
	}
	public static native void nativeNotifyLocation(int callback, String res);
	public static native void nativeReleaseLuaCallback(int callback);
	public static void notifyLocation(String res)
	{
		if(m_locationLuaCallback > 0)
		{
			nativeNotifyLocation(m_locationLuaCallback, res);
		}
	}
	public static String getPackageName(Context ctx, String params)
	{
		return ctx.getApplicationContext().getPackageName();
	}
	public static String getAuthorizationStatus(Context ctx, String params)//0没有 1没有询问状态  2 已经获得授权
	{
		return ""+m_locationAuthorizationStatus;
	}
	public static String isGPSEnabled(Context ctx, String params)
	{
		LocationManager locMgr = (LocationManager)ctx.getApplicationContext().getSystemService(Context.LOCATION_SERVICE);
		boolean bEnabled = locMgr.isProviderEnabled(LocationManager.GPS_PROVIDER);
		return bEnabled ? "enabled" : "disabled";
	}
	public static String jumpToGPSSetting(Context ctx, String params)
	{
		Intent intent = new Intent(Settings.ACTION_LOCATION_SOURCE_SETTINGS);
		ctx.startActivity(intent);
		return "";
	}
}
