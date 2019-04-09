#define EXTENSION_NAME misc
#define LIB_NAME "misc"
#define MODULE_NAME "misc"

#include <dmsdk/sdk.h>
#include "misc.h"

static bool g_inited = false;
static bool g_has_final = false;

static const luaL_reg Module_methods[] =
{
	{"installApk", misc_installApk},
#if defined(DM_PLATFORM_ANDROID)
	{"isApkInstalled", misc_isApkInstalled},
#elif defined(DM_PLATFORM_IOS)
	{"canOpenApp", misc_canOpenApp},
#endif
#if defined(DM_PLATFORM_IOS) || defined(DM_PLATFORM_OSX)
	{"getBundleResourcePath", misc_getBundleResourcePath},
#endif
	{"androidCheckPermission", misc_androidCheckPermission},
	{"androidRequestPermissions", misc_androidRequestPermissions},
	{"androidGetTargetSdkVersion", misc_androidGetTargetSdkVersion},
	{"androidGotoAppSetting", misc_androidGotoAppSetting},
	{"getBatteryInfo", misc_getBatteryInfo},
	{"setAppEventListener", misc_setAppEventListener},
	{"getCurrentThreadId", misc_getCurrentThreadId},
	{"vibrate", misc_vibrate},
	{"goHome", misc_goHome},
	{0, 0}
};

static void LuaInit(lua_State* L)
{
	int top = lua_gettop(L);
	// Register lua names
	luaL_register(L, MODULE_NAME, Module_methods);

	lua_pop(L, 1);
	assert(top == lua_gettop(L));
}

static dmExtension::Result ext_AppInit(dmExtension::AppParams* params)
{
	if(!g_inited)
	{
#if defined(DM_PLATFORM_ANDROID)
		bool ok;
		ext_callJavaStaticMethod("com.xishanju.plm.misc.MiscFunc", "init", "", &ok);
#endif
		g_inited = true;
	}
	return dmExtension::RESULT_OK;
}

static dmExtension::Result ext_Init(dmExtension::Params* params)
{
	LuaInit(params->m_L);
	g_has_final = false;
	return dmExtension::RESULT_OK;
}

static dmExtension::Result ext_AppFinal(dmExtension::AppParams* params)
{
	return dmExtension::RESULT_OK;
}

static dmExtension::Result ext_Final(dmExtension::Params* params)
{
	g_has_final = true;
	return dmExtension::RESULT_OK;
}

static dmExtension::Result ext_update(dmExtension::Params *params)
{
	return dmExtension::RESULT_OK;
}

static void ext_OnEvent(dmExtension::Params* params, const dmExtension::Event* event)
{
	dmExtension::EventID eventId = event->m_Event;
	switch(eventId)
	{
	case dmExtension::EVENT_ID_ACTIVATEAPP:
		misc_invokeAppEventListener("EVENT_ID_ACTIVATEAPP");
		break;
	case dmExtension::EVENT_ID_DEACTIVATEAPP:
		if(!g_has_final)
			misc_invokeAppEventListener("EVENT_ID_DEACTIVATEAPP");
		break;
	}
}

// Defold SDK uses a macro for setting up extension entry points:
// DM_DECLARE_EXTENSION(symbol, name, app_init, app_final, init, update, on_event, final)
DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, ext_AppInit, ext_AppFinal, ext_Init, ext_update, ext_OnEvent, ext_Final)
