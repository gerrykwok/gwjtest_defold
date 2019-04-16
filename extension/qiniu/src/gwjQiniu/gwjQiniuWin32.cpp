//////////////////////////////////////////////////////////////////////////
#if defined(DM_PLATFORM_WINDOWS)

#include "../qiniu.h"
#include "qiniu-sdk-win32/io.h"

extern "C" int Qiniu_win32_upload(const char *token, const char *name, const char *localPath, char *message)
{
	Qiniu_Client client;
	Qiniu_Io_PutRet putRet;
	Qiniu_Io_PutExtra putExtra;
	Qiniu_Error error;
	Qiniu_Global_Init(-1);
	Qiniu_Client_InitNoAuth(&client, 1024);
	Qiniu_Zero(putExtra);
	error = Qiniu_Io_PutFile(&client, &putRet, token, name, localPath, &putExtra);
//	if(error.code == 200)
//	{
//		message[0] = 0;
//	}
//	else if(error.code == 614)//目标资源已存在
//	{
//		strcpy(message, error.message);
//	}
//	else
//	{
//		dmLogError("qiniu upload error,code=%d", error.code);
//		strcpy(message, error.message);
//	}
	strcpy(message, error.message);
	Qiniu_Client_Cleanup(&client);
	Qiniu_Global_Cleanup();
	return error.code;
}

#endif
