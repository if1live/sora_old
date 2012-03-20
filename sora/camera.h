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
#ifndef SORA_CAMERA_H_
#define SORA_CAMERA_H_

#include "vector.h"

namespace sora {;
class SR_DLL Camera {
public:
  Camera();
  ~Camera();

  Vec3f &eye() { return eye_; }
  Vec3f &dir() { return dir_; }
  Vec3f &up() { return up_; }
  const Vec3f &eye() const { return eye_; }
  const Vec3f &dir() const { return dir_; }
  const Vec3f &up() const { return up_; }
  void set_eye(const Vec3f &v); 
  void set_dir(const Vec3f &v);
  void set_up(const Vec3f &v);

private:
  Vec3f eye_;
  Vec3f dir_;
  Vec3f up_;
};
}

#endif  // SORA_CAMERA_H_