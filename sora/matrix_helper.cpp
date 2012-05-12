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
#include "matrix_helper.h"

using namespace glm;

namespace sora {;

glm::vec3 MatrixHelper::ViewUpVec(const glm::mat4 &view_mat) {
  //y
  vec3 side;
  for(int i = 0 ; i < 3 ; i++) {
    side[i] = view_mat[i][1];
  }
  return side;
}

glm::vec3 MatrixHelper::ViewDirVec(const glm::mat4 &view_mat) {
  vec3 side;
  for(int i = 0 ; i < 3 ; i++) {
    side[i] = -view_mat[i][2];
  }
  return side;
}

glm::vec3 MatrixHelper::ViewSideVec(const glm::mat4 &view_mat) {
  //x
  vec3 side;
  for(int i = 0 ; i < 3 ; i++) {
    side[i] = view_mat[i][0];
  }
  return side;
}

glm::vec3 MatrixHelper::ViewPos(const glm::mat4 &view_mat) {
  mat3 view3_mat;
  for(int i = 0 ; i < 3 ; i++) {
    for(int j = 0 ; j < 3 ; j++) {
      view3_mat[i][j] = view_mat[i][j];
    }
  }
  mat3 view3_inv_mat = glm::inverse(view3_mat);
  vec3 last_vec;
  for(int i = 0 ; i < 3 ; i++) {
    last_vec[i] = view_mat[3][i];  //view행렬의 다른것과 혼자 방향이 다르다
  }

  vec3 view_pos = view3_inv_mat * last_vec;
  return -view_pos;
}


} //namespace sora
