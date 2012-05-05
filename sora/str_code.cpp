/*  Copyright (C) 2011-2012 by if1live */
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
#include "str_code.h"
#include "string_hash.h"
#include "template_lib.h"

namespace sora {;

typedef RSHash HashClass;

StrCode::StrCode()
  : hashcode(0), str(NULL) {
}
StrCode::StrCode(const std::string &s)
: hashcode(0), str(NULL) {
  Init(s.data(), s.length());
}
StrCode::StrCode(const char *s)
: hashcode(0), str(NULL) {
  int str_length = strlen(s);
  Init(s, str_length);
}
StrCode::~StrCode() {
  if(str != NULL) {
    sora::global_free(str);
    str = NULL;
    hashcode = 0;
  }
}
void StrCode::Init(const char *ptr, int length) {
  str = reinterpret_cast<uchar*>(sora::global_malloc(sizeof(uchar) * length));
  memcpy(str, ptr, sizeof(uchar) * length);
  hashcode = HashClass::Hash((char*)str, length);
}

bool StrCode::operator==(const StrCode &o) const {
  //비교는 해시만 해도 충분하다
  if(o.hashcode != hashcode) { return false; }
  return true;
}

////////////////////////////
StrCodeDictionary::StrCodeDictionary()
: pool_(sizeof(StrCode)) {

}
StrCodeDictionary::~StrCodeDictionary() {
  auto it = dict_.begin();
  auto endit = dict_.end();
  for( ; it != endit ; ++it) {
    StrCode *ptr = it->second;
    ptr->~StrCode();
    //어차피 pool사라질떄 모든 할당된 객체가 사라질테니까 굳이 따로 free할 필요 없다
    //pool_.free(ptr);
  }
}
SCode StrCodeDictionary::Get(const std::string &str) {
  unsigned int hashcode = HashClass::Hash(str);
  auto found = dict_.find(hashcode);
  if(found != dict_.end()) {
    StrCode *obj = found->second;
    return obj->str;
  }

  StrCode *str_code = reinterpret_cast<StrCode*>(pool_.malloc());
  //str_code = new(str_code) StrCode;
  CallConstructor(str_code);
  str_code->Init(str.c_str(), str.length());
  dict_[str_code->hashcode] = str_code;
  return str_code->str;
}

bool StrCodeDictionary::IsExist(const std::string &str) const {
  unsigned int hashcode = HashClass::Hash(str);
  auto found = dict_.find(hashcode);
  return (found != dict_.end());
}
} //namespace sora