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
#ifndef BASE_SORA_FLAG_GENERATOR_H_
#define BASE_SORA_FLAG_GENERATOR_H_

#include <string>
#include <vector>
#include <algorithm>
#include "sora/macro.h"

namespace sora {;
template<typename T>  class FlagGenerator;
typedef FlagGenerator<unsigned int> UintFlagGenerator;
typedef FlagGenerator<unsigned short> UshortFlagGenerator;
typedef FlagGenerator<unsigned char> UbyteFlagGenerator;

template<typename T>
class FlagGenerator {
 public:
  typedef T FlagType;
 public:
  FlagGenerator();
  ~FlagGenerator();
  int MaxFlagSize() const;
  T Get(const std::string &key);
  bool IsExist(const std::string &key) const;
  int Count() const;
 private:
  std::vector<std::string> key_list_;
};

template<typename T>
FlagGenerator<T>::FlagGenerator() {
}

template<typename T>
FlagGenerator<T>::~FlagGenerator() {
}

template<typename T>
int FlagGenerator<T>::MaxFlagSize() const {
  return sizeof(T);
}

template<typename T>
T FlagGenerator<T>::Get(const std::string &key) {
  using std::vector;
  using std::string;
  vector<string>::iterator it = find(key_list_.begin(), key_list_.end(), key);
  if (it == key_list_.end()) {
    if (key_list_.size() >= MaxFlagSize()) {
      SR_ASSERT(!"cannot create new flag");
    }
    key_list_.push_back(key);
    int index = key_list_.size();
    return (1 << (index - 1));
  } else {
    int dist = distance(key_list_.begin(), it);
    return (1 << dist);
  }
}

template<typename T>
bool FlagGenerator<T>::IsExist(const std::string &key) const {
  using std::vector;
  using std::string;
  vector<string>::const_iterator it = find(key_list_.begin(),
    key_list_.end(), key);
  if (it == key_list_.end()) {
    return false;
  } else {
    return true;
  }
}

template<typename T>
int FlagGenerator<T>::Count() const {
  return key_list_.size();
}
}
#endif  // BASE_SORA_FLAG_GENERATOR_H_
