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
#ifndef SORA_MEMORY_FILE_H_
#define SORA_MEMORY_FILE_H_

#include "filesystem.h"

#if SR_USE_PCH == 0
#include <string>
#endif

#if SR_ANDROID
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/native_activity.h>

#endif

namespace sora {;
class SR_DLL MemoryFile {
public:
  MemoryFile(const char *filepath);
  MemoryFile(const std::string &filepath);
  ~MemoryFile();

  bool Open();
  bool IsOpened() const { return (data != NULL); }
  void Close();
  int GetLength() const { return end - start; }
  int Read(void *buf, int size);
  
  const char *filepath() const { return filepath_; }

public:
  // data
  uchar *start;
  uchar *end;
  uchar *curr;
  void *data;

  char filepath_[kMaxPathLength];
};

}

#endif  // SORA_MEMORY_FILE_H_
