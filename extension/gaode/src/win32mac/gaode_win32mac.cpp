#if defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_OSX)

#include "../gaode.h"
#include "../rapidjson/document.h"

void gaode_onAppInit(const char *environment)
{
}

int gaode_locationInit(lua_State *L)
{
	return 0;
}

static int g_callback = 0;
static int g_tickCount = 0;
static std::string g_ip;
static bool g_requestingIP = false;
static int g_retryCount = 2;
static int g_idSchedule = 0;
int gaode_locationStart(lua_State *L)
{
	gaode_locationStop(L);
	
	int callback = 0;
	std::string paramsJson = ext_jsonFromLuaTable(L, -1);
	const char *params = paramsJson.c_str();
//	dmLogInfo("params=%s", params);
	rapidjson::Document doc;
	doc.Parse<0>(params);
	rapidjson::Value val;
	if(doc.HasMember("callback"))
	{
		val = doc["callback"];
		callback = val.GetInt();
		dmLogInfo("callback = %d", callback);
	}
	g_callback = callback;

	g_tickCount = 0;
	g_idSchedule = ext_scheduleUpdate([=](){
		g_tickCount++;
		if(g_ip.length() <= 0 && !g_requestingIP && g_retryCount > 0)
		{
			g_retryCount--;
			g_requestingIP = true;
			const char *url = "http://api.ip138.com/query/?datatype=jsonp";
			std::vector<std::string> headers;
			std::map<std::string, std::string> options;
			headers.push_back("token:21f804323de35f540b5aea2cb3c9efb5");
			dmLogInfo("request for ip");
			httpreq_request(url, "get", headers, NULL, options, [=](int status, void* respData, size_t respDataSize, std::vector<std::string> &headers) {
				if(status == 200)
				{
					const char *respString = (const char *)respData;
					dmLogInfo("responseString=%s", respString);
					rapidjson::Document doc;
					doc.Parse<0>(respString);
					const char *ret = doc["ret"].GetString();
					if(strcmp(ret, "ok") == 0)
					{
						g_ip = doc["ip"].GetString();
					}
				}
				g_requestingIP = false;
			});
		}
		if(g_tickCount >= 60)
		{
			g_tickCount -= 60;
			if(g_ip.length() > 0)
			{
//				dmLogInfo("request location of ip %s", g_ip.c_str());
				gaode_requestLocationByIp(callback);
			}
		}
	});
	return 0;
}

void gaode_requestLocationByIp(int callback)
{
	char url[256];
	std::vector<std::string> headers;
	std::map<std::string, std::string> options;
	sprintf(url, "http://restapi.amap.com/v3/ip?ip=%s&output=json&key=%s", g_ip.c_str(), GAODE_APIKEY_WEB);
	httpreq_request(url, "post", headers, NULL, options, [=](int status, void* respData, size_t respDataSize, std::vector<std::string> &headers) {
		const char *respString = (const char *)respData;
//		dmLogInfo("gaode:status=%d,respData=%s", status, respString);
//		dmLogInfo("gaode:headers:");

		if(status == 200)
		{
			rapidjson::Document doc;
			doc.Parse<0>(respString);
			const char *sStatus = doc["status"].GetString();
			if(strcmp(sStatus, "1") == 0)
			{
				const char *province;
				const char *city;
				const char *rectangle;
				float longitude1,latitude1;
				float longitude2,latitude2;
				float longitude,latitude;
				province = doc["province"].GetString();
				city = doc["city"].GetString();
				rectangle = doc["rectangle"].GetString();
				sscanf(rectangle, "%f,%f;%f,%f", &longitude1, &latitude1, &longitude2, &latitude2);
				longitude = (longitude1+longitude2)/2;
				latitude = (latitude1+latitude2)/2;
				char res[256];
				sprintf(res, R"raw_string({"result":"%s","longitude":%f,"latitude":%f,"province":"%s","city":"%s"})raw_string",
					"success", longitude, latitude, province, city);
//				dmLogInfo("res=%s", res);
				if(callback > 0)
				{
					ext_invokeLuaCallbackWithString(callback, res);
				}
			}
		}
	});
}

int gaode_locationStop(lua_State *L)
{
	if(g_callback > 0)
	{
		ext_unregisterLuaCallback(g_callback);
		g_callback = 0;
	}
	if(g_idSchedule != 0)
	{
		ext_unscheduleUpdate(g_idSchedule);
		g_idSchedule = 0;
	}
	return 0;
}

int gaode_getAuthorizationStatus(lua_State *L)
{
	lua_pushinteger(L, 2);
	return 1;
}

int gaode_jumpLocationSetting(lua_State *L)
{
	return 0;
}

#endif