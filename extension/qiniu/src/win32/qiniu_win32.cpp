#if defined(DM_PLATFORM_WINDOWS)

#include "../qiniu.h"
#include "../gwjQiniu/gwjQiniu.h"

int qiniu_uploadFile(lua_State *L)
{
	std::string token, path, key;
	int callback = 0;
	int index = -1;

	std::string sKey, sValue;
	int i;
	char buf[256];
	lua_Number fValue;
	lua_pushnil(L);  /* first key */
	while(lua_next(L, index - 1) != 0)
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
			for(i = strlen(buf) - 1; i >= 0; --i)
			{
				if(buf[i] != '0')
					break;
				if(i > 0)
				{
					buf[i] = 0;
					if(buf[i - 1] == '.')
						buf[i - 1] = 0;
				}
			}
			sValue = buf;
			break;
		case LUA_TSTRING:
			sValue = lua_tostring(L, -1);
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

		if(sKey.length() > 0 && valueValid)
		{
			if(sKey.compare("token") == 0)
				token = sValue;
			else if(sKey.compare("path") == 0)
				path = sValue;
			else if(sKey.compare("key") == 0)
				key = sValue;
			else if(sKey.compare("callback") == 0)
				callback = atoi(sValue.c_str());
//			dmLogInfo("%s = %s", sKey.c_str(), sValue.c_str());
		}

		lua_pop(L, 1);  /* removes 'value'; keeps 'key' for next iteration */
	}
	dmLogInfo("uploadfile,token=%s,path=%s,key=%s,callback=%d", token.c_str(), path.c_str(), key.c_str(), callback);
	gwjQiniu::startNewUpload(token.c_str(), path.c_str(), key.c_str(), callback);
	return 0;
}

#endif