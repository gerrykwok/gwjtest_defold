#pragma once

#include <dmsdk/sdk.h>

typedef int LUA_FUNCTION;

void plm_take_photo(const char *localPath, int luaCallback);
void plm_get_local_picture(const char *localPath, int luaCallback);
