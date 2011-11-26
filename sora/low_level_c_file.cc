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
#include "sora/low_level_c_file.h"

namespace sora {;
LowLevelCFile::LowLevelCFile(const std::string &filepath)
  : file_(NULL), filepath_(filepath), buffer_(NULL) {
}
LowLevelCFile::LowLevelCFile(const char *filepath)
 : file_(NULL), filepath_(filepath), buffer_(NULL) {
}
LowLevelCFile::~LowLevelCFile() {
  if (IsOpened()) {
    Close();
  }
}

bool LowLevelCFile::Open(const char *mode) {
  if (file_ != NULL) {
    return false;
  }
  file_ = fopen(filepath_.c_str(), mode);
  if (file_ == NULL) {
    return false;
  } else {
    return true;
  }
}
bool LowLevelCFile::Close() {
  if (buffer_ != NULL) {
    free(buffer_);
    buffer_ = NULL;
  }
  if (file_ != NULL) {
    fclose(file_);
    file_ = NULL;
    return true;
  } else {
    return false;
  }
}
bool LowLevelCFile::IsOpened() const {
  if (file_ == NULL) {
    return false;
  } else {
    return true;
  }
}

int LowLevelCFile::Read(void *buf, int size) {
  if (file_ == NULL) {
    return -1;
  }
  return fread(buf, size, 1, file_);
}
int LowLevelCFile::Seek(int offset, int origin) {
  if (file_ == NULL) {
    return -1;
  }
  return fseek(file_, offset, origin);
}
int LowLevelCFile::Write(const void *buf, int size) {
  if (file_ == NULL) {
    return -1;
  }
  return fwrite(buf, size, 1, file_);
}

int LowLevelCFile::GetLength() const {
  if (file_ == NULL) {
    return 0;
  }
  long curr_pos = ftell(file_);
  fseek(file_, 0, kSeekEnd);
  int length = ftell(file_);
  fseek(file_, curr_pos, kSeekSet);
  return length;
}
int LowLevelCFile::GetRemainLength() const {
  if (file_ == NULL) {
    return 0;
  }
  int length = GetLength();
  int curr_pos = ftell(file_);
  return length - curr_pos;
}
const void *LowLevelCFile::GetBuffer() {
  if (buffer_ == NULL) {
    int length = GetLength();
    // 1바이트 더 할당해서 \0로 넣어놓자
    buffer_ = malloc(length + 1);

    long curr_pos = ftell(file_);
    fseek(file_, 0, kSeekSet);
    fread(buffer_, 1, length, file_);
    fseek(file_, curr_pos, kSeekSet);

    ((char*)buffer_)[length] = '\0';
  }
  return buffer_;
}
}