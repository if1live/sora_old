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
#ifndef SORA_COMMON_STRING_H_
#define SORA_COMMON_STRING_H_

#if SR_USE_PCH == 0
#include <string>
#include <vector>
#endif

namespace sora {;
i32 StringToInt(const char *str);
f32 StringToFloat(const char *str);
i32 StringToInt(const std::string &str);
f32 StringToFloat(const std::string &str);

std::string Trim(const std::string &str);
std::string LeftTrim(const std::string &str);
std::string RightTrim(const std::string &str);

i32 Split(const std::string &str,
  char ch, std::vector<std::string> *retval);

std::string Join(const std::string &str,
  const std::vector<std::string> &tokenlist);
}

#endif  // SORA_COMMON_STRING_H_

