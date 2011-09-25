// Ŭnicode please
#pragma once

#include "sora/platform.h"
#include <cassert>

//assert
#ifndef ST_ASSERT
#define SR_ASSERT(STATMENT) do { assert(STATMENT); } while(0)
#endif
/*
#undef SR_ASSERT
#if RELEASE
#define SR_ASSERT(STATMENT) do { (void)sizeof(STATMENT); } while(0)
#else
#if SR_IOS
//#define SR_ASSERT(STATMENT) do { assert(STATMENT); } while(0)
#elif SR_WIN
//#if USE_GTEST
//ref gpg 1
//#define SR_ASSERT(STATMENT) do { assert(STATMENT); } while(0)
//extern bool CustomAssertFunction(bool, char*, int, char*);
#define SR_ASSERT(STATMENT) \
if(CustomAssertFunction((int)(STATMENT), "assert occur", __LINE__, __FILE__))	{ _asm { int 3 } }	///debugger stop
//내장 assert를 쓰지 못한 이유는 google test가 assert의 함수를 바꿔치기하는데 이것때문에 assert에 걸려도
//프로그램이 멈추지않고 그냥 죽어버린다. 그래서 강제로 죽이기 위해서 도입
#define SR_ASSERT(STATMENT) if(!(STATMENT)) { _asm { int 3} }
//#else
//#define SR_ASSERT(STATMENT) do { assert(STATMENT); } while(0)
//#endif
#else
#error "cannot use assert"
#endif
#endif
*/
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
#if SR_IOS
/*#import <Foundation/Foundation.h>
//#define MT_LOG(format, ...)	\
CFShow([NSString stringWithFormat:[NSString stringWithUTF8String:format], ## __VA_ARGS__]);
//#define MT_LINE_LOG(msg)	CFShow([NSString stringWithUTF8String:msg]);
//아이폰에서 로그를 일단 똇다. 다른 함수로 구현해야될듯*/
#define MT_LOG(format, ...) { mio::Console::log(format, ## __VA_ARGS__); }
#define MT_LINE_LOG(msg) { mio::Console::log(msg); }

#elif SR_ANDROID
#define MT_LOG(format, ...) do{;}while(0)
#define MT_LINE_LOG(msg) { mio::Console::log(msg); }

#elif SR_WIN
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


#ifndef SR_GETTER_SETTER
#define SR_GETTER_SETTER(TYPE, NAME)  \
  public: \
  const TYPE &get_##NAME() const { return NAME##_; }  \
  void set_##NAME(const TYPE &v) { NAME##_ = v; } \
private:  \
  TYPE NAME##_
#endif

#ifndef SR_GETTER
#define SR_GETTER(TYPE, NAME) \
public: \
  const TYPE &get_##NAME() const { return NAME##_; }  \
private:  \
  void set_##NAME(const TYPE &v) { NAME##_ = v; } \
  TYPE NAME##_
#endif

#ifndef SR_SETTER
#define SR_SETTER(TYPE, NAME) \
public: \
  void set_##NAME(const TYPE &v) { NAME##_ = v; } \
private:  \
  const TYPE &get_##NAME() const { return NAME##_; }  \
  TYPE NAME##_
#endif