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
#pragma once

#include <boost/utility.hpp>
#include <boost/pool/pool.hpp>
#include "unordered_map_inc.h"
#include "template_lib.h"

namespace sora {;

class SysVar {
public:
  typedef enum {
    kSysVarString,
    kSysVarInt,
    kSysVarFloat,
  } SysVarType;

public:
  static SysVar String(const std::string &str);
  static SysVar Int(int i);
  static SysVar Float(float f);

public:
  SysVar();
  ~SysVar();

  void SetString(const std::string &str);
  void SetInt(int i);
  void SetFloat(float f);

  SysVarType type() const { return type_; }
  const std::string &str() const { return str_value_; }
  const int &int_ref() const { return i_value_; }
  const float &float_ref() const { return f_value_; }

private:
  SysVarType type_;
  std::string str_value_;
  union {
    int i_value_;
    float f_value_;
  };
};

////////////////////////////

class SysVarDictionary : public SharedObject<SysVarDictionary> {
public:
  SysVarDictionary();
  ~SysVarDictionary();

  SysVar *Get(const std::string &key);
  bool Add(const std::string &key, const std::string &str);
  bool Add(const std::string &key, int i);
  bool Add(const std::string &key, float f);
  bool IsExist(const std::string &key);

private:
  typedef std::tr1::unordered_map<std::string, SysVar*> SysVarDict;
  SysVarDict dict_;
  boost::pool<> pool_;
};

} //namespace sora 