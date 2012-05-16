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
#include "post_effect.h"

#include "vertex.h"
#include "render_state.h"

using namespace glm;
using namespace std;

namespace sora {;

PostEffect::PostEffect() {

}
PostEffect::~PostEffect() {

}

void PostEffect::Deinit() {
  post_effect_.Deinit();
}

void PostEffect::Draw(Texture &tex, RenderState *dev) {
  dev->Set2D();
  dev->UseShader(post_effect_);
  dev->UseTexture(tex, 0);

  ShaderVariable mvp_var = post_effect_.uniform_var(kMVPHandleName);
  mat4 world_mat(1.0f);
  SetUniformMatrix(mvp_var, world_mat);

  Vertex2DList vert_list;
  vert_list.push_back(CreateVertex2D(-1, -1, 0, 0));
  vert_list.push_back(CreateVertex2D(1, -1, 1, 0));
  vert_list.push_back(CreateVertex2D(1, 1, 1, 1));
  vert_list.push_back(CreateVertex2D(-1, 1, 0, 1));
  post_effect_.SetVertexList(vert_list);
  post_effect_.DrawArrays(kDrawTriangleFan, vert_list.size());
}
void PostEffect::Draw(Texture &tex, RenderState *dev, int x, int y, int w, int h) {
  dev->Set2D();
  dev->UseShader(post_effect_);
  dev->UseTexture(tex, 0);

  ShaderVariable mvp_var = post_effect_.uniform_var(kMVPHandleName);
  mat4 world_mat(1.0f);
  SetUniformMatrix(mvp_var, world_mat);
  
  //화면을 얼마나 그릴것인가
  float rect_x = (float)x;
  float rect_y = (float)y;
  float rect_w = (float)w;
  float rect_h = (float)h;
  float win_w = static_cast<float>(dev->win_width());
  float win_h = static_cast<float>(dev->win_height());

  float pos_left = (2.0f / win_w * rect_x) - 1;
  float pos_bottom = (2.0f / win_h * rect_y) - 1;
  float pos_w = 2.0f * rect_w / win_w;
  float pos_h = 2.0f * rect_h / win_h;

  Vertex2DList vert_list;
  vert_list.push_back(CreateVertex2D(pos_left, pos_bottom, 0, 0));
  vert_list.push_back(CreateVertex2D(pos_left + pos_w, pos_bottom, 1, 0));
  vert_list.push_back(CreateVertex2D(pos_left + pos_w, pos_bottom + pos_h, 1, 1));
  vert_list.push_back(CreateVertex2D(pos_left, pos_bottom + pos_h, 0, 1));
  post_effect_.SetVertexList(vert_list);
  post_effect_.DrawArrays(kDrawTriangleFan, vert_list.size());
}

void PostEffect::DrawScissor(Texture &tex, RenderState *dev, int x, int y, int w, int h) {
  glEnable(GL_SCISSOR_TEST);
  glScissor(x, y, w, h);
  Draw(tex, dev);
  glDisable(GL_SCISSOR_TEST);
}

void PostEffect::InitFromFile(const std::string &vert_path, const std::string &frag_path) {
  post_effect_.LoadFromFile(vert_path, frag_path);
}
}