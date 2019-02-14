#if defined(DM_PLATFORM_OSX)

#include "../testext.h"

int test_callnative(lua_State *L)
{
	return 0;
}

int test_centerWindow(lua_State *L)
{
	float x,y;
	int w,h;
	NSWindow* window = dmGraphics::GetNativeOSXNSWindow();
	NSRect frame = window.screen.frame;
	w = frame.size.width;
	h = frame.size.height;

	x = floorf(frame.origin.x + (frame.size.width - w) * 0.5f);

	float win_y;
	win_y = floorf(frame.origin.y + (frame.size.height - h) * 0.5f);

	NSView* view = dmGraphics::GetNativeOSXNSView();
	NSRect viewFrame = [view convertRect: view.bounds toView: nil];
	NSRect windowFrame = window.frame;
	NSRect rect = NSMakeRect(
		x - viewFrame.origin.x,
		win_y - viewFrame.origin.x,
		w + viewFrame.origin.x + windowFrame.size.width - viewFrame.size.width,
		h + viewFrame.origin.y + windowFrame.size.height - viewFrame.size.height
	);
	[window setFrame: rect display:YES];
	return 0;
}

#endif
