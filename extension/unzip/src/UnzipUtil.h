#ifndef __UNZIPUTIL_H__
#define __UNZIPUTIL_H__

#include <string>

typedef int LUA_FUNCTION;

#ifdef GEN_BINDING
#else
#include "unzip.h"
#endif

#ifdef DM_PLATFORM_WINDOWS
#include <windows.h>
#define THREAD_RETURN		DWORD WINAPI
#else
#define THREAD_RETURN		void*
#endif

class UnzipUtil
{
public:
	static UnzipUtil* create(const char *zipFile);
	/*
	* 从压缩文件解压一个文件
	* @return 0=成功
	*/
	void unzipOneFile(const char *pathInZip, const char *targetPath, bool needPassword, const char *password, LUA_FUNCTION handler);
	/*
	* 从压缩文件解压所有文件到指定目录中(有指定handler即为异步方式)
	* @return 0=成功
	*/
	void unzipAllFile(const char *targetDir, bool needPassword, const char *password, LUA_FUNCTION handler);
	/*
	* 获取Zip中的所有文件
	*/
	void getAllFiles(LUA_FUNCTION handler);
private:
	std::string m_zipFile;

	std::string m_pathInZip;
	std::string m_targetPath;

	std::string m_targetDir;
	bool m_needPassword;
	std::string m_password;

	LUA_FUNCTION m_handler;
	int m_unzipStatus;
	int m_unzipResult;
	int m_filenum;
	int m_iFile;

	unsigned int m_handleSchedule;

	void unzipAllFileBase();
	void unzipOneFileBase();
	void update_unzipAll(float dt);
	void update_unzipOne(float dt);
	static THREAD_RETURN ThreadUnzipAll(void* userdata);
	static THREAD_RETURN ThreadUnzipOne(void* userdata);
};

#endif
