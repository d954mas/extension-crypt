// Extension lib defines
#define LIB_NAME "CryptExtension"
#define MODULE_NAME "crypt"

#include <dmsdk/sdk.h>
#include <dmsdk/dlib/crypt.h>
#include <string.h>
#include <stdlib.h>

static int Crypt_HashSha1(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);

    size_t buflen;
    const char* buf = luaL_checklstring(L, 1, &buflen);
    uint8_t digest[20] = {0};

    dmCrypt::HashSha1((const uint8_t*)buf, buflen, digest);
    lua_pushlstring(L, (char*)&digest, 20);
    return 1;
}

static int Crypt_HashSha256(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);

    size_t buflen;
    const char* buf = luaL_checklstring(L, 1, &buflen);
    uint8_t digest[32] = {0};

    dmCrypt::HashSha256((const uint8_t*)buf, buflen, digest);
    lua_pushlstring(L, (char*)&digest, 32);
    return 1;
}

static int Crypt_HashSha512(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);

    size_t buflen;
    const char* buf = luaL_checklstring(L, 1, &buflen);
    uint8_t digest[64] = {0};

    dmCrypt::HashSha512((const uint8_t*)buf, buflen, digest);
    lua_pushlstring(L, (char*)&digest, 64);
    return 1;
}

static int Crypt_HashMd5(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);

    size_t buflen;
    const char* buf = luaL_checklstring(L, 1, &buflen);
    uint8_t digest[16] = {0};

    dmCrypt::HashMd5((const uint8_t*)buf, buflen, digest);
    lua_pushlstring(L, (char*)&digest, 16);
    return 1;
}

static int Crypt_Base64Encode(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);

    size_t srclen;
    const char* src = luaL_checklstring(L, 1, &srclen);

    // 4 characters to represent every 3 bytes with padding applied
    // for binary data which isn't an exact multiple of 3 bytes.
    // https://stackoverflow.com/a/7609180/1266551
    uint32_t dstlen = srclen * 4 / 3 + 4;
    uint8_t* dst = (uint8_t*)malloc(dstlen);

    if (dmCrypt::Base64Encode((const uint8_t*)src, srclen, dst, &dstlen))
    {
        lua_pushlstring(L, (char*)dst, dstlen);
    }
    else
    {
        lua_pushnil(L);
    }
    free(dst);
    return 1;
}

static int Crypt_Base64Decode(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);

    size_t srclen;
    const char* src = luaL_checklstring(L, 1, &srclen);

    uint32_t dstlen = srclen * 3 / 4;
    uint8_t* dst = (uint8_t*)malloc(dstlen);

    if (dmCrypt::Base64Decode((const uint8_t*)src, srclen, dst, &dstlen))
    {
        lua_pushlstring(L, (char*)dst, dstlen);
    }
    else
    {
        lua_pushnil(L);
    }
    free(dst);
    return 1;
}

static const luaL_reg Module_methods[] =
{
    {"hash_sha1", Crypt_HashSha1},
    {"hash_sha256", Crypt_HashSha256},
    {"hash_sha512", Crypt_HashSha512},
    {"hash_md5", Crypt_HashMd5},
    {"encode_base64", Crypt_Base64Encode},
    {"decode_base64", Crypt_Base64Decode},
    {0, 0}
};

static void LuaInit(lua_State* L)
{
    int top = lua_gettop(L);

    // Register lua names
    luaL_register(L, MODULE_NAME, Module_methods);

    lua_pop(L, 1);
    assert(top == lua_gettop(L));
}

dmExtension::Result InitializeCryptExtension(dmExtension::Params* params)
{
    // Init Lua
    LuaInit(params->m_L);
    dmLogInfo("Registered %s Extension\n", MODULE_NAME);
    return dmExtension::RESULT_OK;
}

DM_DECLARE_EXTENSION(CryptExtension, LIB_NAME, 0, 0, InitializeCryptExtension, 0, 0, 0)
