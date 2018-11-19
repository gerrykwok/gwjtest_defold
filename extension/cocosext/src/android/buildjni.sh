#!/bin/sh

# 根据LuaJavaBridge.java生成对应的.h

scriptpath=${BASH_SOURCE[0]}
scriptdir="$( cd "$( dirname "$scriptpath" )" && pwd )"
scriptname=$(basename $scriptpath)

#echo scriptdir=$scriptdir,scriptname=$scriptname
cd $scriptdir
                    
classname=LuaJavaBridge

str=$(grep package ${classname}.java)
len=${#str}
package=${str:8:len-9}
headerfile=${package//./_}_${classname}.h
dir=${package//.//}

rm -r com 2> /dev/null
rm $headerfile 2> /dev/null
javac ${classname}.java
mkdir -p $dir
mv ${classname}.class $dir/${classname}.class
javah -jni -d . -classpath . ${package}.${classname}
rm -r com 2> /dev/null
