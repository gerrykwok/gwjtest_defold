#pragma once

#include <dmsdk/sdk.h>
#include <extension/luajavaoc/src/luacallback.h>

void takephoto_onAppInit(const char *environment);

int takephoto_get_photo(lua_State *L);
