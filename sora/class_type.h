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
#ifndef SORA_CLASS_TYPE_H_
#define SORA_CLASS_TYPE_H_

#include "sora/string_code_dictionary.h"

// virtual 기반으로 구현된것
// SR_SUPER_CLASS와 SR_CHILD_CLASS가 한쌍을 구성
#define SR_SUPER_CLASS()  \
public: \
  virtual sora::i32 GetClassType() const = 0; \
  virtual const std::string &GetClassName() const = 0;  \
public:  \
  static sora::StringIntCodeDictionary &GetCodeDictionary() {  \
    static sora::StringIntCodeDictionary dict_;  \
    return dict_; \
  } \
  static const std::string &ClassTypeToName(sora::i32 code) { \
    return GetCodeDictionary().CodeToName(code);  \
  } \
  static sora::i32 ClassNameToType(const std::string &name) { \
    return GetCodeDictionary().NameToCode(name);  \
  }

#define SR_CHILD_CLASS(CLASS_TYPE)  \
public: \
  static sora::i32 ClassType() {  \
    static sora::boolean first_run = true; \
    static sora::i32 code = -1; \
    if (first_run == true) {  \
      first_run = true; \
      code = GetCodeDictionary().Register(std::string(CLASS_TYPE));  \
    } \
    return code;  \
  } \
  static const std::string &ClassName() { \
    static sora::i32 code = ClassType();  \
    static std::string name(CLASS_TYPE);  \
    return name;  \
  } \
public: \
  virtual sora::i32 GetClassType() const { return ClassType(); }  \
  virtual const std::string &GetClassName() const { return ClassName(); }

// virtual 기반이 아닌것
// 자식의 생성자에서 타입을 얻어서 부모생성자로 꽂아줘야되지만
// virtual을 줄일수 있다
// 기본생성자와 같은 곳에서 인자로 받은 class_type를 초기화 시켜줄수있어야한다
#define SR_SUPER_CLASS_2(CLASS_NAME)  \
public: \
  sora::i32 GetClassType() const { return class_type_; }  \
  const std::string &GetClassName() const { return ClassTypeToName(class_type_); }  \
public: \
  static sora::StringIntCodeDictionary &GetCodeDictionary() {  \
    static sora::StringIntCodeDictionary dict_;  \
    return dict_; \
  } \
  static const std::string &ClassTypeToName(sora::i32 code) { \
    return GetCodeDictionary().CodeToName(code);  \
  } \
  static sora::i32 ClassNameToType(const std::string &name) { \
    return GetCodeDictionary().NameToCode(name);  \
  } \
private:  \
  void set_class_type(sora::i32 class_type) { class_type_ = class_type; } \
  sora::i32 class_type_;

#define SR_CHILD_CLASS_2(CLASS_TYPE)  \
public: \
  static sora::i32 ClassType() {  \
    static sora::boolean first_run = true; \
    static sora::i32 code = -1; \
    if (first_run == true) {  \
      first_run = true; \
      code = GetCodeDictionary().Register(std::string(CLASS_TYPE));  \
    } \
    return code;  \
  } \
  static const std::string &ClassName() { \
    static sora::i32 code = ClassType();  \
    static std::string name(CLASS_TYPE);  \
    return name;  \
  }

#endif  // SORA_CLASS_TYPE_H_
