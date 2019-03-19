package com.xishanju.plm;

import java.util.Locale;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import org.xianliao.im.sdk.api.ISGAPI;
import org.xianliao.im.sdk.api.ISGAPIEventHandler;
import org.xianliao.im.sdk.api.SGAPIFactory;
import org.xianliao.im.sdk.constants.SGConstants;
import org.xianliao.im.sdk.modelbase.BaseReq;
import org.xianliao.im.sdk.modelbase.BaseResp;
import org.xianliao.im.sdk.modelmsg.SendAuth;

import com.xishanju.plm.xianliao.XianliaoUtil;

public class MySGEntryActivity extends Activity implements ISGAPIEventHandler
{
	ISGAPI m_api;
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		
		m_api = SGAPIFactory.createSGAPI(this, XianliaoUtil.m_appid);
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
//		Log.d(XianliaoUtil.TAG, "xiao resp:"+resp);
		int nType = resp.getType();
		int errCode = resp.errCode;
		String res = "";
		switch(nType)
		{
		case SGConstants.COMMAND_SHARE://分享回调
			Log.d(XianliaoUtil.TAG, "share resp,errCode="+resp.errCode+",errStr="+resp.errStr);
			res = String.format(Locale.US, "{\"errCode\":%d}", resp.errCode);
			XianliaoUtil.notifyShareResult(res);
			break;
		case SGConstants.COMMAND_AUTH://授权登录
			SendAuth.Resp authResp = (SendAuth.Resp)resp;
			Log.d(XianliaoUtil.TAG, "xianliao auth,errCode="+errCode+",code="+authResp.code);
			res = String.format(Locale.US, "{\"errCode\":%d,\"code\":\"%s\",\"lang\":\"%s\",\"country\":\"%s\"}",
				errCode, authResp.code, authResp.lang, authResp.country);
			XianliaoUtil.notifyLoginResult(res);
			break;
		default: break;
		}
		finish();
	}
}
