#include "../crypto.h"
#include "libbase64.h"

char* crypto_encode_base64(const void *inData, int dataLen)
{
	int bufsize = Base64encode_len(dataLen);
	char *buffer = new char[bufsize];
	int size = Base64encode(buffer, (const char*)inData, dataLen) - 1;
	if(size <= 0)
	{
		delete[] buffer;
		buffer = NULL;
	}
	return buffer;
}

void* crypto_decode_base64(const char *codedString, int *decodeSize)
{
	const char *input = codedString;
	int bufsize = Base64decode_len(input);
	char *buffer = new char[bufsize];
	int size = Base64decode(buffer, input);
	if(size <= 0)
	{
		delete[] buffer;
		buffer = NULL;
		size = 0;
	}
	if(decodeSize) *decodeSize = size;
	return buffer;
}

void crypto_delete_buffer(void *buffer)
{
	if(buffer)
		delete[] buffer;
}
