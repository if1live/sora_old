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
#include "common_string.h"


namespace sora {;
int Split(const std::string &str,
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

int StringToInt(const std::string &str) {
  int value = 0;
  value = atoi(str.c_str());
  return value;
}
float StringToFloat(const std::string &str) {
  float value = 0;
  value = static_cast<float>(atof(str.c_str()));
  return value;
}
}
