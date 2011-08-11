// Ŭnicode please
#include "../SoraStdAfx.h"

#include "mio/MioPath.h"
#include "mio/MioPlatform_iOS.h"

#if _WIN_
#include <Windows.h>
#include <tchar.h>
#endif

using namespace std;

namespace mio
{
/*
	std::string Path::extension(const std::string &file)
	{
		std::size_t found = file.find_last_of(".");
		if(found == std::string::npos)
			return std::string("");
		else
			return file.substr(found+1, file.size());
	}
	std::string Path::extension(const char *file)
	{
		const std::string fileStr(file);
		return extension(fileStr);
	}
*/
	std::string Path::docPath(const char *name, const char *ext)
	{
		string filestr;
		filestr += name;
		filestr += ".";
		filestr += ext;
		return path().docPath(filestr);
	}
	std::string Path::docPath(const char *file)
	{
		string filestr(file);
		return path().docPath(filestr);
	}
	std::string Path::docPath(const std::string &file)
	{
		return path().docPath(file);
	}
	
	std::string Path::appPath(const char *name, const char *ext)
	{
		string filestr;
		filestr += name;
		filestr += ".";
		filestr += ext;
		return appPath(filestr);
	}
	std::string Path::appPath(const char *file)
	{
		string filestr(file);
		return appPath(filestr);
	}
	std::string Path::appPath(const std::string &file)
	{
		return path().appPath(file);
	}
	
	bool Path::isFileExist(const std::string &file)
	{
		return path().isFileExist(file);
	}

	PathCore& Path::path()
	{
#if _WIN_
		static PathCoreWin ctx;
		return ctx;
#elif _IPHONE_
		static PathCoreIOS ctx;
		return ctx;
#else 
#error "not support this platform yet"
#endif
	}

	PathCoreWin::PathCoreWin()
	{
		//winapi를 사용해서 경로 얻기
		TCHAR path[MAX_PATH];  
		::GetModuleFileName( 0, path, _MAX_PATH );  
		TCHAR* p = _tcsrchr( path, '\\' );  
		path[p - path] = 0;
		rootPath_ = path;
	}
	std::string PathCoreWin::docPath(const std::string &file)
	{
		return file;
	}
	std::string PathCoreWin::appPath(const std::string &file)
	{
		string filepath = rootPath_ + "\\" + file;
		if(isFileExist(filepath) == true)
		{
			return filepath;
		}
		else
		{
			//파일이 없는 경우 공백문자로 바꿔서 반환한다
			//왜냐하면 app경로로 얻을것은 App안에 내장되어있는것이니까 반드시 존재해야되니까
			return string("");
		}
	}
	bool PathCoreWin::isFileExist(const std::string &file)
	{
		//http://www.gamedev.net/topic/266856-how-to-check-if-a-file-exists-in-c/
		if(FILE *f = fopen(file.c_str(), "r")) 
		{
			fclose(f);
			return true;
		}
		//else..
		return false;
	}
	
}
