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
#ifndef BASE_SORA_PROPERTY_H_
#define BASE_SORA_PROPERTY_H_

#define SR_PROPERTY(TYPE, CLASS_NAME, GETTER, SETTER, NAME)  \
  template< \
  typename T, \
  typename ConType, \
  T (ConType::*GetFunc)(),  \
  T (ConType::*SetFunc)(const T &) \
  > \
class Property_##CLASS_NAME##_##NAME {  \
public: \
  Property_##CLASS_NAME##_##NAME() {} \
  Property_##CLASS_NAME##_##NAME(const T i) : value(i) {} \
  T &operator=(const T &i) {  \
    unsigned long offset = offsetof(CLASS_NAME, NAME); \
    ConType *parent = (ConType*)((unsigned long)(this) - offset);  \
    value = (parent->*SetFunc)(i);  \
    return value; \
  } \
  operator T() const {  \
    unsigned long offset = offsetof(CLASS_NAME, NAME); \
    ConType *parent = (ConType*)((unsigned long)(this) - offset);  \
    T result = (parent->*GetFunc)(); \
    return result;  \
  } \
  T value;  \
};  \
  Property_##CLASS_NAME##_##NAME <TYPE, CLASS_NAME, &CLASS_NAME::GETTER, &CLASS_NAME::SETTER> NAME

#endif  // BASE_SORA_PROPERTY_H_
