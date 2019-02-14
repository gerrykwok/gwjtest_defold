#if defined(DM_PLATFORM_WINDOWS)

#include "../testext.h"

int test_callnative(lua_State *L)
{
	return 0;
}

int test_printMacro(lua_State *L)
{
	dmLogInfo("MY_DEFINE=%d", MY_DEFINE);
	return 0;
}

int test_centerWindow(lua_State *L)
{
	HWND window = dmGraphics::GetNativeWindowsHWND();
	int cxScreen = GetSystemMetrics(SM_CXSCREEN);
	int cyScreen = GetSystemMetrics(SM_CYSCREEN);
	RECT rect;
	GetWindowRect(window, &rect);
	SetWindowPos(window, NULL, (cxScreen - (rect.right-rect.left))/2, (cyScreen - (rect.bottom-rect.top))/2, rect.right-rect.left, rect.bottom-rect.top, SWP_NOZORDER);
	return 0;
}

#endif
