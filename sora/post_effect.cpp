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
#include "post_effect.h"

#include "vertex.h"
#include "render_state.h"
#include "filesystem.h"
#include "device.h"
#include "render_state.h"
#include "shader.h"

using namespace glm;
using namespace std;

namespace sora {;

PostEffect::PostEffect() {

}
PostEffect::~PostEffect() {
  Deinit();
}

Shader &PostEffect::post_effect() {
  if(post_effect_ == NULL) {
    post_effect_.reset(new Shader());
  }
  return *post_effect_;
}
void PostEffect::Deinit() {
  if(post_effect_ != NULL) {
    post_effect_->Deinit();
    post_effect_.reset(NULL);
  }
}

void PostEffect::Draw(Texture &tex) {
  RenderState &render_state = Device::GetInstance()->render_state();
  Shader &shader = post_effect();

  render_state.Set2D();
  render_state.UseShader(shader);
  render_state.UseTexture(tex, 0);

  ShaderVariable mvp_var = shader.uniform_var(kMVPHandleName);
  mat4 world_mat(1.0f);
  SetUniformMatrix(mvp_var, world_mat);

  Vertex2DList vert_list;
  vert_list.push_back(CreateVertex2D(-1, -1, 0, 0));
  vert_list.push_back(CreateVertex2D(1, -1, 1, 0));
  vert_list.push_back(CreateVertex2D(1, 1, 1, 1));
  vert_list.push_back(CreateVertex2D(-1, 1, 0, 1));
  shader.SetVertexList(vert_list);
  shader.DrawArrays(kDrawTriangleFan, vert_list.size());
}
void PostEffect::Draw(Texture &tex, int x, int y, int w, int h) {
  RenderState &render_state = Device::GetInstance()->render_state();
  Shader &shader = post_effect();

  render_state.Set2D();
  render_state.UseShader(shader);
  render_state.UseTexture(tex, 0);

  ShaderVariable mvp_var = shader.uniform_var(kMVPHandleName);
  mat4 world_mat(1.0f);
  SetUniformMatrix(mvp_var, world_mat);
  
  //화면을 얼마나 그릴것인가
  float rect_x = (float)x;
  float rect_y = (float)y;
  float rect_w = (float)w;
  float rect_h = (float)h;
  float win_w = static_cast<float>(render_state.win_width());
  float win_h = static_cast<float>(render_state.win_height());

  float pos_left = (2.0f / win_w * rect_x) - 1;
  float pos_bottom = (2.0f / win_h * rect_y) - 1;
  float pos_w = 2.0f * rect_w / win_w;
  float pos_h = 2.0f * rect_h / win_h;

  Vertex2DList vert_list;
  vert_list.push_back(CreateVertex2D(pos_left, pos_bottom, 0, 0));
  vert_list.push_back(CreateVertex2D(pos_left + pos_w, pos_bottom, 1, 0));
  vert_list.push_back(CreateVertex2D(pos_left + pos_w, pos_bottom + pos_h, 1, 1));
  vert_list.push_back(CreateVertex2D(pos_left, pos_bottom + pos_h, 0, 1));
  shader.SetVertexList(vert_list);
  shader.DrawArrays(kDrawTriangleFan, vert_list.size());
}

void PostEffect::DrawScissor(Texture &tex, int x, int y, int w, int h) {
  glEnable(GL_SCISSOR_TEST);
  glScissor(x, y, w, h);
  Draw(tex);
  glDisable(GL_SCISSOR_TEST);
}

void PostEffect::InitFromFile(const std::string &vert_path, const std::string &frag_path) {
  post_effect().LoadFromFile(vert_path, frag_path);
}
void PostEffect::InitFromFile(const std::string &frag_path) {
  string vs_path = Filesystem::GetAppPath("posteffect/shared.vs");
  post_effect().LoadFromFile(vs_path, frag_path);
}
}