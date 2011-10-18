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
#ifndef BASE_SORA_STRING_TABLE_H_
#define BASE_SORA_STRING_TABLE_H_

#include "sora/tr1_include.h"

namespace sora {;
template<typename T>
class StringTable {
 public:
  typedef unsigned int HashCodeType;
  typedef std::tr1::unordered_map<HashCodeType, std::string> HashTableType;

 public:
  StringTable();
  ~StringTable();

  HashCodeType ToCode(const std::string &str) const;
  HashCodeType Save(const std::string &str);
  const std::string &Load(HashCodeType code) const;
  bool IsExist(const std::string &str) const;
  bool IsExist(HashCodeType code) const;

 private:
  HashTableType table_;
  T hash_functor_;
};
}

namespace sora{;
template<typename T>
StringTable<T>::StringTable() {
}
template<typename T>
StringTable<T>::~StringTable() {
}
template<typename T>
typename StringTable<T>::HashCodeType StringTable<T>::ToCode(const std::string &str) const {
  return hash_functor_(str);
}
template<typename T>
typename StringTable<T>::HashCodeType StringTable<T>::Save(const std::string &str) {
  HashCodeType code = ToCode(str);
  table_[code] = str;
  return code;
}
template<typename T>
const std::string &StringTable<T>::Load(HashCodeType code) const {
  HashTableType::const_iterator found = table_.find(code);
  if(found == table_.end()) {
    static std::string empty("");
    return empty;
  } else {
    return found->second;
  }
}
template<typename T>
bool StringTable<T>::IsExist(const std::string &str) const {
  HashCodeType code = ToCode(str);
  return IsExist(code);
}
template<typename T>
bool StringTable<T>::IsExist(HashCodeType code) const {
  HashTableType::const_iterator found = table_.find(code);
  return (found != table_.end());
}
}

#endif  // BASE_SORA_STRING_TABLE_H_
