package com.xishanju.plm.xianliao;

import org.json.JSONObject;
import java.io.InputStream;

import android.content.Context;
import android.util.Log;

import org.xianliao.im.sdk.api.ISGAPI;
import org.xianliao.im.sdk.api.SGAPIFactory;
import org.xianliao.im.sdk.constants.SGConstants;
import org.xianliao.im.sdk.modelmsg.SGGameObject;
import org.xianliao.im.sdk.modelmsg.SGImageObject;
import org.xianliao.im.sdk.modelmsg.SGTextObject;
import org.xianliao.im.sdk.modelmsg.SGMediaMessage;
import org.xianliao.im.sdk.modelmsg.SendAuth;
import org.xianliao.im.sdk.modelmsg.SendMessageToSG;

public class XianliaoUtil
{
	public static String TAG = "xianliaoutil";
	public static ISGAPI m_api;
	public static String m_appid;
	private static int m_loginCallback = 0;

	public static String init(Context ctx, String appid)
	{
//		Log.d(TAG, "gwjgwj,create ISGAPI with appid " + appid);
		m_appid = appid;
		m_api = SGAPIFactory.createSGAPI(ctx, appid);
		return "success";
	}

	public static String login(Context ctx, JSONObject json)
	{
		if(m_api == null)
		{
			Log.d(TAG, "please init");
			return "api null";
		}

		try
		{
			if(json.has("callback")) m_loginCallback = json.getInt("callback");
		} catch(Exception e)
		{
			e.printStackTrace();
		}

		SendAuth.Req req = new SendAuth.Req();
		req.state = "plmhd_xianliao_auth";
		m_api.sendReq(req);

		return "success";
	}

	public static String logout(Context ctx, JSONObject json)
	{
		return "success";
	}

	public static native void nativeNotifyLogin(int callback, String res);
	public static void notifyLoginResult(String res)
	{
		if(m_loginCallback > 0)
		{
			nativeNotifyLogin(m_loginCallback, res);
			m_loginCallback = 0;
		}
	}

	public static int s_shareCallback = 0;
	public static String share(Context ctx, JSONObject json)
	{
		String type = "";
		String text = "";
		String image = "";
		int imageFrom = 1;//1=全路径图片，2=包内图片
		String url = "";
		String title = "";
		try
		{
			if(json.has("type")) type = json.getString("type");
			if(json.has("callback")) s_shareCallback = json.getInt("callback");
			if(json.has("text")) text = json.getString("text");
			if(json.has("image")) image = json.getString("image");
			if(json.has("imageFrom")) imageFrom = json.getInt("imageFrom");
			if(json.has("url")) url = json.getString("url");
			if(json.has("title")) title = json.getString("title");
		} catch(Exception e)
		{
			e.printStackTrace();
		}
		if(type.equals("text"))
		{
			//初始化一个SGTextObject对象，填写分享的文本内容
			SGTextObject textObject = new SGTextObject();
			textObject.text = text;  //要分享的内容
			//用SGTextObject对象初始化一个SGMediaMessage对象
			SGMediaMessage msg = new SGMediaMessage();
			msg.mediaObject = textObject;
			//构造一个Req
			SendMessageToSG.Req req = new SendMessageToSG.Req();
			req.transaction = SGConstants.T_TEXT; //分享文本类型
			req.mediaMessage = msg;
			req.scene = SendMessageToSG.Req.SGSceneSession; //代表分享到会话列表
			//调用api接口发送数据到闲聊
			m_api.sendReq(req);
		}
		else if(type.equals("image"))
		{
			//初始化一个SGImageObject对象，设置所分享的图片内容
			SGImageObject imageObject = new SGImageObject();
			if(imageFrom == 2)
				imageObject.imageData = getBytesFromAsset(ctx, image);
			else
				imageObject.imagePath = image;
			//用SGImageObject对象初始化一个SGMediaMessage对象
			SGMediaMessage msg = new SGMediaMessage();
			msg.mediaObject = imageObject;
			//构造一个Req
			SendMessageToSG.Req req = new SendMessageToSG.Req();
			req.transaction = SGConstants.T_IMAGE; //分享图片
			req.mediaMessage = msg;
			req.scene = SendMessageToSG.Req.SGSceneSession; //代表分享到会话列表
			//调用api接口发送数据到闲聊
			m_api.sendReq(req);
		}
		else if(type.equals("app"))
		{
			//初始化一个SGGameObject对象，设置所分享的图片内容
			SGGameObject gameObject = new SGGameObject();
			if(imageFrom == 2)
				gameObject.imageData = getBytesFromAsset(ctx, image);
			else
				gameObject.imagePath = image;
			gameObject.roomId = " ";
			gameObject.roomToken = "token";
			//可以自定义邀请应用的下载链接，也可以不填，不填会默认使用应用申请appid时填写的链接
			gameObject.androidDownloadUrl = url;
			gameObject.iOSDownloadUrl = url;
			//用SGGameObject对象初始化一个SGMediaMessage对象
			SGMediaMessage msg = new SGMediaMessage();
			msg.mediaObject = gameObject;
			msg.title = title;
			msg.description = text;
			//构造一个Req
			SendMessageToSG.Req req = new SendMessageToSG.Req();
			req.transaction = SGConstants.T_GAME;
			req.mediaMessage = msg;
			req.scene = SendMessageToSG.Req.SGSceneSession; //代表分享到会话列表
			//调用api接口发送数据到闲聊
			m_api.sendReq(req);
		}
		else
		{
			return "invalid type " + type;
		}
		return "success";
	}

	public static native void nativeNotifyShare(int callback, String res);
	public static void notifyShareResult(String res)
	{
		if(s_shareCallback > 0)
		{
			nativeNotifyShare(s_shareCallback, res);
			s_shareCallback = 0;
		}
	}

	public static byte[] getBytesFromAsset(Context ctx, String path)
	{
		InputStream is;
		byte[] ret = null;
		try
		{
			is = ctx.getAssets().open(path);
			int size = is.available();
			ret = new byte[size];
			is.read(ret);
			is.close();
		} catch(Exception e)
		{
			ret = null;
			e.printStackTrace();
		}
		return ret;
	}
}
