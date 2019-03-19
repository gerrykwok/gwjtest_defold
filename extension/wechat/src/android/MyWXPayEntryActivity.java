package com.xishanju.plm;

import java.util.Locale;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import com.tencent.mm.opensdk.constants.ConstantsAPI;
import com.tencent.mm.opensdk.modelbase.BaseReq;
import com.tencent.mm.opensdk.modelbase.BaseResp;
import com.tencent.mm.opensdk.openapi.IWXAPI;
import com.tencent.mm.opensdk.openapi.IWXAPIEventHandler;
import com.tencent.mm.opensdk.openapi.WXAPIFactory;

import com.xishanju.plm.wechat.WeixinPayUtil;

public class MyWXPayEntryActivity extends Activity implements IWXAPIEventHandler
{
	private IWXAPI m_api;
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		m_api = WXAPIFactory.createWXAPI(this, WeixinPayUtil.APP_ID);
		m_api.handleIntent(getIntent(), this);
	}
	
	@Override
	protected void onNewIntent(Intent intent)
	{
		super.onNewIntent(intent);
		setIntent(intent);
		m_api.handleIntent(intent, this);
	}
	
	@Override
	public void onReq(BaseReq req)
	{
	}

	@Override
	public void onResp(BaseResp resp)
	{
		if(resp.getType()==ConstantsAPI.COMMAND_PAY_BY_WX)
		{
			Log.d(WeixinPayUtil.TAG, "onPayFinish,errCode="+resp.errCode+",errStr="+resp.errStr);
			String res = String.format(Locale.US, "{\"errCode\":%d}", resp.errCode);
			WeixinPayUtil.notifyPayResult(res);
		}
		finish();
	} 
}
