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
#include "sora/common_string.h"
#if SR_USE_PCH == 0
#include <sstream>
#include <cstdlib>
#endif

namespace sora {;
std::string Trim(const std::string &str) {
  using std::string;

  // do no use boost to compile speed + dependenty
  const char *whitespace = "\t\n\x0b\x0c\r ";

  i32 leftFound = str.find_first_not_of(whitespace);
  i32 rightFound = str.find_last_not_of(whitespace);

  if (leftFound == string::npos) {
    leftFound = 0;
  }
  if (rightFound == string::npos) {
    rightFound = str.length();
  }
  string result = str.substr(leftFound, rightFound-leftFound+1);
  return result;
}

std::string LeftTrim(const std::string &str) {
  using std::string;
  i32 n = str.find_first_not_of("\t\n\x0b\x0c\r ");
  string left(n == string::npos ? str : str.substr(n, str.length()));
  return left;
}

std::string RightTrim(const std::string &str) {
  using std::string;
  i32 n = str.find_last_not_of("\t\n\x0b\x0c\r ");
  string right(n == string::npos ? str : str.substr(0, n + 1));
  return right;
}
i32 Split(const std::string &str,
  char ch, std::vector<std::string> *retval) {
  using std::string;
  //SR_ASSERT(retval->empty() == true);

  // if no delimeter
  if (str.length() == 0) {
    retval->push_back(str);
    return retval->size();
  }

  // simple impl
  string tmp_token;
  for (size_t i = 0 ; i < str.length() ; i++) {
    char str_ch = str[i];
    if (str_ch == ch) {
      // split poi32
      retval->push_back(tmp_token);
      tmp_token.clear();
    } else {
      tmp_token.push_back(str_ch);
    }
  }
  retval->push_back(tmp_token);
  return retval->size();
}

std::string Join(const std::string &str,
    const std::vector<std::string> &tokenlist) {
  using std::ostringstream;
  ostringstream oss;
  for (size_t i = 0 ; i < tokenlist.size() ; i++) {
    oss << tokenlist[i];
    if (i != tokenlist.size() - 1) {
      oss << str;
    }
  }
  return oss.str();
}
i32 StringToInt(const char *str) {
  i32 value = 0;
  value = atoi(str);
  return value;
}
f32 StringToFloat(const char *str) {
  f32 value = 0;
  value = static_cast<f32>(atof(str));
  return value;
}
i32 StringToInt(const std::string &str) {
  return StringToInt(str.c_str());
}
f32 StringToFloat(const std::string &str) {
  return StringToFloat(str.c_str());
}
}
