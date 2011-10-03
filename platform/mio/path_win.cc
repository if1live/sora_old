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
// Ŭnicode pleasep
#include "mio/path.h"
#include <string>
#include "sora/platform.h"

#if SR_WIN
#include <Windows.h>
#include <tchar.h>

using std::string;
namespace mio {;
static string root_path;
PathCore::PathCore() {
  // winapi를 사용해서 경로 얻기
  TCHAR path[MAX_PATH];
  ::GetModuleFileName(0, path, _MAX_PATH);
  TCHAR* p = _tcsrchr(path, '\\');
  path[p - path] = 0;
  root_path = path;
}
PathCore::~PathCore() {
}
std::string PathCore::DocPath(const std::string &file) {
  string filepath = root_path + "\\" + file;
  return filepath;
}
std::string PathCore::AppPath(const std::string &file) {
  string filepath = root_path + "\\" + file;
  if (Path::IsFileExist(filepath) == true) {
    return filepath;
  } else {
    // 파일이 없는 경우 공백문자로 바꿔서 반환한다
    // 왜냐하면 app경로로 얻을것은 App안에 내장되어있는것이니까
    // 반드시 존재해야되니까
    return string("");
  }
}
}
#endif
