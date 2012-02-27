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
#ifndef SORA_IMMEDIATE_MODE_EMULATOR_H_
#define SORA_IMMEDIATE_MODE_EMULATOR_H_

#include "sora/common/template_library.h"
#include "sora/render/matrix_stack.h"
#include "sora/render/immediate_mesh_builder.h"

namespace yuriko {;

class ImmediateModeEmulatorImpl;

class ImmediateModeEmulator : public sora::Singleton<ImmediateModeEmulator> {
public:
  ImmediateModeEmulator();
  ~ImmediateModeEmulator();

  void Init();
  void Cleanup();

  void ApplyGlobalMatrixStack();

  //immediate mode
  void Begin(GLenum mode, unsigned int flag);
  void Vertex3f(float x, float y, float z);
  void Vertex2f(float x, float y) { Vertex3f(x, y, 0); }
  void TexCoord2f(float s, float t);
  void End();
  void Color4f(float r, float g, float b, float a);
  void Color3f(float r, float g, float b) { Color4f(r, g, b, 1); }

  // 점을 낱개로 찍는것도 허용하지만 vbo같은 느낌으로 한방에 그리는것도
  // 가능하도록 뚫어놓자

private:
  sora::ImmediateMeshBuilder builder_;
};
}

inline void srglBegin(int mode, unsigned int flag) {
  yuriko::ImmediateModeEmulator::GetInstance().Begin(mode, flag);
}
inline void srglEnd() {
  yuriko::ImmediateModeEmulator::GetInstance().End();
}
inline void srglVertex3f(float x, float y, float z) {
  yuriko::ImmediateModeEmulator::GetInstance().Vertex3f(x, y, z);
}
inline void srglVertex2f(float x, float y) {
  yuriko::ImmediateModeEmulator::GetInstance().Vertex2f(x, y);
}
inline void srglTexCoord2f(float s, float t) {
  yuriko::ImmediateModeEmulator::GetInstance().TexCoord2f(s, t);
}
inline void srglColor4f(float r, float g, float b, float a) {
  yuriko::ImmediateModeEmulator::GetInstance().Color4f(r, g, b, a);
  //glColor4f(r, g, b, a);  //필요할 경우 원본도 호출. 양쪽의 값을 다 바꿔준다
}
inline void srglColor3f(float r, float g, float b) {
  yuriko::ImmediateModeEmulator::GetInstance().Color3f(r, g, b);
  //glColor3f(r, g, b);  //원본도 호출. 양쪽의 값을 다 바꿔준다
}

#undef glBegin
#undef glEnd
#undef glVertex3f
#undef glVertex2f
#undef glTexCoord2f
#undef glColor4f
#undef glColor3f

#endif  // SORA_IMMEDIATE_MODE_EMULATOR_H_
