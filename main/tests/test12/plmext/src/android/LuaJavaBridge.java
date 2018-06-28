package com.xishanju.plm.plmext;

import android.content.Context;

public class LuaJavaBridge
{
	public static native int callLuaFunctionWithString(int luaFunctionId, String value);
	public static native int callLuaGlobalFunctionWithString(String luaFunctionName, String value);
	public static native int retainLuaFunction(int luaFunctionId);
	public static native int releaseLuaFunction(int luaFunctionId);

	static Context s_context;
	public static void setActivity(Context ctx)
	{
		LuaJavaBridge.s_context = ctx;
	}
}
