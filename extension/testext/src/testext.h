#pragma once

#include <dmsdk/sdk.h>
#include <string>
#include <functional>
#include <extension/luajavaoc/src/luacallback.h>

int test_oc(lua_State *L);
int test_callnative(lua_State *L);
int test_printMacro(lua_State *L);
int test_centerWindow(lua_State *L);
