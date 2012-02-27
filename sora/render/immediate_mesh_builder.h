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
#ifndef SORA_IMMEDIATE_MESH_BUILDER_H_
#define SORA_IMMEDIATE_MESH_BUILDER_H_

#if SR_ANDROID
//gles에는 없으니까 따로 선언
#define GL_QUADS 0x0010 
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

namespace sora {;

enum {
  kUseColor = 0x01,
  kUseTexcoord = 0x02,
  kUseNormal = 0x04
};

struct BasicVertex {
  float xyz[3];
  float st[2];
  float rgba[4];
  float normal[3];
};


class ImmediateMeshBuilderImpl;

class ImmediateMeshBuilder {
public:
  ImmediateMeshBuilder();
  ~ImmediateMeshBuilder();

public:
  void Begin(GLenum mode, unsigned int flag);
  //end를 단순히 end로 끝내지 mesh정보를 기반으로 호출한 함수정보까지 넘기기
  void End();
  void Draw();

  void Vertex3f(float x, float y, float z);
  void Vertex2f(float x, float y) { Vertex3f(x, y, 0); }
  void TexCoord2f(float s, float t);
  void Color4f(float r, float g, float b, float a);
  void Color3f(float r, float g, float b) { Color4f(r, g, b, 1); }
  void Normal3f(float x, float y, float z);

  unsigned int flag() const;
  GLenum mode() const;

  const void *position_ptr();
  const void *color_ptr();
  const void *texcoord_ptr();
  const void *normal_ptr();

private:
  ImmediateMeshBuilderImpl *impl_;
  ImmediateMeshBuilderImpl *impl() { return impl_; }
};
}


#endif