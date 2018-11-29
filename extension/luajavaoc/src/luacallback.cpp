#include <dmsdk/sdk.h>
#include <mutex>
#include <vector>
#include "luacallback.h"

struct LuaCallbackInfo
{
	LuaCallbackInfo() : m_L(0), m_Callback(LUA_NOREF), m_Self(LUA_NOREF), m_id(0) {}
	lua_State* m_L;
	int        m_Callback;
	int        m_Self;
	int        m_id;
};

static int g_SequenceId = 0;

static void RegisterCallback(lua_State* L, int index, LuaCallbackInfo* cbk)
{
	if(cbk->m_Callback != LUA_NOREF)
	{
		dmScript::Unref(cbk->m_L, LUA_REGISTRYINDEX, cbk->m_Callback);
		dmScript::Unref(cbk->m_L, LUA_REGISTRYINDEX, cbk->m_Self);
	}

	cbk->m_L = dmScript::GetMainThread(L);

	luaL_checktype(L, index, LUA_TFUNCTION);
	lua_pushvalue(L, index);
	cbk->m_Callback = dmScript::Ref(L, LUA_REGISTRYINDEX);

	dmScript::GetInstance(L);
	cbk->m_Self = dmScript::Ref(L, LUA_REGISTRYINDEX);

	cbk->m_id = ++g_SequenceId;
}

static void UnregisterCallback(LuaCallbackInfo* cbk)
{
	if(cbk->m_Callback != LUA_NOREF)
	{
		dmScript::Unref(cbk->m_L, LUA_REGISTRYINDEX, cbk->m_Callback);
		dmScript::Unref(cbk->m_L, LUA_REGISTRYINDEX, cbk->m_Self);
		cbk->m_Callback = LUA_NOREF;
	}
}

static void InvokeCallbackWithString(LuaCallbackInfo* cbk, const char *value)
{
	if(cbk->m_Callback == LUA_NOREF)
	{
		return;
	}

	lua_State* L = cbk->m_L;
	int top = lua_gettop(L);

	lua_rawgeti(L, LUA_REGISTRYINDEX, cbk->m_Callback);		/* L: callback */

	// Setup self (the script instance)
	lua_rawgeti(L, LUA_REGISTRYINDEX, cbk->m_Self);			/* L: callback self */
	lua_pushvalue(L, -1);									/* L: callback self self */

	dmScript::SetInstance(L);								/* L: callback self */
	int number_of_arguments = 1;

	lua_pushstring(L, value);
	number_of_arguments++;

	int ret = lua_pcall(L, number_of_arguments, 0, 0);
	if(ret != 0) {
		dmLogError("Error running callback: %s", lua_tostring(L, -1));
		lua_pop(L, 1);
	}
	assert(top == lua_gettop(L));
}

static dmArray<LuaCallbackInfo> g_callbacks;

int ext_registerLuaCallback(lua_State* L, int index)
{
	int top = lua_gettop(L);
	LuaCallbackInfo cbk;
	RegisterCallback(L, 1, &cbk);
	if (g_callbacks.Full()) {
		g_callbacks.SetCapacity(g_callbacks.Capacity() + 8);
	}
	g_callbacks.Push(cbk);
	assert(top + 0 == lua_gettop(L));
	return cbk.m_id;
}

void ext_unregisterLuaCallback(int callbackId)
{
	for (int i = g_callbacks.Size() - 1; i >= 0; i--) {
		if (g_callbacks[i].m_id == callbackId) {
			UnregisterCallback(&g_callbacks[i]);
			g_callbacks.EraseSwap(i);
			break;
		}
	}
}

void ext_invokeLuaCallbackWithString(int callbackId, const char *value)
{
	LuaCallbackInfo *pCbk = NULL;
	for (int i = g_callbacks.Size() - 1; i >= 0; i--) {
		if (g_callbacks[i].m_id == callbackId) {
			pCbk = &g_callbacks[i];
			break;
		}
	}
	if(pCbk == NULL)
	{
		dmLogInfo("gwjgwj,not found lua callback for id %d", callbackId);
		return;
	}
	InvokeCallbackWithString(pCbk, value);
}

//////////////////////////////////////////////////////////////////////////////////////////////

static std::mutex g_performMutex;
static std::vector<std::function<void(void)>> g_functionsToPerform;

void ext_performInUpdateThread(const std::function<void(void)> &func)
{
	g_performMutex.lock();
	g_functionsToPerform.push_back(func);
	g_performMutex.unlock();
}

void ext_onUpdate()
{
	if( !g_functionsToPerform.empty() ) {
		g_performMutex.lock();
		// fixed #4123: Save the callback functions, they must be invoked after 'g_performMutex.unlock()', otherwise if new functions are added in callback, it will cause thread deadlock.
		auto temp = g_functionsToPerform;
		g_functionsToPerform.clear();
		g_performMutex.unlock();
		for(const auto &func : temp) {
			func();
		}
	}
}
