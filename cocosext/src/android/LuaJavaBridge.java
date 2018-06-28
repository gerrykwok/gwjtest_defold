package com.xishanju.defold.cocosext;

public class LuaJavaBridge
{
	public static native int callLuaFunctionWithString(int luaFunctionId, String value);
	public static native int callLuaGlobalFunctionWithString(String luaFunctionName, String value);
	public static native int retainLuaFunction(int luaFunctionId);
	public static native int releaseLuaFunction(int luaFunctionId);
}
