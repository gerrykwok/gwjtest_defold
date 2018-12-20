#include "../agora.h"
#include "MyAgora.h"
#if defined(DM_PLATFORM_ANDROID)
#include <dlfcn.h>
#endif

static IRtcEngine *g_agoraEngine;
static MyAgoraEventHandler g_agoraEventHandler;
static int g_luaCallback = 0;
typedef IRtcEngine* (AGORA_CALL *PFN_createAgoraRtcEngine)();

void agora_initAgora()
{
#if defined(DM_PLATFORM_ANDROID)
	const char *soName = "libagora-rtc-sdk-jni.so";
	void* hDll = dlopen(soName, RTLD_NOW);
//	dmLogInfo("gwjgwj,agora library handle:0x%x", hDll);
	if(hDll)
	{
		PFN_createAgoraRtcEngine funcCreateEngine = (PFN_createAgoraRtcEngine)dlsym(hDll, "createAgoraRtcEngine");
		g_agoraEngine = funcCreateEngine();
//		dmLogInfo("gwjgwj,engine=0x%x", g_agoraEngine);
		RtcEngineContext ctx;
		ctx.eventHandler = &g_agoraEventHandler;
		ctx.appId = AGORA_APPID;
//		dmLogInfo("gwjgwj,agora debug 1");
		g_agoraEngine->initialize(ctx);
//		dmLogInfo("gwjgwj,agora debug 1000");
	}
	else
	{
		dmLogError("cannot load %s", soName);
	}
#else
	g_agoraEngine = createAgoraRtcEngine();
	RtcEngineContext ctx;
	ctx.eventHandler = &g_agoraEventHandler;
	ctx.appId = AGORA_APPID;
	g_agoraEngine->initialize(ctx);
#endif
}

int agora_initCallback(lua_State *L)
{
	if(!lua_isfunction(L, -1))
	{
		dmLogError("not a function");
		return 0;
	}
	if(g_luaCallback > 0)
	{
		ext_unregisterLuaCallback(g_luaCallback);
		g_luaCallback = 0;
	}
	g_luaCallback = ext_registerLuaCallback(L, -1);
	return 0;
}

int agora_joinChannel(lua_State *L)
{
	const char *channelName;
	int uid;
	lua_pushstring(L, "channel");				/* L: channel */
	lua_rawget(L, -2);							/* L: t["channel"] */
	channelName = lua_tostring(L, -1);
	lua_pop(L, 1);								/* L: */
	lua_pushstring(L, "uid");					/* L: uid */
	lua_rawget(L, -2);							/* L: t["uid"] */
	uid = lua_tointeger(L, -1);
	lua_pop(L, 1);								/* L: */

//	dmLogInfo("gwjgwj,joinchannel,channel=%s,uid=%d,engine=0x%x", channelName, uid, g_agoraEngine);

	g_agoraEngine->setChannelProfile(CHANNEL_PROFILE_GAME);
	g_agoraEngine->joinChannel(NULL, channelName, NULL, uid);
	return 0;
}

int agora_leaveChannel(lua_State *L)
{
	g_agoraEngine->leaveChannel();
	return 0;
}

void MyAgoraEventHandler::onJoinChannelSuccess(const char* channel, uid_t userId, int elapsed)
{
	dmLogInfo("gwjgwj,join channel success,channel=%s,userId=%d,elapsed=%d", channel, userId, elapsed);
	if(g_luaCallback > 0)
	{
		static char szChannel[1024];
		strcpy(szChannel, channel);
		ext_performInUpdateThread([=]() {
			char sz[1024];
			sprintf(sz, "{\"name\":\"joinChannelSuccess\",\"uid\":%d}", userId);
			ext_invokeLuaCallbackWithString(g_luaCallback, sz);
		});
	}
}

void MyAgoraEventHandler::onRejoinChannelSuccess(const char* channel, uid_t userId, int elapsed)
{
	dmLogInfo("gwjgwj,rejoin channel success,channel=%s,userId=%d,elapsed=%d", channel, userId, elapsed);
	if(g_luaCallback > 0)
	{
		static char szChannel[1024];
		strcpy(szChannel, channel);
		ext_performInUpdateThread([=]() {
			char sz[1024];
			sprintf(sz, "{\"name\":\"rejoinChannelSuccess\",\"uid\":%d}", userId);
			ext_invokeLuaCallbackWithString(g_luaCallback, sz);
		});
	}
}

void MyAgoraEventHandler::onLeaveChannel(const RtcStats& stats)
{
	dmLogInfo("gwjgwj,on leave channel");
	if(g_luaCallback > 0)
	{
		ext_performInUpdateThread([=]() {
			ext_invokeLuaCallbackWithString(g_luaCallback, "{\"name\":\"leaveChannel\"}");
		});
	}
}

void MyAgoraEventHandler::onUserJoined(uid_t userId, int elapsed)
{
	dmLogInfo("gwjgwj,onUserJoined,userId=%d,elapsed=%d", userId, elapsed);
	if(g_luaCallback > 0)
	{
		ext_performInUpdateThread([=]() {
			char sz[1024];
			sprintf(sz, "{\"name\":\"userJoined\",\"uid\":%d}", userId);
			ext_invokeLuaCallbackWithString(g_luaCallback, sz);
		});
	}
}

void MyAgoraEventHandler::onUserOffline(uid_t userId, USER_OFFLINE_REASON_TYPE reason)
{
	dmLogInfo("gwjgwj,onUserOffline,userId=%d,reason=%d", userId, reason);
	if(g_luaCallback > 0)
	{
		ext_performInUpdateThread([=]() {
			char sz[1024];
			sprintf(sz, "{\"name\":\"userOffline\",\"uid\":%d,\"reason\":%d}", userId, reason);
			ext_invokeLuaCallbackWithString(g_luaCallback, sz);
		});
	}
}

void MyAgoraEventHandler::onUserMuteAudio(uid_t userId, bool muted)
{
	dmLogInfo("gwjgwj,onUserMuteAudio,userid=%d,muted=%d", userId, muted);
	if(g_luaCallback > 0)
	{
		ext_performInUpdateThread([=]() {
			char sz[1024];
			sprintf(sz, "{\"name\":\"userMuteAudio\",\"uid\":%d,\"mute\":%s}", userId, muted?"true":"false");
			ext_invokeLuaCallbackWithString(g_luaCallback, sz);
		});
	}
}

void MyAgoraEventHandler::onMicrophoneEnabled(bool enabled)
{
	dmLogInfo("gwjgwj,onMicrophoneEnabled:%d", enabled);
	if(g_luaCallback > 0)
	{
		ext_performInUpdateThread([=]() {
			char sz[1024];
			sprintf(sz, "{\"name\":\"microphoneEnabled\",\"enable\":%s}", enabled?"true":"false");
			ext_invokeLuaCallbackWithString(g_luaCallback, sz);
		});
	}
}
