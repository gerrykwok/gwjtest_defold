package com.xishanju.plm.plmext;

public class LuaJavaBridge
{
	public static native int callLuaFunctionWithString(int luaFunctionId, String value);
	public static native int releaseLuaFunction(int luaFunctionId);
}
