/*  Copyright (C) 2011 by if1live */
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
#include "yuriko_stdafx.h"
#include "immediate_mode_emulator.h"

#include "sora/common/matrix.h"
#include "sora/common/math_helper.h"
#include "sora/render/matrix_stack.h"
#include "sora/render/gl_inc.h"
#include "basic_shader.h"

#if SR_ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

namespace yuriko {;

ImmediateModeEmulator::ImmediateModeEmulator() {
}
ImmediateModeEmulator::~ImmediateModeEmulator() {
  Cleanup();
}

void ImmediateModeEmulator::Cleanup() {
  BasicShader &basic_shader = BasicShader::GetInstance();
  basic_shader.Cleanup();
}
void ImmediateModeEmulator::Init() {  
  BasicShader &basic_shader = BasicShader::GetInstance();
  if(basic_shader.IsInited() == false) {
    basic_shader.Init();
  }
  basic_shader.Use();

  // default set
  sora::mat4 identity;
  SetIdentity(&identity);
  glUniformMatrix4fv(basic_shader.projection_location, 1, GL_FALSE, identity.value);
  glUniformMatrix4fv(basic_shader.modelview_location, 1, GL_FALSE, identity.value);

  // GL환경 설정
  srglEnable(GL_BLEND);
  srglBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ImmediateModeEmulator::Begin(GLenum mode, unsigned int flag) {
  builder_.Begin(mode, flag);
}
void ImmediateModeEmulator::Vertex3f(float x, float y, float z) {
  builder_.Vertex3f(x, y, z);
}
void ImmediateModeEmulator::TexCoord2f(float s, float t) {
  builder_.TexCoord2f(s, t);
}
void ImmediateModeEmulator::ApplyGlobalMatrixStack() {
  using sora::MatrixStack;
  using sora::mat4;
  
  // modelview, projection을 적절히 적용
  MatrixStack &mat_stack = MatrixStack::GetInstance();
  const mat4 &projection = mat_stack.projection_mat();
  const mat4 &modelview = mat_stack.modelview_mat();

  BasicShader &basic_shader = BasicShader::GetInstance();
  glUniformMatrix4fv(basic_shader.projection_location, 1, GL_FALSE, projection.value);
  glUniformMatrix4fv(basic_shader.modelview_location, 1, GL_FALSE, modelview.value);

}
void ImmediateModeEmulator::End() {
  BasicShader &basic_shader = BasicShader::GetInstance();
  basic_shader.Use();

  ApplyGlobalMatrixStack();

  int use_color = ((builder_.flag() & sora::kUseColor) == sora::kUseColor);
  int use_texcoord = ((builder_.flag() & sora::kUseTexcoord) == sora::kUseTexcoord);
  glUniform1i(basic_shader.use_color_location, use_color);
  glUniform1i(basic_shader.use_texcoord_location, use_texcoord);

  glEnableVertexAttribArray(basic_shader.position_location);
  if(use_color) {
    glEnableVertexAttribArray(basic_shader.color_location); 
  } else {
    glDisableVertexAttribArray(basic_shader.color_location);
  }
  if(use_texcoord) {
    glEnableVertexAttribArray(basic_shader.texcoord_location);
  } else {
    glDisableVertexAttribArray(basic_shader.texcoord_location);
  }

  //안바뀔거같은 속성이더라도 bais shader 재설정
  const void *position_ptr = builder_.position_ptr();
  const void *texcoord_ptr = builder_.texcoord_ptr();
  const void *color_ptr = builder_.color_ptr();
  glVertexAttribPointer(basic_shader.position_location, 3, GL_FLOAT,
    GL_FALSE, sizeof(sora::BasicVertex), position_ptr);
  glVertexAttribPointer(basic_shader.texcoord_location, 2, GL_FLOAT,
    GL_FALSE, sizeof(sora::BasicVertex), texcoord_ptr);
  glVertexAttribPointer(basic_shader.color_location, 4, GL_FLOAT,
    GL_FALSE, sizeof(sora::BasicVertex), color_ptr);

  builder_.Draw();
}

void ImmediateModeEmulator::Color4f(float r, float g, float b, float a) {
  builder_.Color4f(r, g, b, a);
}
}