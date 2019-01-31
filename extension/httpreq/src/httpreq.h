#pragma once

#include <dmsdk/sdk.h>
#include <functional>

typedef int LUA_FUNCTION;
typedef int LUA_STRING;
typedef struct
{
	void *ptr;
	size_t size;
} LUA_BUFFER;

void httpreq_printCurlVersion();
//something like "192.168.2.2:8888"
void httpreq_setCurlProxy(const char *proxy);

extern unsigned int ext_scheduleUpdate(const std::function<void(void)> &func);
extern void ext_unscheduleUpdate(unsigned int entryId);
extern void ext_unregisterLuaCallback(int callbackId);
extern void ext_invokeLuaCallbackWithString(int callbackId, const char *value);
extern void ext_performInUpdateThread(const std::function<void(void)> &func);
