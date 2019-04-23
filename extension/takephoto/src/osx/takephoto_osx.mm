#if defined(DM_PLATFORM_OSX)

#import <AppKit/AppKit.h>
#include "../takephoto.h"

void takephoto_onAppInit(const char *environment)
{
}

int takephoto_get_photo(lua_State *L)
{
	int callback = 0;
	char localPath[1024];
	int index = -1;
	localPath[0] = 0;

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
			if(sKey.compare("callback") == 0)
				callback = atoi(sValue.c_str());
			else if(sKey.compare("localPath") == 0)
				strcpy(localPath, sValue.c_str());
		}

		lua_pop(L, 1);  /* removes 'value'; keeps 'key' for next iteration */
	}

	dmLogInfo("callback = %d, localPath=%s", callback, localPath);

	NSOpenPanel *panel = [NSOpenPanel openPanel];
	//[panel setDirectory:NSHomeDirectory()];
	[panel setAllowsMultipleSelection:NO];
	[panel setCanChooseDirectories:NO];
	[panel setCanChooseFiles:YES];
	[panel setAllowedFileTypes:@[@"jpg", @"png"]];
	[panel setAllowsOtherFileTypes:YES];
	int bReturn = 0;
	if([panel runModal] == NSOKButton)
	{
		bReturn = 1;
		NSString *path = [panel.URLs.firstObject path];
		dmLogInfo("copy %s to %s", [path UTF8String], localPath);
		NSFileManager *manager = [NSFileManager defaultManager];
		NSError *error;
		[manager removeItemAtURL:[NSURL fileURLWithPath:[NSString stringWithUTF8String:localPath]] error:nil];
		if(![manager copyItemAtPath:path toPath:[NSString stringWithUTF8String:localPath] error:&error])
		{
			dmLogError("error copying to %s: %s", localPath, [[error description] UTF8String]);
		}
	}
	if(callback > 0)
	{
		char res[256];
		sprintf(res, "{\"result\":%d}", bReturn ? 1 : 0);
		ext_invokeLuaCallbackWithString(callback, res);
		ext_unregisterLuaCallback(callback);
	}
	lua_pushboolean(L, true);
	lua_pushstring(L, "success");
	return 2;
}

#endif