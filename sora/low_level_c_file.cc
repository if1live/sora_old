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
  : fd_(-1), filepath_(filepath), buffer_(NULL) {
}
LowLevelCFile::LowLevelCFile(const char *filepath)
 : fd_(-1), filepath_(filepath), buffer_(NULL) {
}
LowLevelCFile::~LowLevelCFile() {
  if (IsOpened()) {
    Close();
  }
}

boolean LowLevelCFile::Open(int flag) {
  if (fd_ != -1) {
    return false;
  }
  fd_ = open(filepath_.c_str(), flag);
  if (fd_ == -1) {
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
  if (fd_ != -1) {
    close(fd_);
    fd_ = -1;
    return true;
  } else {
    return false;
  }
}
boolean LowLevelCFile::IsOpened() const {
  if (fd_ == -1) {
    return false;
  } else {
    return true;
  }
}

i32 LowLevelCFile::Read(void *buf, i32 size) {
  if (fd_ == -1) {
    return -1;
  }
  return read(fd_, buf, size);
}
i32 LowLevelCFile::Seek(i32 offset, i32 origin) {
  if (fd_ == -1) {
    return -1;
  }
  return lseek(fd_, offset, origin);
}
i32 LowLevelCFile::Write(const void *buf, i32 size) {
  if (fd_ == -1) {
    return -1;
  }
  return write(fd_, buf, size);
}

i32 LowLevelCFile::GetLength() const {
  if (fd_ == -1) {
    return 0;
  }
  return GetFileSize(fd_);
}
i32 LowLevelCFile::GetRemainLength() const {
  if (fd_ == NULL) {
    return 0;
  }
  i32 length = GetLength();
  i32 curr_pos = tell(fd_);
  return length - curr_pos;
}
const void *LowLevelCFile::GetBuffer() {
  if (buffer_ == NULL) {
    i32 length = GetLength();
    // 1바이트 더 할당해서 \0로 넣어놓자
    buffer_ = SR_MALLOC(length + 1);

    long curr_pos = tell(fd_);
    lseek(fd_, 0, SEEK_SET);
    read(fd_, buffer_, length);
    lseek(fd_, curr_pos, SEEK_SET);

    ((char*)buffer_)[length] = '\0';
  }
  return buffer_;
}
}