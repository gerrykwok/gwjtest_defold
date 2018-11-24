#include "CCLuaStack.h"
#include "cocosext.h"
#include "tolua_fix.h"

static LuaStack g_luastack;

LuaStack* LuaStack::getInstance()
{
	return &g_luastack;
}

bool LuaStack::initWithLuaState(lua_State *L)
{
	_state = L;
	return true;
}

void LuaStack::removeScriptHandler(int nHandler)
{
	toluafix_remove_function_by_refid(_state, nHandler);
}

int LuaStack::executeGlobalFunction(const char* functionName)
{
	lua_getglobal(_state, functionName);       /* query function by name, stack: function */
	if (!lua_isfunction(_state, -1))
	{
		dmLogError("[LUA ERROR] name '%s' does not represent a Lua function", functionName);
		lua_pop(_state, 1);
		return 0;
	}
	return executeFunction(0);
}

void LuaStack::clean(void)
{
	lua_settop(_state, 0);
}

void LuaStack::pushInt(int intValue)
{
	lua_pushinteger(_state, intValue);
}

void LuaStack::pushFloat(float floatValue)
{
	lua_pushnumber(_state, floatValue);
}

void LuaStack::pushLong(long longValue)
{
	lua_pushnumber(_state, longValue);
}

void LuaStack::pushBoolean(bool boolValue)
{
	lua_pushboolean(_state, boolValue);
}

void LuaStack::pushString(const char* stringValue)
{
	lua_pushstring(_state, stringValue);
}

void LuaStack::pushString(const char* stringValue, int length)
{
	lua_pushlstring(_state, stringValue, length);
}

void LuaStack::pushNil(void)
{
	lua_pushnil(_state);
}

bool LuaStack::pushFunctionByHandler(int nHandler)
{
	toluafix_get_function_by_refid(_state, nHandler);                  /* L: ... func */
	if (!lua_isfunction(_state, -1))
	{
		dmLogError("[LUA ERROR] function refid '%d' does not reference a Lua function", nHandler);
		lua_pop(_state, 1);
		return false;
	}
	return true;
}

int LuaStack::executeFunction(int numArgs)
{
	int functionIndex = -(numArgs + 1);
	if (!lua_isfunction(_state, functionIndex))
	{
		dmLogError("value at stack [%d] is not function", functionIndex);
		lua_pop(_state, numArgs + 1); // remove function and arguments
		return 0;
	}

	int traceback = 0;
	lua_getglobal(_state, "__G__TRACKBACK__");                         /* L: ... func arg1 arg2 ... G */
	if (!lua_isfunction(_state, -1))
	{
		lua_pop(_state, 1);                                            /* L: ... func arg1 arg2 ... */
	}
	else
	{
		lua_insert(_state, functionIndex - 1);                         /* L: ... G func arg1 arg2 ... */
		traceback = functionIndex - 1;
	}

	int error = 0;
	++_callFromLua;
	error = lua_pcall(_state, numArgs, 1, traceback);                  /* L: ... [G] ret */
	--_callFromLua;
	if (error)
	{
		if (traceback == 0)
		{
			dmLogError("[LUA ERROR] %s", lua_tostring(_state, - 1));        /* L: ... error */
			lua_pop(_state, 1); // remove error message from stack
		}
		else                                                            /* L: ... G error */
		{
			lua_pop(_state, 2); // remove __G__TRACKBACK__ and error message from stack
		}
		return 0;
	}

	// get return value
	int ret = 0;
	if (lua_isnumber(_state, -1))
	{
		ret = (int)lua_tointeger(_state, -1);
	}
	else if (lua_isboolean(_state, -1))
	{
		ret = (int)lua_toboolean(_state, -1);
	}
	// remove return value from stack
	lua_pop(_state, 1);                                                /* L: ... [G] */

	if (traceback)
	{
		lua_pop(_state, 1); // remove __G__TRACKBACK__ from stack      /* L: ... */
	}

	return ret;
}

int LuaStack::executeFunctionByHandler(int nHandler, int numArgs)
{
	int ret = 0;
	if (pushFunctionByHandler(nHandler))                                /* L: ... arg1 arg2 ... func */
	{
		if (numArgs > 0)
		{
			lua_insert(_state, -(numArgs + 1));                        /* L: ... func arg1 arg2 ... */
		}
		ret = executeFunction(numArgs);
	}
	lua_settop(_state, 0);
	return ret;
}
