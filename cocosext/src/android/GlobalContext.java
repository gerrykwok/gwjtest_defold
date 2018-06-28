package com.xishanju.defold.cocosext;

import android.content.Context;

public class GlobalContext
{
	public static Context s_context;
	public static void setActivity(Context ctx)
	{
		GlobalContext.s_context = ctx;
	}
}
