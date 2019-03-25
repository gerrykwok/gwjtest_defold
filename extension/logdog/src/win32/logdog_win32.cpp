#if defined(DM_PLATFORM_WINDOWS)

#include "../logdog.h"
#include "MyLogWindow.h"

static IGwjLogWindow* g_logWnd;

void logdog_onAppInit()
{
	gwjLogWindowCreate(IID_IGwjLogWindow, (void**)&g_logWnd, NULL);
	g_logWnd->create(NULL, L"logdog");
	for(int i = 0; i < 100; ++i)
	{
		HMENU hmenu = GetSystemMenu(g_logWnd->getHWND(), FALSE);
		if(hmenu != NULL)
		{
			DeleteMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
			break;
		}
		Sleep(30);
	}
}

void logdog_doLog(const char *message)
{
//	dmLogInfo("do log %s,logwnd=%lx", message, g_logWnd);
	if(g_logWnd)
		g_logWnd->addLog("dm", RGB(255, 255, 255), message);
}

#endif