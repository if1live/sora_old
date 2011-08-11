#pragma once
#include "mio/MioPath.h"
#include "mio/MioConsole.h"

namespace mio
{
	class PathCoreIOS : public PathCore {
	public:
		std::string docPath(const std::string &file);
		std::string appPath(const std::string &file);
		bool isFileExist(const std::string &file);
	};
	
	class ConsoleCoreIOS : public ConsoleCore {
	public:
		virtual void log(const std::string &msg) const;
		virtual void timelog(const std::string &msg) const;
	};
}