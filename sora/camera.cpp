﻿/*  Copyright (C) 2011-2012 by if1live */
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
#include "camera.h"
#include "template_lib.h"

namespace sora {;
struct CameraImpl {
  CameraImpl()
    : eye(0, 0, 1), dir(0, 0, -1), up(0, 1, 0) {}
  Vec3f eye;
  Vec3f dir;
  Vec3f up;
};

Camera::Camera() : impl(new CameraImpl()) {
}
Camera::~Camera() {
  SafeDeleteWithNullCheck(impl);
}
Vec3f &Camera::eye() {
  return impl->eye;
}
Vec3f &Camera::dir() {
  return impl->dir;
}
Vec3f &Camera::up() {
  return impl->up;
}
const Vec3f &Camera::eye() const {
  return impl->eye;
}
const Vec3f &Camera::dir() const {
  return impl->dir;
}
const Vec3f &Camera::up() const {
  return impl->up;
}
void Camera::set_eye(const Vec3f &v) {
  impl->eye = v;
}
void Camera::set_dir(const Vec3f &v) {
  impl->dir = v;
  impl->dir.Normalized();
}
void Camera::set_up(const Vec3f &v) {
  impl->up = v;
  impl->up.Normalized();
}
}

