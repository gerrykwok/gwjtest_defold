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
