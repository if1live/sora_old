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
#include "sora_test_stdafx.h"
#include "sora/render_device.h"

using namespace glm;
using namespace std;
using namespace sora;

TEST(RenderDevice, view_vec) {
  vec3 eye_vec(0, 1, 0);
  //vec3 eye_vec(0, 0, 0);
  vec3 center_vec(-1, 0, -1);
  vec3 up_vec(0, 0.6f, -0.8f);  //up vector의 방향은 lookat에서 90도로 맞춰버린다
  mat4 view_mat = lookAt(eye_vec, center_vec, up_vec);

  vec3 dir_vec = glm::normalize(center_vec - eye_vec);
  vec3 side_vec = glm::normalize(glm::cross(dir_vec, up_vec));
  up_vec = glm::cross(side_vec, dir_vec);

  RenderDevice dev;
  dev.set_view_mat(view_mat);

  vec3 expected_view_dir = dev.view_dir_vec();
  vec3 expected_view_up = dev.view_up_vec();
  vec3 expected_view_side = dev.view_side_vec();
  float dir_error = glm::length(expected_view_dir - dir_vec);
  float up_error = glm::length(expected_view_up - up_vec);
  float side_error = glm::length(expected_view_side - side_vec);
  EXPECT_EQ(true, dir_error < 0.001f);
  EXPECT_EQ(true, up_error < 0.001f);
  EXPECT_EQ(true, side_error < 0.001f);
}