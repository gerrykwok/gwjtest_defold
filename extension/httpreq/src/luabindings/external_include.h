#pragma once

#include <string>
#include <unordered_map>

extern int ext_registerLuaCallback(lua_State* L, int index);

//tolua++.h

#ifndef TOLUA_API
#define TOLUA_API extern
#endif

#define tolua_pushcppstring(x,y)                tolua_pushstring(x,y.c_str())

#ifdef __cplusplus
extern "C" {
#endif

struct tolua_Error
{
	int index;
	int array;
	const char* type;
};
typedef struct tolua_Error tolua_Error;

TOLUA_API void tolua_open (lua_State* L);
TOLUA_API void* tolua_tousertype (lua_State* L, int narg, void* def);
TOLUA_API void tolua_error (lua_State* L, const char* msg, tolua_Error* err);
TOLUA_API void tolua_usertype (lua_State* L, const char* type);
TOLUA_API void tolua_beginmodule (lua_State* L, const char* name);
TOLUA_API void tolua_endmodule (lua_State* L);
TOLUA_API void tolua_module (lua_State* L, const char* name, int hasvar);
TOLUA_API void tolua_cclass (lua_State* L, const char* lname, const char* name, const char* base, lua_CFunction col);
TOLUA_API void tolua_function (lua_State* L, const char* name, lua_CFunction func);

TOLUA_API void tolua_pushvalue (lua_State* L, int lo);
TOLUA_API void tolua_pushboolean (lua_State* L, int value);
TOLUA_API void tolua_pushnumber (lua_State* L, lua_Number value);
TOLUA_API void tolua_pushstring (lua_State* L, const char* value);
TOLUA_API void tolua_pushuserdata (lua_State* L, void* value);
TOLUA_API void tolua_pushusertype (lua_State* L, void* value, const char* type);

#ifdef __cplusplus
}
#endif

//LuaBasicConversions.h

extern std::unordered_map<std::string, std::string>  g_luaType;
extern std::unordered_map<std::string, std::string>  g_typeCast;

//Because all override functions wouldn't be bound,so we must use `typeid` to get the real class name
template <class T>
const char* getLuaTypeName(T* ret,const char* type)
{
	if (nullptr != ret)
	{
		std::string hashName = typeid(*ret).name();
		auto iter =  g_luaType.find(hashName);
		if(g_luaType.end() != iter)
		{
			return iter->second.c_str();
		}
		else
		{
			return type;
		}
	}

	return nullptr;
}

template <class T>
void object_to_luaval(lua_State* L,const char* type, T* ret)
{
	if(nullptr != ret)
	{
//		if (std::is_base_of<cocos2d::Ref, T>::value)
//		{
//			// use c style cast, T may not polymorphic
//			cocos2d::Ref* dynObject = (cocos2d::Ref*)(ret);
//			int ID = (int)(dynObject->_ID) ;
//			int* luaID = &(dynObject->_luaID);
//			toluafix_pushusertype_ccobject(L,ID, luaID, (void*)ret,type);
//		}
//		else
//		{
			tolua_pushusertype(L,(void*)ret,getLuaTypeName(ret, type));
//		}
	}
	else
	{
		lua_pushnil(L);
	}
}

extern bool luaval_to_int32(lua_State* L,int lo,int* outValue, const char* funcName = "");
extern bool luaval_to_uint32(lua_State* L, int lo, unsigned int* outValue, const char* funcName = "");
extern bool luaval_to_std_string(lua_State* L, int lo, std::string* outValue, const char* funcName = "");
