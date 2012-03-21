﻿/*  Copyright (C) 2011 by if1live */
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

#include <cstdio>
#include <cstdarg>
#include "template_lib.h"

#if SR_ANDROID
// 안드로이드의 경우는 printf가 먹히지 않는다. 그리고 콘솔 출력도 다르다
#include <android/log.h>
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,"SYS",__VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,"SYS",__VA_ARGS__)
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,"SYS",__VA_ARGS__)
#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN,"SYS",__VA_ARGS__)
#define LOGF(...)  __android_log_print(ANDROID_LOG_FATAL,"SYS",__VA_ARGS__)
#define LOGS(...)  __android_log_print(ANDROID_LOG_SILENT,"SYS",__VA_ARGS__)
#define SHOW_LINE()  __android_log_print(ANDROID_LOG_DEBUG,"SYS", "%s / %d", __FILE__, __LINE__)

#define TAG_LOGI(TAG, ...)  __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__)
#define TAG_LOGE(TAG, ...)  __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)
#define TAG_LOGD(TAG, ...)  __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)
#define TAG_LOGW(TAG, ...)  __android_log_print(ANDROID_LOG_WARN,TAG,__VA_ARGS__)
#define TAG_LOGF(TAG, ...)  __android_log_print(ANDROID_LOG_FATAL,TAG,__VA_ARGS__)
#define TAG_LOGS(TAG, ...)  __android_log_print(ANDROID_LOG_SILENT,TAG,__VA_ARGS__)
#define TAG_SHOW_LINE(TAG)  __android_log_print(ANDROID_LOG_DEBUG,TAG, "%s / %d", __FILE__, __LINE__)

#else // 안드로이드 아닌 플랫폼 구현

//그냥 한줄 로그
#define LOGI(...)       { sora::SharedConsoleLogger().Infof(__VA_ARGS__); }
#define LOGE(...)       { sora::SharedConsoleLogger().Errorf(__VA_ARGS__); }
#define LOGD(...)       { sora::SharedConsoleLogger().Debugf(__VA_ARGS__); }
#define LOGW(...)       { sora::SharedConsoleLogger().Warnf(__VA_ARGS__); }
#define LOGF(...)       { sora::SharedConsoleLogger().Fatalf(__VA_ARGS__); }
#define LOGS(...)       { sora::SharedConsoleLogger().Silentf(__VA_ARGS__); }
#define SHOW_LINE()     { sora::SharedConsoleLogger().Infof("%s / %d", __FILE__, __LINE__); }

//로그 시스템 지정해서 로그 찍기
#define TAG_LOGI(TAG, ...)      { sora::AutoGenLoggerList::GetLogger(TAG).Infof(__VA_ARGS__); }
#define TAG_LOGE(TAG, ...)      { sora::AutoGenLoggerList::GetLogger(TAG).Errorf(__VA_ARGS__); }
#define TAG_LOGD(TAG, ...)      { sora::AutoGenLoggerList::GetLogger(TAG).Debugf(__VA_ARGS__); }
#define TAG_LOGW(TAG, ...)      { sora::AutoGenLoggerList::GetLogger(TAG).Warnf(__VA_ARGS__); }
#define TAG_LOGF(TAG, ...)      { sora::AutoGenLoggerList::GetLogger(TAG).Fatalf(__VA_ARGS__); }
#define TAG_LOGS(TAG, ...)      { sora::AutoGenLoggerList::GetLogger(TAG).Silentf(__VA_ARGS__); }
#define TAG_SHOW_LINE(TAG)      { sora::AutoGenLoggerList::GetLogger(TAG).Infof("%s / %d", __FILE__, __LINE__); }

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
    int level = 0;
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
  Logger(const char *name) : level(kLogLevelDebug), name_(name), enable(true) {}
  Logger(const std::string &name) : level(kLogLevelDebug),name_(name), enable(true) {}
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
  char buffer[1024*16];  \
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
    if (enable == false) {
      return;
    }
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
  bool enable;
private:
  std::string name_;
  LogStream stream_;
};


//자동으로 생성되는 로그시스템 관리하는거
class AutoGenLoggerList : public sora::Singleton<AutoGenLoggerList> {
public:
  typedef sora::Logger<ConsoleLogStream> LoggerType;
  typedef std::vector<LoggerType> LoggerListType;
public:
  AutoGenLoggerList();
  ~AutoGenLoggerList();

  //get instance쓰기 귀찮으니까 static으로 때려박기
public:
  static LoggerType &GetLogger(const char *name);
  static void AllDisable() { SetAllState(false); }
  static void AllEnable() { SetAllState(true); }
  static void Disable(const char *name) { SetState(name, false); }
  static void Enable(const char *name) { SetState(name, true); }

  static void SetAllState(bool state);
  static void SetState(const char *name, bool state);
private:
  LoggerListType logger_list_;
};

sora::Logger<ConsoleLogStream> &SharedConsoleLogger();

}
#endif  // win or ios

#endif  // SORA_LOGGER_H_
