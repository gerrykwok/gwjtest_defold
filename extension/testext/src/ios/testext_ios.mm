#if defined(DM_PLATFORM_IOS)

#include "../testext.h"

int test_oc(lua_State *L)
{
	const char *clazz = lua_tostring(L, -3);
	const char *method = lua_tostring(L, -2);
	std::string param = ext_jsonFromLuaTable(L, -1);
	bool ok;
	std::string ret = ext_callOcStaticMethod(clazz, method, param.c_str(), &ok);
	lua_pushboolean(L, ok);
	lua_pushstring(L, ret.c_str());
	return 2;
}

int test_takephoto(lua_State *L)
{
	return ext_callNativeStaticMethod("TakePhoto", "takePicture", L, -1);
}

int test_callnative(lua_State *L)
{
	return ext_callNativeStaticMethod("MyClass", "test1", L, -1);
}

int test_centerWindow(lua_State *L)
{
	return 0;
}

#endif
