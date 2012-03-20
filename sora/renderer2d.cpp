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
#include "renderer2d.h"

#include "shader_variable.h"
#include "shader.h"

using namespace std;

namespace sora {;
Renderer2D::Renderer2D() {
}
Renderer2D::~Renderer2D() {
}

void Renderer2D::ApplyMatrix(const glm::mat4 &world_mat) {
  //world-view-projection
  //world, view, projection 같은것을 등록할수 잇으면 등록하기
  int mvp_handle = last_prog()->GetLocation(ShaderVariable::kWorldViewProjection);
  if(mvp_handle != -1) {
    //glm::mat4 mvp = impl->projection * impl->view * impl->world;  
    glm::mat4 mvp = glm::mat4(1.0f);
    mvp *= projection_mat();
    mvp *= view_mat();
    mvp *= world_mat;
    glUniformMatrix4fv(mvp_handle, 1, GL_FALSE, glm::value_ptr(mvp));
  }

  int world_handle = last_prog()->GetLocation(ShaderVariable::kWorld);
  if(world_handle != -1) {
    glUniformMatrix4fv(world_handle, 1, GL_FALSE, glm::value_ptr(world_mat));
  }

  int view_handle = last_prog()->GetLocation(ShaderVariable::kView);
  if(view_handle != -1) {
    glUniformMatrix4fv(view_handle, 1, GL_FALSE, glm::value_ptr(view_mat()));
  }

  int projection_handle = last_prog()->GetLocation(ShaderVariable::kProjection);
  if(projection_handle != -1) {
    glUniformMatrix4fv(projection_handle, 1, GL_FALSE, glm::value_ptr(projection_mat()));
  }

  //2d에는 카메라가 필요없다. world행렬를 가지고 그냥 다루자
}
void Renderer2D::SetInitState() {
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  //reset matrix
  glm::mat4 &projection = projection_mat();
  glm::mat4 &view = view_mat();
  projection = glm::ortho(0.0f, (float)win_width(), 0.0f, (float)win_height());
  view = glm::mat4(1.0f);
}
}