#!/bin/sh

# 根据LuaJavaBridge.java生成对应的.h

rm -r com 2> /dev/null
rm com_xishanju_plm_plmext_LuaJavaBridge.h 2> /dev/null

javac LuaJavaBridge.java
mkdir -p com/xishanju/plm/plmext
mv LuaJavaBridge.class com/xishanju/plm/plmext/LuaJavaBridge.class
javah -d . -jni -classpath . com.xishanju.plm.plmext.LuaJavaBridge

rm -r com 2> /dev/null
