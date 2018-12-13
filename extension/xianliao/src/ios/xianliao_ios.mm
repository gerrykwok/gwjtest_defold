#if defined(DM_PLATFORM_IOS)

#include "../xianliao.h"
#import "XianliaoUtil.h"

void xianliao_onAppInit()
{
	[XianliaoUtil initXianliao:@XIANLIAO_APPID];
}

int xianliao_login(lua_State *L)
{
	return 0;
}

int xianliao_logout(lua_State *L)
{
	return 0;
}

int xianliao_share(lua_State *L)
{
	return 0;
}

#endif