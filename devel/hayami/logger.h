// Ŭnicode please
#pragma once

#include "sora/macro.h"
#include "hayami/hayami_enum.h"
#include <string>
#include <cstdarg>

namespace hayami {;
template<typename LogStream>
class Logger {
public:
  Logger(const std::string &name);
  ~Logger();

  const std::string &name() const;

#define LOGGER_FUNC(NAME) \
  void NAME(const std::string &msg) { \
  WriteLog(kLogLevel##NAME, msg); \
  } \
  void NAME(const char *msg) {  \
  WriteLog(kLogLevel##NAME, std::string(msg));  \
  } \
  void NAME##f(const char *fmt, ...) { /*print'f'같은 느낌으로 f넣음*/ \
  char buffer[4096];  \
  va_list argptr; \
  va_start(argptr, fmt);  \
  vsprintf(buffer, fmt, argptr);  \
  va_end(argptr); \
  WriteLog(kLogLevel##NAME, std::string(buffer));  \
  }

  LOGGER_FUNC(Debug);
  LOGGER_FUNC(Notice);
  LOGGER_FUNC(Warning);
  LOGGER_FUNC(Error);
#undef LOGGER_FUNC

  void Flush();
  SR_GETTER_SETTER(LogLevel, level);

private:
  void WriteLog(LogLevel log_level, const std::string &msg);
  std::string name_;
  LogStream stream_;
};
}

#include "hayami/logger.hpp"