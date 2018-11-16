package com.xishanju.defold.cocosext;

import android.content.Context;
import android.util.Log;

public class GlobalContext
{
	public static Context s_context;
	public static void setActivity(Context ctx)
	{
		Log.d("gwjgwj", "set Activity:" + ctx);
		GlobalContext.s_context = ctx;
	}
}
