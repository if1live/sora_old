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

#include "core/vertex.h"
#include "renderer/render_device.h"

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

void PostEffect::Draw(Texture &tex, RenderDevice *dev) {
  // -1~+1을 차지하게 tex를 그리면 될듯
  dev->Set2D();
  dev->UseShader(post_effect_);
  ShaderVariable mvp_var = post_effect_.uniform_var(kMVPHandleName);
  mat4 world_mat(1.0f);
  SetUniformMatrix(mvp_var, world_mat);

  //device->render_device().UseTexture(depth_fbo.color_tex());
  dev->UseTexture(tex);

  Vertex2DList vert_list;
  vert_list.push_back(CreateVertex2D(-1, -1, 0, 0));
  vert_list.push_back(CreateVertex2D(1, -1, 1, 0));
  vert_list.push_back(CreateVertex2D(1, 1, 1, 1));
  vert_list.push_back(CreateVertex2D(-1, 1, 0, 1));
  post_effect_.SetVertexList(vert_list);
  post_effect_.DrawArrays(kDrawTriangleFan, vert_list.size());

}
void PostEffect::InitFromFile(const std::string &vert_path, const std::string &frag_path) {
  post_effect_.LoadFromFile(vert_path, frag_path);
}
}