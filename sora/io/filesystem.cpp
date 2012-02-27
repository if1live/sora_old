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
#include "sora/sora_stdafx.h" 

#include "sora/io/filesystem.h"

#if SR_USE_PCH == 0
#include <sys/types.h>
#include <sys/stat.h>
#endif

#if SR_ANDROID
#endif

namespace sora {;
std::string Filesystem::app_root_path;
std::string Filesystem::doc_root_path;

int InitFileSystem();
int init_filesystem = InitFileSystem();
int InitFileSystem() {
#if SR_WIN
  /*
  // 윈도우에서 실행프로그램이 있는 경로 얻기
  // 이것을 이용해서 경로 변경후 파일을 열자
  TCHAR path[MAX_PATH];
  ::GetModuleFileName(0, path, _MAX_PATH);
  TCHAR* p = _tcsrchr(path, '\\');
  path[p - path] = 0;
  Filesystem::doc_root_path = path;
  Filesystem::app_root_path = path;
  chdir(Filesystem::app_root_path.c_str());
  return 1;
  */
  return 1;
#elif SR_ANDROID
  Filesystem::app_root_path = "assets";
  return 1;
#else
  return 0;
#endif
}

i32 Filesystem::GetFileSize(int fd) {
  if (fd == -1) {
    return -1;
  }
  // library하고 꼬여서 자구 죽네... 다른 방법으로 얻어야될듯
  // struct stat s;
  // int result = fstat(fd, &s);
  // return s.st_size;
#if SR_ANDROID
  #define tell(fd) lseek(fd, 0, SEEK_CUR) 
  int curr_pos = tell(fd);
#else
  int curr_pos = tell(fd);
#endif
  lseek(fd, 0, SEEK_END);
  int length = tell(fd);
  lseek(fd, curr_pos, SEEK_SET);
  return length;
}
i32 Filesystem::GetFileSize(FILE *file) {
  int curr_pos = ftell(file);
  fseek(file, 0, SEEK_END);
  int length = ftell(file);
  fseek(file, curr_pos, SEEK_SET);
  return length;
}
std::string Filesystem::GetExtension(const std::string &str) {
  using std::string;
  // 경로 쪼개느거는 2개다 동시 지원할수 있도록함
  // /. \\를 하나로 합치면 되겠지
  string filename = str;
  char *data = (char*)filename.c_str();
  for (size_t i = 0 ; i < filename.size() ; i++) {
    if (data[i] == '\\') {
      data[i] = '/';
    }
  }

  size_t comma_found = filename.rfind(".");
  size_t separator_found = filename.rfind('/');

  if (comma_found == string::npos) {
		//.을 못찾은 경우, 아마도 확장자 없다
		return string("");
	}
  if (comma_found != string::npos && separator_found == string::npos) {
    string name = filename.substr(comma_found+1);
		return name;
  }
  if (comma_found > separator_found) {
		string name = filename.substr(comma_found+1);
		return name;
  } else {
    return string("");
  }
}

std::string Filesystem::GetAppPath(const char *filename) {
  char last_ch = app_root_path[app_root_path.size()-1];
#if SR_WIN || SR_IOS
  if (last_ch == '/' || last_ch == '\\') {
    return app_root_path + filename;
  } else {
    return app_root_path + PATH_SEPARATOR + filename;
  }
#else
  using namespace std;
  string app_path;
  if (last_ch == '/' || last_ch == '\\') {
    app_path = app_root_path + filename;
  } else {
    app_path = app_root_path + PATH_SEPARATOR + filename;
  }
  //\대신 /로 전부 교체
  for(size_t i = 0 ; i < app_path.size() ; i++) {
    if (app_path[i] == '\\') {
      app_path[i] = '/';
    }
  }
  // /가 연속으로 들어가지 않도록 수정하기
  string result;
  for(size_t i = 0 ; i < app_path.size() ; i++) {
    char ch = app_path[i];
    if(result.empty() == false && result[result.size() - 1] == '/') {
      if(ch == '/') {
        continue;
      }
    }
    result.push_back(ch);
  }
  return result;
#endif
}
}