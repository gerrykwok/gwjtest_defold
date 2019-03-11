#include "lua_unzip_auto.hpp"
#include "external_include.h"
#include "../UnzipUtil.h"

int lua_unzip_UnzipUtil_unzipOneFile(lua_State* tolua_S)
{
    int argc = 0;
    UnzipUtil* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnzipUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnzipUtil*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_unzip_UnzipUtil_unzipOneFile'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 5) 
    {
        const char* arg0;
        const char* arg1;
        bool arg2;
        const char* arg3;
        LUA_FUNCTION arg4;

        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "UnzipUtil:unzipOneFile"); arg0 = arg0_tmp.c_str();

        std::string arg1_tmp; ok &= luaval_to_std_string(tolua_S, 3, &arg1_tmp, "UnzipUtil:unzipOneFile"); arg1 = arg1_tmp.c_str();

        ok &= luaval_to_boolean(tolua_S, 4,&arg2, "UnzipUtil:unzipOneFile");

        std::string arg3_tmp; ok &= luaval_to_std_string(tolua_S, 5, &arg3_tmp, "UnzipUtil:unzipOneFile"); arg3 = arg3_tmp.c_str();

        arg4 = ext_registerLuaCallback(tolua_S, 6); ok &= arg4 >= 0;
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_unzip_UnzipUtil_unzipOneFile'", nullptr);
            return 0;
        }
        cobj->unzipOneFile(arg0, arg1, arg2, arg3, arg4);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "UnzipUtil:unzipOneFile",argc, 5);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_unzip_UnzipUtil_unzipOneFile'.",&tolua_err);
#endif

    return 0;
}
int lua_unzip_UnzipUtil_unzipAllFile(lua_State* tolua_S)
{
    int argc = 0;
    UnzipUtil* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnzipUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnzipUtil*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_unzip_UnzipUtil_unzipAllFile'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 4) 
    {
        const char* arg0;
        bool arg1;
        const char* arg2;
        LUA_FUNCTION arg3;

        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "UnzipUtil:unzipAllFile"); arg0 = arg0_tmp.c_str();

        ok &= luaval_to_boolean(tolua_S, 3,&arg1, "UnzipUtil:unzipAllFile");

        std::string arg2_tmp; ok &= luaval_to_std_string(tolua_S, 4, &arg2_tmp, "UnzipUtil:unzipAllFile"); arg2 = arg2_tmp.c_str();

        arg3 = ext_registerLuaCallback(tolua_S, 5); ok &= arg3 >= 0;
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_unzip_UnzipUtil_unzipAllFile'", nullptr);
            return 0;
        }
        cobj->unzipAllFile(arg0, arg1, arg2, arg3);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "UnzipUtil:unzipAllFile",argc, 4);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_unzip_UnzipUtil_unzipAllFile'.",&tolua_err);
#endif

    return 0;
}
int lua_unzip_UnzipUtil_getAllFiles(lua_State* tolua_S)
{
    int argc = 0;
    UnzipUtil* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"UnzipUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (UnzipUtil*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_unzip_UnzipUtil_getAllFiles'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        LUA_FUNCTION arg0;

        arg0 = ext_registerLuaCallback(tolua_S, 2); ok &= arg0 >= 0;
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_unzip_UnzipUtil_getAllFiles'", nullptr);
            return 0;
        }
        cobj->getAllFiles(arg0);
        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "UnzipUtil:getAllFiles",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_unzip_UnzipUtil_getAllFiles'.",&tolua_err);
#endif

    return 0;
}
int lua_unzip_UnzipUtil_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"UnzipUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        const char* arg0;
        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "UnzipUtil:create"); arg0 = arg0_tmp.c_str();
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_unzip_UnzipUtil_create'", nullptr);
            return 0;
        }
        UnzipUtil* ret = UnzipUtil::create(arg0);
        object_to_luaval<UnzipUtil>(tolua_S, "UnzipUtil",(UnzipUtil*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "UnzipUtil:create",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_unzip_UnzipUtil_create'.",&tolua_err);
#endif
    return 0;
}
int lua_unzip_UnzipUtil_createFromMem(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"UnzipUtil",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        LUA_BUFFER arg0;
        ok &= luaval_to_buffer(tolua_S, 2,(LUA_BUFFER *)&arg0, "UnzipUtil:createFromMem");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_unzip_UnzipUtil_createFromMem'", nullptr);
            return 0;
        }
        UnzipUtil* ret = UnzipUtil::createFromMem(arg0);
        object_to_luaval<UnzipUtil>(tolua_S, "UnzipUtil",(UnzipUtil*)ret);
        return 1;
    }
    if (argc == 2)
    {
        LUA_BUFFER arg0;
        bool arg1;
        ok &= luaval_to_buffer(tolua_S, 2,(LUA_BUFFER *)&arg0, "UnzipUtil:createFromMem");
        ok &= luaval_to_boolean(tolua_S, 3,&arg1, "UnzipUtil:createFromMem");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_unzip_UnzipUtil_createFromMem'", nullptr);
            return 0;
        }
        UnzipUtil* ret = UnzipUtil::createFromMem(arg0, arg1);
        object_to_luaval<UnzipUtil>(tolua_S, "UnzipUtil",(UnzipUtil*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "UnzipUtil:createFromMem",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_unzip_UnzipUtil_createFromMem'.",&tolua_err);
#endif
    return 0;
}
static int lua_unzip_UnzipUtil_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (UnzipUtil)");
    return 0;
}

int lua_register_unzip_UnzipUtil(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"UnzipUtil");
    tolua_cclass(tolua_S,"UnzipUtil","UnzipUtil","",nullptr);

    tolua_beginmodule(tolua_S,"UnzipUtil");
        tolua_function(tolua_S,"unzipOneFile",lua_unzip_UnzipUtil_unzipOneFile);
        tolua_function(tolua_S,"unzipAllFile",lua_unzip_UnzipUtil_unzipAllFile);
        tolua_function(tolua_S,"getAllFiles",lua_unzip_UnzipUtil_getAllFiles);
        tolua_function(tolua_S,"create", lua_unzip_UnzipUtil_create);
        tolua_function(tolua_S,"createFromMem", lua_unzip_UnzipUtil_createFromMem);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(UnzipUtil).name();
    g_luaType[typeName] = "UnzipUtil";
    g_typeCast["UnzipUtil"] = "UnzipUtil";
    return 1;
}
TOLUA_API int register_all_unzip(lua_State* tolua_S)
{
    lua_getglobal(tolua_S, "_G");
    if (lua_istable(tolua_S, -1))//stack:...,_G,
    {
        tolua_open(tolua_S);

        tolua_module(tolua_S,"unzip",0);
        tolua_beginmodule(tolua_S,"unzip");

        lua_register_unzip_UnzipUtil(tolua_S);

        tolua_endmodule(tolua_S);
    }
    lua_pop(tolua_S, 1);
    return 1;
}

