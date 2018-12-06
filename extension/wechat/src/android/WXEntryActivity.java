package cn.com.xishanju.qyq.wxapi;

import java.util.Locale;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import com.tencent.mm.opensdk.constants.ConstantsAPI;
import com.tencent.mm.opensdk.modelbase.BaseReq;
import com.tencent.mm.opensdk.modelbase.BaseResp;
import com.tencent.mm.opensdk.modelmsg.SendAuth;
import com.tencent.mm.opensdk.openapi.IWXAPI;
import com.tencent.mm.opensdk.openapi.IWXAPIEventHandler;
import com.tencent.mm.opensdk.openapi.WXAPIFactory;

import com.xishanju.plm.wechat.PlatformWechat;
import com.xishanju.plm.wechat.ShareUtil;

public class WXEntryActivity extends Activity implements IWXAPIEventHandler
{
	private IWXAPI m_api;
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		m_api = PlatformWechat.s_wxApi;
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
//		Log.d("gwjgwj", "gwjgwj,onResp:"+resp);
		int type = resp.getType();
		if(type == ConstantsAPI.COMMAND_SENDAUTH)//微信登录
		{
			SendAuth.Resp res = (SendAuth.Resp)resp;
			Log.d("gwjgwj", "gwjgwj,onAuthFinish,errCode="+res.errCode+",errStr="+res.errStr);
			Log.d("gwjgwj", String.format(Locale.US, "gwjgwj,code=%s,openId=%s", res.code, res.openId));
			PlatformWechat.notifyLoginResult(res.errCode, res.errStr, res.code, res.lang, res.country);
		}
		else if(type == ConstantsAPI.COMMAND_SENDMESSAGE_TO_WX)//微信分享
		{
			String result = "";
			switch(resp.errCode)
			{
			case BaseResp.ErrCode.ERR_OK:
				result = "success";
				break;
			case BaseResp.ErrCode.ERR_USER_CANCEL:
				result = "cancel";
				break;
			default:
				result = "failed";
				break;
			}
			String res = String.format(Locale.US, "{\"result\":\"%s\"}", result);
			ShareUtil.notifyShareResult(res);
		}
		finish();
	}
}
