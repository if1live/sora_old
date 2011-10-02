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
#ifndef DEVEL_HAYAMI_HAYAMI_ENUM_H_
#define DEVEL_HAYAMI_HAYAMI_ENUM_H_

namespace hayami {;
// 위에 있는 로그 레벨일수록 높은 수준
typedef enum {
  kLogLevelDebug,
  kLogLevelNotice,
  kLogLevelWarning,
  kLogLevelError,
} LogLevel;

// LogStream 계열
/// @brief 로그를 어떤방식으로 보여줄것인가?
/// 내용을 상속받아 잘 구현한후에 로거에 꽂으면
/// 로거가 메세지를 콘솔에 찍을수도 있고 파일에 쓸수도 있다
/// 또한 로거에 따라서 파일 포맷도 어느정도 정할수있다
/// 스트림 클래스는 void WriteLog(logger_name, msg)를 구현하면 된다
/// 몰아서 하거나, 에러시 강제를 하기위해서서
/// void Flush()도 구현할것
class ConsoleLogStream;

template<typename LogStream>  class Logger;
typedef Logger<ConsoleLogStream> ConsoleLogger;
}
#endif  // DEVEL_HAYAMI_HAYAMI_ENUM_H_
