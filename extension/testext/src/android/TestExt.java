package abc.def.ghi;

import org.json.JSONObject;
import android.content.Context;
import android.util.Log;

class TestExt
{
	public static final String TAG = "testext";
	public static String theFunc(Context ctx, JSONObject json)
	{
		Log.d(TAG, "gwjgwj,theFunc,json=" + json);
		return "result";
	}
}
