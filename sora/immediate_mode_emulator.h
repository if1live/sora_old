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

#include "template_library.h"

namespace sora {;

class ImmediateModeEmulatorImpl;

class ImmediateModeEmulator : public sora::Singleton<ImmediateModeEmulator> {
public:
  ImmediateModeEmulator();
  ~ImmediateModeEmulator();

  void Init();

  //immediate mode
  static void Begin(GLenum mode);
  static void Vertex3f(float x, float y, float z);
  static void Vertex2f(float x, float y) { Vertex3f(x, y, 0); }
  static void TexCoord2f(float s, float t);
  static void End();

  static void UseProjectionMatrixMode();
  static void UseModelviewMatrixMode();
  static void SetMatrixToIdentity();
  static void PushMatrix();
  static void PopMatrix();
  static void Scale(float x, float y, float z);
  static void Translate(float x, float y, float z);
  static void Rotate(float degree, float x, float y, float z);
  static void MultMatrix(float *m);
  static void MatrixMode(int matrix_mode);

  static void LookAt(float eye_x, float eye_y, float eye_z,
    float target_x, float target_y, float target_z,
    float up_x, float up_y, float up_z);
  static void Perspective(float fovy, float aspect, float zNear, float zFar);

  // 점을 낱개로 찍는것도 허용하지만 vbo같은 느낌으로 한방에 그리는것도
  // 가능하도록 뚫어놓자

private:
  ImmediateModeEmulatorImpl *impl_;
  static ImmediateModeEmulatorImpl *impl();
};
}

#define srglBegin       sora::ImmediateModeEmulator::Begin
#define srglEnd         sora::ImmediateModeEmulator::End
#define srglVertex3f    sora::ImmediateModeEmulator::Vertex3f
#define srglVertex2f    sora::ImmediateModeEmulator::Vertex2f
#define srglTexCoord2f  sora::ImmediateModeEmulator::TexCoord2f

#define srglPushMatrix  sora::ImmediateModeEmulator::PushMatrix
#define srglPopMatrix   sora::ImmediateModeEmulator::PopMatrix
#define srglScalef      sora::ImmediateModeEmulator::Scale
#define srglTranslatef  sora::ImmediateModeEmulator::Translate
#define srglRotatef     sora::ImmediateModeEmulator::Rotate

#define srglLoadIdentity  sora::ImmediateModeEmulator::SetMatrixToIdentity
#define srglMultMatrix  sora::ImmediateModeEmulator::MultMatrix
#define srglMatrixMode  sora::ImmediateModeEmulator::MatrixMode
#define srglPerspective sora::ImmediateModeEmulator::Perspective
#define srglLookAt      sora::ImmediateModeEmulator::LookAt

#define SR_PROJECTION 1
#define SR_MODELVIEW  2

#endif  // SORA_IMMEDIATE_MODE_EMULATOR_H_
