#define EXTENSION_NAME luajavaoc
#define LIB_NAME "luajavaoc"
#define MODULE_NAME "luajavaoc"

#include <dmsdk/sdk.h>
#include "luajavaoc.h"

static dmExtension::Result ext_AppInit(dmExtension::AppParams* params)
{
	luajavaoc_onAppInit();
	return dmExtension::RESULT_OK;
}

static dmExtension::Result ext_Init(dmExtension::Params* params)
{
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

void ext_onUpdate();

static dmExtension::Result ext_update(dmExtension::Params *params)
{
	ext_onUpdate();
	return dmExtension::RESULT_OK;
}

// Defold SDK uses a macro for setting up extension entry points:
// DM_DECLARE_EXTENSION(symbol, name, app_init, app_final, init, update, on_event, final)
DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, ext_AppInit, ext_AppFinal, ext_Init, ext_update, 0, ext_Final)
