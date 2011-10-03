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
#include "mio/path.h"
#include <string>
#include "sora/platform.h"

#if SR_WIN
#include <Windows.h>
#include <tchar.h>
#endif

using std::string;
namespace mio {;
std::string Path::DocPath(const char *name, const char *ext) {
  string filestr;
  filestr += name;
  filestr += ".";
  filestr += ext;
  return DocPath(filestr);
}
std::string Path::DocPath(const char *file) {
  return DocPath(string(file));
}
std::string Path::DocPath(const std::string &file) {
  return GetPath().DocPath(file);
}

std::string Path::AppPath(const char *name, const char *ext) {
  string filestr;
  filestr += name;
  filestr += ".";
  filestr += ext;
  return AppPath(filestr);
}
std::string Path::AppPath(const char *file) {
  return AppPath(string(file));
}
std::string Path::AppPath(const std::string &file) {
  return GetPath().AppPath(file);
}

bool Path::IsFileExist(const std::string &file) {
  // http://www.gamedev.net/topic/266856-how-to-check-if-a-file-exists-in-c/
  if (FILE *f = fopen(file.c_str(), "r")) {
    fclose(f);
    return true;
  }
  return false;
}

PathCore& Path::GetPath() {
  static PathCore ctx;
  return ctx;
}
}
