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
#ifndef BASE_SORA_SHARED_OBJECT_H_
#define BASE_SORA_SHARED_OBJECT_H_

namespace sora {;
template<typename T>
class SharedObject {
 public:
  static T& GetInstance() {
    if (ctx_ == NULL) {;
      ctx_ = new T;
    }
      return *ctx_;
    }
  static void DestoryInstance() {
    if (ctx_ != NULL) {
      delete(ctx_)
      ctx_ = NULL;
    }
  }
  static bool IsCreated() {
    return (ctx_ != NULL);
  }
  SharedObject() {}
  virtual ~SharedObject() {}
 private:
  static T *ctx_;
};

template<typename T>
T *SharedObject<T>::ctx_ = NULL;
}
#endif  // BASE_SORA_SHARED_OBJECT_H_
