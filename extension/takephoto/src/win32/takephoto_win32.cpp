#if defined(DM_PLATFORM_WINDOWS)

#include "../takephoto.h"
#include <commdlg.h>
#include <winbase.h>
#define takephoto_countof(arr) (sizeof(arr)/sizeof(arr[0]))

void takephoto_cp(LPCWSTR srcFile, LPCWSTR dstFile);

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

	dmLogInfo("callback = %d", callback);

	wchar_t strFileName[1024] = L"";
	wchar_t strFileTitle[1024] = L"";

	OPENFILENAMEW ofn;
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = dmGraphics::GetNativeWindowsHWND();
	ofn.lpstrFilter = L"Supported Format\0*.jpg;*.png\0all file\0*.*\0\0";
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0L;
	ofn.nFilterIndex = 1L;
	ofn.lpstrFile = strFileName;
	ofn.nMaxFile = takephoto_countof(strFileName);
	ofn.lpstrFileTitle = strFileTitle;
	ofn.nMaxFileTitle = takephoto_countof(strFileTitle);
	ofn.lpstrInitialDir = L"";
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = NULL;
	ofn.Flags = OFN_EXPLORER | OFN_ENABLESIZING;
//	ofn.lpfnHook = (LPOFNHOOKPROC)CFileDialog32W::HookProc;
//	ofn.hInstance = hInstTemplate;
//	ofn.lpTemplateName = lpTemplateName;

	BOOL bReturn;
	wchar_t wsz[1024];
	GetCurrentDirectoryW(takephoto_countof(wsz), wsz);
	bReturn = GetOpenFileNameW(&ofn);
	SetCurrentDirectoryW(wsz);
	if(!bReturn)
	{
		char ssErrorMsg[1024];
		DWORD dwExtErr = CommDlgExtendedError();
		switch(dwExtErr)
		{
		case 0:
			ssErrorMsg[0] = 0;
			break;
		case FNERR_FILENAMECODES:
			lstrcpyA(ssErrorMsg, "FNERR_FILENAMECODES");
			break;
		case FNERR_SUBCLASSFAILURE:
			lstrcpyA(ssErrorMsg, "FNERR_SUBCLASSFAILURE");
			break;
		case FNERR_INVALIDFILENAME:
			lstrcpyA(ssErrorMsg, "FNERR_INVALIDFILENAME");
			break;
		case FNERR_BUFFERTOOSMALL:
			lstrcpyA(ssErrorMsg, "FNERR_BUFFERTOOSMALL");
			break;
		default:
			sprintf(ssErrorMsg, "FNERR_UNKNOWN: 0x%lx", dwExtErr);
			break;
		}
		if(ssErrorMsg[0] != 0)
			dmLogError("%s", ssErrorMsg);
	}
	if(bReturn)
	{
		wchar_t wszLocalPath[1024];
		MultiByteToWideChar(CP_UTF8, 0, localPath, -1, wszLocalPath, sizeof(wszLocalPath));
		dmLogInfo("copy to %s", localPath);
//		takephoto_cp(strFileName, wszLocalPath);
		if(!CopyFileW(strFileName, wszLocalPath, FALSE))
		{
			DWORD err = GetLastError();
			dmLogError("cannot copy to %s,code=%d", localPath, err);
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

void takephoto_cp(LPCWSTR srcFile, LPCWSTR dstFile)
{
	FILE *fpSrc, *fpDst;
	fpSrc = _wfopen(srcFile, L"rb");
	if(fpSrc == NULL)
	{
		dmLogError("cannot open %S", srcFile);
		return;
	}
	fpDst = _wfopen(dstFile, L"wb");
	if(fpDst == NULL)
	{
		dmLogError("cannot create %S", dstFile);
		return;
	}
	size_t bufSize = 4*1024*1024;
	size_t readed = 0;
	unsigned char *buffer = new unsigned char[bufSize];
	while(1)
	{
		readed = fread(buffer, 1, bufSize, fpSrc);
		if(readed <= 0)
			break;
		fwrite(buffer, 1, readed, fpDst);
	}
	delete[] buffer;
	fclose(fpSrc);
	fclose(fpDst);
}

#endif