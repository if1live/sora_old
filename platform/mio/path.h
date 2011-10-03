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
#ifndef PLATFORM_MIO_PATH_H_
#define PLATFORM_MIO_PATH_H_

#include <string>
#include <algorithm>

namespace mio {;
class Path;
class PathCore;

// namespace 함수/클래스 동시지원
// (클래스를 살린건 크로스 플랫폼으로 갈 경우 정리를 편하게 하기 위해서)
class Path {
 public:
  static std::string DocPath(const char *name, const char *ext);
  static std::string DocPath(const char *file);
  static std::string DocPath(const std::string &file);

  static std::string AppPath(const char *name, const char *ext);
  static std::string AppPath(const char *file);
  static std::string AppPath(const std::string &file);

  static bool IsFileExist(const std::string &file);

  // path객체를 얻는 함수. 플랫폼별로 적절히 구현하기
  static PathCore& GetPath();
};

// 플랫폼에 따라서 다르게 구현하면 virtual을 쓰는 짓을 안해도 되고
// 헤더나 클래스를 따로 만들 필요도없다
class PathCore {
 public:
  PathCore();
  ~PathCore();
  std::string DocPath(const std::string &file);
  /// @return 해당경로에 아무것도 없으면 빈 문자열
  std::string AppPath(const std::string &file);
};
}

#endif  // PLATFORM_MIO_PATH_H_
