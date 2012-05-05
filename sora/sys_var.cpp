// Copyright (C) 2011-2012 by if1live
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
#include "sys_var.h"
#include "template_lib.h"

namespace sora {;
SysVar SysVar::String(const std::string &str) {
  SysVar var;
  var.SetString(str);
  return var;
}
SysVar SysVar::Int(int i) {
  SysVar var;
  var.SetInt(i);
  return var;
}
SysVar SysVar::Float(float f) {
  SysVar var;
  var.SetFloat(f);
  return var;
}

SysVar::SysVar() {
  SetInt(0);
}
SysVar::~SysVar() {
}
void SysVar::SetString(const std::string &str) {
  type_ = kSysVarString;
  i_value_ = 0;
  str_value_ = str;
}
void SysVar::SetInt(int i) {
  type_ = kSysVarInt;
  i_value_ = i;
  std::ostringstream oss;
  oss << i;
  str_value_ = oss.str();
}
void SysVar::SetFloat(float f) {
  type_ = kSysVarFloat;
  f_value_ = f;
  char buf[64];
  sprintf(buf, "%.4f", f);
  str_value_ = buf;
}

////////////////////////////
SysVarDictionary::SysVarDictionary() :pool_(sizeof(SysVar)) {
}

SysVarDictionary::~SysVarDictionary() {
  auto it = dict_.begin();
  auto endit = dict_.end();
  for( ; it != endit ; ++it) {
    SysVar *var = it->second;
    var->~SysVar();
    pool_.free(var);
  }
}

SysVar *SysVarDictionary::Get(const std::string &key) {
  auto found = dict_.find(key);
  if(found == dict_.end()) {
    return NULL;
  }
  return found->second;
}
bool SysVarDictionary::Add(const std::string &key, const std::string &str) {
  if(IsExist(key) == true) {
    return false;
  }
  SysVar *var = reinterpret_cast<SysVar*>(pool_.malloc());
  CallConstructor(var);
  var->SetString(str);
  dict_[key] = var;
  return true;
}
bool SysVarDictionary::Add(const std::string &key, int i) {
  if(IsExist(key) == true) {
    return false;
  }
  SysVar *var = reinterpret_cast<SysVar*>(pool_.malloc());
  CallConstructor(var);
  var->SetInt(i);
  dict_[key] = var;
  return true;
}
bool SysVarDictionary::Add(const std::string &key, float f) {
  if(IsExist(key) == true) {
    return false;
  }
  SysVar *var = reinterpret_cast<SysVar*>(pool_.malloc());
  CallConstructor(var);
  var->SetFloat(f);
  dict_[key] = var;
  return true;
}
bool SysVarDictionary::IsExist(const std::string &key) {
  auto found = dict_.find(key);
  return (found != dict_.end());
}

} //namespace sora