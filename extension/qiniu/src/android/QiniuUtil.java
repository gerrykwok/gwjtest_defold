package com.xishanju.plm.qiniu;

import java.util.Locale;
import java.util.HashMap;
import java.util.Map;

import android.content.Context;
import android.util.Log;

import org.json.JSONObject;

import com.qiniu.android.http.ResponseInfo;
import com.qiniu.android.storage.UpCompletionHandler;
import com.qiniu.android.storage.UploadManager;
import com.qiniu.android.storage.UploadOptions;

//        //————http上传,指定zone的具体区域——
//        //Zone.zone0:华东
//        //Zone.zone1:华北
//        //Zone.zone2:华南
//        //Zone.zoneNa0:北美
//        //———http上传，自动识别上传区域——
//        //Zone.httpAutoZone
//        //———https上传，自动识别上传区域——
//        //Zone.httpsAutoZone
//
//        Configuration config = new Configuration.Builder()
//                .chunkSize(256 * 1024)  //分片上传时，每片的大小。 默认256K
//                .putThreshhold(512 * 1024)  // 启用分片上传阀值。默认512K
//                .connectTimeout(10) // 链接超时。默认10秒
//                .responseTimeout(60) // 服务器响应超时。默认60秒
//                .recorder(null)  // recorder分片上传时，已上传片记录器。默认null
//                .recorder(null, null)  // keyGen 分片上传时，生成标识符，用于片记录器区分是那个文件的上传记录
//                .zone(Zone.zone2) // 设置区域，指定不同区域的上传域名、备用域名、备用IP。
//                .build();
//        this.uploadManager = new UploadManager(config);

public class QiniuUtil
{
	private static final String TAG = "QiniuUtil";

	public static String uploadFile(Context ctx, JSONObject params)
	{
		String token = "", path = "", key = "";
		int luaCallbackFunction = 0;
		try
		{
			if(params.has("token")) token = params.getString("token");
			if(params.has("path")) path = params.getString("path");
			if(params.has("key")) key = params.getString("key");
			if(params.has("callback")) luaCallbackFunction = params.getInt("callback");
		} catch(Exception e)
		{
			e.printStackTrace();
		}

		Log.i(TAG, String.format(Locale.US, "token=%s,path=%s,key=%s,callback=%d", token, path, key, luaCallbackFunction));
		if(key == "") key = null;
		final int callback = luaCallbackFunction;
		Map<String, String> param = new HashMap<String, String>();
		//param.put("x:foo", "fooval");
		final UploadOptions opt = new UploadOptions(param, null, true, null, null);
		UploadManager uploadManager = new UploadManager();
		uploadManager.put(path, key, token, new UpCompletionHandler() {
			public void complete(final String key, final ResponseInfo rinfo, JSONObject response) {
				Log.i(TAG, "status:" + rinfo.statusCode + ",key=" + key + ",info=" + rinfo + ",res=" + response);
				String str = "";
				if(rinfo.statusCode == 200)
				{
					str = "{\"result\":0}";
				}
				else if(rinfo.statusCode == 614)//目标资源已存在
				{
					str = String.format(Locale.US, "{\"result\":1,\"errMsg\":\"%s\"}", rinfo.error);
				}
				else
				{
					str = String.format(Locale.US, "{\"result\":2,\"errMsg\":\"%s\"}", rinfo.error);
				}
				if(callback > 0)
				{
					nativeNotifyUploadResult(callback, str);
				}
			}
		}, opt);
		return "";
	}

	private static native void nativeNotifyUploadResult(int callback, String res);
}
