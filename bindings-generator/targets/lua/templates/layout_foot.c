TOLUA_API int register_all_${prefix}(lua_State* tolua_S)
{
    lua_getglobal(tolua_S, "_G");
    if (lua_istable(tolua_S, -1))//stack:...,_G,
    {
        tolua_open(tolua_S);

        #if $target_ns
        tolua_module(tolua_S,"${target_ns}",0);
        tolua_beginmodule(tolua_S,"${target_ns}");
        #else
        tolua_module(tolua_S,nullptr,0);
        tolua_beginmodule(tolua_S,nullptr);
        #end if

        #for jsclass in $sorted_classes
        #if $in_listed_classes(jsclass)
        lua_register_${prefix}_${jsclass}(tolua_S);
        #end if
        #end for

        tolua_endmodule(tolua_S);
    }
    lua_pop(tolua_S, 1);
    return 1;
}

#if $macro_judgement
\#endif
#end if
