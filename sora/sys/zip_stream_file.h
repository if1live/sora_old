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
#ifndef SORA_ZIP_STREAM_FILE_H_
#define SORA_ZIP_STREAM_FILE_H_

//zip으로 압축된것을 읽는것은 일단 안드로이드에서만 사용한다
//장치를 안드로이드로만 한정하면 apk파일도 1개니까 구조를 더 간단하게 만들수있다
#if SR_ANDROID
#include <zip.h>
#include <string>

namespace sora {;

// 안드로이드 + apk의 조합은
// zip을 그냥 읽어야되는데 이것은 스트림으로 처리하는것이 좋을거같아서
// 스트림에 특화된 클래스를 만들엇다. 읽는것이 단방향으로만 진행된다
// zip파일 1개를 여는게 아니라 zip파일안에 잇는 파일을 읽을떄 사용한다
class ZipStreamFile {
public:
  static void SetApkFile(const std::string &apk_file);
  static void SetApkFile(const char *apk_file);
public:
  ZipStreamFile(const char *filepath);
  ZipStreamFile(const std::string &filepath);
  ~ZipStreamFile();

  bool Open();
  void Close();
  int Read(void *buf, int size);
  int GetLength() const;
  bool IsOpened() const { return (file_ == NULL ? false : true); }

  const char *filepath() const { return filepath_.c_str(); }

private:
  std::string filepath_;
  zip_file* file_;

  static zip* apk_archive_;
};

}

#endif  // SR_ANDROID

#endif  // SORA_STREAM_FILE_H_
