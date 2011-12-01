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
#include "sora_stdafx.h"
#include "sora/filesystem.h"

#if SR_USE_PCH == 0
#include <sys/types.h>
#include <sys/stat.h>
#endif

namespace sora {;
std::string app_root_path;
std::string doc_root_path;

int InitFileSystem();
int init_filesystem = InitFileSystem();
int InitFileSystem() {
#if SR_WIN
  // 윈도우에서 실행프로그램이 있는 경로 얻기
  // 이것을 이용해서 경로 변경후 파일을 열자
  TCHAR path[MAX_PATH];
  ::GetModuleFileName(0, path, _MAX_PATH);
  TCHAR* p = _tcsrchr(path, '\\');
  path[p - path] = 0;
  doc_root_path = path;
  app_root_path = path;
  chdir(app_root_path.c_str());
  return 1;
#else
  return 0;
#endif
}

i32 GetFileSize(int fd) {
  if (fd == -1) {
    return -1;
  }
  // library하고 꼬여서 자구 죽네... 다른 방법으로 얻어야될듯
  // struct stat s;
  // int result = fstat(fd, &s);
  // return s.st_size;
  int curr_pos = tell(fd);
  lseek(fd, 0, SEEK_END);
  int length = tell(fd);
  lseek(fd, curr_pos, SEEK_SET);
  return length;
}
i32 GetFileSize(FILE *file) {
  int curr_pos = ftell(file);
  fseek(file, 0, SEEK_END);
  int length = ftell(file);
  fseek(file, curr_pos, SEEK_SET);
  return length;
}
}