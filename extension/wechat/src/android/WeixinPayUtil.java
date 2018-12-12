package com.xishanju.plm.wechat;

import org.json.JSONObject;

import android.util.Log;
import android.content.Context;

import com.tencent.mm.opensdk.modelpay.PayReq;
import com.tencent.mm.opensdk.openapi.IWXAPI;
import com.tencent.mm.opensdk.openapi.WXAPIFactory;

public class WeixinPayUtil
{
	public static final String TAG = "wxpay";
	public static int s_payCallback = 0;
	private static IWXAPI s_api = null;
	public static String APP_ID = "";
	public static String makePurchase(Context ctx, JSONObject json)
	{
		boolean success = true;
		PayReq request = new PayReq();
		try
		{
			if(json.has("callback")) s_payCallback = json.getInt("callback");
			if(json.has("appid")) request.appId = json.getString("appid");
			if(json.has("partnerid")) request.partnerId = json.getString("partnerid");
			if(json.has("prepayid")) request.prepayId = json.getString("prepayid");
			if(json.has("package")) request.packageValue = json.getString("package");
			if(json.has("noncestr")) request.nonceStr = json.getString("noncestr");
			if(json.has("timestamp")) request.timeStamp = json.getString("timestamp");
			if(json.has("sign")) request.sign = json.getString("sign");
//			request.extData = "app data";//optional
		} catch(Exception e)
		{
			e.printStackTrace();
			success = false;
		}
		if(s_api == null)
		{
			WeixinPayUtil.APP_ID = request.appId;
			s_api = WXAPIFactory.createWXAPI(ctx, null);
			s_api.registerApp(request.appId);
		}
		if(success)
			success = s_api.sendReq(request);
		return success ? "success" : "failed";
	}

	public static native void nativeNotifyPay(int callback, String result);
	public static void notifyPayResult(String result)
	{
		if(s_payCallback > 0)
		{
			nativeNotifyPay(s_payCallback, result);
			s_payCallback = 0;
		}
	}
}
