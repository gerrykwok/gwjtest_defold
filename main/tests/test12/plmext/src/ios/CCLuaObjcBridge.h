
#ifndef __LUA_OBJC_BRIDGE_H_
#define __LUA_OBJC_BRIDGE_H_

#include "CCLuaBridge.h"

class LuaObjcBridge : public LuaBridge
{
public:
    static void luaopen_luaoc(lua_State *L);
    
protected:
    static int callObjcStaticMethod(lua_State *L);
    
    static void pushValue(lua_State *L, void *val);
};

#endif // __LUA_OBJC_BRIDGE_H_
