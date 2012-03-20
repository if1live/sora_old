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
#ifndef SORA_STRING_CODE_DICTIONARY_H_
#define SORA_STRING_CODE_DICTIONARY_H_

#include <vector>
#include <string>
#include <algorithm>

namespace sora {;

template<typename T>  class StringCodeDictionary;
typedef StringCodeDictionary<int> StringIntCodeDictionary;

template<typename T>
class StringCodeDictionary {
public:
  typedef T CodeType;
  typedef std::string NameType;
public:
  StringCodeDictionary() {}
  ~StringCodeDictionary() {}
  T Register(const std::string &name) {
    using std::vector;
    using std::string;
    using std::find;
    using std::distance;
    vector<string> &name_list = str_list_;
    vector<string>::iterator found = find(name_list.begin(),
      name_list.end(), name);
    if (found == name_list.end()) {
      str_list_.push_back(name);
      return str_list_.size() - 1;
    } else {
      return distance(name_list.begin(), found);
    }
  }
  const std::string &CodeToName(T code) {
    using std::vector;
    using std::string;
    static std::string empty("");
    vector<string> &name_list = str_list_;
    if ((int)code < 0 || code >= (int)name_list.size()) {
      return empty;
    } else {
      return name_list[code];
    }
  }
  T NameToCode(const std::string &name) {
    using std::vector;
    using std::string;
    using std::find;
    using std::distance;
    vector<string> &name_list = str_list_;
    vector<string>::iterator found = find(name_list.begin(),
      name_list.end(), name);
    if (found == name_list.end()) {
      return -1;
    } else {
      return distance(name_list.begin(), found);
    }
  }
private:
  std::vector<std::string> str_list_;
};
}

#endif  // BASE_SORA_STRING_CODE_DICTIONARY_H_
