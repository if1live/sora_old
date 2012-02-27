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
#include "immediate_mesh_builder.h"
#include "sora/common/template_library.h"

namespace sora {;

class ImmediateMeshBuilderImpl {
public:
  ImmediateMeshBuilderImpl()
    : mode(0), curr_index(0), render_flag(0) {
    // make quad index
    int base_index = 0;
    for (int i = 0 ; i < kTriangleCount / 2; i++) {
      SR_ASSERT(base_index+5 < kTriangleCount * 3);
      index_list[base_index+0] = i*3 + 0;
      index_list[base_index+1] = i*3 + 1;
      index_list[base_index+2] = i*3 + 2;

      index_list[base_index+3] = i*3 + 0;
      index_list[base_index+4] = i*3 + 2;
      index_list[base_index+5] = i*3 + 3;

      base_index += 6;
    }

    for (int i = 0 ; i < 4 ; i++) {
      color[i] = 1.0f;
    }
  }

  static const int kTriangleCount = 2048;
  static const int kVertexCount = kTriangleCount * 3;
  BasicVertex vertex_list[kVertexCount];
  // GL_QUAD 구현에 사용됨
  GLushort index_list[kTriangleCount * 3];
  BasicVertex vert;
  GLenum mode;
  int curr_index;

  //color값은 1회 설정한것이 지속된다
  float color[4];

  unsigned int render_flag;
};

unsigned int ImmediateMeshBuilder::flag() const {
  return impl_->render_flag;
}
GLenum ImmediateMeshBuilder::mode() const {
  return impl_->mode;
}

ImmediateMeshBuilder::ImmediateMeshBuilder() 
  : impl_(new ImmediateMeshBuilderImpl()) {
}
ImmediateMeshBuilder::~ImmediateMeshBuilder() {
  sora::SafeDelete(impl_);
}

void ImmediateMeshBuilder::Begin(GLenum mode, unsigned int flag) {
  ImmediateMeshBuilderImpl *builder = impl();
  builder->mode = mode;
  builder->curr_index = 0;
  builder->render_flag = flag;

  memset(&builder->vert, 0, sizeof(BasicVertex));
}
void ImmediateMeshBuilder::Draw() {
  ImmediateMeshBuilderImpl *builder = impl();
  if (builder->mode == GL_QUADS) {
    srglDrawElements(GL_TRIANGLES, 
      builder->curr_index * 3 / 2,
      GL_UNSIGNED_SHORT, 
      builder->index_list);
  } else {
    srglDrawArrays(builder->mode, 0, builder->curr_index);
  }
}
void ImmediateMeshBuilder::End() {
  
}

void ImmediateMeshBuilder::Vertex3f(float x, float y, float z) {
  ImmediateMeshBuilderImpl *builder = impl();
  builder->vert.xyz[0] = x;
  builder->vert.xyz[1] = y;
  builder->vert.xyz[2] = z;

  memcpy(builder->vert.rgba, builder->color, 4 * sizeof(float));

  builder->vertex_list[builder->curr_index] = builder->vert;
  builder->curr_index++;
}
void ImmediateMeshBuilder::TexCoord2f(float s, float t) {
  ImmediateMeshBuilderImpl *builder = impl();
  builder->vert.st[0] = s;
  builder->vert.st[1] = t;
}
void ImmediateMeshBuilder::Color4f(float r, float g, float b, float a) {
  ImmediateMeshBuilderImpl *builder = impl();
  builder->color[0] = r;
  builder->color[1] = g;
  builder->color[2] = b;
  builder->color[3] = a;
}
void ImmediateMeshBuilder::Normal3f(float x, float y, float z) {
  ImmediateMeshBuilderImpl *builder = impl();
  builder->vert.normal[0] = x;
  builder->vert.normal[1] = y;
  builder->vert.normal[2] = z;
}

const void *ImmediateMeshBuilder::position_ptr() {
  ImmediateMeshBuilderImpl *builder = impl();
  return builder->vertex_list[0].xyz;
}
const void *ImmediateMeshBuilder::color_ptr() {
  if((flag() & kUseColor) == kUseColor) {
    ImmediateMeshBuilderImpl *builder = impl();
    return builder->vertex_list[0].rgba;
  } else {
    return NULL;
  }
}
const void *ImmediateMeshBuilder::texcoord_ptr() {
  if((flag() & kUseTexcoord) == kUseTexcoord) {
    ImmediateMeshBuilderImpl *builder = impl();
    return builder->vertex_list[0].st;
  } else {
    return NULL;
  }
}
const void *ImmediateMeshBuilder::normal_ptr() {
  if((flag() & kUseNormal) == kUseNormal) {
    ImmediateMeshBuilderImpl *builder = impl();
    return builder->vertex_list[0].st;
  } else {
    return NULL;
  }
}
}
