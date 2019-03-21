#include <dmsdk/sdk.h>
#include <math.h>
#include <mutex>
#include <vector>
#include <map>
#include "luacallback.h"

struct LuaCallbackInfo
{
	LuaCallbackInfo() : m_L(0), m_Callback(LUA_NOREF), m_Self(LUA_NOREF), m_id(0) {}
	lua_State* m_L;
	int        m_Callback;
	int        m_Self;
	int        m_id;
};

static void ext_replaceString(std::string &str, const char *sub, const char *repl)
{
	std::string::size_type lenRepl = strlen(repl);
	std::string::size_type lenSub = strlen(sub);
	std::string::size_type pos, start;
	start = 0;
	while(1)
	{
		pos = str.find(sub, start);
		if(pos == std::string::npos)
		break;
		str.replace(pos, lenSub, repl);
		start = pos + lenRepl;
	}
}

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

static void InvokeCallback(LuaCallbackInfo *cbk, int num_args)
{
	if(cbk->m_Callback == LUA_NOREF)
	{
		return;
	}

	lua_State* L = cbk->m_L;

	lua_rawgeti(L, LUA_REGISTRYINDEX, cbk->m_Callback);		/* L: arg1 arg2 ... callback */
	lua_insert(L, -(num_args+1));							/* L: callback arg1 arg2 ... */

	// Setup self (the script instance)
	lua_rawgeti(L, LUA_REGISTRYINDEX, cbk->m_Self);			/* L: callback arg1 arg2 ... self */
	lua_insert(L, -(num_args+1));							/* L: callback self arg1 arg2 ... */
	lua_pushvalue(L, -(num_args+1));						/* L: callback self arg1 arg2 ... self */

	dmScript::SetInstance(L);								/* L: callback self arg1 arg2 ... */
	int number_of_arguments = num_args+1;

	int ret = lua_pcall(L, number_of_arguments, 0, 0);
	if(ret != 0) {
		dmLogError("Error running callback: %s", lua_tostring(L, -1));
		lua_pop(L, 1);
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

//	lua_rawgeti(L, LUA_REGISTRYINDEX, cbk->m_Callback);		/* L: callback */
//
//	// Setup self (the script instance)
//	lua_rawgeti(L, LUA_REGISTRYINDEX, cbk->m_Self);			/* L: callback self */
//	lua_pushvalue(L, -1);									/* L: callback self self */
//
//	dmScript::SetInstance(L);								/* L: callback self */
//	int number_of_arguments = 1;
//
//	lua_pushstring(L, value);
//	number_of_arguments++;
//
//	int ret = lua_pcall(L, number_of_arguments, 0, 0);
//	if(ret != 0) {
//		dmLogError("Error running callback: %s", lua_tostring(L, -1));
//		lua_pop(L, 1);
//	}
	lua_pushstring(L, value);
	InvokeCallback(cbk, 1);

	assert(top == lua_gettop(L));
}

static dmArray<LuaCallbackInfo> g_callbacks;

static LuaCallbackInfo* FindCallbackWithId(int callbackId)
{
	LuaCallbackInfo *pCbk = NULL;
	for(int i = g_callbacks.Size() - 1; i >= 0; --i) {
		if(g_callbacks[i].m_id == callbackId) {
			pCbk = &g_callbacks[i];
			break;
		}
	}
	return pCbk;
}

int ext_registerLuaCallback(lua_State* L, int index)
{
	if(!lua_isfunction(L, index)) return 0;
	int top = lua_gettop(L);
	LuaCallbackInfo cbk;
	RegisterCallback(L, index, &cbk);
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
	LuaCallbackInfo *pCbk = FindCallbackWithId(callbackId);
	if(pCbk == NULL)
	{
		dmLogInfo("gwjgwj,not found lua callback for id %d", callbackId);
		return;
	}
	InvokeCallbackWithString(pCbk, value);
}

lua_State* ext_getLuaStateWithCallbackId(int callbackId)
{
	LuaCallbackInfo *pCbk = FindCallbackWithId(callbackId);
	if(pCbk == NULL) return NULL;
	return pCbk->m_L;
}

void ext_invokeLuaCallback(int callbackId, int num_args)
{
	LuaCallbackInfo *pCbk = FindCallbackWithId(callbackId);
	if(pCbk == NULL)
	{
		dmLogInfo("gwjgwj,not found lua callback for id %d", callbackId);
		return;
	}
	InvokeCallback(pCbk, num_args);
}

void ext_pushLuaValue(lua_State *L, const LuaValue& value)
{
	const LuaValueType type = value.getType();
	if(type == LuaValueTypeInt)
	{
		lua_pushinteger(L, value.intValue());
	}
	else if(type == LuaValueTypeFloat)
	{
		lua_pushnumber(L, value.floatValue());
	}
	else if(type == LuaValueTypeBoolean)
	{
		lua_pushboolean(L, value.booleanValue());
	}
	else if(type == LuaValueTypeString)
	{
		lua_pushstring(L, value.stringValue().c_str());
	}
	else if(type == LuaValueTypeDict)
	{
		ext_pushLuaValueDict(L, value.dictValue());
	}
	else if(type == LuaValueTypeArray)
	{
		ext_pushLuaValueArray(L, value.arrayValue());
	}
//	else if(type == LuaValueTypeObject)
//	{
//		pushObject(value.ccobjectValue(), value.getObjectTypename().c_str());
//	}
}

void ext_pushLuaValueDict(lua_State *L, const LuaValueDict& dict)
{
	lua_newtable(L);                                              /* L: table */
	for(LuaValueDictIterator it = dict.begin(); it != dict.end(); ++it)
	{
		lua_pushstring(L, it->first.c_str());                     /* L: table key */
		ext_pushLuaValue(L, it->second);                                     /* L: table key value */
		lua_rawset(L, -3);                     /* table.key = value, L: table */
	}
}

void ext_pushLuaValueArray(lua_State *L, const LuaValueArray& array)
{
	lua_newtable(L);                                              /* L: table */
	int index = 1;
	for(LuaValueArrayIterator it = array.begin(); it != array.end(); ++it)
	{
		ext_pushLuaValue(L, *it);                                            /* L: table value */
		lua_rawseti(L, -2, index);          /* table[index] = value, L: table */
		++index;
	}
}

std::string ext_jsonFromLuaTable(lua_State *L, int index)
{
	int top = lua_gettop(L);
	std::string sKey, sValue;
	std::string sRet;
	int i;
	char buf[256];
	lua_Number fValue;

	sRet = "{";
	lua_pushnil(L);  /* first key */
	while(lua_next(L, index-1) != 0)
	{
		/* 'key' is at index -2 and 'value' at index -1 */
		switch(lua_type(L, -2))
		{
		case LUA_TNIL:
			sKey = "nil";
			break;
		case LUA_TBOOLEAN:
			sKey = lua_toboolean(L, -2) ? "true" : "false";
			break;
		case LUA_TNUMBER:
			sprintf(buf, "%d", (int)lua_tonumber(L, -2));
			sKey = buf;
			break;
		case LUA_TSTRING:
			sKey = lua_tostring(L, -2);
			break;
		default:
			sKey = "";
			break;
		}

		bool valueValid = true;
		switch(lua_type(L, -1))
		{
		case LUA_TBOOLEAN:
			sValue = lua_toboolean(L, -1) ? "true" : "false";
			break;
		case LUA_TNUMBER:
			fValue = lua_tonumber(L, -1);
			sprintf(buf, "%f", fValue);
			for(i = strlen(buf)-1; i >= 0; --i)
			{
				if(buf[i] != '0')
					break;
				if(i > 0)
				{
					buf[i] = 0;
					if(buf[i-1] == '.')
						buf[i-1] = 0;
				}
			}
			sValue = buf;
			break;
		case LUA_TSTRING:
			sValue = (std::string)"\"" + lua_tostring(L, -1) + (std::string)"\"";
			break;
		case LUA_TFUNCTION:
			sprintf(buf, "%d", ext_registerLuaCallback(L, -1));
			sValue = buf;
			break;
		case LUA_TTABLE:
			sValue = ext_jsonFromLuaTable(L, -1);
			break;
		default:
			sValue = "\"\"";
			valueValid = false;
			break;
		}

//		dmLogInfo("%s - %s", lua_typename(L, lua_type(L, -2)), lua_typename(L, lua_type(L, -1)));
//		dmLogInfo("\"%s\":%s", sKey.c_str(), sValue.c_str());
		if(sKey.length() > 0 && valueValid)
		{
			if(sRet.length() > 1)
				sRet += ", ";
			sRet += "\"";
			sRet += sKey;
			sRet += "\":";
			sRet += sValue;
		}
		else
		{
//			dmLogError("invalid key");
		}

		lua_pop(L, 1);  /* removes 'value'; keeps 'key' for next iteration */
	}

	assert(top + 0 == lua_gettop(L));
	sRet += "}";
	return sRet;
}

//////////////////////////////////////////////////////////////////////////////////////////////

static std::mutex g_performMutex;
static std::vector<std::function<void(void)> > g_functionsToPerform;
struct SCHEDULE_FUNCTION
{
	bool m_delete;
	std::function<void(void)> func;
};
typedef std::map<unsigned int, SCHEDULE_FUNCTION> ScheduleFunctions;
static ScheduleFunctions g_functionsToSchedule;
static unsigned int g_counterSchedule = 0;
struct DELAY_STRUCT
{
	bool m_valid;
	std::function<void(void)> m_callback;
	long m_startTimeSec, m_startTimeUsec;
	double m_delay;
};
#define DELAY_MAX	8
static DELAY_STRUCT g_allDelay[DELAY_MAX];

void ext_performInUpdateThread(const std::function<void(void)> &func)
{
	g_performMutex.lock();
	g_functionsToPerform.push_back(func);
	g_performMutex.unlock();
}

unsigned int ext_scheduleUpdate(const std::function<void(void)> &func)
{
	unsigned int id = ++g_counterSchedule;
	SCHEDULE_FUNCTION entry;
	entry.func = func;
	entry.m_delete = false;
	g_functionsToSchedule[id] = entry;
	return id;
}

void ext_unscheduleUpdate(unsigned int entryId)
{
	//just mark for delete
	ScheduleFunctions::iterator it = g_functionsToSchedule.find(entryId);
	if(it != g_functionsToSchedule.end())
	{
		it->second.m_delete = true;
	}
}

void ext_performWithDelay(float delayInSeconds, const std::function<void(void)> &func)
{
	DELAY_STRUCT *p, *pEnd;
	p = g_allDelay;
	pEnd = p + DELAY_MAX;
	while(p < pEnd)
	{
		if(!p->m_valid)
		{
			p->m_callback = func;
			ext_gettimeofday(&p->m_startTimeSec, &p->m_startTimeUsec);
			p->m_delay = delayInSeconds;
			p->m_valid = true;
			break;
		}
		p++;
	}
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
	//delay in seconds
	long nowSec, nowUsec;
	ext_gettimeofday(&nowSec, &nowUsec);
	DELAY_STRUCT *p2, *pEnd2;
	p2 = g_allDelay;
	pEnd2 = p2 + DELAY_MAX;
	while(p2 < pEnd2)
	{
		if(p2->m_valid)
		{
			double elapsed = (double)(nowSec - p2->m_startTimeSec) + (double)(nowUsec - p2->m_startTimeUsec) / 1000000.0;
			if(elapsed >= p2->m_delay)
			{
				p2->m_callback();
				p2->m_callback = nullptr;
				p2->m_valid = false;
			}
		}
		p2++;
	}
	//schedule
	if(!g_functionsToSchedule.empty())
	{
		//dmLogInfo("schedule num:%d", g_functionsToSchedule.size());
		for(const auto &entry : g_functionsToSchedule)
		{
			if(!entry.second.m_delete)
				entry.second.func();
		}
		//delete all schedule that is marked delete
		ScheduleFunctions::iterator it;
		for(it = g_functionsToSchedule.begin(); it != g_functionsToSchedule.end(); )
		{
			if(it->second.m_delete)
			{
				g_functionsToSchedule.erase(it++);
			}
			else it++;
		}
	}
}

int ext_callNativeStaticMethod(const char *clazz, const char *method, lua_State *L, int idxParam)
{
	bool ok;
	std::string ret = ext_callNativeStaticMethodBase(clazz, method, L, idxParam, &ok);
	lua_pushboolean(L, ok);
	lua_pushstring(L, ret.c_str());
	return 2;
}

void luajavaoc_onAppInit()
{
	int i;
	g_SequenceId = 0;
	while(g_callbacks.Size() > 0)
	{
		g_callbacks.Pop();
	}
	g_functionsToPerform.clear();
	g_functionsToSchedule.clear();
	g_counterSchedule = 0;

	for(i = 0; i < DELAY_MAX; ++i)
		g_allDelay[i].m_valid = false;
}
