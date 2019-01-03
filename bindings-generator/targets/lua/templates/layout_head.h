#if $macro_judgement
$macro_judgement
#end if 
\#ifndef __${prefix}_h__
\#define __${prefix}_h__

\#include <dmsdk/sdk.h>

int register_all_${prefix}(lua_State* tolua_S);
