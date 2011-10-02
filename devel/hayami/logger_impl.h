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
#ifndef DEVEL_HAYAMI_LOGGER_IMPL_H_
#define DEVEL_HAYAMI_LOGGER_IMPL_H_

#include <string>

namespace hayami {;
template<typename LogStream>
Logger<LogStream>::Logger(const std::string &name)
  : name_(name), level(kLogLevelDebug) {
}
template<typename LogStream>
Logger<LogStream>::~Logger() {
}
template<typename LogStream>
const std::string &Logger<LogStream>::name() const {
  return name_;
}
template<typename LogStream>
void Logger<LogStream>::WriteLog(LogLevel log_level, const std::string &msg) {
  if (log_level >= level) {
    stream_.WriteLog(name_, msg);
  }
}

template<typename LogStream>
void Logger<LogStream>::Flush() {
  stream_.Flush();
}
}

#endif  // DEVEL_HAYAMI_LOGGER_IMPL_H_
