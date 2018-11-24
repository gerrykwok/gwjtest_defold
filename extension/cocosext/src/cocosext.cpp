#define LIB_NAME "cocosext"
#define MODULE_NAME "cocosext"

// include the Defold SDK
#include <dmsdk/sdk.h>
#include "cocosext.h"
#include "tolua/tolua++.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"
#include "CCLuaStack.h"
#if defined(DM_PLATFORM_ANDROID)
#include "android/CCLuaJavaBridge.h"
#include "android/cocosext_android.h"
#endif
#if defined(DM_PLATFORM_IOS)
#include "ios/CCLuaObjcBridge.h"
#endif

#include <functional>

static void COCOSEXT_update();

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
	toluafix_open(L);
	LuaStack::getInstance()->initWithLuaState(L);

	lua_pop(L, 1);
	assert(top == lua_gettop(L));
#if defined(DM_PLATFORM_ANDROID)
	LuaJavaBridge::luaopen_luaj(L);
	cocosext_android_init();
#endif

#if defined(DM_PLATFORM_IOS)
	LuaObjcBridge::luaopen_luaoc(L);
#endif
}

static dmExtension::Result ext_AppInit(dmExtension::AppParams* params)
{
	return dmExtension::RESULT_OK;
}

static dmExtension::Result ext_Init(dmExtension::Params* params)
{
	// Init Lua
	LuaInit(params->m_L);
	dmLogInfo("Registered %s Extension", MODULE_NAME);
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
	COCOSEXT_update();
	return dmExtension::RESULT_OK;
}

struct COCOSEXT_CALLBACK
{
	std::function<void(void)> m_callback;
	bool m_hasCallback;
};

#define COCOSEXT_MAX_CALLBACK		64
static COCOSEXT_CALLBACK g_allCallback[COCOSEXT_MAX_CALLBACK];

void COCOSEXT_runOnGLThread(const std::function<void(void)> &callback)
{
	COCOSEXT_CALLBACK *pCB = g_allCallback;
	COCOSEXT_CALLBACK *pEnd = pCB + COCOSEXT_MAX_CALLBACK;
	bool hasEmpty = false;
	while(pCB < pEnd)
	{
		if(!pCB->m_hasCallback)
		{
			pCB->m_callback = callback;
			pCB->m_hasCallback = true;
			hasEmpty = true;
			break;
		}
		pCB++;
	}
	if(!hasEmpty)
	dmLogError("not enough slot to store callback");
}

void COCOSEXT_update()
{
	COCOSEXT_CALLBACK *pCB = g_allCallback;
	COCOSEXT_CALLBACK *pEnd = pCB + COCOSEXT_MAX_CALLBACK;
	while(pCB < pEnd)
	{
		if(pCB->m_hasCallback)
		{
			pCB->m_callback();
			pCB->m_hasCallback = false;
		}
		pCB++;
	}
}

// Defold SDK uses a macro for setting up extension entry points:
// DM_DECLARE_EXTENSION(symbol, name, app_init, app_final, init, update, on_event, final)
DM_DECLARE_EXTENSION(cocosext, LIB_NAME, ext_AppInit, ext_AppFinal, ext_Init, ext_update, 0, ext_Final)
