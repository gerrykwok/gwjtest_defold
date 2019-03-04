#include "UnzipUtil.h"
#include "unzip_src/unzip.h"
#include "FileUtil.h"
#include <extension/luajavaoc/src/LuaValue.h>
#ifndef DM_PLATFORM_WINDOWS
#include <pthread.h>
#endif

#define UNZIP_STATUS_STOP		0
#define UNZIP_STATUS_RUNNING	1
#define UNZIP_STATUS_FINISH		2

#define UNZIP_DEBUGLOG			1

UnzipUtil* UnzipUtil::create(const char *zipFile)
{
	UnzipUtil *ret = new UnzipUtil;
	ret->m_zipFile = zipFile;
	ret->m_unzipStatus = UNZIP_STATUS_STOP;
	ret->m_unzipResult = 0;
	ret->m_handleSchedule = 0;
	return ret;
}

void UnzipUtil::unzipOneFile(const char *pathInZip, const char *targetPath, bool needPassword, const char *password, LUA_FUNCTION handler)
{
	if(this->m_unzipStatus != UNZIP_STATUS_STOP)
	{
		dmLogInfo("still unzipping");
		return;
	}
	m_pathInZip = pathInZip;
	m_targetPath = targetPath;
	m_needPassword = needPassword;
	m_password = password;
	m_handler = handler;
	if(handler == 0)
		this->unzipOneFileBase();
	else
	{
		this->m_unzipStatus = UNZIP_STATUS_RUNNING;
		this->m_handleSchedule = ext_scheduleUpdate([=]() {
			this->update_unzipOne(0);
		});
#ifdef DM_PLATFORM_WINDOWS
		CreateThread(NULL,          // default security attributes
			0,             // use default stack size
			ThreadUnzipOne,   // thread function name
			this,          // argument to thread function
			0,             // use default creation flags
			NULL);
#else
		pthread_t thread;
		pthread_create(&thread, NULL, ThreadUnzipOne, this);
		pthread_detach(thread);
#endif
	}
}

void UnzipUtil::unzipOneFileBase()
{
	const char *zipFile = m_zipFile.c_str();
	const char *pathInZip = m_pathInZip.c_str();
	const char *targetPath = m_targetPath.c_str();
	bool needPassword = m_needPassword;
	const char *password = m_password.c_str();
	unzFile f;
	int ret = 0;
	int openRet;
	f = unzOpen(zipFile);
	if(f)
	{
		if(unzLocateFile(f, pathInZip, 0) == UNZ_OK)
		{
			openRet = needPassword ? unzOpenCurrentFilePassword(f, password) : unzOpenCurrentFile(f);
			if(openRet == UNZ_OK)
			{
				int bufSize = 4096;
				int readed;
				unsigned char *buffer = new unsigned char[bufSize];
				FILE *fp = NULL;
				while(unzeof(f) != 1)
				{
					readed = unzReadCurrentFile(f, buffer, bufSize);
					if(readed <= 0)
					{
						if(readed < 0)
							ret = 4;//读取zip中文件出错
						break;
					}
					else
					{
						if(fp == NULL)
							fp = fopen(targetPath, "wb");
					}
					if(fp) fwrite(buffer, 1, readed, fp);
				}
				if(fp) fclose(fp);
				delete[] buffer;

				unzCloseCurrentFile(f);
			}
			else
			{
				if(UNZIP_DEBUGLOG) dmLogError("unzip:open inzip %s failed", pathInZip);
				ret = 3;//打开zip中文件出错
			}
		}
		else
		{
			if(UNZIP_DEBUGLOG) dmLogError("unzip:locateFile failed");
			ret = 2;//定位zip中文件出错
		}
		unzClose(f);
	}
	else
	{
		if(UNZIP_DEBUGLOG) dmLogError("unzip:open %s failed", zipFile);
		ret = 1;//打开zip文件失败
	}
	this->m_unzipResult = ret;
	this->m_unzipStatus = UNZIP_STATUS_FINISH;
}

void UnzipUtil::unzipAllFile(const char *targetDir, bool needPassword, const char *password, LUA_FUNCTION handler)
{
	this->m_targetDir = targetDir;
	this->m_needPassword = needPassword;
	this->m_password = password;
	if(handler == 0)
		this->unzipAllFileBase();
	else
	{
		this->m_handler = handler;
		this->m_unzipStatus = UNZIP_STATUS_RUNNING;
		this->m_iFile = 0;
		this->m_handleSchedule = ext_scheduleUpdate([=](){
			this->update_unzipAll(0);
		});
#ifdef DM_PLATFORM_WINDOWS
		CreateThread(NULL,          // default security attributes
			0,             // use default stack size
			ThreadUnzipAll,   // thread function name
			this,          // argument to thread function
			0,             // use default creation flags
			NULL);
#else
		pthread_t thread;
		pthread_create(&thread, NULL, ThreadUnzipAll, this);
		pthread_detach(thread);
#endif
	}
}

void UnzipUtil::unzipAllFileBase()
{
	const char *zipFile = m_zipFile.c_str();
	const char *targetDir = m_targetDir.c_str();
	bool needPassword = m_needPassword;
	const char *password = m_password.c_str();
	int i;
	unzFile f;
	int ret = 0;
	unz_file_info64 info64;
	char filename[1024];
	char target_dir[1024];
	char fileDir[1024];
	char targetPath[1024];
	int bufSize = 4096;
	int readed;
	unsigned char *buffer;
	FILE *fp;
	int numSuccess = 0;

	this->m_iFile = 0;
	buffer = new unsigned char[bufSize];
	strcpy(target_dir, targetDir);
	if(target_dir[strlen(target_dir) - 1] != '/')
		strcat(target_dir, "/");
	f = unzOpen(zipFile);
	if(f)
	{
		//计算文件总个数
		int num = 0;
		int rr;
		rr = unzGoToFirstFile64(f, &info64, filename, sizeof(filename));
		while(rr == UNZ_OK)
		{
			if(filename[info64.size_filename - 1] != '/')
			{
				num++;
			}
			rr = unzGoToNextFile64(f, &info64, filename, sizeof(filename));
		}
		this->m_filenum = num;

		rr = unzGoToFirstFile64(f, &info64, filename, sizeof(filename));
		while(rr == UNZ_OK)
		{
			if(filename[info64.size_filename - 1] != '/')
			{
//				dmLogInfo("extracting %s", filename);
				sprintf(targetPath, "%s%s", target_dir, filename);
				strcpy(fileDir, targetPath);
				i = strlen(fileDir);
				while(i >= 0)
				{
					if(fileDir[i] == '/')
					{
						fileDir[i] = 0;
						break;
					}
					i--;
				}

				int openRet = needPassword ? unzOpenCurrentFilePassword(f, password) : unzOpenCurrentFile(f);
				if(openRet == UNZ_OK)
				{
					fp = NULL;
					while(unzeof(f) != 1)
					{
						readed = unzReadCurrentFile(f, buffer, bufSize);
						if(readed <= 0)
						{
							break;
						}
						else
						{
							if(fp == NULL)
							{
								fileutil_createDirectory(fileDir);
								fp = fopen(targetPath, "wb");
								if(fp == NULL)
								{
									dmLogError("cannot open %s for writing", targetPath);
								}
							}
						}
						if(fp) fwrite(buffer, 1, readed, fp);
					}
					if(fp)
					{
						fclose(fp);
						numSuccess++;
					}
					else
					{
						dmLogError("extract %s error", filename);
					}

					unzCloseCurrentFile(f);
				}
				else
				{
					dmLogError("unzOpenCurrentFile error:%d", openRet);
				}

				this->m_iFile++;
			}
			rr = unzGoToNextFile64(f, &info64, filename, sizeof(filename));
		}
		unzClose(f);
	}
	else
	{
		ret = 1;//打开zip文件失败
	}
	delete[] buffer;
	if(ret == 0)
	{
		if(numSuccess < this->m_filenum)//部分成功
		{
			dmLogInfo("partial success:%d/%d", numSuccess, this->m_filenum);
			this->m_unzipResult = 1000;
		}
		else this->m_unzipResult = 0;
	}
	else
	{
		this->m_unzipResult = ret;
	}
	this->m_unzipStatus = UNZIP_STATUS_FINISH;
}

THREAD_RETURN UnzipUtil::ThreadUnzipAll(void* userdata)
{
	UnzipUtil *thiz = (UnzipUtil*)userdata;
	thiz->unzipAllFileBase();
	return 0;
}

void UnzipUtil::update_unzipAll(float dt)
{
	switch(this->m_unzipStatus)
	{
	case UNZIP_STATUS_RUNNING:
		if(this->m_handler != 0)
		{
			LuaValueDict dict;
			dict["name"] = LuaValue::stringValue("progress");
			dict["current"] = LuaValue::intValue(this->m_iFile);
			dict["filenum"] = LuaValue::intValue(this->m_filenum);
			lua_State *L = ext_getLuaStateWithCallbackId(this->m_handler);
			ext_pushLuaValueDict(L, dict);
			ext_invokeLuaCallback(this->m_handler, 1);
		}
		break;
	case UNZIP_STATUS_FINISH:
		if(this->m_handler != 0)
		{
			LuaValueDict dict;
			dict["name"] = LuaValue::stringValue("completed");
			dict["filenum"] = LuaValue::intValue(this->m_filenum);
			dict["result"] = LuaValue::intValue(this->m_unzipResult);
			lua_State *L = ext_getLuaStateWithCallbackId(this->m_handler);
			ext_pushLuaValueDict(L, dict);
			ext_invokeLuaCallback(this->m_handler, 1);
		}
		this->m_unzipStatus = UNZIP_STATUS_STOP;
		ext_unscheduleUpdate(m_handleSchedule);
		m_handleSchedule = 0;
		delete this;
		break;
	default:
		ext_unscheduleUpdate(m_handleSchedule);
		m_handleSchedule = 0;
		delete this;
		break;
	}
}

THREAD_RETURN UnzipUtil::ThreadUnzipOne(void* userdata)
{
	UnzipUtil *thiz = (UnzipUtil*)userdata;
	thiz->unzipOneFileBase();
	return 0;
}

void UnzipUtil::update_unzipOne(float dt)
{
	switch(this->m_unzipStatus)
	{
	case UNZIP_STATUS_RUNNING:
		if(this->m_handler != 0)
		{
			LuaValueDict dict;
			dict["name"] = LuaValue::stringValue("progress");
			lua_State *L = ext_getLuaStateWithCallbackId(this->m_handler);
			ext_pushLuaValueDict(L, dict);
			ext_invokeLuaCallback(this->m_handler, 1);
		}
		break;
	case UNZIP_STATUS_FINISH:
		if(this->m_handler != 0)
		{
			LuaValueDict dict;
			dict["name"] = LuaValue::stringValue("completed");
			dict["result"] = LuaValue::intValue(this->m_unzipResult);
			lua_State *L = ext_getLuaStateWithCallbackId(this->m_handler);
			ext_pushLuaValueDict(L, dict);
			ext_invokeLuaCallback(this->m_handler, 1);
		}
		this->m_unzipStatus = UNZIP_STATUS_STOP;
		ext_unscheduleUpdate(m_handleSchedule);
		m_handleSchedule = 0;
		delete this;
		break;
	default:
		ext_unscheduleUpdate(m_handleSchedule);
		m_handleSchedule = 0;
		delete this;
		break;
	}
}

void UnzipUtil::getAllFiles(LUA_FUNCTION handler)
{
	LuaValueArray arr;
	unzFile f;
	unz_file_info64 info64;
	char filename[1024];
	f = unzOpen(m_zipFile.c_str());
	if(f)
	{
		int rr;
		rr = unzGoToFirstFile64(f, &info64, filename, sizeof(filename));
		while(rr == UNZ_OK)
		{
//			if(filename[info64.size_filename - 1] != '/')
//			{
				arr.push_back(LuaValue::stringValue(filename));
//			}
			rr = unzGoToNextFile64(f, &info64, filename, sizeof(filename));
		}
		unzClose(f);
	}
	lua_State *L = ext_getLuaStateWithCallbackId(handler);
	ext_pushLuaValueArray(L, arr);
	ext_invokeLuaCallback(handler, 1);
	ext_unregisterLuaCallback(handler);
}
