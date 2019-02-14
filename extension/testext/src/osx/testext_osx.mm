#if defined(DM_PLATFORM_OSX)

#include <AppKit/AppKit.h>
#include "../testext.h"

int test_callnative(lua_State *L)
{
	return 0;
}

int test_centerWindow(lua_State *L)
{
	NSWindow* window = dmGraphics::GetNativeOSXNSWindow();
	NSRect screenFrame = window.screen.frame;
	NSRect windowFrame = window.frame;

	[window setFrameOrigin:NSMakePoint((screenFrame.size.width-windowFrame.size.width)/2, (screenFrame.size.height-windowFrame.size.height)/2)];

	return 0;
}

#endif
