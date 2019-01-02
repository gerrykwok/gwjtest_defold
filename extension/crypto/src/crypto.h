#pragma once

#include <dmsdk/sdk.h>
#include <string>

int crypto_encodeBase64(lua_State *L);
int crypto_decodeBase64(lua_State *L);

char* crypto_encode_base64(const void *inData, int dataLen);
void* crypto_decode_base64(const char *codedString, int *decodeSize);
void crypto_delete_buffer(void *buffer);
