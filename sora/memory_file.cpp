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
#include "sora/memory_file.h"
#include "sora/filesystem.h"

#if SR_USE_PCH == 0
#include <fcntl.h>
#endif

namespace sora {;
MemoryFile::MemoryFile(const char *filepath)
  : filepath_(filepath),
  start(NULL),
  end(NULL),
  curr(NULL),
  data(NULL) {
}
MemoryFile::MemoryFile(const std::string &filepath)
  : filepath_(filepath),
  start(NULL),
  end(NULL),
  curr(NULL),
  data(NULL) {
}
MemoryFile::~MemoryFile() {
  Close();
}
boolean MemoryFile::Open() {
  int flag = O_RDONLY;
#if SR_WIN
  flag |= O_BINARY;
#endif
  int fd = open(filepath_.c_str(), flag);
  SR_ASSERT(fd != -1 && "file is not exist");
  if (fd == -1) {
    return false;
  }

  int length = Filesystem::GetFileSize(fd);
  start = (u8*)MM_MALLOC(length + 1);
  data = start;
  read(fd, start, length);

  curr = start;
  end = curr + length;
  *end = '\0';

  close(fd);
  return true;
}
void MemoryFile::Close() {
  MM_FREE(data);
  start = NULL;
  curr = NULL;
  end = NULL;
  data = NULL;
}
i32 MemoryFile::Read(void *buf, i32 size) {
  int curr_pos = curr - start;
  int length = GetLength();
  int remain = length - curr_pos;
  if (remain < size) {
    memcpy((unsigned char*)buf, curr, remain);
    curr += remain;
    return remain;
  } else {
    memcpy((unsigned char*)buf, curr, size);
    curr += size;
    return size;
  }
}
}