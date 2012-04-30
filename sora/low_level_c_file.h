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
#ifndef SORA_LOW_LEVEL_C_FILE_H_
#define SORA_LOW_LEVEL_C_FILE_H_

#include "filesystem.h"

#if SR_USE_PCH == 0
#include <cstdio>
#include <string>
#endif

namespace sora {;

class ReadonlyCFile;
class WriteonlyCFile;
class LowLevelCFile;

class SR_DLL LowLevelCFile {
public:
  LowLevelCFile(const std::string &filepath);
  LowLevelCFile(const char *filepath);
  ~LowLevelCFile();

  bool Open(const char *mode);
  bool Close();
  bool IsOpened() const;

  int Read(void *buf, int size);
  int Seek(int offset, int origin);
  int Write(const void *buf, int size);

  const void *GetBuffer();
  int GetLength() const;
  int GetRemainLength() const;
  const char *filepath() const { return filepath_; }

private:
  FILE *file_;
  char filepath_[kMaxPathLength];
  void *buffer_;
};

class SR_DLL ReadonlyCFile {
public:
  ReadonlyCFile(const std::string &filepath) : file_(filepath) {}
  ReadonlyCFile(const char *filepath) : file_(filepath) {}
  ~ReadonlyCFile() { file_.Close(); }

  bool Open() { return file_.Open("rb"); }
  bool Close() { return file_.Close(); }
  bool IsOpened() const { return file_.IsOpened(); }

  int Read(void *buf, int size) { return file_.Read(buf, size); }
  int Seek(int offset, int origin) { return file_.Seek(offset, origin); }
  const void *GetBuffer() { return file_.GetBuffer(); }

  int GetLength() const { return file_.GetLength(); }
  int GetRemainLength() const { return file_.GetRemainLength(); }
  const char *filepath() const { return file_.filepath(); }

private:
  LowLevelCFile file_;
};

class SR_DLL WriteonlyCFile {
public:
  WriteonlyCFile(const std::string &filepath) : file_(filepath) {}
  WriteonlyCFile(const char *filepath) : file_(filepath) {}
  ~WriteonlyCFile() { file_.Close(); }

  bool Open() { return file_.Open("w"); }
  bool Close() { return file_.Close(); }
  bool IsOpened() const { return file_.IsOpened(); }
  int Write(const void *buf, int size) { return file_.Write(buf, size); }
  int GetLength() const { return file_.GetLength(); }
  int GetRemainLength() const { return file_.GetRemainLength(); }
  const char *filepath() const { return file_.filepath(); }

private:
  LowLevelCFile file_;
};

}

#endif  // SORA_LOW_LEVEL_C_FILE_H_
