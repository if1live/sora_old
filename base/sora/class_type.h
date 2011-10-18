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
#ifndef BASE_SORA_CLASS_TYPE_H_
#define BASE_SORA_CLASS_TYPE_H_

#include <vector>
#include <string>
#include <algorithm>

namespace sora {;
class StringCodeDictionary {
public:
  StringCodeDictionary() {}
  ~StringCodeDictionary() {}
  int Register(const std::string &name) {
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
  const std::string &CodeToName(int code) {
    using std::vector;
    using std::string;
    static std::string empty("");
    vector<string> &name_list = str_list_;
    if (code < 0 || code >= name_list.size()) {
      return empty;
    } else {
      return name_list[code];
    }
  }
  int NameToCode(const std::string &name) {
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

// virtual 기반으로 구현된것
// SR_SUPER_CLASS와 SR_CHILD_CLASS가 한쌍을 구성
#define SR_SUPER_CLASS()  \
public: \
  virtual int GetClassType() const = 0; \
  virtual const std::string &GetClassName() const = 0;  \
public:  \
  static sora::StringCodeDictionary &GetCodeDictionary() {  \
    static sora::StringCodeDictionary dict_;  \
    return dict_; \
  } \
  static const std::string &ClassTypeToName(int code) { \
    return GetCodeDictionary().CodeToName(code);  \
  } \
  static int ClassNameToType(const std::string &name) { \
    return GetCodeDictionary().NameToCode(name);  \
  }

#define SR_CHILD_CLASS(CLASS_TYPE)  \
public: \
  static int ClassType() {  \
    static bool first_run = true; \
    static int code = -1; \
    if (first_run == true) {  \
      first_run = true; \
      code = GetCodeDictionary().Register(std::string(CLASS_TYPE));  \
    } \
    return code;  \
  } \
  static const std::string &ClassName() { \
    static int code = ClassType();  \
    static std::string name(CLASS_TYPE);  \
    return name;  \
  } \
public: \
  virtual int GetClassType() const { return ClassType(); }  \
  virtual const std::string &GetClassName() const { return ClassName(); }

// virtual 기반이 아닌것
// 자식의 생성자에서 타입을 얻어서 부모생성자로 꽂아줘야되지만
// virtual을 줄일수 있다
// 기본생성자와 같은 곳에서 인자로 받은 class_type를 초기화 시켜줄수있어야한다
#define SR_SUPER_CLASS_2(CLASS_NAME)  \
public: \
  int GetClassType() const { return class_type_; }  \
  const std::string &GetClassName() const { return ClassTypeToName(class_type_); }  \
public: \
  static sora::StringCodeDictionary &GetCodeDictionary() {  \
    static sora::StringCodeDictionary dict_;  \
    return dict_; \
  } \
  static const std::string &ClassTypeToName(int code) { \
    return GetCodeDictionary().CodeToName(code);  \
  } \
  static int ClassNameToType(const std::string &name) { \
    return GetCodeDictionary().NameToCode(name);  \
  } \
private:  \
  void set_class_type(int class_type) { class_type_ = class_type; } \
  int class_type_;

#define SR_CHILD_CLASS_2(CLASS_TYPE)  \
public: \
  static int ClassType() {  \
    static bool first_run = true; \
    static int code = -1; \
    if (first_run == true) {  \
      first_run = true; \
      code = GetCodeDictionary().Register(std::string(CLASS_TYPE));  \
    } \
    return code;  \
  } \
  static const std::string &ClassName() { \
    static int code = ClassType();  \
    static std::string name(CLASS_TYPE);  \
    return name;  \
  }

#endif  // BASE_SORA_CLASS_TYPE_H_
