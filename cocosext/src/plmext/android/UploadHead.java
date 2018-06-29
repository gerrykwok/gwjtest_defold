package com.xishanju.plm.plmext;

import java.io.File;
import java.util.Locale;
import java.util.HashMap;
import java.util.Map;

import android.util.Log;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import com.qiniu.android.http.ResponseInfo;
import com.qiniu.android.storage.UpCompletionHandler;
import com.qiniu.android.storage.UploadManager;
import com.qiniu.android.storage.UploadOptions;

import com.xishanju.defold.cocosext.*;

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

public class UploadHead
{
	private static final int AVATAR_UPLOAD_RES_SUCCES = 1;
	private static final int AVATAR_UPLOAD_RES_FAIL = 0;

	public static void uploadHead(String token, String path, String key, final int luaCallbackFunction)
	{
		Log.i(plmext.TAG, String.format(Locale.US, "token=%s,path=%s,key=%s,callback=%d", token, path, key, luaCallbackFunction));
		if(key == "") key = null;
		Map<String, String> params = new HashMap<String, String>();
		params.put("x:foo", "fooval");
		final UploadOptions opt = new UploadOptions(params, null, true, null, null);
		UploadManager uploadManager = new UploadManager();
		uploadManager.put(path, key, token, new UpCompletionHandler() {
			public void complete(final String key, final ResponseInfo rinfo, JSONObject response) {
				Log.i(plmext.TAG, "status:" + rinfo.statusCode + ",key=" + key + "info=" + rinfo + "res=" + response);
				int res = UploadHead.AVATAR_UPLOAD_RES_FAIL;
				if (200 == rinfo.statusCode) {
					res = UploadHead.AVATAR_UPLOAD_RES_SUCCES;
				}
				UploadHead.notifyAvatarUploadResult(luaCallbackFunction, res);
			}
		}, opt);
	}

	private static void notifyAvatarUploadResult(final int callback, final int res)
	{
		if (callback >= 0) {
			LuaJavaBridge.callLuaFunctionWithString(callback, "" + res);
			LuaJavaBridge.releaseLuaFunction(callback);
		}
	}
}
