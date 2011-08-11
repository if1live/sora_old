// Ŭnicode please
#include "../SoraStdAfx.h"
#include "mio/MioConsole.h"
#include "mio/MioPlatform_iOS.h"

/*
//가변인자 변환
#define FMT_TO_STRING(fmt, ...)	\
const int bufsize = 1024 * 64;	\
char buffer[bufsize];	\
va_list argptr;	\
int ret;	\
va_start(argptr, fmt);	\
ret = sprintf(buffer, fmt, argptr);	\
va_end(argptr);	\
std::string msg(buffer);
 #define LOG(format, ...)	\
 CFShow([NSString stringWithFormat:[NSString stringWithUTF8String:format], ## __VA_ARGS__]);
 //워닝안뜨는 한줄로그
 #define LINE_LOG(msg)	CFShow([NSString stringWithUTF8String:msg]);
 */

namespace mio
{
	void Console::log(const char *fmt, ...)
	{
		FMT_TO_STRING(fmt, ...);
		log(msg);
	}
	
	void Console::log(const std::string &msg)
	{
		console().log(msg);
	}
	
	void Console::timelog(const char *fmt, ...)
	{
		FMT_TO_STRING(fmt, ...);
		timelog(msg);
	}
	
	void Console::timelog(const std::string &msg)
	{
		console().timelog(msg);
	}
	
	ConsoleCore &Console::console()
	{
#if _IPHONE_
		static ConsoleCoreIOS c;
		return c;
#elif _WIN_
		static ConsoleCoreWin c;
		return c;
#else
#error "not support platform"
#endif
	}

#if _WIN_
	void ConsoleCoreWin::log(const std::string &msg) const
	{
		printf("%s\n", msg.c_str());
	}
	void ConsoleCoreWin::timelog(const std::string &msg) const
	{
		printf("%s\n", msg.c_str());
	}
#endif
}