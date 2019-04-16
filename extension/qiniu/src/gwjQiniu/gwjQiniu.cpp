//////////////////////////////////////////////////////////////////////////
#if defined(DM_PLATFORM_WINDOWS) || defined(DM_PLATFORM_OSX)

#include <thread>
#include "../qiniu.h"
#include "gwjQiniu.h"

extern "C" int Qiniu_win32_upload(const char *token, const char *name, const char *localPath, char *message);
extern "C" void Qiniu_mac_upload(const char *sToken, const char *sName, const char *sLocalPath, const std::function<void(int, const char*)> &callback);

gwjQiniu::gwjQiniu()
{
	m_luaCallback = 0;
	m_idSchedule = 0;
}

gwjQiniu* gwjQiniu::startNewUpload(const char *token, const char *localPath, const char *name, int luaCallback)
{
	gwjQiniu *qiniu = new gwjQiniu();
	qiniu->init(token, localPath, name, luaCallback);
	qiniu->startUpload();
	return qiniu;
}

void gwjQiniu::init(const char *token, const char *localPath, const char *name, int luaCallback)
{
	m_token = token;
	m_localPath = localPath;
	m_name = name;
	m_luaCallback = luaCallback;
}

void gwjQiniu::startUpload()
{
#if defined(DM_PLATFORM_WINDOWS)
	std::thread thr([=]()
	{
		int code;
		char message[2048];
		message[0] = 0;
		code = Qiniu_win32_upload(m_token.c_str(), m_name.c_str(), m_localPath.c_str(), message);
		onUploadEnd(code, message);
	});
	thr.detach();
#else
	Qiniu_mac_upload(m_token.c_str(), m_name.c_str(), m_localPath.c_str(), [=](int code, const char *message)
	{
		onUploadEnd(code, message);
	});
#endif
}

void gwjQiniu::onUploadEnd(int code, const char *message)
{
//	dmLogInfo("upload end,code=%d,message=%s", code, message);
	int uploadResult = 0;
	if(code == 200)
	{
		uploadResult = 0;
	}
	else if(code == 614)//目标资源已存在
	{
		uploadResult = 1;
	}
	else
	{
		uploadResult = 2;
	}
	int callback = m_luaCallback;
	if(callback > 0)
	{
		char *pMsg = new char[strlen(message) + 4];
		strcpy(pMsg, message);
		ext_performInUpdateThread([=](){
			char str[256];
			sprintf(str, "{\"result\":%d,\"errMsg\":\"%s\"}", uploadResult, pMsg);
			delete[] pMsg;
			ext_invokeLuaCallbackWithString(callback, str);
			ext_unregisterLuaCallback(callback);
		});
	}
	delete this;
}

#endif
