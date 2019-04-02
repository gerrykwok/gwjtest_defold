//////////////////////////////////////////////////////////////////////////
#include <dmsdk/sdk.h>
#include <vector>
#if defined(DM_PLATFORM_WINDOWS)
#include <windows.h>
#else
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#endif
#include "FileUtil.h"

bool fileutil_isDirectoryExist(const std::string& dirPath)
{
#if defined(DM_PLATFORM_WINDOWS)
	unsigned long fAttrib = GetFileAttributesA(dirPath.c_str());
	if(fAttrib != INVALID_FILE_ATTRIBUTES && (fAttrib & FILE_ATTRIBUTE_DIRECTORY))
	{
		return true;
	}
	return false;
#else
	struct stat st;
	if(stat(dirPath.c_str(), &st) == 0)
	{
		return S_ISDIR(st.st_mode);
	}
	return false;
#endif
}

bool fileutil_createDirectory(const std::string& path)
{
	if(path.empty())
	{
		dmLogError("Invalid path");
		return false;
	}

	if(fileutil_isDirectoryExist(path))
		return true;

	// Split the path
	size_t start = 0;
	size_t found = path.find_first_of("/\\", start);
	std::string subpath;
	std::vector<std::string> dirs;

	if(found != std::string::npos)
	{
		while(true)
		{
			subpath = path.substr(start, found - start + 1);
			if(!subpath.empty())
				dirs.push_back(subpath);
			start = found + 1;
			found = path.find_first_of("/\\", start);
			if(found == std::string::npos)
			{
				if(start < path.length())
				{
					dirs.push_back(path.substr(start));
				}
				break;
			}
		}
	}


#if defined(DM_PLATFORM_WINDOWS)
	if((GetFileAttributesA(path.c_str())) == INVALID_FILE_ATTRIBUTES)
	{
		subpath = "";
		for(int i = 0; i < dirs.size(); ++i)
		{
			subpath += dirs[i];
			if(!fileutil_isDirectoryExist(subpath))
			{
				BOOL ret = CreateDirectoryA(subpath.c_str(), NULL);
				if(!ret && ERROR_ALREADY_EXISTS != GetLastError())
				{
					return false;
				}
			}
		}
	}
	return true;
#else
	DIR *dir = NULL;

	// Create path recursively
	subpath = "";
	for(int i = 0; i < dirs.size(); ++i)
	{
		subpath += dirs[i];
		dir = opendir(subpath.c_str());

		if(!dir)
		{
			// directory doesn't exist, should create a new one

			int ret = mkdir(subpath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
			if(ret != 0 && (errno != EEXIST))
			{
				// current directory can not be created, sub directories can not be created too
				// should return
				return false;
			}
		}
		else
		{
			// directory exists, should close opened dir
			closedir(dir);
		}
	}
	return true;
#endif
}
