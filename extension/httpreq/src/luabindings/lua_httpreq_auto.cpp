#include "lua_httpreq_auto.hpp"
#include "external_include.h"
#include "../HTTPRequest.h"

int lua_httpreq_HTTPRequest_getErrorCode(lua_State* tolua_S)
{
    int argc = 0;
    HTTPRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"HTTPRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (HTTPRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_httpreq_HTTPRequest_getErrorCode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_httpreq_HTTPRequest_getErrorCode'", nullptr);
            return 0;
        }
        int ret = cobj->getErrorCode();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "HTTPRequest:getErrorCode",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_httpreq_HTTPRequest_getErrorCode'.",&tolua_err);
#endif

    return 0;
}
int lua_httpreq_HTTPRequest_addPOSTValue(lua_State* tolua_S)
{
    int argc = 0;
    HTTPRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"HTTPRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (HTTPRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_httpreq_HTTPRequest_addPOSTValue'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        const char* arg0;
        const char* arg1;

        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "HTTPRequest:addPOSTValue"); arg0 = arg0_tmp.c_str();

        std::string arg1_tmp; ok &= luaval_to_std_string(tolua_S, 3, &arg1_tmp, "HTTPRequest:addPOSTValue"); arg1 = arg1_tmp.c_str();
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_httpreq_HTTPRequest_addPOSTValue'", nullptr);
            return 0;
        }
        cobj->addPOSTValue(arg0, arg1);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "HTTPRequest:addPOSTValue",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_httpreq_HTTPRequest_addPOSTValue'.",&tolua_err);
#endif

    return 0;
}
int lua_httpreq_HTTPRequest_addRequestHeader(lua_State* tolua_S)
{
    int argc = 0;
    HTTPRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"HTTPRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (HTTPRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_httpreq_HTTPRequest_addRequestHeader'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        const char* arg0;

        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "HTTPRequest:addRequestHeader"); arg0 = arg0_tmp.c_str();
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_httpreq_HTTPRequest_addRequestHeader'", nullptr);
            return 0;
        }
        cobj->addRequestHeader(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "HTTPRequest:addRequestHeader",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_httpreq_HTTPRequest_addRequestHeader'.",&tolua_err);
#endif

    return 0;
}
int lua_httpreq_HTTPRequest_setReadTimeout(lua_State* tolua_S)
{
    int argc = 0;
    HTTPRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"HTTPRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (HTTPRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_httpreq_HTTPRequest_setReadTimeout'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "HTTPRequest:setReadTimeout");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_httpreq_HTTPRequest_setReadTimeout'", nullptr);
            return 0;
        }
        cobj->setReadTimeout(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "HTTPRequest:setReadTimeout",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_httpreq_HTTPRequest_setReadTimeout'.",&tolua_err);
#endif

    return 0;
}
int lua_httpreq_HTTPRequest_setPOSTData(lua_State* tolua_S)
{
    int argc = 0;
    HTTPRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"HTTPRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (HTTPRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_httpreq_HTTPRequest_setPOSTData'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        const char* arg0;

        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "HTTPRequest:setPOSTData"); arg0 = arg0_tmp.c_str();
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_httpreq_HTTPRequest_setPOSTData'", nullptr);
            return 0;
        }
        cobj->setPOSTData(arg0);
        return 0;
    }
    if (argc == 2) 
    {
        const char* arg0;
        unsigned int arg1;

        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "HTTPRequest:setPOSTData"); arg0 = arg0_tmp.c_str();

        ok &= luaval_to_uint32(tolua_S, 3,&arg1, "HTTPRequest:setPOSTData");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_httpreq_HTTPRequest_setPOSTData'", nullptr);
            return 0;
        }
        cobj->setPOSTData(arg0, arg1);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "HTTPRequest:setPOSTData",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_httpreq_HTTPRequest_setPOSTData'.",&tolua_err);
#endif

    return 0;
}
int lua_httpreq_HTTPRequest_getResponseStatusCode(lua_State* tolua_S)
{
    int argc = 0;
    HTTPRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"HTTPRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (HTTPRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_httpreq_HTTPRequest_getResponseStatusCode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_httpreq_HTTPRequest_getResponseStatusCode'", nullptr);
            return 0;
        }
        int ret = cobj->getResponseStatusCode();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "HTTPRequest:getResponseStatusCode",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_httpreq_HTTPRequest_getResponseStatusCode'.",&tolua_err);
#endif

    return 0;
}
int lua_httpreq_HTTPRequest_getResponseDataLua(lua_State* tolua_S)
{
    int argc = 0;
    HTTPRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"HTTPRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (HTTPRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_httpreq_HTTPRequest_getResponseDataLua'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_httpreq_HTTPRequest_getResponseDataLua'", nullptr);
            return 0;
        }
        LUA_BUFFER ret = cobj->getResponseDataLua();
        lua_pushlstring(tolua_S, (const char*)ret.ptr, ret.size);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "HTTPRequest:getResponseDataLua",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_httpreq_HTTPRequest_getResponseDataLua'.",&tolua_err);
#endif

    return 0;
}
int lua_httpreq_HTTPRequest_getResponseString(lua_State* tolua_S)
{
    int argc = 0;
    HTTPRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"HTTPRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (HTTPRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_httpreq_HTTPRequest_getResponseString'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_httpreq_HTTPRequest_getResponseString'", nullptr);
            return 0;
        }
        const std::string ret = cobj->getResponseString();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "HTTPRequest:getResponseString",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_httpreq_HTTPRequest_getResponseString'.",&tolua_err);
#endif

    return 0;
}
int lua_httpreq_HTTPRequest_setCookieString(lua_State* tolua_S)
{
    int argc = 0;
    HTTPRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"HTTPRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (HTTPRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_httpreq_HTTPRequest_setCookieString'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        const char* arg0;

        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "HTTPRequest:setCookieString"); arg0 = arg0_tmp.c_str();
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_httpreq_HTTPRequest_setCookieString'", nullptr);
            return 0;
        }
        cobj->setCookieString(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "HTTPRequest:setCookieString",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_httpreq_HTTPRequest_setCookieString'.",&tolua_err);
#endif

    return 0;
}
int lua_httpreq_HTTPRequest_getCookieString(lua_State* tolua_S)
{
    int argc = 0;
    HTTPRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"HTTPRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (HTTPRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_httpreq_HTTPRequest_getCookieString'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_httpreq_HTTPRequest_getCookieString'", nullptr);
            return 0;
        }
        const std::string ret = cobj->getCookieString();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "HTTPRequest:getCookieString",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_httpreq_HTTPRequest_getCookieString'.",&tolua_err);
#endif

    return 0;
}
int lua_httpreq_HTTPRequest_start(lua_State* tolua_S)
{
    int argc = 0;
    HTTPRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"HTTPRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (HTTPRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_httpreq_HTTPRequest_start'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_httpreq_HTTPRequest_start'", nullptr);
            return 0;
        }
        cobj->start();
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "HTTPRequest:start",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_httpreq_HTTPRequest_start'.",&tolua_err);
#endif

    return 0;
}
int lua_httpreq_HTTPRequest_getResponseHeadersString(lua_State* tolua_S)
{
    int argc = 0;
    HTTPRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"HTTPRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (HTTPRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_httpreq_HTTPRequest_getResponseHeadersString'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_httpreq_HTTPRequest_getResponseHeadersString'", nullptr);
            return 0;
        }
        const std::string ret = cobj->getResponseHeadersString();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "HTTPRequest:getResponseHeadersString",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_httpreq_HTTPRequest_getResponseHeadersString'.",&tolua_err);
#endif

    return 0;
}
int lua_httpreq_HTTPRequest_getState(lua_State* tolua_S)
{
    int argc = 0;
    HTTPRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"HTTPRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (HTTPRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_httpreq_HTTPRequest_getState'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_httpreq_HTTPRequest_getState'", nullptr);
            return 0;
        }
        int ret = cobj->getState();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "HTTPRequest:getState",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_httpreq_HTTPRequest_getState'.",&tolua_err);
#endif

    return 0;
}
int lua_httpreq_HTTPRequest_getErrorMessage(lua_State* tolua_S)
{
    int argc = 0;
    HTTPRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"HTTPRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (HTTPRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_httpreq_HTTPRequest_getErrorMessage'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_httpreq_HTTPRequest_getErrorMessage'", nullptr);
            return 0;
        }
        const std::string ret = cobj->getErrorMessage();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "HTTPRequest:getErrorMessage",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_httpreq_HTTPRequest_getErrorMessage'.",&tolua_err);
#endif

    return 0;
}
int lua_httpreq_HTTPRequest_saveResponseData(lua_State* tolua_S)
{
    int argc = 0;
    HTTPRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"HTTPRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (HTTPRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_httpreq_HTTPRequest_saveResponseData'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        const char* arg0;

        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "HTTPRequest:saveResponseData"); arg0 = arg0_tmp.c_str();
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_httpreq_HTTPRequest_saveResponseData'", nullptr);
            return 0;
        }
        unsigned int ret = cobj->saveResponseData(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "HTTPRequest:saveResponseData",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_httpreq_HTTPRequest_saveResponseData'.",&tolua_err);
#endif

    return 0;
}
int lua_httpreq_HTTPRequest_cancel(lua_State* tolua_S)
{
    int argc = 0;
    HTTPRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"HTTPRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (HTTPRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_httpreq_HTTPRequest_cancel'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_httpreq_HTTPRequest_cancel'", nullptr);
            return 0;
        }
        cobj->cancel();
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "HTTPRequest:cancel",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_httpreq_HTTPRequest_cancel'.",&tolua_err);
#endif

    return 0;
}
int lua_httpreq_HTTPRequest_setConnectTimeout(lua_State* tolua_S)
{
    int argc = 0;
    HTTPRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"HTTPRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (HTTPRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_httpreq_HTTPRequest_setConnectTimeout'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "HTTPRequest:setConnectTimeout");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_httpreq_HTTPRequest_setConnectTimeout'", nullptr);
            return 0;
        }
        cobj->setConnectTimeout(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "HTTPRequest:setConnectTimeout",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_httpreq_HTTPRequest_setConnectTimeout'.",&tolua_err);
#endif

    return 0;
}
int lua_httpreq_HTTPRequest_setTimeout(lua_State* tolua_S)
{
    int argc = 0;
    HTTPRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"HTTPRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (HTTPRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_httpreq_HTTPRequest_setTimeout'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "HTTPRequest:setTimeout");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_httpreq_HTTPRequest_setTimeout'", nullptr);
            return 0;
        }
        cobj->setTimeout(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "HTTPRequest:setTimeout",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_httpreq_HTTPRequest_setTimeout'.",&tolua_err);
#endif

    return 0;
}
int lua_httpreq_HTTPRequest_getResponseDataLength(lua_State* tolua_S)
{
    int argc = 0;
    HTTPRequest* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"HTTPRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (HTTPRequest*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_httpreq_HTTPRequest_getResponseDataLength'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_httpreq_HTTPRequest_getResponseDataLength'", nullptr);
            return 0;
        }
        int ret = cobj->getResponseDataLength();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "HTTPRequest:getResponseDataLength",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_httpreq_HTTPRequest_getResponseDataLength'.",&tolua_err);
#endif

    return 0;
}
int lua_httpreq_HTTPRequest_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"HTTPRequest",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 3)
    {
        LUA_FUNCTION arg0;
        const char* arg1;
        const char* arg2;
        arg0 = ext_registerLuaCallback(tolua_S, 2); ok &= arg0 >= 0;
        std::string arg1_tmp; ok &= luaval_to_std_string(tolua_S, 3, &arg1_tmp, "HTTPRequest:create"); arg1 = arg1_tmp.c_str();
        std::string arg2_tmp; ok &= luaval_to_std_string(tolua_S, 4, &arg2_tmp, "HTTPRequest:create"); arg2 = arg2_tmp.c_str();
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_httpreq_HTTPRequest_create'", nullptr);
            return 0;
        }
        HTTPRequest* ret = HTTPRequest::create(arg0, arg1, arg2);
        object_to_luaval<HTTPRequest>(tolua_S, "HTTPRequest",(HTTPRequest*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "HTTPRequest:create",argc, 3);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_httpreq_HTTPRequest_create'.",&tolua_err);
#endif
    return 0;
}
static int lua_httpreq_HTTPRequest_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (HTTPRequest)");
    return 0;
}

int lua_register_httpreq_HTTPRequest(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"HTTPRequest");
    tolua_cclass(tolua_S,"HTTPRequest","HTTPRequest","",nullptr);

    tolua_beginmodule(tolua_S,"HTTPRequest");
        tolua_function(tolua_S,"getErrorCode",lua_httpreq_HTTPRequest_getErrorCode);
        tolua_function(tolua_S,"addPOSTValue",lua_httpreq_HTTPRequest_addPOSTValue);
        tolua_function(tolua_S,"addRequestHeader",lua_httpreq_HTTPRequest_addRequestHeader);
        tolua_function(tolua_S,"setReadTimeout",lua_httpreq_HTTPRequest_setReadTimeout);
        tolua_function(tolua_S,"setPOSTData",lua_httpreq_HTTPRequest_setPOSTData);
        tolua_function(tolua_S,"getResponseStatusCode",lua_httpreq_HTTPRequest_getResponseStatusCode);
        tolua_function(tolua_S,"getResponseDataLua",lua_httpreq_HTTPRequest_getResponseDataLua);
        tolua_function(tolua_S,"getResponseString",lua_httpreq_HTTPRequest_getResponseString);
        tolua_function(tolua_S,"setCookieString",lua_httpreq_HTTPRequest_setCookieString);
        tolua_function(tolua_S,"getCookieString",lua_httpreq_HTTPRequest_getCookieString);
        tolua_function(tolua_S,"start",lua_httpreq_HTTPRequest_start);
        tolua_function(tolua_S,"getResponseHeadersString",lua_httpreq_HTTPRequest_getResponseHeadersString);
        tolua_function(tolua_S,"getState",lua_httpreq_HTTPRequest_getState);
        tolua_function(tolua_S,"getErrorMessage",lua_httpreq_HTTPRequest_getErrorMessage);
        tolua_function(tolua_S,"saveResponseData",lua_httpreq_HTTPRequest_saveResponseData);
        tolua_function(tolua_S,"cancel",lua_httpreq_HTTPRequest_cancel);
        tolua_function(tolua_S,"setConnectTimeout",lua_httpreq_HTTPRequest_setConnectTimeout);
        tolua_function(tolua_S,"setTimeout",lua_httpreq_HTTPRequest_setTimeout);
        tolua_function(tolua_S,"getResponseDataLength",lua_httpreq_HTTPRequest_getResponseDataLength);
        tolua_function(tolua_S,"create", lua_httpreq_HTTPRequest_create);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(HTTPRequest).name();
    g_luaType[typeName] = "HTTPRequest";
    g_typeCast["HTTPRequest"] = "HTTPRequest";
    return 1;
}
TOLUA_API int register_all_httpreq(lua_State* tolua_S)
{
    lua_getglobal(tolua_S, "_G");
    if (lua_istable(tolua_S, -1))//stack:...,_G,
    {
        tolua_open(tolua_S);

        tolua_module(tolua_S,"httpreq",0);
        tolua_beginmodule(tolua_S,"httpreq");

        lua_register_httpreq_HTTPRequest(tolua_S);

        tolua_endmodule(tolua_S);
    }
    lua_pop(tolua_S, 1);
    return 1;
}

