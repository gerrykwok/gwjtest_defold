#define EXTENSION_NAME misc
#define LIB_NAME "misc"
#define MODULE_NAME "misc"

#include <dmsdk/sdk.h>
#include "misc.h"

static const luaL_reg Module_methods[] =
{
	{"installApk", misc_installApk},
#if defined(DM_PLATFORM_ANDROID)
	{"isApkInstalled", misc_isApkInstalled},
#elif defined(DM_PLATFORM_IOS)
	{"canOpenApp", misc_canOpenApp},
#endif
	{"androidCheckPermission", misc_androidCheckPermission},
	{"androidRequestPermissions", misc_androidRequestPermissions},
	{"androidGetTargetSdkVersion", misc_androidGetTargetSdkVersion},
	{"androidGotoAppSetting", misc_androidGotoAppSetting},
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
#if defined(DM_PLATFORM_ANDROID)
	bool ok;
	ext_callJavaStaticMethod("com.xishanju.plm.misc.MiscFunc", "init", "", &ok);
#endif
	return dmExtension::RESULT_OK;
}

static dmExtension::Result ext_Init(dmExtension::Params* params)
{
	LuaInit(params->m_L);
	return dmExtension::RESULT_OK;
}

static dmExtension::Result ext_AppFinal(dmExtension::AppParams* params)
{
	return dmExtension::RESULT_OK;
}

static dmExtension::Result ext_Final(dmExtension::Params* params)
{
	return dmExtension::RESULT_OK;
}

static dmExtension::Result ext_update(dmExtension::Params *params)
{
	return dmExtension::RESULT_OK;
}

// Defold SDK uses a macro for setting up extension entry points:
// DM_DECLARE_EXTENSION(symbol, name, app_init, app_final, init, update, on_event, final)
DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, ext_AppInit, ext_AppFinal, ext_Init, ext_update, 0, ext_Final)
