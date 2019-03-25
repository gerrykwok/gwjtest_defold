#include "misc.h"

static int g_appEventListener = 0;

static const char *misc_getLuaTypeDesc(lua_State *L, int index)
{
	int t = lua_type(L, index);
	switch(t)
	{
	case LUA_TNIL: return "nil";
	case LUA_TBOOLEAN: return "boolean";
	case LUA_TLIGHTUSERDATA: return "lightuserdata";
	case LUA_TNUMBER: return "number";
	case LUA_TSTRING: return "string";
	case LUA_TTABLE: return "table";
	case LUA_TFUNCTION: return "function";
	case LUA_TUSERDATA: return "userdata";
	case LUA_TTHREAD: return "thread";
	default: return "unknown";
	}
}

int misc_setAppEventListener(lua_State *L)
{
	int top = lua_gettop(L);
//	dmLogInfo("top=%d", top);
	if(top <= 0)
	{
		dmLogError("expecting param");
		return 0;
	}
	if(!lua_isfunction(L, -1))
	{
		const char *t = misc_getLuaTypeDesc(L, -1);
		dmLogError("param 1 is '%s', expecting function", t);
		return 0;
	}
	if(g_appEventListener > 0)
	{
		ext_unregisterLuaCallback(g_appEventListener);
		g_appEventListener = 0;
	}
	g_appEventListener = ext_registerLuaCallback(L, -1);
	return 0;
}

void misc_invokeAppEventListener(const char *res)
{
	if(g_appEventListener <= 0) return;
	ext_invokeLuaCallbackWithString(g_appEventListener, res);
}

int misc_vibrate(lua_State *L)
{
	int top = lua_gettop(L);
	if(top <= 0)
	{
		dmLogError("expecting param");
		return 0;
	}
	if(!lua_isnumber(L, -1))
	{
		const char *t = misc_getLuaTypeDesc(L, -1);
		dmLogError("param 1 is '%s', expecting number", t);
		return 0;
	}

	float second = lua_tonumber(L, -1);
	misc_doVibrate(second);
	return 0;
}
