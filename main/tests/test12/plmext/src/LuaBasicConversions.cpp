/****************************************************************************
 Copyright (c) 2013-2014 Chukong Technologies Inc.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "LuaBasicConversions.h"
#include "tolua_fix.h"

#ifndef CCASSERT
#define CCASSERT(cond, msg) do {                              \
          if (!(cond)) {                                          \
              printf("Assert failed: %s\n", msg);             \
            assert(cond);                                      \
		            } \
		        } while (0)
#endif


//g std::unordered_map<std::string, std::string>  g_luaType;
//g std::unordered_map<std::string, std::string>  g_typeCast;

#if COCOS2D_DEBUG >=1
void luaval_to_native_err(lua_State* L,const char* msg,tolua_Error* err, const char* funcName)
{
    if (NULL == L || NULL == err || NULL == msg || 0 == strlen(msg))
        return;

    if (msg[0] == '#')
    {
        const char* expected = err->type;
        const char* provided = tolua_typename(L,err->index);
        if (msg[1]=='f')
        {
            int narg = err->index;
            if (err->array)
                CCLOG("%s\n     %s argument #%d is array of '%s'; array of '%s' expected.\n",msg+2,funcName,narg,provided,expected);
            else
                CCLOG("%s\n     %s argument #%d is '%s'; '%s' expected.\n",msg+2,funcName,narg,provided,expected);
        }
        else if (msg[1]=='v')
        {
            if (err->array)
                CCLOG("%s\n     %s value is array of '%s'; array of '%s' expected.\n",funcName,msg+2,provided,expected);
            else
                CCLOG("%s\n     %s value is '%s'; '%s' expected.\n",msg+2,funcName,provided,expected);
        }
    }
}
#endif

//#ifdef __cplusplus
//extern "C" {
//#endif
extern int lua_isusertype (lua_State* L, int lo, const char* type);
//#ifdef __cplusplus
//}
//#endif

bool luaval_is_usertype(lua_State* L,int lo,const char* type, int def)
{
    if (def && lua_gettop(L)<abs(lo))
        return true;
    
    if (lua_isnil(L,lo) || lua_isusertype(L,lo,type))
        return true;
    
    return false;
}

bool luaval_to_ushort(lua_State* L, int lo, unsigned short* outValue, const char* funcName)
{
    if (NULL == L || NULL == outValue)
        return false;
    
    bool ok = true;
    
    tolua_Error tolua_err;
    if (!tolua_isnumber(L,lo,0,&tolua_err))
    {
#if COCOS2D_DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }
    
    if (ok)
    {
        *outValue = (unsigned short)tolua_tonumber(L, lo, 0);
    }
    
    return ok;
}


bool luaval_to_int32(lua_State* L,int lo,int* outValue, const char* funcName)
{
    if (NULL == L || NULL == outValue)
        return false;
    
    bool ok = true;

    tolua_Error tolua_err;
    if (!tolua_isnumber(L,lo,0,&tolua_err))
    {
#if COCOS2D_DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }
    
    if (ok)
    {
        *outValue = (int)tolua_tonumber(L, lo, 0);
    }
    
    return ok;
}

bool luaval_to_uint32(lua_State* L, int lo, unsigned int* outValue, const char* funcName)
{
    if (NULL == L || NULL == outValue)
        return false;
    
    bool ok = true;

    tolua_Error tolua_err;
    if (!tolua_isnumber(L,lo,0,&tolua_err))
    {
#if COCOS2D_DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }
    
    if (ok)
    {
        *outValue = (unsigned int)tolua_tonumber(L, lo, 0);
    }
    
    return ok;
}

bool luaval_to_uint16(lua_State* L,int lo,uint16_t* outValue, const char* funcName)
{
    if (NULL == L || NULL == outValue)
        return false;
    
    bool ok = true;

    tolua_Error tolua_err;
    if (!tolua_isnumber(L,lo,0,&tolua_err))
    {
#if COCOS2D_DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }
    
    if (ok)
    {
        *outValue = (unsigned char)tolua_tonumber(L, lo, 0);
    }
    
    return ok;
}

bool luaval_to_boolean(lua_State* L,int lo,bool* outValue, const char* funcName)
{
    if (NULL == L || NULL == outValue)
        return false;
    
    bool ok = true;

    tolua_Error tolua_err;
    if (!tolua_isboolean(L,lo,0,&tolua_err))
    {
#if COCOS2D_DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }
    
    if (ok)
    {
        *outValue = (bool)tolua_toboolean(L, lo, 0);
    }
    
    return ok;
}

bool luaval_to_number(lua_State* L,int lo,double* outValue, const char* funcName)
{
    if (NULL == L || NULL == outValue)
        return false;
    
    bool ok = true;

    tolua_Error tolua_err;
    if (!tolua_isnumber(L,lo,0,&tolua_err))
    {
#if COCOS2D_DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }
    
    if (ok)
    {
        *outValue = tolua_tonumber(L, lo, 0);
    }
    
    return ok;
}

bool luaval_to_long_long(lua_State* L,int lo,long long* outValue, const char* funcName)
{
    if (NULL == L || NULL == outValue)
        return false;
    
    bool ok = true;

    tolua_Error tolua_err;
    if (!tolua_isnumber(L,lo,0,&tolua_err))
    {
#if COCOS2D_DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }
    
    if (ok)
    {
        *outValue = (long long)tolua_tonumber(L, lo, 0);
    }
    
    return ok;
}

bool luaval_to_std_string(lua_State* L, int lo, std::string* outValue, const char* funcName)
{
    if (NULL == L || NULL == outValue)
        return false;
    
    bool ok = true;

    tolua_Error tolua_err;
    if (!tolua_iscppstring(L,lo,0,&tolua_err))
    {
#if COCOS2D_DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }
    
    if (ok)
    {
        *outValue = tolua_tocppstring(L,lo,NULL);
    }
    
    return ok;
}

//g bool luaval_to_vec2(lua_State* L,int lo,cocos2d::Vec2* outValue, const char* funcName)
//g {
//g     if (nullptr == L || nullptr == outValue)
//g         return false;
//g     
//g     bool ok = true;
//g     
//g     tolua_Error tolua_err;
//g     if (!tolua_istable(L, lo, 0, &tolua_err) )
//g     {
//g #if COCOS2D_DEBUG >=1
//g         luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
//g #endif
//g         ok = false;
//g     }
//g     
//g     
//g     if (ok)
//g     {
//g         lua_pushstring(L, "x");
//g         lua_gettable(L, lo);
//g         outValue->x = lua_isnil(L, -1) ? 0 : lua_tonumber(L, -1);
//g         lua_pop(L, 1);
//g         
//g         lua_pushstring(L, "y");
//g         lua_gettable(L, lo);
//g         outValue->y = lua_isnil(L, -1) ? 0 : lua_tonumber(L, -1);
//g         lua_pop(L, 1);
//g     }
//g     return ok;
//g }
//g 
//g bool luaval_to_vec3(lua_State* L,int lo,cocos2d::Vec3* outValue, const char* funcName)
//g {
//g     if (nullptr == L || nullptr == outValue)
//g         return false;
//g     
//g     bool ok = true;
//g     
//g     tolua_Error tolua_err;
//g     if (!tolua_istable(L, lo, 0, &tolua_err) )
//g     {
//g #if COCOS2D_DEBUG >=1
//g         luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
//g #endif
//g         ok = false;
//g     }
//g     
//g     
//g     if (ok)
//g     {
//g         lua_pushstring(L, "x");
//g         lua_gettable(L, lo);
//g         outValue->x = lua_isnil(L, -1) ? 0 : lua_tonumber(L, -1);
//g         lua_pop(L, 1);
//g         
//g         lua_pushstring(L, "y");
//g         lua_gettable(L, lo);
//g         outValue->y = lua_isnil(L, -1) ? 0 : lua_tonumber(L, -1);
//g         lua_pop(L, 1);
//g         
//g         lua_pushstring(L, "z");
//g         lua_gettable(L, lo);
//g         outValue->z = lua_isnil(L, -1) ? 0 : lua_tonumber(L, -1);
//g         lua_pop(L, 1);
//g     }
//g     return ok;
//g }
//g 
//g bool luaval_to_vec4(lua_State* L,int lo,cocos2d::Vec4* outValue, const char* funcName)
//g {
//g     if (nullptr == L || nullptr == outValue)
//g         return false;
//g     
//g     bool ok = true;
//g     
//g     tolua_Error tolua_err;
//g     if (!tolua_istable(L, lo, 0, &tolua_err) )
//g     {
//g #if COCOS2D_DEBUG >=1
//g         luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
//g #endif
//g         ok = false;
//g     }
//g     
//g     
//g     if (ok)
//g     {
//g         lua_pushstring(L, "x");
//g         lua_gettable(L, lo);
//g         outValue->x = lua_isnil(L, -1) ? 0 : lua_tonumber(L, -1);
//g         lua_pop(L, 1);
//g         
//g         lua_pushstring(L, "y");
//g         lua_gettable(L, lo);
//g         outValue->y = lua_isnil(L, -1) ? 0 : lua_tonumber(L, -1);
//g         lua_pop(L, 1);
//g         
//g         lua_pushstring(L, "z");
//g         lua_gettable(L, lo);
//g         outValue->z = lua_isnil(L, -1) ? 0 : lua_tonumber(L, -1);
//g         lua_pop(L, 1);
//g         
//g         lua_pushstring(L, "w");
//g         lua_gettable(L, lo);
//g         outValue->w = lua_isnil(L, -1) ? 0 : lua_tonumber(L, -1);
//g         lua_pop(L, 1);
//g     }
//g     return ok;
//g }
//g 
//g bool luaval_to_blendfunc(lua_State* L, int lo, cocos2d::BlendFunc* outValue, const char* funcName)
//g {
//g     if (nullptr == L || nullptr == outValue)
//g         return false;
//g     
//g     bool ok = true;
//g     
//g     tolua_Error tolua_err;
//g     if (!tolua_istable(L, lo, 0, &tolua_err) )
//g     {
//g #if COCOS2D_DEBUG >=1
//g         luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
//g #endif
//g         ok = false;
//g     }
//g     
//g     
//g     if (ok)
//g     {
//g         lua_pushstring(L, "src");
//g         lua_gettable(L, lo);
//g         outValue->src = lua_isnil(L, -1) ? 0 : lua_tonumber(L, -1);
//g         lua_pop(L, 1);
//g         
//g         lua_pushstring(L, "dst");
//g         lua_gettable(L, lo);
//g         outValue->dst = lua_isnil(L, -1) ? 0 : lua_tonumber(L, -1);
//g         lua_pop(L, 1);
//g     }
//g     return ok;
//g }
//g #if CC_USE_PHYSICS
//g bool luaval_to_physics_material(lua_State* L,int lo,PhysicsMaterial* outValue, const char* funcName)
//g {
//g     if (NULL == L || NULL == outValue)
//g         return false;
//g     
//g     bool ok = true;
//g     
//g     tolua_Error tolua_err;
//g     if (!tolua_istable(L, lo, 0, &tolua_err) )
//g     {
//g #if COCOS2D_DEBUG >=1
//g         luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
//g #endif
//g         ok = false;
//g     }
//g     
//g     
//g     if (ok)
//g     {
//g         lua_pushstring(L, "density");
//g         lua_gettable(L, lo);
//g         outValue->density = lua_isnil(L, -1) ? 0 : lua_tonumber(L, -1);
//g         lua_pop(L, 1);
//g         
//g         lua_pushstring(L, "restitution");
//g         lua_gettable(L, lo);
//g         outValue->restitution = lua_isnil(L, -1) ? 0 : lua_tonumber(L, -1);
//g         lua_pop(L, 1);
//g         
//g         lua_pushstring(L, "friction");
//g         lua_gettable(L, lo);
//g         outValue->friction = lua_isnil(L, -1) ? 0 : lua_tonumber(L, -1);
//g         lua_pop(L, 1);
//g     }
//g     return ok;
//g }
//g #endif //CC_USE_PHYSICS
//g bool luaval_to_ssize(lua_State* L,int lo, ssize_t* outValue, const char* funcName)
//g {
//g     return luaval_to_long(L, lo, reinterpret_cast<long*>(outValue));
//g }

bool luaval_to_long(lua_State* L,int lo, long* outValue, const char* funcName)
{
    if (NULL == L || NULL == outValue)
        return false;
    
    bool ok = true;
    
    tolua_Error tolua_err;
    if (!tolua_isnumber(L,lo,0,&tolua_err))
    {
#if COCOS2D_DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }
    
    if (ok)
    {
        *outValue = (long)tolua_tonumber(L, lo, 0);
    }
    
    return ok;
}

bool luaval_to_ulong(lua_State* L,int lo, unsigned long* outValue, const char* funcName)
{
    if (NULL == L || NULL == outValue)
        return false;
    
    bool ok = true;
    
    tolua_Error tolua_err;
    if (!tolua_isnumber(L,lo,0,&tolua_err))
    {
#if COCOS2D_DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }
    
    if (ok)
    {
        *outValue = (unsigned long)tolua_tonumber(L, lo, 0);
    }
    
    return ok;
}

//g bool luaval_to_size(lua_State* L,int lo,Size* outValue, const char* funcName)
//g {
//g     if (NULL == L || NULL == outValue)
//g         return false;
//g     
//g     bool ok = true;
//g 
//g     tolua_Error tolua_err;
//g     if (!tolua_istable(L, lo, 0, &tolua_err) )
//g     {
//g #if COCOS2D_DEBUG >=1
//g         luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
//g #endif
//g         ok = false;
//g     }
//g     
//g     if (ok)
//g     {
//g         lua_pushstring(L, "width");  /* L: paramStack key */
//g         lua_gettable(L,lo);/* L: paramStack paramStack[lo][key] */
//g         outValue->width = lua_isnil(L,-1) ? 0 : lua_tonumber(L,-1);
//g         lua_pop(L,1);/* L: paramStack*/
//g         
//g         lua_pushstring(L, "height");
//g         lua_gettable(L,lo);
//g         outValue->height = lua_isnil(L,-1) ? 0 : lua_tonumber(L,-1);
//g         lua_pop(L,1);
//g     }
//g     
//g     return ok;
//g }
//g 
//g bool luaval_to_rect(lua_State* L,int lo,Rect* outValue, const char* funcName)
//g {
//g     if (NULL == L || NULL == outValue)
//g         return false;
//g     
//g     bool ok = true;
//g 
//g     tolua_Error tolua_err;
//g     if (!tolua_istable(L, lo, 0, &tolua_err) )
//g     {
//g #if COCOS2D_DEBUG >=1
//g         luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
//g #endif
//g         ok = false;
//g     }
//g     
//g     if (ok)
//g     {
//g         lua_pushstring(L, "x");
//g         lua_gettable(L,lo);
//g         outValue->origin.x = lua_isnil(L,-1) ? 0 : lua_tonumber(L,-1);
//g         lua_pop(L,1);
//g         
//g         lua_pushstring(L, "y");
//g         lua_gettable(L,lo);
//g         outValue->origin.y = lua_isnil(L,-1) ? 0 : lua_tonumber(L,-1);
//g         lua_pop(L,1);
//g         
//g         lua_pushstring(L, "width");
//g         lua_gettable(L,lo);
//g         outValue->size.width = lua_isnil(L,-1) ? 0 : lua_tonumber(L,-1);
//g         lua_pop(L,1);
//g         
//g         lua_pushstring(L, "height");
//g         lua_gettable(L,lo);
//g         outValue->size.height = lua_isnil(L,-1) ? 0 : lua_tonumber(L,-1);
//g         lua_pop(L,1);
//g     }
//g     
//g     return ok;
//g }
//g 
//g bool luaval_to_color4b(lua_State* L,int lo,Color4B* outValue, const char* funcName)
//g {
//g     if (NULL == L || NULL == outValue)
//g         return false;
//g     
//g     bool ok = true;
//g 
//g     tolua_Error tolua_err;
//g     if (!tolua_istable(L, lo, 0, &tolua_err) )
//g     {
//g #if COCOS2D_DEBUG >=1
//g         luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
//g #endif
//g         ok = false;
//g     }
//g     
//g     if(ok)
//g     {
//g         lua_pushstring(L, "r");
//g         lua_gettable(L,lo);
//g         outValue->r = lua_isnil(L,-1) ? 0 : lua_tonumber(L,-1);
//g         lua_pop(L,1);
//g         
//g         lua_pushstring(L, "g");
//g         lua_gettable(L,lo);
//g         outValue->g = lua_isnil(L,-1) ? 0 : lua_tonumber(L,-1);
//g         lua_pop(L,1);
//g         
//g         lua_pushstring(L, "b");
//g         lua_gettable(L,lo);
//g         outValue->b = lua_isnil(L,-1) ? 0 : lua_tonumber(L,-1);
//g         lua_pop(L,1);
//g         
//g         lua_pushstring(L, "a");
//g         lua_gettable(L,lo);
//g         outValue->a = lua_isnil(L,-1) ? 0 : lua_tonumber(L,-1);
//g         lua_pop(L,1);
//g     }
//g     
//g     return ok;
//g }
//g 
//g bool luaval_to_color4f(lua_State* L,int lo,Color4F* outValue, const char* funcName)
//g {
//g     if (NULL == L || NULL == outValue)
//g         return false;
//g     
//g     bool ok = true;
//g 
//g     tolua_Error tolua_err;
//g     if (!tolua_istable(L, lo, 0, &tolua_err) )
//g     {
//g #if COCOS2D_DEBUG >=1
//g         luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
//g #endif
//g         ok = false;
//g     }
//g     
//g     if (ok)
//g     {
//g         lua_pushstring(L, "r");
//g         lua_gettable(L,lo);
//g         outValue->r = lua_isnil(L,-1) ? 0 : lua_tonumber(L,-1);
//g         lua_pop(L,1);
//g         
//g         lua_pushstring(L, "g");
//g         lua_gettable(L,lo);
//g         outValue->g = lua_isnil(L,-1) ? 0 : lua_tonumber(L,-1);
//g         lua_pop(L,1);
//g         
//g         lua_pushstring(L, "b");
//g         lua_gettable(L,lo);
//g         outValue->b = lua_isnil(L,-1) ? 0 : lua_tonumber(L,-1);
//g         lua_pop(L,1);
//g         
//g         lua_pushstring(L, "a");
//g         lua_gettable(L,lo);
//g         outValue->a = lua_isnil(L,-1) ? 0 : lua_tonumber(L,-1);
//g         lua_pop(L,1);
//g     }
//g     
//g     return ok;
//g }
//g 
//g bool luaval_to_color3b(lua_State* L,int lo,Color3B* outValue, const char* funcName)
//g {
//g     if (NULL == L || NULL == outValue)
//g         return false;
//g     
//g     bool ok = true;
//g 
//g     tolua_Error tolua_err;
//g     if (!tolua_istable(L, lo, 0, &tolua_err) )
//g     {
//g #if COCOS2D_DEBUG >=1
//g         luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
//g #endif
//g         ok = false;
//g     }
//g     
//g     if (ok)
//g     {
//g         lua_pushstring(L, "r");
//g         lua_gettable(L,lo);
//g         outValue->r = lua_isnil(L,-1) ? 0 : lua_tonumber(L,-1);
//g         lua_pop(L,1);
//g         
//g         lua_pushstring(L, "g");
//g         lua_gettable(L,lo);
//g         outValue->g = lua_isnil(L,-1) ? 0 : lua_tonumber(L,-1);
//g         lua_pop(L,1);
//g         
//g         lua_pushstring(L, "b");
//g         lua_gettable(L,lo);
//g         outValue->b = lua_isnil(L,-1) ? 0 : lua_tonumber(L,-1);
//g         lua_pop(L,1);
//g     }
//g     
//g     return ok;
//g }
//g 
//g bool luaval_to_affinetransform(lua_State* L,int lo, AffineTransform* outValue, const char* funcName)
//g {
//g     if (NULL == L || NULL == outValue)
//g         return false;
//g     
//g     bool ok = true;
//g 
//g     tolua_Error tolua_err;
//g     if (!tolua_istable(L, lo, 0, &tolua_err) )
//g     {
//g #if COCOS2D_DEBUG >=1
//g         luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
//g #endif
//g         ok = false;
//g     }
//g     
//g     if (ok)
//g     {
//g         do
//g         {
//g             lua_pushstring(L, "a");
//g             lua_gettable(L,lo);
//g             if (lua_isnil(L,-1))
//g             {
//g                 ok = false;
//g                 lua_pop(L, 1);
//g                 break;
//g             }
//g             outValue->a = (float)lua_tonumber(L,-1);
//g             
//g             lua_pushstring(L, "b");
//g             lua_gettable(L,lo);
//g             if (lua_isnil(L,-1))
//g             {
//g                 ok = false;
//g                 lua_pop(L, 1);
//g                 break;
//g             }
//g             outValue->b = (float)lua_tonumber(L,-1);
//g             lua_pop(L,1);
//g             
//g             lua_pushstring(L, "c");
//g             lua_gettable(L,lo);
//g             if (lua_isnil(L,-1))
//g             {
//g                 ok = false;
//g                 lua_pop(L, 1);
//g                 break;
//g             }
//g             outValue->c =(float)lua_tonumber(L,-1);
//g             lua_pop(L,1);
//g             
//g             lua_pushstring(L, "d");
//g             lua_gettable(L,lo);
//g             if (lua_isnil(L,-1))
//g             {
//g                 ok = false;
//g                 lua_pop(L, 1);
//g                 break;
//g             }
//g             outValue->d = (float)lua_tonumber(L,-1);
//g             lua_pop(L,1);
//g             
//g             lua_pushstring(L, "tx");
//g             lua_gettable(L,lo);
//g             if (lua_isnil(L,-1))
//g             {
//g                 ok = false;
//g                 lua_pop(L, 1);
//g                 break;
//g             }
//g             outValue->tx = lua_isnil(L,-1) ? 0 : (float)lua_tonumber(L,-1);
//g             lua_pop(L,1);
//g             
//g             lua_pushstring(L, "ty");
//g             lua_gettable(L,lo);
//g             if (lua_isnil(L,-1))
//g             {
//g                 ok = false;
//g                 lua_pop(L, 1);
//g                 break;
//g             }
//g             outValue->ty = lua_isnil(L,-1) ? 0 : (float)lua_tonumber(L,-1);
//g             lua_pop(L,1);
//g         }
//g         while (0);
//g     }
//g     return ok;
//g }
//g 
//g bool luaval_to_fontdefinition(lua_State* L, int lo, FontDefinition* outValue , const char* funcName)
//g {
//g     if (NULL == L || NULL == outValue)
//g         return false;
//g     
//g     bool ok = true;
//g 
//g     tolua_Error tolua_err;
//g     if (!tolua_istable(L, lo, 0, &tolua_err) )
//g     {
//g #if COCOS2D_DEBUG >=1
//g         luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
//g         ok = false;
//g #endif
//g     }
//g     
//g     if (ok)
//g     {
//g         // defaul values
//g         const char *            defautlFontName         = "Arial";
//g         const int               defaultFontSize         = 32;
//g         TextHAlignment          defaultTextAlignment    = TextHAlignment::LEFT;
//g         TextVAlignment          defaultTextVAlignment   = TextVAlignment::TOP;
//g         
//g         // by default shadow and stroke are off
//g         outValue->_shadow._shadowEnabled = false;
//g         outValue->_stroke._strokeEnabled = false;
//g         
//g         // white text by default
//g         outValue->_fontFillColor = Color3B::WHITE;
//g         
//g         lua_pushstring(L, "fontName");
//g         lua_gettable(L,lo);
//g         outValue->_fontName = tolua_tocppstring(L,lo,defautlFontName);
//g         lua_pop(L,1);
//g         
//g         lua_pushstring(L, "fontSize");
//g         lua_gettable(L,lo);
//g         outValue->_fontSize = lua_isnil(L,-1) ? defaultFontSize : (int)lua_tonumber(L,-1);
//g         lua_pop(L,1);
//g         
//g         lua_pushstring(L, "fontAlignmentH");
//g         lua_gettable(L,lo);
//g         outValue->_alignment = lua_isnil(L,-1) ? defaultTextAlignment : (TextHAlignment)(int)lua_tonumber(L,-1);
//g         lua_pop(L,1);
//g         
//g         lua_pushstring(L, "fontAlignmentV");
//g         lua_gettable(L,lo);
//g         outValue->_vertAlignment = lua_isnil(L,-1) ? defaultTextVAlignment : (TextVAlignment)(int)lua_tonumber(L,-1);
//g         lua_pop(L,1);
//g         
//g         lua_pushstring(L, "fontFillColor");
//g         lua_gettable(L,lo);
//g         if (!lua_isnil(L,-1))
//g         {
//g             luaval_to_color3b(L, -1, &outValue->_fontFillColor);
//g         }
//g         lua_pop(L,1);
//g         
//g         lua_pushstring(L, "fontDimensions");
//g         lua_gettable(L,lo);
//g         if (!lua_isnil(L,-1))
//g         {
//g             luaval_to_size(L, -1, &outValue->_dimensions);
//g         }
//g         lua_pop(L,1);
//g         
//g         lua_pushstring(L, "shadowEnabled");
//g         lua_gettable(L,lo);
//g         if (!lua_isnil(L,-1))
//g         {
//g             luaval_to_boolean(L, -1, &outValue->_shadow._shadowEnabled);
//g             if (outValue->_shadow._shadowEnabled)
//g             {
//g                 // default shadow values
//g                 outValue->_shadow._shadowOffset  = Size(5, 5);
//g                 outValue->_shadow._shadowBlur    = 1;
//g                 outValue->_shadow._shadowOpacity = 1;
//g             }
//g             
//g             lua_pushstring(L, "shadowOffset");
//g             lua_gettable(L,lo);
//g             if (!lua_isnil(L,-1))
//g             {
//g                 luaval_to_size(L, -1, &outValue->_shadow._shadowOffset);                
//g             }
//g             lua_pop(L,1);
//g             
//g             lua_pushstring(L, "shadowBlur");
//g             lua_gettable(L,lo);
//g             if (!lua_isnil(L,-1))
//g             {
//g                outValue->_shadow._shadowBlur = (float)lua_tonumber(L,-1);
//g             }
//g             lua_pop(L,1);
//g             
//g             lua_pushstring(L, "shadowOpacity");
//g             lua_gettable(L,lo);
//g             if (!lua_isnil(L,-1))
//g             {
//g                 outValue->_shadow._shadowOpacity = lua_tonumber(L,-1);
//g             }
//g             lua_pop(L,1);
//g         }
//g         lua_pop(L,1);
//g         
//g         lua_pushstring(L, "strokeEnabled");
//g         lua_gettable(L,lo);
//g         if (!lua_isnil(L,-1))
//g         {
//g             luaval_to_boolean(L, -1, &outValue->_stroke._strokeEnabled);
//g             if (outValue->_stroke._strokeEnabled)
//g             {
//g                 // default stroke values
//g                 outValue->_stroke._strokeSize  = 1;
//g                 outValue->_stroke._strokeColor = Color3B::BLUE;
//g                 
//g                 lua_pushstring(L, "strokeColor");
//g                 lua_gettable(L,lo);
//g                 if (!lua_isnil(L,-1))
//g                 {
//g                      luaval_to_color3b(L, -1, &outValue->_stroke._strokeColor);
//g                 }
//g                 lua_pop(L,1);
//g                 
//g                 lua_pushstring(L, "strokeSize");
//g                 lua_gettable(L,lo);
//g                 if (!lua_isnil(L,-1))
//g                 {
//g                     outValue->_stroke._strokeSize = (float)lua_tonumber(L,-1);
//g                 }
//g                 lua_pop(L,1);
//g             }
//g         }
//g         lua_pop(L,1);
//g     }
//g 
//g     
//g     return ok;
//g }
//g 
//g bool luaval_to_ttfconfig(lua_State* L,int lo, cocos2d::TTFConfig* outValue, const char* funcName)
//g {
//g     if (nullptr == L || nullptr == outValue)
//g         return false;
//g     
//g     bool ok = true;
//g     
//g     tolua_Error tolua_err;
//g     if (!tolua_istable(L, lo, 0, &tolua_err) )
//g     {
//g #if COCOS2D_DEBUG >=1
//g         luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
//g #endif
//g         ok = false;
//g     }
//g     
//g     if (ok)
//g     {
//g         lua_pushstring(L, "fontFilePath");         /* L: paramStack key */
//g         lua_gettable(L,lo);                        /* L: paramStack paramStack[lo][key] */
//g         outValue->fontFilePath = lua_isstring(L, -1)? lua_tostring(L, -1) : "";
//g         lua_pop(L,1);                              /* L: paramStack*/
//g         
//g         lua_pushstring(L, "fontSize");
//g         lua_gettable(L,lo);
//g         outValue->fontSize = lua_isnumber(L, -1)?(int)lua_tointeger(L, -1) : 0;
//g         lua_pop(L,1);
//g         
//g         lua_pushstring(L, "glyphs");
//g         lua_gettable(L, lo);
//g         outValue->glyphs = lua_isnumber(L, -1)?static_cast<GlyphCollection>(lua_tointeger(L, -1)) : GlyphCollection::NEHE;
//g         lua_pop(L, 1);
//g         
//g         lua_pushstring(L, "customGlyphs");
//g         lua_gettable(L, lo);
//g         outValue->customGlyphs = lua_isstring(L, -1)?lua_tostring(L, -1) : "";
//g         lua_pop(L, 1);
//g         
//g         lua_pushstring(L, "distanceFieldEnabled");
//g         lua_gettable(L, lo);
//g         outValue->distanceFieldEnabled = lua_isboolean(L, -1)?lua_toboolean(L, -1) : false;
//g         lua_pop(L, 1);
//g         
//g         lua_pushstring(L, "outlineSize");
//g         lua_gettable(L, lo);
//g         outValue->outlineSize = lua_isnumber(L, -1)?(int)lua_tointeger(L, -1) : 0;
//g         lua_pop(L, 1);
//g         
//g         return true;
//g     }
//g     
//g     return false;
//g }
//g 
//g 
//g bool luaval_to_uniform(lua_State* L, int lo, cocos2d::Uniform* outValue, const char* funcName)
//g {
//g     if (nullptr == L || nullptr == outValue)
//g         return false;
//g     
//g     bool ok = true;
//g     
//g     tolua_Error tolua_err;
//g     if (!tolua_istable(L, lo, 0, &tolua_err) )
//g     {
//g #if COCOS2D_DEBUG >=1
//g         luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
//g #endif
//g         ok = false;
//g     }
//g     
//g     if (ok)
//g     {
//g         lua_pushstring(L, "location");             /* L: paramStack key */
//g         lua_gettable(L,lo);                        /* L: paramStack paramStack[lo][key] */
//g         outValue->location = lua_isnumber(L, -1)? (GLint)lua_tointeger(L, -1) : 0;
//g         lua_pop(L,1);                              /* L: paramStack*/
//g         
//g         lua_pushstring(L, "size");
//g         lua_gettable(L,lo);
//g         outValue->size = lua_isnumber(L, -1)?(GLint)lua_tointeger(L, -1) : 0;
//g         lua_pop(L,1);
//g         
//g         lua_pushstring(L, "type");
//g         lua_gettable(L, lo);
//g         outValue->type = lua_isnumber(L, -1)?(GLenum)lua_tointeger(L, -1) : 0;
//g         lua_pop(L, 1);
//g         
//g         lua_pushstring(L, "name");
//g         lua_gettable(L, lo);
//g         outValue->name = lua_isstring(L, -1)?lua_tostring(L, -1) : "";
//g         lua_pop(L, 1);
//g         
//g         return true;
//g     }
//g     
//g     return false;
//g }
//g 
//g bool luaval_to_vertexattrib(lua_State* L, int lo, cocos2d::VertexAttrib* outValue, const char* funcName)
//g {
//g     if (nullptr == L || nullptr == outValue)
//g         return false;
//g     
//g     bool ok = true;
//g     
//g     tolua_Error tolua_err;
//g     if (!tolua_istable(L, lo, 0, &tolua_err) )
//g     {
//g #if COCOS2D_DEBUG >=1
//g         luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
//g #endif
//g         ok = false;
//g     }
//g 
//g     if (ok)
//g     {
//g         lua_pushstring(L, "index");                 /* L: paramStack key */
//g         lua_gettable(L,lo);                         /* L: paramStack paramStack[lo][key] */
//g         outValue->index = lua_isnumber(L, -1)? (GLint)lua_tointeger(L, -1) : 0;
//g         lua_pop(L,1);                              /* L: paramStack*/
//g         
//g         lua_pushstring(L, "size");
//g         lua_gettable(L,lo);
//g         outValue->size = lua_isnumber(L, -1)?(GLint)lua_tointeger(L, -1) : 0;
//g         lua_pop(L,1);
//g         
//g         lua_pushstring(L, "type");
//g         lua_gettable(L, lo);
//g         outValue->type = lua_isnumber(L, -1)?(GLenum)lua_tointeger(L, -1) : 0;
//g         lua_pop(L, 1);
//g         
//g         lua_pushstring(L, "name");
//g         lua_gettable(L, lo);
//g         outValue->name = lua_isstring(L, -1)?lua_tostring(L, -1) : "";
//g         lua_pop(L, 1);
//g         
//g         return true;
//g     }
//g     
//g     return false;
//g }
//g 
//g bool luaval_to_mat4(lua_State* L, int lo, cocos2d::Mat4* outValue , const char* funcName)
//g {
//g     if (nullptr == L || nullptr == outValue)
//g         return false;
//g     
//g     bool ok = true;
//g     
//g     tolua_Error tolua_err;
//g     if (!tolua_istable(L, lo, 0, &tolua_err) )
//g     {
//g #if COCOS2D_DEBUG >=1
//g         luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
//g         ok = false;
//g #endif
//g     }
//g     
//g     if (ok)
//g     {
//g         do
//g         {
//g             size_t len = lua_objlen(L, lo);
//g             if (len != 16) {
//g                 ok = false;
//g                 break;
//g             }
//g             for (size_t i = 0; i < len; i++)
//g             {
//g                 lua_pushnumber(L,i + 1);
//g                 lua_gettable(L,lo);
//g                 if (tolua_isnumber(L, -1, 0, &tolua_err))
//g                 {
//g                     outValue->m[i] = tolua_tonumber(L, -1, 0);
//g                 }
//g                 else
//g                 {
//g                     outValue->m[i] = 0;
//g                 }
//g                 lua_pop(L, 1);
//g             }
//g         }while (0);
//g     }
//g     
//g     return ok;
//g }
//g 
//g bool luaval_to_array(lua_State* L,int lo, __Array** outValue, const char* funcName)
//g {
//g     if (NULL == L || NULL == outValue)
//g         return false;
//g     
//g     bool ok = true;
//g 
//g     tolua_Error tolua_err;
//g     if (!tolua_istable(L, lo, 0, &tolua_err) )
//g     {
//g #if COCOS2D_DEBUG >=1
//g         luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
//g #endif
//g         ok = false;
//g     }
//g     
//g     if (ok)
//g     {
//g         size_t len = lua_objlen(L, lo);
//g         if (len > 0)
//g         {
//g             __Array* arr =  __Array::createWithCapacity(len);
//g             if (NULL == arr)
//g                 return false;
//g             
//g             for (size_t i = 0; i < len; i++)
//g             {
//g                 lua_pushnumber(L,i + 1);
//g                 lua_gettable(L,lo);
//g                 if (lua_isnil(L,-1))
//g                 {
//g                     lua_pop(L, 1);
//g                     continue;
//g                 }
//g                 
//g                 if (lua_isuserdata(L, -1))
//g                 {
//g                     Ref* obj = static_cast<Ref*>(tolua_tousertype(L, -1, NULL) );
//g                     if (NULL != obj)
//g                     {
//g                         arr->addObject(obj);
//g                     }
//g                 }
//g                 else if(lua_istable(L, -1))
//g                 {
//g                     lua_pushnumber(L,1);
//g                     lua_gettable(L,-2);
//g                     if (lua_isnil(L, -1) )
//g                     {
//g                         lua_pop(L,1);
//g                         __Dictionary* dictVal = NULL;
//g                         if (luaval_to_dictionary(L,-1,&dictVal))
//g                         {
//g                             arr->addObject(dictVal);
//g                         }
//g                     }
//g                     else
//g                     {
//g                        lua_pop(L,1);
//g                        __Array* arrVal = NULL;
//g                        if(luaval_to_array(L, -1, &arrVal))
//g                        {
//g                            arr->addObject(arrVal);
//g                        }
//g                     }
//g                 }
//g                 else if(lua_type(L, -1) == LUA_TSTRING)
//g                 {
//g                     std::string stringValue = "";
//g                     if(luaval_to_std_string(L, -1, &stringValue) )
//g                     {
//g                         arr->addObject(String::create(stringValue));
//g                     }
//g                 }
//g                 else if(lua_type(L, -1) == LUA_TBOOLEAN)
//g                 {
//g                     bool boolVal = false;
//g                     if (luaval_to_boolean(L, -1, &boolVal))
//g                     {
//g                         arr->addObject(Bool::create(boolVal));
//g                     }
//g                 }
//g                 else if(lua_type(L, -1) == LUA_TNUMBER)
//g                 {
//g                     arr->addObject(Double::create(tolua_tonumber(L, -1, 0)));
//g                 }
//g                 else
//g                 {
//g                     CCASSERT(false, "not supported type");
//g                 }
//g                 lua_pop(L, 1);
//g             }
//g             
//g             *outValue = arr;
//g         }
//g     }
//g     
//g     return ok;
//g }
//g 
//g bool luaval_to_dictionary(lua_State* L,int lo, __Dictionary** outValue, const char* funcName)
//g {
//g     if (NULL == L || NULL == outValue)
//g         return  false;
//g     
//g     bool ok = true;
//g 
//g     tolua_Error tolua_err;
//g     if (!tolua_istable(L, lo, 0, &tolua_err) )
//g     {
//g #if COCOS2D_DEBUG >=1
//g         luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
//g #endif
//g         ok = false;
//g     }
//g     
//g     if (ok)
//g     {
//g         std::string stringKey = "";
//g         std::string stringValue = "";
//g         bool boolVal = false;
//g         __Dictionary* dict = NULL;
//g         lua_pushnil(L);                                             /* L: lotable ..... nil */
//g         while ( 0 != lua_next(L, lo ) )                             /* L: lotable ..... key value */
//g         {
//g             if (!lua_isstring(L, -2))
//g             {
//g                 lua_pop(L, 1);
//g                 continue;
//g             }
//g             
//g             if (NULL == dict)
//g             {
//g                 dict = Dictionary::create();
//g             }
//g             
//g             if(luaval_to_std_string(L, -2, &stringKey))
//g             {
//g                 if (lua_isuserdata(L, -1))
//g                 {
//g                     Ref* obj = static_cast<Ref*>(tolua_tousertype(L, -1, NULL) );
//g                     if (NULL != obj)
//g                     {
//g                         //get the key to string
//g                         dict->setObject(obj, stringKey);
//g                     }
//g                 }
//g                 else if(lua_istable(L, -1))
//g                 {
//g                     lua_pushnumber(L,1);
//g                     lua_gettable(L,-2);
//g                     if (lua_isnil(L, -1) )
//g                     {
//g                         lua_pop(L,1);
//g                         __Dictionary* dictVal = NULL;
//g                         if (luaval_to_dictionary(L,-1,&dictVal))
//g                         {
//g                             dict->setObject(dictVal,stringKey);
//g                         }
//g                     }
//g                     else
//g                     {
//g                         lua_pop(L,1);
//g                         __Array* arrVal = NULL;
//g                         if(luaval_to_array(L, -1, &arrVal))
//g                         {
//g                             dict->setObject(arrVal,stringKey);
//g                         }
//g                     }
//g                 }
//g                 else if(lua_type(L, -1) == LUA_TSTRING)
//g                 {
//g                     if(luaval_to_std_string(L, -1, &stringValue))
//g                     {
//g                         dict->setObject(String::create(stringValue), stringKey);
//g                     }
//g                 }
//g                 else if(lua_type(L, -1) == LUA_TBOOLEAN)
//g                 {
//g                     if (luaval_to_boolean(L, -1, &boolVal))
//g                     {
//g                         dict->setObject(Bool::create(boolVal),stringKey);
//g                     }
//g                 }
//g                 else if(lua_type(L, -1) == LUA_TNUMBER)
//g                 {
//g                      dict->setObject(Double::create(tolua_tonumber(L, -1, 0)),stringKey);
//g                 }
//g                 else
//g                 {
//g                     CCASSERT(false, "not supported type");
//g                 }
//g             }
//g             
//g             lua_pop(L, 1);                                          /* L: lotable ..... key */
//g         }
//g         
//g                                                                     /* L: lotable ..... */
//g         *outValue = dict;
//g     }
//g     
//g     return ok;
//g }
//g 
//g bool luaval_to_array_of_vec2(lua_State* L,int lo,cocos2d::Vec2 **points, int *numPoints, const char* funcName)
//g {
//g     if (NULL == L)
//g         return false;
//g     
//g     bool ok = true;
//g     
//g     tolua_Error tolua_err;
//g     
//g     if (!tolua_istable(L, lo, 0, &tolua_err) )
//g     {
//g #if COCOS2D_DEBUG >=1
//g         luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
//g #endif
//g         ok = false;
//g     }
//g     
//g     if (ok)
//g     {
//g         size_t len = lua_objlen(L, lo);
//g         if (len > 0)
//g         {
//g             cocos2d::Vec2* array = (cocos2d::Vec2*) new Vec2[len];
//g             if (NULL == array)
//g                 return false;
//g             for (size_t i = 0; i < len; ++i)
//g             {
//g                 lua_pushnumber(L,i + 1);
//g                 lua_gettable(L,lo);
//g                 if (!tolua_istable(L,-1, 0, &tolua_err))
//g                 {
//g #if COCOS2D_DEBUG >=1
//g                     luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
//g #endif
//g                     lua_pop(L, 1);
//g                     CC_SAFE_DELETE_ARRAY(array);
//g                     return false;
//g                 }
//g                 ok &= luaval_to_vec2(L, lua_gettop(L), &array[i]);
//g                 if (!ok)
//g                 {
//g                     lua_pop(L, 1);
//g                     CC_SAFE_DELETE_ARRAY(array);
//g                     return false;
//g                 }
//g                 lua_pop(L, 1);
//g             }
//g             
//g             *numPoints = (int)len;
//g             *points    = array;
//g         }
//g     }
//g     return ok;
//g }


//g bool luavals_variadic_to_array(lua_State* L,int argc, __Array** ret)
//g {
//g     if (nullptr == L || argc == 0 )
//g         return false;
//g     
//g     bool ok = true;
//g     
//g     __Array* array = __Array::create();
//g     for (int i = 0; i < argc; i++)
//g     {
//g         double num = 0.0;
//g         if (lua_type(L, i + 2) == LUA_TNUMBER )
//g         {
//g             ok &= luaval_to_number(L, i + 2, &num);
//g             if (!ok)
//g                 break;
//g             
//g             array->addObject(Integer::create((int)num));
//g         }
//g         else if (lua_type(L, i + 2) == LUA_TSTRING )
//g         {
//g             std::string str = lua_tostring(L, i + 2);
//g             array->addObject(String::create(str));
//g         }
//g         else if (lua_isuserdata(L, i + 2))
//g         {
//g             tolua_Error err;
//g             if (!tolua_isusertype(L, i + 2, "cc.Ref", 0, &err))
//g             {
//g #if COCOS2D_DEBUG >=1
//g                 luaval_to_native_err(L,"#ferror:",&err);
//g #endif
//g                 ok = false;
//g                 break;
//g             }
//g             Ref* obj = static_cast<Ref*>(tolua_tousertype(L, i + 2, nullptr));
//g             array->addObject(obj);
//g         }
//g     }
//g     
//g     *ret = array;
//g     
//g     return ok;
//g }
//g 
//g bool luavals_variadic_to_ccvaluevector(lua_State* L, int argc, cocos2d::ValueVector* ret)
//g {
//g     if (nullptr == L || argc == 0 )
//g         return false;
//g     
//g     for (int i = 0; i < argc; i++)
//g     {
//g         if(lua_istable(L, i + 2))
//g         {
//g             lua_pushnumber(L, 1);
//g             lua_gettable(L, i + 2);
//g             if (lua_isnil(L, -1) )
//g             {
//g                 lua_pop(L,1);
//g                 ValueMap dictVal;
//g                 if (luaval_to_ccvaluemap(L, i + 2, &dictVal))
//g                 {
//g                     ret->push_back(Value(dictVal));
//g                 }
//g             }
//g             else
//g             {
//g                 lua_pop(L,1);
//g                 ValueVector arrVal;
//g                 if(luaval_to_ccvaluevector(L, i + 2, &arrVal))
//g                 {
//g                     ret->push_back(Value(arrVal));
//g                 }
//g             }
//g         }
//g         else if(lua_type(L, i + 2) == LUA_TSTRING )
//g         {
//g             std::string stringValue = "";
//g             if(luaval_to_std_string(L, i + 2, &stringValue) )
//g             {
//g                 ret->push_back(Value(stringValue));
//g             }
//g         }
//g         else if(lua_isboolean(L, i + 2))
//g         {
//g             bool boolVal = false;
//g             if (luaval_to_boolean(L, i + 2, &boolVal))
//g             {
//g                 ret->push_back(Value(boolVal));
//g             }
//g         }
//g         else if(lua_type(L, i + 2) == LUA_TNUMBER )
//g         {
//g             ret->push_back(Value(tolua_tonumber(L, i + 2, 0)));
//g         }
//g         else
//g         {
//g             CCASSERT(false, "not supported type");
//g         }
//g     }
//g     
//g     return true;
//g  }
//g 
//g bool luaval_to_ccvalue(lua_State* L, int lo, cocos2d::Value* ret, const char* funcName)
//g {
//g     if ( nullptr == L || nullptr == ret)
//g         return false;
//g     
//g     bool ok = true;
//g     
//g     tolua_Error tolua_err;
//g     
//g     if (tolua_istable(L, lo, 0, &tolua_err))
//g     {
//g         lua_pushnumber(L,1);
//g         lua_gettable(L,lo);
//g         
//g         if (lua_isnil(L, -1) )                          /** if table[1] = nil,we don't think it is a pure array */
//g         {
//g             lua_pop(L,1);
//g             ValueMap dictVal;
//g             if (luaval_to_ccvaluemap(L, lo, &dictVal))
//g             {
//g                 *ret = Value(dictVal);
//g             }
//g         }
//g         else
//g         {
//g             lua_pop(L,1);
//g             ValueVector arrVal;
//g             if (luaval_to_ccvaluevector(L, lo, &arrVal))
//g             {
//g                 *ret = Value(arrVal);
//g             }
//g         }
//g     }
//g     else if ((lua_type(L, lo) == LUA_TSTRING)  && tolua_isstring(L, lo, 0, &tolua_err))
//g     {
//g         std::string stringValue = "";
//g         if (luaval_to_std_string(L, lo, &stringValue))
//g         {
//g              *ret = Value(stringValue);
//g         }
//g     }
//g     else if ((lua_type(L, lo) == LUA_TBOOLEAN) && tolua_isboolean(L, lo, 0, &tolua_err))
//g     {
//g         bool boolVal = false;
//g         if (luaval_to_boolean(L, lo, &boolVal))
//g         {
//g             *ret = Value(boolVal);
//g         }
//g     }
//g     else if ((lua_type(L, lo) == LUA_TNUMBER) && tolua_isnumber(L, lo, 0, &tolua_err))
//g     {
//g         *ret = Value(tolua_tonumber(L, lo, 0));
//g     }
//g     
//g     return ok;
//g }
//g bool luaval_to_ccvaluemap(lua_State* L, int lo, cocos2d::ValueMap* ret, const char* funcName)
//g {
//g     if ( nullptr == L || nullptr == ret)
//g         return false;
//g     
//g     tolua_Error tolua_err;
//g     bool ok = true;
//g     if (!tolua_istable(L, lo, 0, &tolua_err))
//g     {
//g #if COCOS2D_DEBUG >=1
//g         luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
//g #endif
//g         ok = false;
//g     }
//g     
//g     if (ok)
//g     {
//g         std::string stringKey = "";
//g         std::string stringValue = "";
//g         bool boolVal = false;
//g         ValueMap& dict = *ret;
//g         lua_pushnil(L);                                             /* first key L: lotable ..... nil */
//g         while ( 0 != lua_next(L, lo ) )                             /* L: lotable ..... key value */
//g         {
//g             if (!lua_isstring(L, -2))
//g             {
//g                 lua_pop(L, 1);                                      /* removes 'value'; keep 'key' for next iteration*/
//g                 continue;
//g             }
//g             
//g             if(luaval_to_std_string(L, -2, &stringKey))
//g             {
//g 
//g                 if(lua_istable(L, -1))
//g                 {
//g                     lua_pushnumber(L,1);
//g                     lua_gettable(L,-2);
//g 
//g                     if (lua_isnil(L, -1) )                          /** if table[1] = nil,we don't think it is a pure array */
//g                     {
//g                         lua_pop(L,1);
//g                         ValueMap dictVal;
//g                         if (luaval_to_ccvaluemap(L, lua_gettop(L), &dictVal))
//g                         {
//g                             dict[stringKey] = Value(dictVal);
//g                         }
//g                     }
//g                     else
//g                     {
//g                         lua_pop(L,1);
//g                         ValueVector arrVal;
//g                         if (luaval_to_ccvaluevector(L, lua_gettop(L), &arrVal))
//g                         {
//g                             dict[stringKey] = Value(arrVal);
//g                         }
//g                     }
//g                 }
//g                 else if(lua_type(L, -1) == LUA_TSTRING)
//g                 {
//g                     if(luaval_to_std_string(L, -1, &stringValue))
//g                     {
//g                         dict[stringKey] = Value(stringValue);
//g                     }
//g                 }
//g                 else if(lua_type(L, -1) == LUA_TBOOLEAN)
//g                 {
//g                     if (luaval_to_boolean(L, -1, &boolVal))
//g                     {
//g                         dict[stringKey] = Value(boolVal);
//g                     }
//g                 }
//g                 else if(lua_type(L, -1) == LUA_TNUMBER)
//g                 {
//g                     dict[stringKey] = Value(tolua_tonumber(L, -1, 0));
//g                 }
//g                 else
//g                 {
//g                     CCASSERT(false, "not supported type");
//g                 }
//g             }
//g             
//g             lua_pop(L, 1);                                          /* L: lotable ..... key */
//g         }
//g     }
//g     
//g     return ok;
//g }
//g bool luaval_to_ccvaluemapintkey(lua_State* L, int lo, cocos2d::ValueMapIntKey* ret, const char* funcName)
//g {
//g     if (nullptr == L || nullptr == ret)
//g         return false;
//g     
//g     tolua_Error tolua_err;
//g     bool ok = true;
//g     if (!tolua_istable(L, lo, 0, &tolua_err))
//g     {
//g #if COCOS2D_DEBUG >=1
//g         luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
//g #endif
//g         ok = false;
//g     }
//g     
//g     if (ok)
//g     {
//g         std::string stringKey = "";
//g         std::string stringValue = "";
//g         int intKey = 0;
//g         bool boolVal = false;
//g         ValueMapIntKey& dict = *ret;
//g         lua_pushnil(L);                                             /* first key L: lotable ..... nil */
//g         while ( 0 != lua_next(L, lo ) )                             /* L: lotable ..... key value */
//g         {
//g             if (!lua_isstring(L, -2))
//g             {
//g                 lua_pop(L, 1);                                      /* removes 'value'; keep 'key' for next iteration*/
//g                 continue;
//g             }
//g             
//g             if(luaval_to_std_string(L, -2, &stringKey))
//g             {
//g                 intKey = atoi(stringKey.c_str());
//g                 if(lua_istable(L, -1))
//g                 {
//g                     lua_pushnumber(L,1);
//g                     lua_gettable(L,-2);
//g                     
//g                     if (lua_isnil(L, -1) )                          /** if table[1] = nil,we don't think it is a pure array */
//g                     {
//g                         lua_pop(L,1);
//g                         ValueMap dictVal;
//g                         if (luaval_to_ccvaluemap(L, lua_gettop(L), &dictVal))
//g                         {
//g                             dict[intKey] = Value(dictVal);
//g                         }
//g                     }
//g                     else
//g                     {
//g                         lua_pop(L,1);
//g                         ValueVector arrVal;
//g                         if (luaval_to_ccvaluevector(L, lua_gettop(L), &arrVal))
//g                         {
//g                             dict[intKey] = Value(arrVal);
//g                         }
//g                     }
//g                 }
//g                 else if(lua_type(L, -1) == LUA_TSTRING)
//g                 {
//g                     if(luaval_to_std_string(L, -1, &stringValue))
//g                     {
//g                         dict[intKey] = Value(stringValue);
//g                     }
//g                 }
//g                 else if(lua_type(L, -1) == LUA_TBOOLEAN)
//g                 {
//g                     if (luaval_to_boolean(L, -1, &boolVal))
//g                     {
//g                         dict[intKey] = Value(boolVal);
//g                     }
//g                 }
//g                 else if(lua_type(L, -1) == LUA_TNUMBER)
//g                 {
//g                     dict[intKey] = Value(tolua_tonumber(L, -1, 0));
//g                 }
//g                 else
//g                 {
//g                     CCASSERT(false, "not supported type");
//g                 }
//g             }
//g             
//g             lua_pop(L, 1);                                          /* L: lotable ..... key */
//g         }
//g     }
//g     
//g     return ok;
//g }
//g bool luaval_to_ccvaluevector(lua_State* L, int lo, cocos2d::ValueVector* ret, const char* funcName)
//g {
//g     if (nullptr == L || nullptr == ret)
//g         return false;
//g     
//g     tolua_Error tolua_err;
//g     bool ok = true;
//g     if (!tolua_istable(L, lo, 0, &tolua_err))
//g     {
//g #if COCOS2D_DEBUG >=1
//g         luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
//g #endif
//g         ok = false;
//g     }
//g     
//g     if (ok)
//g     {
//g         size_t len = lua_objlen(L, lo);
//g         for (size_t i = 0; i < len; i++)
//g         {
//g             lua_pushnumber(L,i + 1);
//g             lua_gettable(L,lo);
//g             if (lua_isnil(L,-1))
//g             {
//g                 lua_pop(L, 1);
//g                 continue;
//g             }
//g             
//g             if(lua_istable(L, -1))
//g             {
//g                 lua_pushnumber(L,1);
//g                 lua_gettable(L,-2);
//g                 if (lua_isnil(L, -1) )
//g                 {
//g                     lua_pop(L,1);
//g                     ValueMap dictVal;
//g                     if (luaval_to_ccvaluemap(L, lua_gettop(L), &dictVal))
//g                     {
//g                         ret->push_back(Value(dictVal));
//g                     }
//g                 }
//g                 else
//g                 {
//g                     lua_pop(L,1);
//g                     ValueVector arrVal;
//g                     if(luaval_to_ccvaluevector(L, lua_gettop(L), &arrVal))
//g                     {
//g                         ret->push_back(Value(arrVal));
//g                     }
//g                 }
//g             }
//g             else if(lua_type(L, -1) == LUA_TSTRING)
//g             {
//g                 std::string stringValue = "";
//g                 if(luaval_to_std_string(L, -1, &stringValue) )
//g                 {
//g                     ret->push_back(Value(stringValue));
//g                 }
//g             }
//g             else if(lua_type(L, -1) == LUA_TBOOLEAN)
//g             {
//g                 bool boolVal = false;
//g                 if (luaval_to_boolean(L, -1, &boolVal))
//g                 {
//g                     ret->push_back(Value(boolVal));
//g                 }
//g             }
//g             else if(lua_type(L, -1) == LUA_TNUMBER)
//g             {
//g                 ret->push_back(Value(tolua_tonumber(L, -1, 0)));
//g             }
//g             else
//g             {
//g                 CCASSERT(false, "not supported type");
//g             }
//g             lua_pop(L, 1);
//g         }
//g     }
//g     
//g     return ok;
//g }

bool luaval_to_std_vector_string(lua_State* L, int lo, std::vector<std::string>* ret, const char* funcName)
{
    if (NULL == L || NULL == ret || lua_gettop(L) < lo)
        return false;
    
    tolua_Error tolua_err;
    bool ok = true;
    if (!tolua_istable(L, lo, 0, &tolua_err))
    {
#if COCOS2D_DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }
    
    if (ok)
    {
        size_t len = lua_objlen(L, lo);
        std::string value = "";
        for (size_t i = 0; i < len; i++)
        {
            lua_pushnumber(L, i + 1);
            lua_gettable(L,lo);
            if(lua_isstring(L, -1))
            {
                ok = luaval_to_std_string(L, -1, &value);
                if(ok)
                    ret->push_back(value);
            }
            else
            {
                CCASSERT(false, "string type is needed");
            }
            
            lua_pop(L, 1);
        }
    }
    
    return ok;
}

bool luaval_to_std_vector_int(lua_State* L, int lo, std::vector<int>* ret, const char* funcName)
{
    if (NULL == L || NULL == ret || lua_gettop(L) < lo)
        return false;
    
    tolua_Error tolua_err;
    bool ok = true;
    if (!tolua_istable(L, lo, 0, &tolua_err))
    {
#if COCOS2D_DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }
    
    if (ok)
    {
        size_t len = lua_objlen(L, lo);
        for (size_t i = 0; i < len; i++)
        {
            lua_pushnumber(L, i + 1);
            lua_gettable(L,lo);
            if(lua_isnumber(L, -1))
            {
                ret->push_back((int)tolua_tonumber(L, -1, 0));
            }
            else
            {
                CCASSERT(false, "int type is needed");
            }
            
            lua_pop(L, 1);
        }
    }
    
    return ok;
}

//g bool luaval_to_mesh_vertex_attrib(lua_State* L, int lo, cocos2d::MeshVertexAttrib* ret, const char* funcName)
//g {
//g     if (nullptr == L || nullptr == ret || lua_gettop(L) < lo)
//g         return false;
//g     
//g     tolua_Error tolua_err;
//g     bool ok = true;
//g     
//g     if (!tolua_istable(L, lo, 0, &tolua_err))
//g     {
//g #if COCOS2D_DEBUG >=1
//g         luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
//g #endif
//g         ok = false;
//g     }
//g     
//g     
//g     if (ok)
//g     {
//g         lua_pushstring(L, "size");                  /* L: paramStack key */
//g         lua_gettable(L,lo);                         /* L: paramStack paramStack[lo][key] */
//g         ret->size  = (GLint)lua_tonumber(L, -1);
//g         lua_pop(L,1);
//g         
//g         lua_pushstring(L, "type");                  /* L: paramStack key */
//g         lua_gettable(L,lo);                         /* L: paramStack paramStack[lo][key] */
//g         ret->type  = (GLenum)lua_tonumber(L, -1);
//g         lua_pop(L,1);
//g         
//g         lua_pushstring(L, "vertexAttrib");          /* L: paramStack key */
//g         lua_gettable(L,lo);                         /* L: paramStack paramStack[lo][key] */
//g         ret->type  = (GLenum)lua_tonumber(L, -1);
//g         lua_pop(L,1);
//g         
//g         lua_pushstring(L, "vertexAttrib");          /* L: paramStack key */
//g         lua_gettable(L,lo);                         /* L: paramStack paramStack[lo][key] */
//g         ret->type  = (GLenum)lua_tonumber(L, -1);
//g         lua_pop(L,1);
//g     }
//g     
//g     return ok;
//g     
//g }

bool luaval_to_std_vector_float(lua_State* L, int lo, std::vector<float>* ret, const char* funcName)
{
    if (NULL == L || NULL == ret || lua_gettop(L) < lo)
        return false;
    
    tolua_Error tolua_err;
    bool ok = true;
    
    if (!tolua_istable(L, lo, 0, &tolua_err))
    {
#if COCOS2D_DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }
    
    if (ok)
    {
        size_t len = lua_objlen(L, lo);
        for (size_t i = 0; i < len; i++)
        {
            lua_pushnumber(L, i + 1);
            lua_gettable(L,lo);
            if(lua_isnumber(L, -1))
            {
                ret->push_back((float)tolua_tonumber(L, -1, 0));
            }
            else
            {
                CCASSERT(false, "float type is needed");
            }
            
            lua_pop(L, 1);
        }
    }
    
    return ok;
}


bool luaval_to_std_vector_ushort(lua_State* L, int lo, std::vector<unsigned short>* ret, const char* funcName)
{
    if (NULL == L || NULL == ret || lua_gettop(L) < lo)
        return false;
    
    tolua_Error tolua_err;
    bool ok = true;
    
    if (!tolua_istable(L, lo, 0, &tolua_err))
    {
#if COCOS2D_DEBUG >=1
        luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
#endif
        ok = false;
    }
    
    if (ok)
    {
        size_t len = lua_objlen(L, lo);
        for (size_t i = 0; i < len; i++)
        {
            lua_pushnumber(L, i + 1);
            lua_gettable(L,lo);
            if(lua_isnumber(L, -1))
            {
                ret->push_back((unsigned short)tolua_tonumber(L, -1, 0));
            }
            else
            {
                CCASSERT(false, "unsigned short type is needed");
            }
            
            lua_pop(L, 1);
        }
    }
    
    return ok;
}

//g bool luaval_to_quaternion(lua_State* L,int lo,cocos2d::Quaternion* outValue, const char* funcName)
//g {
//g     if (nullptr == L || nullptr == outValue)
//g         return false;
//g     
//g     bool ok = true;
//g     
//g     tolua_Error tolua_err;
//g     if (!tolua_istable(L, lo, 0, &tolua_err) )
//g     {
//g #if COCOS2D_DEBUG >=1
//g         luaval_to_native_err(L,"#ferror:",&tolua_err,funcName);
//g #endif
//g         ok = false;
//g     }
//g     
//g     
//g     if (ok)
//g     {
//g         lua_pushstring(L, "x");
//g         lua_gettable(L, lo);
//g         outValue->x = lua_isnil(L, -1) ? 0 : lua_tonumber(L, -1);
//g         lua_pop(L, 1);
//g         
//g         lua_pushstring(L, "y");
//g         lua_gettable(L, lo);
//g         outValue->y = lua_isnil(L, -1) ? 0 : lua_tonumber(L, -1);
//g         lua_pop(L, 1);
//g         
//g         lua_pushstring(L, "z");
//g         lua_gettable(L, lo);
//g         outValue->z = lua_isnil(L, -1) ? 0 : lua_tonumber(L, -1);
//g         lua_pop(L, 1);
//g         
//g         lua_pushstring(L, "w");
//g         lua_gettable(L, lo);
//g         outValue->w = lua_isnil(L, -1) ? 0 : lua_tonumber(L, -1);
//g         lua_pop(L, 1);
//g     }
//g     return ok;
//g }
//g 
//g void vec2_array_to_luaval(lua_State* L,const cocos2d::Vec2* points, int count)
//g {
//g     if (NULL  == L)
//g         return;
//g     lua_newtable(L);
//g     for (int i = 1; i <= count; ++i)
//g     {
//g         lua_pushnumber(L, i);
//g         vec2_to_luaval(L, points[i-1]);
//g         lua_rawset(L, -3);
//g     }
//g }
//g 
//g void vec2_to_luaval(lua_State* L,const cocos2d::Vec2& vec2)
//g {
//g     if (NULL  == L)
//g         return;
//g     lua_newtable(L);                                    /* L: table */
//g     lua_pushstring(L, "x");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number) vec2.x);               /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     lua_pushstring(L, "y");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number) vec2.y);               /* L: table key value*/
//g     lua_rawset(L, -3);
//g     
//g }
//g 
//g void vec3_to_luaval(lua_State* L,const cocos2d::Vec3& vec3)
//g {
//g     if (NULL  == L)
//g         return;
//g     
//g     lua_newtable(L);                                    /* L: table */
//g     lua_pushstring(L, "x");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number) vec3.x);             /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     lua_pushstring(L, "y");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number) vec3.y);             /* L: table key value*/
//g     lua_rawset(L, -3);
//g     lua_pushstring(L, "z");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number) vec3.z);             /* L: table key value*/
//g     lua_rawset(L, -3);
//g }
//g 
//g void vec4_to_luaval(lua_State* L,const cocos2d::Vec4& vec4)
//g {
//g     if (NULL  == L)
//g         return;
//g     
//g     lua_newtable(L);                                    /* L: table */
//g     lua_pushstring(L, "x");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number) vec4.x);             /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     lua_pushstring(L, "y");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number) vec4.y);             /* L: table key value*/
//g     lua_rawset(L, -3);
//g     lua_pushstring(L, "z");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number) vec4.z);             /* L: table key value*/
//g     lua_rawset(L, -3);
//g     lua_pushstring(L, "w");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number) vec4.w);             /* L: table key value*/
//g     lua_rawset(L, -3);
//g }
//g #if CC_USE_PHYSICS
//g void physics_material_to_luaval(lua_State* L,const PhysicsMaterial& pm)
//g {
//g     if (nullptr  == L)
//g         return;
//g     lua_newtable(L);                                    /* L: table */
//g     lua_pushstring(L, "density");                       /* L: table key */
//g     lua_pushnumber(L, (lua_Number) pm.density);         /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     lua_pushstring(L, "restitution");                   /* L: table key */
//g     lua_pushnumber(L, (lua_Number) pm.restitution);     /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     lua_pushstring(L, "friction");                      /* L: table key */
//g     lua_pushnumber(L, (lua_Number) pm.friction);        /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g }
//g 
//g void physics_raycastinfo_to_luaval(lua_State* L, const PhysicsRayCastInfo& info)
//g {
//g     if (NULL  == L)
//g         return;
//g     
//g     lua_newtable(L);                                    /* L: table */
//g     
//g     lua_pushstring(L, "shape");                       /* L: table key */
//g     PhysicsShape* shape = info.shape;
//g     if (shape == nullptr)
//g     {
//g         lua_pushnil(L);
//g     }else
//g     {
//g         int ID =  (int)(shape->_ID);
//g         int* luaID = &(shape->_luaID);
//g         toluafix_pushusertype_ccobject(L, ID, luaID, (void*)shape,"cc.PhysicsShape");
//g     }
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     
//g     lua_pushstring(L, "start");                   /* L: table key */
//g     vec2_to_luaval(L, info.start);
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     
//g     lua_pushstring(L, "ended");                   /* L: table key */
//g     vec2_to_luaval(L, info.end);
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     
//g     lua_pushstring(L, "contact");                   /* L: table key */
//g     vec2_to_luaval(L, info.contact);
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     
//g     lua_pushstring(L, "normal");                   /* L: table key */
//g     vec2_to_luaval(L, info.normal);
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     
//g     lua_pushstring(L, "fraction");                      /* L: table key */
//g     lua_pushnumber(L, (lua_Number) info.fraction);        /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g }
//g 
//g void physics_contactdata_to_luaval(lua_State* L, const PhysicsContactData* data)
//g {
//g     if (nullptr  == L || nullptr == data)
//g         return;
//g     
//g     lua_newtable(L);                                    /* L: table */
//g     
//g     lua_pushstring(L, "points");
//g     vec2_array_to_luaval(L, data->points, data->count);
//g     lua_rawset(L, -3);
//g     
//g     lua_pushstring(L, "normal");
//g     vec2_to_luaval(L, data->normal);
//g     lua_rawset(L, -3);
//g     
//g     lua_pushstring(L, "POINT_MAX");
//g     lua_pushnumber(L, data->POINT_MAX);
//g     lua_rawset(L, -3);
//g }
//g #endif //CC_USE_PHYSICS
//g void size_to_luaval(lua_State* L,const Size& sz)
//g {
//g     if (NULL  == L)
//g         return;
//g     lua_newtable(L);                                    /* L: table */
//g     lua_pushstring(L, "width");                         /* L: table key */
//g     lua_pushnumber(L, (lua_Number) sz.width);           /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     lua_pushstring(L, "height");                        /* L: table key */
//g     lua_pushnumber(L, (lua_Number) sz.height);          /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g }
//g 
//g void rect_to_luaval(lua_State* L,const Rect& rt)
//g {
//g     if (NULL  == L)
//g         return;
//g     lua_newtable(L);                                    /* L: table */
//g     lua_pushstring(L, "x");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number) rt.origin.x);               /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     lua_pushstring(L, "y");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number) rt.origin.y);               /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     lua_pushstring(L, "width");                         /* L: table key */
//g     lua_pushnumber(L, (lua_Number) rt.size.width);           /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     lua_pushstring(L, "height");                        /* L: table key */
//g     lua_pushnumber(L, (lua_Number) rt.size.height);          /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g }
//g 
//g void color4b_to_luaval(lua_State* L,const Color4B& cc)
//g {
//g     if (NULL  == L)
//g         return;
//g     lua_newtable(L);                                    /* L: table */
//g     lua_pushstring(L, "r");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number) cc.r);               /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     lua_pushstring(L, "g");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number) cc.g);               /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     lua_pushstring(L, "b");                         /* L: table key */
//g     lua_pushnumber(L, (lua_Number) cc.b);           /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     lua_pushstring(L, "a");                        /* L: table key */
//g     lua_pushnumber(L, (lua_Number) cc.a);          /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g }
//g 
//g void color4f_to_luaval(lua_State* L,const Color4F& cc)
//g {
//g     if (NULL  == L)
//g         return;
//g     lua_newtable(L);                                    /* L: table */
//g     lua_pushstring(L, "r");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number) cc.r);               /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     lua_pushstring(L, "g");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number) cc.g);               /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     lua_pushstring(L, "b");                         /* L: table key */
//g     lua_pushnumber(L, (lua_Number) cc.b);           /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     lua_pushstring(L, "a");                        /* L: table key */
//g     lua_pushnumber(L, (lua_Number) cc.a);          /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g }
//g 
//g void color3b_to_luaval(lua_State* L,const Color3B& cc)
//g {
//g     if (NULL  == L)
//g         return;
//g     lua_newtable(L);                                    /* L: table */
//g     lua_pushstring(L, "r");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number) cc.r);               /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     lua_pushstring(L, "g");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number) cc.g);               /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     lua_pushstring(L, "b");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number) cc.b);               /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g }
//g 
//g void affinetransform_to_luaval(lua_State* L,const AffineTransform& inValue)
//g {
//g     if (NULL  == L)
//g         return;
//g     
//g     lua_newtable(L);                                    /* L: table */
//g     lua_pushstring(L, "a");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number) inValue.a);               /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     lua_pushstring(L, "b");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number) inValue.b);               /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     lua_pushstring(L, "c");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number) inValue.c);               /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     lua_pushstring(L, "d");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number) inValue.d);               /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     lua_pushstring(L, "tx");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number) inValue.d);               /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     lua_pushstring(L, "ty");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number) inValue.d);               /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g }
//g 
//g void fontdefinition_to_luaval(lua_State* L,const FontDefinition& inValue)
//g {
//g     if (NULL == L)
//g         return;
//g     
//g     lua_newtable(L);                                    /* L: table */
//g     lua_pushstring(L, "fontName");                      /* L: table key */
//g     tolua_pushcppstring(L, inValue._fontName);          /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     lua_pushstring(L, "fontSize");                      /* L: table key */
//g     lua_pushnumber(L,(lua_Number)inValue._fontSize);                 /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     lua_pushstring(L, "fontAlignmentH");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number) inValue._alignment);               /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     lua_pushstring(L, "fontAlignmentV");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number) inValue._vertAlignment);               /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     lua_pushstring(L, "fontFillColor");                             /* L: table key */
//g     color3b_to_luaval(L, inValue._fontFillColor);               /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     lua_pushstring(L, "fontDimensions");                             /* L: table key */
//g     size_to_luaval(L, inValue._dimensions);              /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     
//g     //Shadow
//g     lua_pushstring(L, "shadowEnabled");                             /* L: table key */
//g     lua_pushboolean(L, inValue._shadow._shadowEnabled);              /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     
//g     lua_pushstring(L, "shadowOffset");                             /* L: table key */
//g     size_to_luaval(L, inValue._shadow._shadowOffset);              /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     
//g     lua_pushstring(L, "shadowBlur");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number)inValue._shadow._shadowBlur);  /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     
//g     lua_pushstring(L, "shadowOpacity");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number)inValue._shadow._shadowOpacity);  /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     
//g     //Stroke
//g     lua_pushstring(L, "shadowEnabled");                             /* L: table key */
//g     lua_pushboolean(L, inValue._stroke._strokeEnabled);              /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     
//g     lua_pushstring(L, "strokeColor");                             /* L: table key */
//g     color3b_to_luaval(L, inValue._stroke._strokeColor);              /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     
//g     lua_pushstring(L, "strokeSize");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number)inValue._stroke._strokeSize);              /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g }
//g 
//g void array_to_luaval(lua_State* L,__Array* inValue)
//g {
//g     lua_newtable(L);
//g     
//g     if (nullptr == L || nullptr == inValue)
//g         return;
//g     
//g     Ref* obj = nullptr;
//g     
//g     std::string className = "";
//g     __String* strVal = nullptr;
//g     __Dictionary* dictVal = nullptr;
//g     __Array* arrVal = nullptr;
//g     __Double* doubleVal = nullptr;
//g     __Bool* boolVal = nullptr;
//g     __Float* floatVal = nullptr;
//g     __Integer* intVal = nullptr;
//g     int indexTable = 1;
//g     
//g     CCARRAY_FOREACH(inValue, obj)
//g     {
//g         if (nullptr == obj)
//g             continue;
//g         
//g         std::string typeName = typeid(*obj).name();
//g         auto iter = g_luaType.find(typeName);
//g         if (g_luaType.end() != iter)
//g         {
//g             className = iter->second;
//g             if (nullptr != dynamic_cast<cocos2d::Ref *>(obj))
//g             {
//g                 lua_pushnumber(L, (lua_Number)indexTable);                
//g                 int ID = (obj) ? (int)obj->_ID : -1;
//g                 int* luaID = (obj) ? &obj->_luaID : NULL;
//g                 toluafix_pushusertype_ccobject(L, ID, luaID, (void*)obj,className.c_str());
//g                 lua_rawset(L, -3);
//g                 obj->retain();
//g                 ++indexTable;
//g             }
//g         }
//g         else if((strVal = dynamic_cast<__String *>(obj)))
//g         {
//g             lua_pushnumber(L, (lua_Number)indexTable);   
//g             lua_pushstring(L, strVal->getCString());
//g             lua_rawset(L, -3);
//g             ++indexTable;
//g         }
//g         else if ((dictVal = dynamic_cast<__Dictionary*>(obj)))
//g         {
//g             dictionary_to_luaval(L, dictVal);
//g         }
//g         else if ((arrVal = dynamic_cast<__Array*>(obj)))
//g         {
//g             array_to_luaval(L, arrVal);
//g         }
//g         else if ((doubleVal = dynamic_cast<__Double*>(obj)))
//g         {
//g             lua_pushnumber(L, (lua_Number)indexTable);   
//g             lua_pushnumber(L, (lua_Number)doubleVal->getValue());
//g             lua_rawset(L, -3);
//g             ++indexTable;
//g         }
//g         else if ((floatVal = dynamic_cast<__Float*>(obj)))
//g         {
//g             lua_pushnumber(L, (lua_Number)indexTable);   
//g             lua_pushnumber(L, (lua_Number)floatVal->getValue());
//g             lua_rawset(L, -3);
//g             ++indexTable;
//g         }
//g         else if ((intVal = dynamic_cast<__Integer*>(obj)))
//g         {
//g             lua_pushnumber(L, (lua_Number)indexTable);   
//g             lua_pushinteger(L, (lua_Integer)intVal->getValue());
//g             lua_rawset(L, -3);
//g             ++indexTable;
//g         }
//g         else if ((boolVal = dynamic_cast<__Bool*>(obj)))
//g         {
//g             lua_pushnumber(L, (lua_Number)indexTable);   
//g             lua_pushboolean(L, boolVal->getValue());
//g             lua_rawset(L, -3);
//g             ++indexTable;
//g         }
//g         else
//g         {
//g             CCASSERT(false, "the type isn't suppored.");
//g         }
//g     }
//g }
//g 
//g void dictionary_to_luaval(lua_State* L, __Dictionary* dict)
//g {
//g     lua_newtable(L);
//g     
//g     if (nullptr == L || nullptr == dict)
//g         return;
//g     
//g     DictElement* element = nullptr;
//g     
//g     std::string className = "";
//g     __String* strVal = nullptr;
//g     __Dictionary* dictVal = nullptr;
//g     __Array* arrVal = nullptr;
//g     __Double* doubleVal = nullptr;
//g     __Bool* boolVal = nullptr;
//g     __Float* floatVal = nullptr;
//g     __Integer* intVal = nullptr;
//g     
//g     CCDICT_FOREACH(dict, element)
//g     {
//g         if (NULL == element)
//g             continue;
//g         
//g         std::string typeName = typeid(element->getObject()).name();
//g         
//g         auto iter = g_luaType.find(typeName);
//g         if (g_luaType.end() != iter)
//g         {
//g             className = iter->second;
//g             if ( nullptr != dynamic_cast<Ref*>(element->getObject()))
//g             {
//g                 lua_pushstring(L, element->getStrKey());
//g                 int ID = (element->getObject()) ? (int)element->getObject()->_ID : -1;
//g                 int* luaID = (element->getObject()) ? &(element->getObject()->_luaID) : NULL;
//g                 toluafix_pushusertype_ccobject(L, ID, luaID, (void*)element->getObject(),className.c_str());
//g                 lua_rawset(L, -3);
//g                 element->getObject()->retain();
//g             }
//g         }
//g         else if((strVal = dynamic_cast<__String *>(element->getObject())))
//g         {
//g             lua_pushstring(L, element->getStrKey());
//g             lua_pushstring(L, strVal->getCString());
//g             lua_rawset(L, -3);
//g         }
//g         else if ((dictVal = dynamic_cast<__Dictionary*>(element->getObject())))
//g         {
//g             dictionary_to_luaval(L, dictVal);
//g         }
//g         else if ((arrVal = dynamic_cast<__Array*>(element->getObject())))
//g         {
//g             array_to_luaval(L, arrVal);
//g         }
//g         else if ((doubleVal = dynamic_cast<__Double*>(element->getObject())))
//g         {
//g             lua_pushstring(L, element->getStrKey());
//g             lua_pushnumber(L, (lua_Number)doubleVal->getValue());
//g             lua_rawset(L, -3);
//g         }
//g         else if ((floatVal = dynamic_cast<__Float*>(element->getObject())))
//g         {
//g             lua_pushstring(L, element->getStrKey());
//g             lua_pushnumber(L, (lua_Number)floatVal->getValue());
//g             lua_rawset(L, -3);
//g         }
//g         else if ((intVal = dynamic_cast<__Integer*>(element->getObject())))
//g         {
//g             lua_pushstring(L, element->getStrKey());
//g             lua_pushinteger(L, (lua_Integer)intVal->getValue());
//g             lua_rawset(L, -3);
//g         }
//g         else if ((boolVal = dynamic_cast<__Bool*>(element->getObject())))
//g         {
//g             lua_pushstring(L, element->getStrKey());
//g             lua_pushboolean(L, boolVal->getValue());
//g             lua_rawset(L, -3);
//g         }
//g         else
//g         {
//g             CCASSERT(false, "the type isn't suppored.");
//g         }
//g     }
//g }
//g 
//g void ccvalue_to_luaval(lua_State* L,const cocos2d::Value& inValue)
//g {
//g     const Value& obj = inValue;
//g     switch (obj.getType())
//g     {
//g         case Value::Type::BOOLEAN:
//g             lua_pushboolean(L, obj.asBool());
//g             break;
//g         case Value::Type::FLOAT:
//g         case Value::Type::DOUBLE:
//g             lua_pushnumber(L, obj.asDouble());
//g             break;
//g         case Value::Type::INTEGER:
//g             lua_pushinteger(L, obj.asInt());
//g             break;
//g         case Value::Type::STRING:
//g             lua_pushstring(L, obj.asString().c_str());
//g             break;
//g         case Value::Type::VECTOR:
//g             ccvaluevector_to_luaval(L, obj.asValueVector());
//g             break;
//g         case Value::Type::MAP:
//g             ccvaluemap_to_luaval(L, obj.asValueMap());
//g             break;
//g         case Value::Type::INT_KEY_MAP:
//g             ccvaluemapintkey_to_luaval(L, obj.asIntKeyMap());
//g             break;
//g         default:
//g             break;
//g     }
//g }
//g void ccvaluemap_to_luaval(lua_State* L,const cocos2d::ValueMap& inValue)
//g {
//g     lua_newtable(L);
//g     
//g     if (nullptr == L)
//g         return;
//g     
//g     for (auto iter = inValue.begin(); iter != inValue.end(); ++iter)
//g     {
//g         std::string key = iter->first;
//g         const Value& obj = iter->second;
//g         switch (obj.getType())
//g         {
//g             case Value::Type::BOOLEAN:
//g                 {
//g                     lua_pushstring(L, key.c_str());
//g                     lua_pushboolean(L, obj.asBool());
//g                     lua_rawset(L, -3);
//g                 }
//g                 break;
//g             case Value::Type::FLOAT:
//g             case Value::Type::DOUBLE:
//g                 {
//g                     lua_pushstring(L, key.c_str());
//g                     lua_pushnumber(L, obj.asDouble());
//g                     lua_rawset(L, -3);
//g                 }
//g                 break;
//g             case Value::Type::INTEGER:
//g                 {
//g                     lua_pushstring(L, key.c_str());
//g                     lua_pushinteger(L, obj.asInt());
//g                     lua_rawset(L, -3);
//g                 }
//g                 break;
//g             case Value::Type::STRING:
//g                 {
//g                     lua_pushstring(L, key.c_str());
//g                     lua_pushstring(L, obj.asString().c_str());
//g                     lua_rawset(L, -3);
//g                 }
//g                 break;
//g             case Value::Type::VECTOR:
//g                 {
//g                     lua_pushstring(L, key.c_str());
//g                     ccvaluevector_to_luaval(L, obj.asValueVector());
//g                     lua_rawset(L, -3);
//g                 }
//g                 break;
//g             case Value::Type::MAP:
//g                 {
//g                     lua_pushstring(L, key.c_str());
//g                     ccvaluemap_to_luaval(L, obj.asValueMap());
//g                     lua_rawset(L, -3);
//g                 }
//g                 break;
//g             case Value::Type::INT_KEY_MAP:
//g                 {
//g                     lua_pushstring(L, key.c_str());
//g                     ccvaluemapintkey_to_luaval(L, obj.asIntKeyMap());
//g                     lua_rawset(L, -3);
//g                 }
//g                 break;
//g             default:
//g                 break;
//g         }
//g     }
//g }
//g void ccvaluemapintkey_to_luaval(lua_State* L, const cocos2d::ValueMapIntKey& inValue)
//g {
//g     lua_newtable(L);
//g     
//g     if (nullptr == L)
//g         return;
//g     
//g     for (auto iter = inValue.begin(); iter != inValue.end(); ++iter)
//g     {
//g         std::stringstream keyss;
//g         keyss << iter->first;
//g         std::string key = keyss.str();
//g         
//g         const Value& obj = iter->second;
//g         
//g         switch (obj.getType())
//g         {
//g             case Value::Type::BOOLEAN:
//g                 {
//g                     lua_pushstring(L, key.c_str());
//g                     lua_pushboolean(L, obj.asBool());
//g                     lua_rawset(L, -3);
//g                 }
//g                 break;
//g             case Value::Type::FLOAT:
//g             case Value::Type::DOUBLE:
//g                 {
//g                     lua_pushstring(L, key.c_str());
//g                     lua_pushnumber(L, obj.asDouble());
//g                     lua_rawset(L, -3);
//g                 }
//g                 break;
//g             case Value::Type::INTEGER:
//g                 {
//g                     lua_pushstring(L, key.c_str());
//g                     lua_pushinteger(L, obj.asInt());
//g                     lua_rawset(L, -3);
//g                 }
//g                 break;
//g             case Value::Type::STRING:
//g                 {
//g                     lua_pushstring(L, key.c_str());
//g                     lua_pushstring(L, obj.asString().c_str());
//g                     lua_rawset(L, -3);
//g                 }
//g                 break;
//g             case Value::Type::VECTOR:
//g                 {
//g                     lua_pushstring(L, key.c_str());
//g                     ccvaluevector_to_luaval(L, obj.asValueVector());
//g                     lua_rawset(L, -3);
//g                 }
//g                 break;
//g             case Value::Type::MAP:
//g                 {
//g                     lua_pushstring(L, key.c_str());
//g                     ccvaluemap_to_luaval(L, obj.asValueMap());
//g                     lua_rawset(L, -3);
//g                 }
//g                 break;
//g             case Value::Type::INT_KEY_MAP:
//g                 {
//g                     lua_pushstring(L, key.c_str());
//g                     ccvaluemapintkey_to_luaval(L, obj.asIntKeyMap());
//g                     lua_rawset(L, -3);
//g                 }
//g                 break;
//g             default:
//g                 break;
//g         }
//g     }
//g }
//g void ccvaluevector_to_luaval(lua_State* L, const cocos2d::ValueVector& inValue)
//g {
//g     lua_newtable(L);
//g     
//g     if (nullptr == L)
//g         return;
//g     
//g     int index  = 1;
//g     for (const auto& obj : inValue)
//g     {
//g         switch (obj.getType())
//g         {
//g             case Value::Type::BOOLEAN:
//g                 {
//g                     lua_pushnumber(L, (lua_Number)index);
//g                     lua_pushboolean(L, obj.asBool());
//g                     lua_rawset(L, -3);
//g                     ++index;
//g                 }
//g                 break;
//g             case Value::Type::FLOAT:
//g             case Value::Type::DOUBLE:
//g                 {
//g                     lua_pushnumber(L, (lua_Number)index);
//g                     lua_pushnumber(L, obj.asDouble());
//g                     lua_rawset(L, -3);
//g                     ++index;
//g                 }
//g                 break;
//g             case Value::Type::INTEGER:
//g                 {
//g                     lua_pushnumber(L, (lua_Number)index);
//g                     lua_pushnumber(L, obj.asInt());
//g                     lua_rawset(L, -3);
//g                     ++index;
//g                 }
//g                 break;
//g             case Value::Type::STRING:
//g                 {
//g                     lua_pushnumber(L, (lua_Number)index);
//g                     lua_pushstring(L, obj.asString().c_str());
//g                     lua_rawset(L, -3);
//g                     ++index;
//g                 }
//g                 break;
//g             case Value::Type::VECTOR:
//g                 {
//g                     lua_pushnumber(L, (lua_Number)index);
//g                     ccvaluevector_to_luaval(L, obj.asValueVector());
//g                     lua_rawset(L, -3);
//g                     ++index;
//g                 }
//g                 break;
//g             case Value::Type::MAP:
//g                 {
//g                     lua_pushnumber(L, (lua_Number)index);
//g                     ccvaluemap_to_luaval(L, obj.asValueMap());
//g                     lua_rawset(L, -3);
//g                     ++index;
//g                 }
//g                 break;
//g             case Value::Type::INT_KEY_MAP:
//g                 {
//g                     lua_pushnumber(L, (lua_Number)index);
//g                     ccvaluemapintkey_to_luaval(L, obj.asIntKeyMap());
//g                     lua_rawset(L, -3);
//g                     ++index;
//g                 }
//g                 break;
//g             default:
//g                 break;
//g         }
//g     }
//g }
//g 
//g void mat4_to_luaval(lua_State* L, const cocos2d::Mat4& mat)
//g {
//g     if (nullptr  == L)
//g         return;
//g     
//g     lua_newtable(L);                                    /* L: table */
//g     int indexTable = 1;
//g     
//g     for (int i = 0; i < 16; i++)
//g     {
//g         lua_pushnumber(L, (lua_Number)indexTable);
//g         lua_pushnumber(L, (lua_Number)mat.m[i]);
//g         lua_rawset(L, -3);
//g         ++indexTable;
//g     }
//g }
//g 
//g void blendfunc_to_luaval(lua_State* L, const cocos2d::BlendFunc& func)
//g {
//g     if (nullptr == L)
//g         return;
//g     
//g     lua_newtable(L);                                    /* L: table */
//g     
//g     lua_pushstring(L, "src");                           /* L: table key */
//g     lua_pushnumber(L, (lua_Number) func.src);           /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     lua_pushstring(L, "dst");                           /* L: table key */
//g     lua_pushnumber(L, (lua_Number) func.dst);           /* L: table key value*/
//g     lua_rawset(L, -3);
//g }
//g 
//g void ttfconfig_to_luaval(lua_State* L, const cocos2d::TTFConfig& config)
//g {
//g     if (nullptr == L)
//g         return;
//g     
//g     lua_newtable(L);
//g     
//g     lua_pushstring(L, "fontFilePath");
//g     lua_pushstring(L, config.fontFilePath.c_str());
//g     lua_rawset(L, -3);
//g     
//g     lua_pushstring(L, "fontSize");
//g     lua_pushnumber(L, (lua_Number)config.fontSize);
//g     lua_rawset(L, -3);
//g     
//g     lua_pushstring(L, "glyphs");
//g     lua_pushnumber(L, (lua_Number)config.glyphs);
//g     lua_rawset(L, -3);
//g     
//g     lua_pushstring(L, "customGlyphs");
//g     if (nullptr != config.customGlyphs && strlen(config.customGlyphs) > 0)
//g     {
//g         lua_pushstring(L, config.customGlyphs);
//g     }
//g     else
//g     {
//g         lua_pushstring(L, "");
//g     }
//g     lua_rawset(L, -3);
//g     
//g     lua_pushstring(L, "distanceFieldEnabled");
//g     lua_pushboolean(L, config.distanceFieldEnabled);
//g     lua_rawset(L, -3);
//g     
//g     lua_pushstring(L, "outlineSize");
//g     lua_pushnumber(L, (lua_Number)config.outlineSize);
//g     lua_rawset(L, -3);
//g }


//g void uniform_to_luaval(lua_State* L, const cocos2d::Uniform& uniform)
//g {
//g     if (nullptr == L)
//g         return;
//g 
//g     lua_newtable(L);
//g     
//g     lua_pushstring(L, "location");
//g     lua_pushnumber(L, (lua_Number)uniform.location);
//g     lua_rawset(L, -3);
//g     
//g     lua_pushstring(L, "size");
//g     lua_pushnumber(L, (lua_Number)uniform.size);
//g     lua_rawset(L, -3);
//g     
//g     lua_pushstring(L, "type");
//g     lua_pushnumber(L, (lua_Number)uniform.type);
//g     lua_rawset(L, -3);
//g     
//g     lua_pushstring(L, "name");
//g     tolua_pushcppstring(L, uniform.name);
//g     lua_rawset(L, -3);
//g }
//g 
//g void vertexattrib_to_luaval(lua_State* L, const cocos2d::VertexAttrib& verAttrib)
//g {
//g     if (nullptr == L)
//g         return;
//g     
//g     lua_newtable(L);
//g     
//g     lua_pushstring(L, "index");
//g     lua_pushnumber(L, (lua_Number)verAttrib.index);
//g     lua_rawset(L, -3);
//g     
//g     lua_pushstring(L, "size");
//g     lua_pushnumber(L, (lua_Number)verAttrib.size);
//g     lua_rawset(L, -3);
//g     
//g     lua_pushstring(L, "type");
//g     lua_pushnumber(L, (lua_Number)verAttrib.type);
//g     lua_rawset(L, -3);
//g     
//g     lua_pushstring(L, "name");
//g     tolua_pushcppstring(L, verAttrib.name);
//g     lua_rawset(L, -3);
//g }
//g 
//g void mesh_vertex_attrib_to_luaval(lua_State* L, const cocos2d::MeshVertexAttrib& inValue)
//g {
//g     if (nullptr == L)
//g         return;
//g     
//g     lua_newtable(L);
//g     
//g     lua_pushstring(L, "size");
//g     lua_pushnumber(L, (lua_Number)inValue.size);
//g     lua_rawset(L, -3);
//g     
//g     lua_pushstring(L, "type");
//g     lua_pushnumber(L, (lua_Number)inValue.type);
//g     lua_rawset(L, -3);
//g     
//g     lua_pushstring(L, "vertexAttrib");
//g     lua_pushnumber(L, (lua_Number)inValue.vertexAttrib);
//g     lua_rawset(L, -3);
//g     
//g     lua_pushstring(L, "attribSizeBytes");
//g     lua_pushnumber(L, (lua_Number)inValue.attribSizeBytes);
//g     lua_rawset(L, -3);
//g }


void ccvector_std_string_to_luaval(lua_State* L, const std::vector<std::string>& inValue)
{
    if (NULL == L)
        return;
    
    lua_newtable(L);
    
    int index = 1;
    
    for (int i = 0; i < inValue.size(); ++i)
    {
		const std::string& value = inValue[i];
        lua_pushnumber(L, (lua_Number)index);
        lua_pushstring(L, value.c_str());
        lua_rawset(L, -3);
        ++index;
    }
}

void ccvector_int_to_luaval(lua_State* L, const std::vector<int>& inValue)
{
    if (NULL == L)
        return;
    
    lua_newtable(L);
    
    int index = 1;
	for (int i = 0; i < inValue.size(); ++i)
    {
		const int value = inValue[i];
        lua_pushnumber(L, (lua_Number)index);
        lua_pushnumber(L, (lua_Number)value);
        lua_rawset(L, -3);
        ++index;
    }
}

void ccvector_float_to_luaval(lua_State* L, const std::vector<float>& inValue)
{
    if (NULL == L)
        return;
    
    lua_newtable(L);
    
    int index = 1;
	for (int i = 0; i < inValue.size(); ++i)
    {
		const float value = inValue[i];
        lua_pushnumber(L, (lua_Number)index);
        lua_pushnumber(L, (lua_Number)value);
        lua_rawset(L, -3);
        ++index;
    }
}

void ccvector_ushort_to_luaval(lua_State* L, const std::vector<unsigned short>& inValue)
{
    if (NULL == L)
        return;
    
    lua_newtable(L);
    
    int index = 1;
	for (int i = 0; i < inValue.size(); ++i)
    {
		const unsigned short value = inValue[i];
        lua_pushnumber(L, (lua_Number)index);
        lua_pushnumber(L, (lua_Number)value);
        lua_rawset(L, -3);
        ++index;
    }
}

//g void quaternion_to_luaval(lua_State* L,const cocos2d::Quaternion& inValue)
//g {
//g     if (NULL  == L)
//g         return;
//g     
//g     lua_newtable(L);                                    /* L: table */
//g     lua_pushstring(L, "x");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number) inValue.x);             /* L: table key value*/
//g     lua_rawset(L, -3);                                  /* table[key] = value, L: table */
//g     lua_pushstring(L, "y");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number) inValue.y);             /* L: table key value*/
//g     lua_rawset(L, -3);
//g     lua_pushstring(L, "z");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number) inValue.z);             /* L: table key value*/
//g     lua_rawset(L, -3);
//g     lua_pushstring(L, "w");                             /* L: table key */
//g     lua_pushnumber(L, (lua_Number) inValue.w);             /* L: table key value*/
//g     lua_rawset(L, -3);
//g }
