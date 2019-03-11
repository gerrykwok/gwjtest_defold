#pragma once

#include <dmsdk/sdk.h>
#include <functional>
#include <vector>
#include <string>
#include <map>
#include <extension/luajavaoc/src/luacallback.h>

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
//for other extension
void httpreq_request(const char *url, const char *method,
	std::vector<std::string> headers, const char *postData, std::map<std::string, std::string> options,
	std::function<void(int status, void* respData, size_t respDataSize, std::vector<std::string> &headers)> callback);
