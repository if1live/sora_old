// Ŭnicode please
#pragma once

//platform check macro
#if __APPLE__
//iphone check
#import <Availability.h>
	#if __IPHONE_2_0
	#define _IPHONE_ 1
	#endif
#else
	#define _IPHONE_ 0
#endif

//android check
#ifdef __ANDROID__
	#define _ANDROID_ 1
#else
	#define _ANDROID_ 0
#endif

#if (defined _WIN32) || (defined _W64)
#define _WIN_ 1
#else
#define _WIN_ 0
#endif

//assert
#include <cassert>
#if RELEASE
#define MT_ASSERT(STATMENT) do { (void)sizeof(STATMENT); } while(0)
#else
	#if _IPHONE_
	#define MT_ASSERT(STATMENT) do { assert(STATMENT); } while(0)
	#elif _WIN_
		//#if USE_GTEST
		//ref gpg 1
		//#define MT_ASSERT(STATMENT) do { assert(STATMENT); } while(0)
		//extern bool CustomAssertFunction(bool, char*, int, char*);
		//#define MT_ASSERT(STATMENT) \
		//if(CustomAssertFunction((int)(STATMENT), "assert occur", __LINE__, __FILE__))	{ _asm { int 3 } }	///debugger stop
		//내장 assert를 쓰지 못한 이유는 google test가 assert의 함수를 바꿔치기하는데 이것때문에 assert에 걸려도
		//프로그램이 멈추지않고 그냥 죽어버린다. 그래서 강제로 죽이기 위해서 도입
		#define MT_ASSERT(STATMENT) if(!(STATMENT)) { _asm { int 3} }
		//#else
		//#define MT_ASSERT(STATMENT) do { assert(STATMENT); } while(0)
		//#endif
	#else
	#error "cannot use assert"
	#endif
#endif

//custom log console function/macro
//arg to string
#define FMT_TO_STRING(fmt, ...)	\
const int bufsize = 1024 * 64;	\
char buffer[bufsize];	\
va_list argptr;	\
int ret;	\
va_start(argptr, fmt);	\
ret = vsprintf(buffer, fmt, argptr);	\
va_end(argptr);	\
std::string msg(buffer);


//log
#if _IPHONE_
/*#import <Foundation/Foundation.h>
//#define MT_LOG(format, ...)	\
CFShow([NSString stringWithFormat:[NSString stringWithUTF8String:format], ## __VA_ARGS__]);
//#define MT_LINE_LOG(msg)	CFShow([NSString stringWithUTF8String:msg]);
//아이폰에서 로그를 일단 똇다. 다른 함수로 구현해야될듯*/
#define MT_LOG(format, ...) { mio::Console::log(format, ## __VA_ARGS__); }
#define MT_LINE_LOG(msg) { mio::Console::log(msg); }

#elif _ANDROID_
#define MT_LOG(format, ...) do{;}while(0)
#define MT_LINE_LOG(msg) { mio::Console::log(msg); }

#elif _WIN_
#define MT_LOG(format, ...) { fprintf(stderr, format, ## __VA_ARGS__); printf("\n"); }
#define MT_LINE_LOG(msg) { fprintf(stderr, msg);  printf("\n");}

#else
	#error "not support platform"
#endif

//TODO macro
//http://www.gpgstudy.com/forum/viewtopic.php?t=126
//----------------------------------------------------------
// FIXMEs / TODOs / NOTE macros
//----------------------------------------------------------
/*
#define _QUOTE(x) #x
#define QUOTE(x) _QUOTE(x)
#define __FILE__LINE__ __FILE__ "(" QUOTE(__LINE__) ") : "

#define NOTE( x )  message( x )
#define FILE_LINE  message( __FILE__LINE__ )

#define TODO( x )  message( __FILE__LINE__"n" \
        " ------------------------------------------------\n" \
        "|  TODO :   ##x n" \
        " -------------------------------------------------\n" )

#define FIXME( x )  message(  __FILE__LINE__"n" \
        " ------------------------------------------------n"  \
        "|  FIXME :  ##x n" \
        " -------------------------------------------------n" )
#define todo( x )  message( __FILE__LINE__" TODO :   ##x n" ) 
#define fixme( x )  message( __FILE__LINE__" FIXME:   ##x n" ) 

#pragma NOTE("fds")
#pragma message("asdfasdf")
*/


//math const
#define MT_PI (3.141592f)

//방향과 같이 왠지 자주쓸거같은 enum
namespace matsu
{
	///@brief 축과 관련된 내용은 왠지 언젠가 다시 쓸거같아서..
	typedef enum {
		AxisDirectionX,
		AxisDirectionY,
		AxisDirectionZ
	} AxisDirection;
	
	///@brief 2차원에서 방향을 표현하는데 쓰이는것
	typedef enum {
		Direction2Left,
		Direction2Right,
		Direction2Up,
		Direction2Down
	} Direction2;
	
	///@brief 3차원에서 방향을 표현하는데 쓰이는것
	typedef enum {
		Direction3Left,
		Direction3Right,
		Direction3Top,
		Direction3Bottom,
		Direction3Front,
		Direction3Back
	} Direction3;
}
