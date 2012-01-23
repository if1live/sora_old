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
#ifndef SORA_FILESYSTEM_H_
#define SORA_FILESYSTEM_H_

#if SR_WIN
#define PATH_SEPARATOR  '\\'
#else
#define PATH_SEPARATOR  '/'
#endif
namespace sora {;
class Filesystem {
public:
  static i32 GetFileSize(int fd);
  static i32 GetFileSize(FILE *file);
  static std::string GetExtension(const char *filename) {
    return GetExtension(std::string(filename));
  }
  static std::string GetExtension(const std::string &str);

  static std::string app_root_path;
  static std::string doc_root_path;

  static std::string GetAppPath(const std::string &str) {
    return GetAppPath(str.c_str());
  }
  static std::string GetAppPath(const char *filename);
};
}

#endif  // SORA_FILESYSTEM_H_
