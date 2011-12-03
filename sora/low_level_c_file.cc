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
#include "sora/filesystem.h"

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

boolean LowLevelCFile::Open(const char *mode) {
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
boolean LowLevelCFile::Close() {
  if (buffer_ != NULL) {
    SR_FREE(buffer_);
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
boolean LowLevelCFile::IsOpened() const {
  if (file_ == NULL) {
    return false;
  } else {
    return true;
  }
}

i32 LowLevelCFile::Read(void *buf, i32 size) {
  if (file_ == NULL) {
    return -1;
  }
  return fread(buf, size, 1, file_);
}
i32 LowLevelCFile::Seek(i32 offset, i32 origin) {
  if (file_ == NULL) {
    return -1;
  }
  return fseek(file_, offset, origin);
}
i32 LowLevelCFile::Write(const void *buf, i32 size) {
  if (file_ == NULL) {
    return -1;
  }
  return fwrite(buf, size, 1, file_);
}

i32 LowLevelCFile::GetLength() const {
  if (file_ == NULL) {
    return 0;
  }
  return Filesystem::GetFileSize(file_);
}
i32 LowLevelCFile::GetRemainLength() const {
  if (file_ == NULL) {
    return 0;
  }
  i32 length = GetLength();
  i32 curr_pos = ftell(file_);
  return length - curr_pos;
}
const void *LowLevelCFile::GetBuffer() {
  if (buffer_ == NULL) {
    i32 length = GetLength();
    // 1바이트 더 할당해서 \0로 넣어놓자
    buffer_ = SR_MALLOC(length + 1);

    long curr_pos = ftell(file_);
    fseek(file_, 0, SEEK_SET);
    fread(buffer_, length, 1, file_);
    fseek(file_, curr_pos, SEEK_SET);

    ((char*)buffer_)[length] = '\0';
  }
  return buffer_;
}
}