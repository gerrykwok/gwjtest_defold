#define EXTENSION_NAME dopenssl
#define LIB_NAME "dopenssl"
#define MODULE_NAME "dopenssl"

#define DLIB_LOG_DOMAIN LIB_NAME
#include <dmsdk/sdk.h>

#include <openssl/rand.h>

static const char hex[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

static bool bin_to_hex(const unsigned char *in, int in_length, char *out, int out_length) {
    if (in_length < 0 || out_length <= in_length * 2) {
        return false;
    }
    for (int i = 0; i < in_length; i++) {
        unsigned char c = in[i];
        out[i * 2] = hex[c >> 4];
        out[i * 2 + 1] = hex[c & 0xf];
    }
    out[in_length * 2] = '\0';
    return true;
}

static bool generate_random_data(unsigned char *buffer, unsigned long size) {
    int result = RAND_bytes(buffer, (int)size);
    return result == 1;
}

static int openssl_generate_uuid(lua_State *L) {
    unsigned char uuid[16];
    char uuid_string[36];
    if (generate_random_data(uuid, sizeof(uuid))) {
        bin_to_hex(uuid, sizeof(uuid), uuid_string, sizeof(uuid_string));
        lua_pushstring(L, uuid_string);
    } else {
        lua_pushnil(L);
    }
    return 1;
}

static int openssl_newindex(lua_State *L) {
    lua_pushliteral(L, "Attempt to change readonly table");
    lua_error(L);
    return 0;
}

static const luaL_Reg openssl_lib[] = {
    { "generate_uuid",      openssl_generate_uuid   },
    { "__newindex",         openssl_newindex        },
    { NULL, NULL }
};

static void CreateMeta(lua_State *L, const char *name, const luaL_Reg *lib)
{
    luaL_newmetatable(L, name);
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_rawset(L, -3);
    luaL_openlib(L, NULL, lib, 0);
    lua_pop(L, 1);
}

static void LuaInit(lua_State* L)
{
    int top = lua_gettop(L);

    // CreateMeta(L, tcp_socket_meta, tcp_socket_lib);

    luaL_register(L, MODULE_NAME, openssl_lib);
    lua_pushvalue(L, -1);
    lua_setmetatable(L, -2);

    lua_pop(L, 1);
    assert(top == lua_gettop(L));
}

static void InitializeDefoldOpenSSL()
{
    RAND_poll();
}

static void FinalizeDefoldOpenSSL()
{}


dmExtension::Result AppInitializeDefoldOpenSSLExtension(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

dmExtension::Result AppFinalizeDefoldOpenSSLExtension(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

dmExtension::Result InitializeDefoldOpenSSLExtension(dmExtension::Params* params)
{
    dmLogInfo("Defold openssl extension initialized.")
    LuaInit(params->m_L);
    InitializeDefoldOpenSSL();
    return dmExtension::RESULT_OK;
}

dmExtension::Result FinalizeDefoldOpenSSLExtension(dmExtension::Params* params)
{
    FinalizeDefoldOpenSSL();
    dmLogInfo("Defold openssl extension finalized.")
    return dmExtension::RESULT_OK;
}

DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, AppInitializeDefoldOpenSSLExtension, AppFinalizeDefoldOpenSSLExtension, InitializeDefoldOpenSSLExtension, 0, 0, FinalizeDefoldOpenSSLExtension)

