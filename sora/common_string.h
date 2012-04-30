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
#include <sstream>
#include <cstdlib>
#endif

namespace sora {;
SR_C_DLL int StringToInt(const std::string &str);
SR_C_DLL float StringToFloat(const std::string &str);

template<typename T>
T Trim(const T &str) {
  using std::size_t;

  // do no use boost to compile speed + dependenty
  const char *whitespace = "\t\n\x0b\x0c\r ";

  size_t leftFound = str.find_first_not_of(whitespace);
  size_t rightFound = str.find_last_not_of(whitespace);

  if (leftFound == T::npos) {
    leftFound = 0;
  }
  if (rightFound == T::npos) {
    rightFound = str.length();
  }
  T result = str.substr(leftFound, rightFound-leftFound+1);
  return result;
}

template<typename T>
T LeftTrim(const T &str) {
  using std::size_t;
  size_t n = str.find_first_not_of("\t\n\x0b\x0c\r ");
  T left(n == T::npos ? str : str.substr(n, str.length()));
  return left;
}

template<typename T>
T RightTrim(const T &str) {
  using std::size_t;
  size_t n = str.find_last_not_of("\t\n\x0b\x0c\r ");
  T right(n == T::npos ? str : str.substr(0, n + 1));
  return right;
}

SR_C_DLL int Split(const std::string &str, char ch, std::vector<std::string> *retval);

template<typename T>
T Join(const T &str, const std::vector<T> &tokenlist) {
  std::ostringstream oss;
  for (size_t i = 0 ; i < tokenlist.size() ; i++) {
    oss << tokenlist[i];
    if (i != tokenlist.size() - 1) {
      oss << str;
    }
  }
  return oss.str();
}
}

#endif  // SORA_COMMON_STRING_H_

