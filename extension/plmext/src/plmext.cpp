#define LIB_NAME "plmext"
#define MODULE_NAME "plmext"

// include the Defold SDK
#include <dmsdk/sdk.h>
#include "plmext.h"

#if defined(DM_PLATFORM_IOS)
#include "ios/plmext_ios.h"
#elif defined(DM_PLATFORM_WINDOWS)
#include "win32/plmext_win32.h"
#endif

#if defined(DM_PLATFORM_ANDROID)
#include "android/LuaJavaBridge_jni.h"
#endif

void plmext_onAppInit();

static int test(lua_State *L)
{
	return 0;
}

// Functions exposed to Lua
static const luaL_reg Module_methods[] =
{
	{"test", test},
	{0, 0}
};

static void LuaInit(lua_State* L)
{
	int top = lua_gettop(L);
	// Register lua names
	luaL_register(L, MODULE_NAME, Module_methods);

	lua_pop(L, 1);
	assert(top == lua_gettop(L));

#if defined(DM_PLATFORM_IOS)
	plm_compile_in();
#elif defined(DM_PLATFORM_WINDOWS)
	plmext_win32_init(L);
#endif
}

static dmExtension::Result ext_AppInit(dmExtension::AppParams* params)
{
	plmext_onAppInit();
	return dmExtension::RESULT_OK;
}

static dmExtension::Result ext_Init(dmExtension::Params* params)
{
	// Init Lua
	LuaInit(params->m_L);
	printf("Registered %s Extension\n", MODULE_NAME);
	return dmExtension::RESULT_OK;
}

static dmExtension::Result ext_AppFinal(dmExtension::AppParams* params)
{
	return dmExtension::RESULT_OK;
}

static dmExtension::Result ext_Final(dmExtension::Params* params)
{
	//保证函数能编译进so
#if defined(DM_PLATFORM_ANDROID)
	DEF_JNI_FUNCTION(callLuaFunctionWithString)(NULL, NULL, 0, NULL);
	DEF_JNI_FUNCTION(callLuaGlobalFunctionWithString)(NULL, NULL, NULL, NULL);
	DEF_JNI_FUNCTION(retainLuaFunction)(NULL, NULL, 0);
	DEF_JNI_FUNCTION(releaseLuaFunction)(NULL, NULL, 0);
	DEF_JNI_FUNCTION(runOnGLThread)(NULL, NULL, NULL);
#endif
	return dmExtension::RESULT_OK;
}

// Defold SDK uses a macro for setting up extension entry points:
// DM_DECLARE_EXTENSION(symbol, name, app_init, app_final, init, update, on_event, final)
DM_DECLARE_EXTENSION(plmext, LIB_NAME, ext_AppInit, ext_AppFinal, ext_Init, 0, 0, ext_Final)
