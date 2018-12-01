#pragma once

#include <string>

int test_oc(lua_State *L);

extern std::string ext_jsonFromLuaTable(lua_State *L, int index);
extern std::string ext_callOcStaticMethod(const char *clazz, const char *method, const char *params, bool *ok);
