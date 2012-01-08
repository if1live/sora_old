/*  Copyright (C) 2011 by if1live */
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// Ŭnicode please
#ifndef SORA_LOGGER_H_
#define SORA_LOGGER_H_

#include <string>
#include <cstdarg>

#if SR_USE_PCH == 0
#include <cstdio>
#include <cstdarg>
#endif

#if SR_ANDROID
// 안드로이드의 경우는 printf가 먹히지 않는다. 그리고 콘솔 출력도 다르다
#include <android/log.h>
//#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,"LOGI",__VA_ARGS__)
//#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,"LOGE",__VA_ARGS__)
#endif

#define LOGI(...)  { sora::SharedConsoleLogger().Infof(__VA_ARGS__); }
#define LOGE(...)  { sora::SharedConsoleLogger().Errorf(__VA_ARGS__); }
#define SHOW_LINE() { sora::SharedConsoleLogger().Infof("%s / %d", __FILE__, __LINE__); }

namespace sora {;
//위에 있는 로그 레벨일수록 높은 수준
typedef enum {
  kLogLevelSilent,
  kLogLevelDefault,
  kLogLevelDebug,
  kLogLevelInfo,
  kLogLevelWarn,
  kLogLevelError,
  kLogLevelFatal,
  //ANDROID_LOG_UNKNOWN = 0,
	//ANDROID_LOG_VERBOSE,
} LogLevel;

class ConsoleLogStream {
public:
  ConsoleLogStream() {}
  ~ConsoleLogStream() {}

  void WriteLog(const char *logger_name, LogLevel log_level, const char *msg) {
#if SR_ANDROID
  i32 level = 0;
  switch(log_level) {
  case kLogLevelSilent:
    level = ANDROID_LOG_SILENT;
    break;
  case kLogLevelDefault:
    level = ANDROID_LOG_DEFAULT;
    break;
  case kLogLevelDebug:
    level = ANDROID_LOG_DEBUG;
    break;
  case kLogLevelInfo:
    level = ANDROID_LOG_INFO;
    break;
  case kLogLevelWarn:
    level = ANDROID_LOG_WARN;
    break;
  case kLogLevelError:
    level = ANDROID_LOG_ERROR;
    break;
  case kLogLevelFatal:
    level = ANDROID_LOG_FATAL;
    break;
  default:
    level = ANDROID_LOG_UNKNOWN;
    break;
  }
	__android_log_print(level, logger_name, msg);
#else
    printf("[%s] %s\n", logger_name, msg);
#endif
  }

  void Flush() {
#if SR_WIN || SR_IOS
    fflush(stdout);
#endif
  }
};

template<typename LogStream>
class Logger {
public:
  Logger(const char *name) : name_(name), level(kLogLevelDebug) {}
  Logger(const std::string &name) : name_(name), level(kLogLevelDebug) {}
  ~Logger() {}

  const std::string &name() const { return name_; }

#define LOGGER_FUNC(NAME) \
  void NAME(const std::string &msg) { \
  WriteLog(kLogLevel##NAME, msg); \
  } \
  void NAME(const char *msg) {  \
  WriteLog(kLogLevel##NAME, std::string(msg));  \
  } \
  void NAME##f(const char *fmt, ...) { /*pri32'f'같은 느낌으로 f넣음*/ \
  char buffer[4096];  \
  va_list argptr; \
  va_start(argptr, fmt);  \
  vsprintf(buffer, fmt, argptr);  \
  va_end(argptr); \
  WriteLog(kLogLevel##NAME, buffer);  \
  }

  LOGGER_FUNC(Silent);
  LOGGER_FUNC(Default);
  LOGGER_FUNC(Debug);
  LOGGER_FUNC(Info);
  LOGGER_FUNC(Warn);
  LOGGER_FUNC(Error);
  LOGGER_FUNC(Fatal);
#undef LOGGER_FUNC

  void Flush() { stream_.Flush(); }

  void WriteLog(LogLevel log_level, const char *msg) {
    if (log_level >= level) {
      stream_.WriteLog(name_.c_str(), log_level, msg);
    }
  }

public:
  // level의 경우는 필요에 따라서 up/down이 될테니까
  // getter/setter가 둘다필요한데 이를 위해
  // sora의 매크로를 쓰면 의존성이 생겨서 독립배포가 귀찮으니까
  // 그냥 public으로 만들었다
  LogLevel level;

  std::string name_;
  LogStream stream_;
};


sora::Logger<ConsoleLogStream> &SharedConsoleLogger();

}

#endif  // SORA_LOGGER_H_
