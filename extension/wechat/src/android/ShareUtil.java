package com.xishanju.plm.wechat;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Locale;

import org.json.JSONObject;
import android.util.Log;
import android.content.Context;
import android.content.Intent;
import android.content.ComponentName;
import android.net.Uri;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.BitmapFactory;

import com.tencent.mm.opensdk.modelmsg.SendMessageToWX;
import com.tencent.mm.opensdk.modelmsg.WXImageObject;
import com.tencent.mm.opensdk.modelmsg.WXMediaMessage;
import com.tencent.mm.opensdk.modelmsg.WXMusicObject;
import com.tencent.mm.opensdk.modelmsg.WXTextObject;
import com.tencent.mm.opensdk.modelmsg.WXVideoObject;
import com.tencent.mm.opensdk.modelmsg.WXWebpageObject;
import com.tencent.mm.opensdk.openapi.IWXAPI;
import com.tencent.mm.opensdk.openapi.WXAPIFactory;

public class ShareUtil
{
	public static final String TAG = "shareutil";
	public static String shareWithIntent(Context ctx, String data)
	{
		Log.d(TAG, "gwjgwj,shareWithIntent:" + data);
		String pkgName = "";
		String clsName = "";
		String mimeType = "";
		String streamPath = "";
		HashMap<String, String> extras = new HashMap<String, String>();

		try
		{
			JSONObject json = new JSONObject(data);
			if(json.has("pkgName")) pkgName = json.getString("pkgName");
			if(json.has("clsName")) clsName = json.getString("clsName");
			if(json.has("mimeType")) mimeType = json.getString("mimeType");
			if(json.has("streamPath")) streamPath = json.getString("streamPath");
			if(json.has("extras"))
			{
				JSONObject extrasJson = json.getJSONObject("extras");
				Iterator<String> it = extrasJson.keys();
				while(it.hasNext())
				{
					String key = it.next();
					String value = extrasJson.getString(key);
					extras.put(key, value);
				}
			}
		} catch(Exception e)
		{
			e.printStackTrace();
		}

		if(pkgName.equals("")) return "package empty";
		if(clsName.equals("")) return "class empty";
		
//		Log.d("gwjgwj", String.format(Locale.US, "gwjgwj,pkgName=%s,clsName=%s,streamPath=%s,mimeType=%s", pkgName, clsName, streamPath, mimeType));

		boolean success = false;
		final Intent intent = new Intent(Intent.ACTION_SEND);
		intent.setComponent(new ComponentName(pkgName, clsName));
		intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		//mime type
		if(mimeType != null && !mimeType.equals(""))
		{
			intent.setType(mimeType);
		}
		//stream
		if(streamPath != null && !streamPath.equals(""))
		{
			intent.putExtra(Intent.EXTRA_STREAM, Uri.fromFile(new File(streamPath)));
		}
		//extra
		for(String key : extras.keySet())
		{
			String value = extras.get(key);
			Log.d(TAG, "gwjgwj,extra,"+key+"="+value);
			intent.putExtra(key, value);
		}
		try
		{
			ctx.startActivity(Intent.createChooser(intent, "分享"));
			success = true;
		} catch(Exception e)
		{
			e.printStackTrace();
		}
		return success ? "success" : "failed";
	}

	public static int s_shareCallback = 0;
	public static String shareWithSDK(Context ctx, String data)
	{
		IWXAPI api = PlatformWechat.s_wxApi;
		boolean success = false;

		String shareType = "";
		int scene = -1;
		String text = "";
		String title = "";
		String description = "";
		String image = "";
		int imageFrom = 1;//1=全路径图片，2=包内图片
		String url = "";
		int callback = 0;
		try
		{
			JSONObject json = new JSONObject(data);
			if(json.has("type")) shareType = json.getString("type");
			if(json.has("scene")) scene = json.getInt("scene");
			if(json.has("text")) text = json.getString("text");
			if(json.has("image")) image = json.getString("image");
			if(json.has("imageFrom")) imageFrom = json.getInt("imageFrom");
			if(json.has("title")) title = json.getString("title");
			if(json.has("description")) description = json.getString("description");
			if(json.has("url")) url = json.getString("url");
			if(json.has("callback")) callback = json.getInt("callback");
		} catch(Exception e)
		{
			e.printStackTrace();
		}
		s_shareCallback = callback;
		if(shareType.equals("text"))
		{
			//初始化一个 WXTextObject 对象，填写分享的文本内容
			WXTextObject textObj = new WXTextObject();
			textObj.text = text;

			//用 WXTextObject 对象初始化一个 WXMediaMessage 对象
			WXMediaMessage msg = new WXMediaMessage();
			msg.mediaObject = textObj;
			msg.title = title;//发送文本类型的消息时，title字段不起作用
			msg.description = description;

			SendMessageToWX.Req req = new SendMessageToWX.Req();
			req.transaction = buildTransaction("text");
			req.message = msg;
			req.scene = scene;
			//调用api接口，发送数据到微信
			success = api.sendReq(req);
		}
		else if(shareType.equals("image"))
		{
			//初始化 WXImageObject 和 WXMediaMessage 对象
			WXImageObject imgObj = new WXImageObject();
			if(imageFrom == 2)
			{
				imgObj.imageData = getBytesFromAsset(ctx, image);
			}
			else
			{
				imgObj.imagePath = image;
			}
			WXMediaMessage msg = new WXMediaMessage();
			msg.mediaObject = imgObj;

			//构造一个Req
			SendMessageToWX.Req req = new SendMessageToWX.Req();
			req.transaction = buildTransaction("img");
			req.message = msg;
			req.scene = scene;
			//调用api接口，发送数据到微信
			success = api.sendReq(req);
		}
		else if(shareType.equals("music"))
		{
			//初始化一个WXMusicObject，填写url
			WXMusicObject music = new WXMusicObject();
			music.musicUrl = url;

			//用 WXMusicObject 对象初始化一个 WXMediaMessage 对象
			WXMediaMessage msg = new WXMediaMessage();
			msg.mediaObject = music;
			msg.title = title;
			msg.description = description;
			//设置音乐缩略图
			if(imageFrom == 2)
			{
				msg.thumbData = getBytesFromAsset(ctx, image);
			}
			else
			{
				Bitmap thumbBmp;
				thumbBmp = BitmapFactory.decodeFile(image);
				msg.thumbData = bmpToByteArray(thumbBmp, true);
				//msg.setThumbImage(thumbBmp);//用该方法分享出来的缩略图没有alpha
			}

			//构造一个Req
			SendMessageToWX.Req req = new SendMessageToWX.Req();
			req.transaction = buildTransaction("music");
			req.message = msg;
			req.scene = scene;
			//调用api接口，发送数据到微信
			success = api.sendReq(req);
		}
		else if(shareType.equals("video"))
		{
			//初始化一个WXVideoObject，填写url
			WXVideoObject video = new WXVideoObject();
			video.videoUrl = url;

			//用 WXVideoObject 对象初始化一个 WXMediaMessage 对象
			WXMediaMessage msg = new WXMediaMessage(video);
			msg.title = title;
			msg.description = description;

			if(imageFrom == 2)
			{
				msg.thumbData = getBytesFromAsset(ctx, image);
			}
			else
			{
				Bitmap thumbBmp;
				thumbBmp = BitmapFactory.decodeFile(image);
				msg.thumbData = bmpToByteArray(thumbBmp, true);
				//msg.setThumbImage(thumbBmp);//用该方法分享出来的缩略图没有alpha
			}

			//构造一个Req
			SendMessageToWX.Req req = new SendMessageToWX.Req();
			req.transaction = buildTransaction("video");
			req.message = msg;
			req.scene = scene;

			//调用api接口，发送数据到微信
			success = api.sendReq(req);
		}
		else if(shareType.equals("webpage"))
		{
			//初始化一个WXWebpageObject，填写url
			WXWebpageObject webpage = new WXWebpageObject();
			webpage.webpageUrl = url;

			//用 WXWebpageObject 对象初始化一个 WXMediaMessage 对象
			WXMediaMessage msg = new WXMediaMessage(webpage);
			msg.title = title;
			msg.description = description;

			if(imageFrom == 2)
			{
				msg.thumbData = getBytesFromAsset(ctx, image);
			}
			else
			{
				Bitmap thumbBmp;
				thumbBmp = BitmapFactory.decodeFile(image);
				msg.thumbData = bmpToByteArray(thumbBmp, true);
				//msg.setThumbImage(thumbBmp);//用该方法分享出来的缩略图没有alpha
			}

			//构造一个Req
			SendMessageToWX.Req req = new SendMessageToWX.Req();
			req.transaction = buildTransaction("webpage");
			req.message = msg;
			req.scene = scene;

			//调用api接口，发送数据到微信
			success = api.sendReq(req);
		}
		else
		{
			return "invalid share type:" + shareType;
		}
		return success ? "success" : "failed";
	}

	public static native void nativeNotifyShare(int callback, String result);
	public static void notifyShareResult(String result)
	{
		if(s_shareCallback > 0)
		{
			nativeNotifyShare(s_shareCallback, result);
			s_shareCallback = 0;
		}
	}

	public static String buildTransaction(final String type) {
		return (type == null) ? String.valueOf(System.currentTimeMillis()) : type + System.currentTimeMillis();
	}
	public static byte[] bmpToByteArray(final Bitmap bmp, final boolean needRecycle) {
		ByteArrayOutputStream output = new ByteArrayOutputStream();
		bmp.compress(CompressFormat.PNG, 100, output);
		if (needRecycle) {
			bmp.recycle();
		}
		
		byte[] result = output.toByteArray();
		try {
			output.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return result;
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
