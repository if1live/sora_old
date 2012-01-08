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

#if SR_USE_PCH == 0
#include <string>
#include <boost/noncopyable.hpp>
#endif

namespace sora {;
class MemoryFile : boost::noncopyable {
public:
  MemoryFile(const char *filepath);
  MemoryFile(const std::string &filepath);
  ~MemoryFile();

  boolean Open();
  boolean IsOpened() const { return (data != NULL); }
  void Close();
  i32 GetLength() const { return end - start; }
  i32 Read(void *buf, i32 size);
  
  const std::string &filepath() const { return filepath_; }

public:
  // data
  u8 *start;
  u8 *curr;
  u8 *end;
  void *data;

  std::string filepath_;
};

}

#endif  // SORA_MEMORY_FILE_H_
