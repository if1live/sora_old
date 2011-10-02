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
#ifndef DEVEL_HAYAMI_LOGGER_H_
#define DEVEL_HAYAMI_LOGGER_H_

#include <string>
#include <cstdarg>
#include "hayami/hayami_enum.h"

namespace hayami {;
template<typename LogStream>
class Logger {
 public:
  explicit Logger(const std::string &name);
  ~Logger();

  const std::string &name() const;

#define LOGGER_FUNC(NAME) \
  void NAME(const std::string &msg) { \
  WriteLog(kLogLevel##NAME, msg); \
  } \
  void NAME(const char *msg) {  \
  WriteLog(kLogLevel##NAME, std::string(msg));  \
  } \
  void NAME##f(const char *fmt, ...) { /* print'f'같은 느낌으로 f넣음 */ \
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

 public:
  // level의 경우는 필요에 따라서 up/down이 될테니까
  // getter/setter가 둘다필요한데 이를 위해
  // sora의 매크로를 쓰면 의존성이 생겨서 독립배포가 귀찮으니까
  // 그냥 public으로 만들었다
  LogLevel level;

 private:
  void WriteLog(LogLevel log_level, const std::string &msg);
  std::string name_;
  LogStream stream_;
};
}

#include "hayami/logger_impl.h"
#endif  // DEVEL_HAYAMI_LOGGER_H_
