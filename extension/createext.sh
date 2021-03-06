#!/bin/sh

#该脚本创建extension模板

scriptpath=${BASH_SOURCE[0]}
cur_dir="$( cd "$( dirname "$scriptpath" )" && pwd )"
cd $cur_dir

name=$1
if [ -z "$name" ]; then
	echo must specify extension name
	exit
fi
echo creating extension for $name

rm -r $name 2> /dev/null

mkdir -p $name
txt="name: \"$name\""
echo $txt > $name/ext.manifest
mkdir -p $name/src/android
mkdir -p $name/src/ios
mkdir -p $name/src/osx
mkdir -p $name/src/win32

mkdir -p $name/lib/android
mkdir -p $name/lib/ios
mkdir -p $name/lib/osx
mkdir -p $name/lib/win32
mkdir -p $name/lib/armv7-ios
mkdir -p $name/lib/arm64-ios
mkdir -p $name/lib/armv7-android
mkdir -p $name/lib/x86-osx
mkdir -p $name/lib/x86_64-osx
mkdir -p $name/lib/x86-win32
mkdir -p $name/lib/x86_64-win32

touch $name/src/$name.h
touch $name/src/$name.cpp
touch $name/src/android/${name}_android.cpp
touch $name/src/ios/${name}_ios.mm
touch $name/src/osx/${name}_osx.mm
touch $name/src/win32/${name}_win32.cpp

echo -e -n "#pragma once\n\n#include <dmsdk/sdk.h>\n" >> $name/src/$name.h

cpppath=$name/src/$name.cpp

echo -e "#define EXTENSION_NAME $name" >> $cpppath
echo -e "#define LIB_NAME \"$name\"" >> $cpppath
echo -e "#define MODULE_NAME \"$name\"" >> $cpppath
echo -e "" >> $cpppath
echo -e "#include <dmsdk/sdk.h>" >> $cpppath
echo -e "#include \"$name.h\"" >> $cpppath
echo -e "" >> $cpppath
echo -e "static const luaL_reg Module_methods[] =" >> $cpppath
echo -e "{" >> $cpppath
echo -e "	{0, 0}" >> $cpppath
echo -e "};" >> $cpppath
echo -e "" >> $cpppath
echo -e "static void LuaInit(lua_State* L)" >> $cpppath
echo -e "{" >> $cpppath
echo -e "	int top = lua_gettop(L);" >> $cpppath
echo -e "	luaL_register(L, MODULE_NAME, Module_methods);" >> $cpppath
echo -e "	lua_pop(L, 1);" >> $cpppath
echo -e "	assert(top == lua_gettop(L));" >> $cpppath
echo -e "}" >> $cpppath
echo -e "" >> $cpppath
echo -e "static dmExtension::Result extAppInit(dmExtension::AppParams* params)" >> $cpppath
echo -e "{" >> $cpppath
echo -e "	return dmExtension::RESULT_OK;" >> $cpppath
echo -e "}" >> $cpppath
echo -e "" >> $cpppath
echo -e "static dmExtension::Result extInit(dmExtension::Params* params)" >> $cpppath
echo -e "{" >> $cpppath
echo -e '	dmLogInfo("Registered %s Extension", MODULE_NAME);' >> $cpppath
echo -e "	LuaInit(params->m_L);" >> $cpppath
echo -e "	return dmExtension::RESULT_OK;" >> $cpppath
echo -e "}" >> $cpppath
echo -e "" >> $cpppath
echo -e "static dmExtension::Result extAppFinal(dmExtension::AppParams* params)" >> $cpppath
echo -e "{" >> $cpppath
echo -e "	return dmExtension::RESULT_OK;" >> $cpppath
echo -e "}" >> $cpppath
echo -e "" >> $cpppath
echo -e "static void extOnEvent(dmExtension::Params* params, const dmExtension::Event* event)" >> $cpppath
echo -e "{" >> $cpppath
echo -e "}" >> $cpppath
echo -e "" >> $cpppath
echo -e "static dmExtension::Result extFinal(dmExtension::Params* params)" >> $cpppath
echo -e "{" >> $cpppath
echo -e "	return dmExtension::RESULT_OK;" >> $cpppath
echo -e "}" >> $cpppath
echo -e "" >> $cpppath
echo -e "// Defold SDK uses a macro for setting up extension entry points" >> $cpppath
echo -e "// DM_DECLARE_EXTENSION(symbol, name, app_init, app_final, init, update, on_event, final)" >> $cpppath
echo -e "DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, extAppInit, extAppFinal, extInit, 0, extOnEvent, extFinal)" >> $cpppath

echo -e -n "#if defined(DM_PLATFORM_ANDROID)\n\n#include \"../$name.h\"\n\n#endif" >> $name/src/android/${name}_android.cpp
echo -e -n "#if defined(DM_PLATFORM_IOS)\n\n#include \"../$name.h\"\n\n#endif" >> $name/src/ios/${name}_ios.mm
echo -e -n "#if defined(DM_PLATFORM_OSX)\n\n#include \"../$name.h\"\n\n#endif" >> $name/src/osx/${name}_osx.mm
echo -e -n "#if defined(DM_PLATFORM_WINDOWS)\n\n#include \"../$name.h\"\n\n#endif" >> $name/src/win32/${name}_win32.cpp