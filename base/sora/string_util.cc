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
#ifndef BASE_SORA_STRING_UTIL_H_
#define BASE_SORA_STRING_UTIL_H_

#include "sora/string_util.h"

#include <sstream>

namespace sora {;
std::string StringUtil::Trim(const std::string &str) {
  using std::string;

  // do no use boost to compile speed + dependenty
  const char *whitespace = "\t\n\x0b\x0c\r ";

  int leftFound = str.find_first_not_of(whitespace);
  int rightFound = str.find_last_not_of(whitespace);

  if (leftFound == string::npos) {
    leftFound = 0;
  }
  if (rightFound == string::npos) {
    rightFound = str.length();
  }
  string result = str.substr(leftFound, rightFound-leftFound+1);
  return result;
}

std::string StringUtil::LeftTrim(const std::string &str) {
  using std::string;
  int n = str.find_first_not_of("\t\n\x0b\x0c\r ");
  string left(n == string::npos ? str : str.substr(n, str.length()));
  return left;
}

std::string StringUtil::RightTrim(const std::string &str) {
  using std::string;
  int n = str.find_last_not_of("\t\n\x0b\x0c\r ");
  string right(n == string::npos ? str : str.substr(0, n + 1));
  return right;
}
int StringUtil::Split(const std::string &str,
  char ch, std::vector<std::string> *retval) {
  using std::string;
  SR_ASSERT(retval->empty() == true);

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
      // split point
      retval->push_back(tmp_token);
      tmp_token.clear();
    } else {
      tmp_token.push_back(str_ch);
    }
  }
  retval->push_back(tmp_token);
  return retval->size();
}

std::string StringUtil::Join(
    const std::string &str,
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
}
#endif  // BASE_SORA_STRING_UTIL_H_
