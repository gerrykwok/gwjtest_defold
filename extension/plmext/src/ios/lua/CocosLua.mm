#if defined(DM_PLATFORM_IOS)

#import "CocosLua.h"
#import "../../../cocosext.h"
#import "../../../ios/CCLuaBridge.h"

@implementation CocosLua

+(void) callLuaFunction:(int)funcId res:(const char *)str
{
    // 1. 将引用 ID 对应的 Lua function 放入 Lua stack
    LuaBridge::pushLuaFunctionById(funcId);
    // 2. 将需要传递给 Lua function 的参数放入 Lua stack
    LuaBridge::getStack()->pushString(str);
    // 3. 执行 Lua function
    LuaBridge::getStack()->executeFunction(1);
    // 4. 释放引用 ID
    LuaBridge::releaseLuaFunctionById(funcId);
}

+(void) callGlocalLuaFunction:(const char*)funcName res:(const char *)str
{
	lua_State *state = LuaBridge::getStack()->getLuaState();
	lua_getglobal(state, funcName);       /* query function by name, stack: function */
	if (!lua_isfunction(state, -1))
	{
		COCOSEXT_printf("[LUA ERROR] name '%s' does not represent a Lua function", funcName);
		lua_pop(state, 1);
	}
	else
	{
		LuaBridge::getStack()->pushString(str);
		LuaBridge::getStack()->executeFunction(1);
	}
}

@end

#endif
