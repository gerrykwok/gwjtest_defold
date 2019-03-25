#pragma once

#include <dmsdk/sdk.h>

void logdog_onAppInit();
void logdog_doLog(const char *message);

int logdog_lualog(lua_State *L);
