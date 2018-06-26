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
#ifndef __COCOS2DX_SCRIPTING_LUA_COCOS2DXSUPPORT_LUABAISCCONVERSIONS_H__
#define __COCOS2DX_SCRIPTING_LUA_COCOS2DXSUPPORT_LUABAISCCONVERSIONS_H__

#include <math.h>
#include <stdlib.h>
#include "tolua/tolua++.h"
//extern "C" {
//#include "lua.h"
//#include "tolua++.h"
//}
#include "tolua_fix.h"
//#include "cocos2d.h"
//using namespace cocos2d;
#include <map>
#include <vector>
#include <string>

//g extern std::unordered_map<std::string, std::string>  g_luaType;
//g extern std::unordered_map<std::string, std::string>  g_typeCast;

#if COCOS2D_DEBUG >=1
void luaval_to_native_err(lua_State* L,const char* msg,tolua_Error* err, const char* funcName = "");
#endif

//#define LUA_PRECONDITION( condition, ...) if( ! (condition) ) {														\
//cocos2d::log("lua: ERROR: File %s: Line: %d, Function: %s", __FILE__, __LINE__, __FUNCTION__ );                                                         \
//cocos2d::log(__VA_ARGS__);                                                  \
//}

#define LUA_PRECONDITION( condition, ...) if( ! (condition) ) {														\
    printf("lua: ERROR: \n"); \
}
    

extern bool luaval_is_usertype(lua_State* L,int lo,const char* type, int def);
// to native
extern bool luaval_to_ulong(lua_State* L,int lo, unsigned long* outValue, const char* funcName="");
extern bool luaval_to_ushort(lua_State* L, int lo, unsigned short* outValue, const char* funcName = "");
extern bool luaval_to_int32(lua_State* L,int lo,int* outValue, const char* funcName = "");
extern bool luaval_to_uint32(lua_State* L, int lo, unsigned int* outValue, const char* funcName = "");
extern bool luaval_to_uint16(lua_State* L,int lo,uint16_t* outValue, const char* funcName = "");
extern bool luaval_to_boolean(lua_State* L,int lo,bool* outValue, const char* funcName = "");
extern bool luaval_to_number(lua_State* L,int lo,double* outValue, const char* funcName = "");
extern bool luaval_to_long_long(lua_State* L,int lo,long long* outValue, const char* funcName = "");
extern bool luaval_to_std_string(lua_State* L, int lo, std::string* outValue, const char* funcName = "");
extern bool luaval_to_long(lua_State* L,int lo, long* outValue, const char* funcName = "");
//g extern bool luaval_to_ssize(lua_State* L,int lo, ssize_t* outValue, const char* funcName = "");
//g extern bool luaval_to_size(lua_State* L,int lo,Size* outValue, const char* funcName = "");
//g extern bool luaval_to_rect(lua_State* L,int lo,Rect* outValue, const char* funcName = "");
//g extern bool luaval_to_color3b(lua_State* L,int lo,Color3B* outValue, const char* funcName = "");
//g extern bool luaval_to_color4b(lua_State* L,int lo,Color4B* outValue, const char* funcName = "");
//g extern bool luaval_to_color4f(lua_State* L,int lo,Color4F* outValue, const char* funcName = "");
#if CC_USE_PHYSICS
extern bool luaval_to_physics_material(lua_State* L,int lo, cocos2d::PhysicsMaterial* outValue, const char* funcName = "");
#endif
//g extern bool luaval_to_affinetransform(lua_State* L,int lo, AffineTransform* outValue, const char* funcName = "");
//g extern bool luaval_to_fontdefinition(lua_State* L, int lo, FontDefinition* outValue , const char* funcName = "");
//g extern bool luaval_to_mat4(lua_State* L, int lo, cocos2d::Mat4* outValue , const char* funcName = "");
//g extern bool luaval_to_array(lua_State* L,int lo, __Array** outValue, const char* funcName = "");
//g extern bool luaval_to_dictionary(lua_State* L,int lo, __Dictionary** outValue, const char* funcName = "");
//g extern bool luaval_to_array_of_vec2(lua_State* L,int lo,cocos2d::Vec2 **points, int *numPoints, const char* funcName = "");
//g extern bool luavals_variadic_to_array(lua_State* L,int argc, __Array** ret);
//g extern bool luavals_variadic_to_ccvaluevector(lua_State* L, int argc, cocos2d::ValueVector* ret);
//g extern bool luaval_to_vec2(lua_State* L,int lo,cocos2d::Vec2* outValue, const char* funcName = "");
//g extern bool luaval_to_vec3(lua_State* L,int lo,cocos2d::Vec3* outValue, const char* funcName = "");
//g extern bool luaval_to_vec4(lua_State* L,int lo,cocos2d::Vec4* outValue, const char* funcName = "");
//g extern bool luaval_to_blendfunc(lua_State* L, int lo, cocos2d::BlendFunc* outValue, const char* funcName = "");
//g extern bool luaval_to_ttfconfig(lua_State* L, int lo, cocos2d::TTFConfig* outValue, const char* funcName = "");
//g extern bool luaval_to_uniform(lua_State* L, int lo, cocos2d::Uniform* outValue, const char* funcName = "");
//g extern bool luaval_to_vertexattrib(lua_State* L, int lo, cocos2d::VertexAttrib* outValue, const char* funcName = "");

//g static inline bool luaval_to_point(lua_State* L,int lo,cocos2d::Vec2* outValue, const char* funcName = "")
//g {
//g     return luaval_to_vec2(L, lo, outValue);
//g }

//g CC_DEPRECATED_ATTRIBUTE static inline bool luaval_to_kmMat4(lua_State* L, int lo, cocos2d::Mat4* outValue , const char* funcName = "")
//g {
//g     return luaval_to_mat4(L, lo, outValue);
//g }
//g CC_DEPRECATED_ATTRIBUTE static inline bool luaval_to_array_of_Point(lua_State* L,int lo,cocos2d::Vec2 **points, int *numPoints, const char* funcName = "")
//g {
//g     return luaval_to_array_of_vec2(L, lo, points, numPoints);
//g }


//g template <class T>
//g bool luavals_variadic_to_ccvector( lua_State* L, int argc, cocos2d::Vector<T>* ret)
//g {
//g     if (nullptr == L || argc == 0 )
//g         return false;
//g     
//g     bool ok = true;
//g     
//g     for (int i = 0; i < argc; i++)
//g     {
//g         if (lua_isuserdata(L, i + 2))
//g         {
//g             tolua_Error err;
//g             
//g             if (!tolua_isusertype(L, i + 2, "cc.Ref", 0, &err))
//g             {
//g                 ok = false;
//g                 break;
//g             }
//g             T obj = static_cast<T>(tolua_tousertype(L, i + 2, nullptr));
//g             ret->pushBack(obj);
//g         }
//g     }
//g     
//g     return ok;
//g }

//g template <class T>
//g bool luaval_to_ccvector(lua_State* L, int lo , cocos2d::Vector<T>* ret, const char* funcName = "")
//g {
//g     if (nullptr == L || nullptr == ret)
//g         return false;
//g     
//g     bool ok = true;
//g     
//g     tolua_Error tolua_err;
//g     if (!tolua_istable(L, lo, 0, &tolua_err) )
//g         ok = false;
//g     
//g     if (ok)
//g     {
//g         size_t len = lua_objlen(L, lo);
//g         for (size_t i = 0; i < len; i++)
//g         {
//g             lua_pushnumber(L, i + 1);
//g             lua_gettable(L, lo);
//g             
//g             if (lua_isnil(L, -1) || !lua_isuserdata(L, -1))
//g             {
//g                 lua_pop(L, 1);
//g                 continue;
//g             }
//g             
//g 
//g             T cobj = static_cast<T>(tolua_tousertype(L, -1, NULL) );
//g             if (NULL != cobj)
//g                 ret->pushBack(cobj);
//g             
//g             lua_pop(L, 1);
//g         }
//g     }
//g     
//g     return ok;
//g }

bool luaval_to_std_vector_string(lua_State* L, int lo, std::vector<std::string>* ret, const char* funcName = "");
bool luaval_to_std_vector_int(lua_State* L, int lo, std::vector<int>* ret, const char* funcName = "");

//g template <class T>
//g bool luaval_to_ccmap_string_key(lua_State* L, int lo, cocos2d::Map<std::string, T>* ret, const char* funcName = "")
//g {
//g     if(nullptr == L || nullptr == ret || lua_gettop(L) < lo)
//g         return false;
//g     
//g     tolua_Error tolua_err;
//g     bool ok = true;
//g     if (!tolua_istable(L, lo, 0, &tolua_err))
//g     {
//g #if COCOS2D_DEBUG >=1
//g         luaval_to_native_err(L,"#ferror:",&tolua_err);
//g #endif
//g         ok = false;
//g     }
//g     
//g     if (ok)
//g     {
//g         std::string stringKey = "";
//g         lua_pushnil(L);                                             /* first key L: lotable ..... nil */
//g         while ( 0 != lua_next(L, lo ) )                             /* L: lotable ..... key value */
//g         {
//g             if (!lua_isstring(L, -2))
//g             {
//g                 lua_pop(L, 1);                                      /* removes 'value'; keep 'key' for next iteration*/
//g                 continue;
//g             }
//g             
//g             if (lua_isnil(L, -1) || !lua_isuserdata(L, -1))
//g             {
//g                 lua_pop(L, 1);
//g                 continue;
//g             }
//g             
//g             luaval_to_std_string(L, -2, &stringKey);
//g             T obj = static_cast<T>(tolua_tousertype(L, -1, NULL) );
//g             if (nullptr != obj)
//g                 ret->insert(stringKey, obj);
//g                 
//g             lua_pop(L, 1);                                          /* L: lotable ..... key */
//g         }
//g     }
//g     
//g     return ok;
//g }


//g extern bool luaval_to_ccvalue(lua_State* L, int lo, cocos2d::Value* ret, const char* funcName = "");
//g extern bool luaval_to_ccvaluemap(lua_State* L, int lo, cocos2d::ValueMap* ret, const char* funcName = "");
//g extern bool luaval_to_ccvaluemapintkey(lua_State* L, int lo, cocos2d::ValueMapIntKey* ret, const char* funcName = "");
//g extern bool luaval_to_ccvaluevector(lua_State* L, int lo, cocos2d::ValueVector* ret, const char* funcName = "");

template <class T>
bool luaval_to_object(lua_State* L, int lo, const char* type, T** ret)
{
    if(NULL == L || lua_gettop(L) < lo)
        return false;
    
    if (!luaval_is_usertype(L, lo, type, 0))
        return false;
    
    *ret = static_cast<T*>(tolua_tousertype(L, lo, 0));
    
    if (NULL == ret)
        LUA_PRECONDITION(ret, "Invalid Native Object");
    
    return true;
}

//g extern bool luaval_to_mesh_vertex_attrib(lua_State* L, int lo, cocos2d::MeshVertexAttrib* ret, const char* funcName = "");
extern bool luaval_to_std_vector_float(lua_State* L, int lo, std::vector<float>* ret, const char* funcName = "");
extern bool luaval_to_std_vector_ushort(lua_State* L, int lo, std::vector<unsigned short>* ret, const char* funcName = "");
//g extern bool luaval_to_quaternion(lua_State* L,int lo,cocos2d::Quaternion* outValue, const char* funcName = "");

// from native
//g extern void vec2_to_luaval(lua_State* L,const cocos2d::Vec2& vec2);
//g extern void vec3_to_luaval(lua_State* L,const cocos2d::Vec3& vec3);
//g extern void vec4_to_luaval(lua_State* L,const cocos2d::Vec4& vec4);
//g extern void vec2_array_to_luaval(lua_State* L,const cocos2d::Vec2* points, int count);
//g extern void size_to_luaval(lua_State* L,const Size& sz);
//g extern void rect_to_luaval(lua_State* L,const Rect& rt);
//g extern void color3b_to_luaval(lua_State* L,const Color3B& cc);
//g extern void color4b_to_luaval(lua_State* L,const Color4B& cc);
//g extern void color4f_to_luaval(lua_State* L,const Color4F& cc);
//g #if CC_USE_PHYSICS
//g extern void physics_material_to_luaval(lua_State* L,const PhysicsMaterial& pm);
//g extern void physics_raycastinfo_to_luaval(lua_State* L, const PhysicsRayCastInfo& info);
//g extern void physics_contactdata_to_luaval(lua_State* L, const PhysicsContactData* data);
//g #endif
//g extern void affinetransform_to_luaval(lua_State* L,const AffineTransform& inValue);
//g extern void fontdefinition_to_luaval(lua_State* L,const FontDefinition& inValue);
//g extern void array_to_luaval(lua_State* L, __Array* inValue);
//g extern void dictionary_to_luaval(lua_State* L, __Dictionary* dict);
//g extern void mat4_to_luaval(lua_State* L, const cocos2d::Mat4& mat);
//g extern void blendfunc_to_luaval(lua_State* L, const cocos2d::BlendFunc& func);
//g extern void ttfconfig_to_luaval(lua_State* L, const cocos2d::TTFConfig& config);
//g extern void uniform_to_luaval(lua_State* L, const cocos2d::Uniform& uniform);
//g extern void vertexattrib_to_luaval(lua_State* L, const cocos2d::VertexAttrib& verAttrib);

//g static inline void point_to_luaval(lua_State* L,const cocos2d::Vec2& pt)
//g {
//g     vec2_to_luaval(L, pt);
//g }
//g 
//g CC_DEPRECATED_ATTRIBUTE static inline void points_to_luaval(lua_State* L,const cocos2d::Vec2* points, int count)
//g {
//g     vec2_array_to_luaval(L, points, count);
//g }
//g 
//g template <class T>
//g void ccvector_to_luaval(lua_State* L,const cocos2d::Vector<T>& inValue)
//g {
//g     lua_newtable(L);
//g     
//g     if (nullptr == L)
//g         return;
//g     
//g     int indexTable = 1;
//g     for (const auto& obj : inValue)
//g     {
//g         if (nullptr == obj)
//g             continue;
//g         
//g 
//g         if (nullptr != dynamic_cast<cocos2d::Ref *>(obj))
//g         {
//g             std::string typeName = typeid(*obj).name();
//g             auto iter = g_luaType.find(typeName);
//g             if (g_luaType.end() != iter)
//g             {
//g                 lua_pushnumber(L, (lua_Number)indexTable);
//g                 int ID = (obj) ? (int)obj->_ID : -1;
//g                 int* luaID = (obj) ? &obj->_luaID : NULL;
//g                 toluafix_pushusertype_ccobject(L, ID, luaID, (void*)obj,iter->second.c_str());
//g                 lua_rawset(L, -3);
//g                 ++indexTable;
//g             }
//g         }
//g     }
//g }
//g 
//g template <class T>
//g void ccmap_string_key_to_luaval(lua_State* L, const cocos2d::Map<std::string, T>& v)
//g {
//g     lua_newtable(L);
//g     
//g     if(nullptr == L)
//g         return;
//g     
//g     for (auto iter = v.begin(); iter != v.end(); ++iter)
//g     {
//g         std::string key = iter->first;
//g         T obj = iter->second;
//g         if (nullptr != dynamic_cast<cocos2d::Ref *>(obj))
//g         {
//g             std::string name = typeid(*obj).name();
//g             auto typeIter = g_luaType.find(name);
//g             if (g_luaType.end() != typeIter)
//g             {
//g                 lua_pushstring(L, key.c_str());
//g                 int ID = (obj) ? (int)obj->_ID : -1;
//g                 int* luaID = (obj) ? &obj->_luaID : NULL;
//g                 toluafix_pushusertype_ccobject(L, ID, luaID, (void*)obj,typeIter->second.c_str());
//g                 lua_rawset(L, -3);
//g             }
//g         }
//g     }
//g }
//g 
//g void ccvalue_to_luaval(lua_State* L,const cocos2d::Value& inValue);
//g void ccvaluemap_to_luaval(lua_State* L,const cocos2d::ValueMap& inValue);
//g void ccvaluemapintkey_to_luaval(lua_State* L, const cocos2d::ValueMapIntKey& inValue);
//g void ccvaluevector_to_luaval(lua_State* L, const cocos2d::ValueVector& inValue);

/**
 Because all override functions wouldn't be bound,so we must use `typeid` to get the real class name
 */
//g template <class T>
//g const char* getLuaTypeName(T* ret,const char* type)
//g {
//g     if (nullptr != ret)
//g     {
//g         std::string hashName = typeid(*ret).name();
//g         auto iter =  g_luaType.find(hashName);
//g         if(g_luaType.end() != iter)
//g         {
//g             return iter->second.c_str();
//g         }
//g         else
//g         {
//g             return type;
//g         }
//g     }
//g     
//g     return nullptr;
//g }

//g template <class T>
//g void object_to_luaval(lua_State* L,const char* type, T* ret)
//g {
//g     if(nullptr != ret)
//g     {
//g         if (std::is_base_of<cocos2d::Ref, T>::value)
//g         {
//g             // use c style cast, T may not polymorphic
//g             cocos2d::Ref* dynObject = (cocos2d::Ref*)(ret);
//g             int ID = (int)(dynObject->_ID) ;
//g             int* luaID = &(dynObject->_luaID);
//g             toluafix_pushusertype_ccobject(L,ID, luaID, (void*)ret,type);
//g         }
//g         else
//g         {
//g             tolua_pushusertype(L,(void*)ret,getLuaTypeName(ret, type));
//g         }
//g     }
//g     else
//g     {
//g         lua_pushnil(L);
//g     }
//g }
//g 
//g void mesh_vertex_attrib_to_luaval(lua_State* L, const cocos2d::MeshVertexAttrib& inValue);
//g void ccvector_std_string_to_luaval(lua_State* L, const std::vector<std::string>& inValue);
//g void ccvector_int_to_luaval(lua_State* L, const std::vector<int>& inValue);
//g void ccvector_float_to_luaval(lua_State* L, const std::vector<float>& inValue);
//g void ccvector_ushort_to_luaval(lua_State* L, const std::vector<unsigned short>& inValue);
//g void quaternion_to_luaval(lua_State* L,const cocos2d::Quaternion& inValue);

#endif //__COCOS2DX_SCRIPTING_LUA_COCOS2DXSUPPORT_LUABAISCCONVERSIONS_H__
