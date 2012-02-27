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
#ifndef SORA_MATRIX_STACK_H_
#define SORA_MATRIX_STACK_H_

#include "sora/common/matrix.h"

#if SR_USE_PCH == 0
#include <vector>
#endif

namespace sora {;
class MatrixStack {
public:
  MatrixStack();
  ~MatrixStack();
  static MatrixStack &GetInstance();

public:
  // member method
  void SetMatrixToIdentity();
  void PushMatrix();
  void PopMatrix();

  void Scale(float x, float y, float z);
  void Translate(float x, float y, float z);
  void Rotate(float degree, float x, float y, float z);
  void MultMatrix(float *m);
  void MatrixMode(int matrix_mode);

  void UseProjectionMatrixMode();
  void UseModelviewMatrixMode();
 
  void LookAt(float eye_x, float eye_y, float eye_z,
    float target_x, float target_y, float target_z,
    float up_x, float up_y, float up_z);
  void Perspective(float fovy, float aspect, float zNear, float zFar);
  void Ortho(float left, float right, float bottom, float top, float near_val, float far_val);

  const mat4 &modelview_mat() const { return modelview_mat_; }
  const mat4 &projection_mat() const { return projection_mat_; }

private:
  //matrix stack
  typedef std::vector<mat4> MatrixStackType;
  enum {
    kProjection,
    kModelview,
  };
  int matrix_mode_;

  mat4 &GetCurrMatrix();
  MatrixStackType matrix_stack_;
  mat4 projection_mat_;
  mat4 modelview_mat_;
};
}

inline void srglPushMatrix() { 
  sora::MatrixStack::GetInstance().PushMatrix(); 
}
inline void srglPopMatrix() {
  sora::MatrixStack::GetInstance().PopMatrix(); 
}
inline void srglScalef(float x, float y, float z) {
  sora::MatrixStack::GetInstance().Scale(x, y, z); 
}
inline void srglTranslatef(float x, float y, float z)  {
  sora::MatrixStack::GetInstance().Translate(x, y, z); 
}
inline void srglRotatef(float deg, float x, float y, float z) {
  sora::MatrixStack::GetInstance().Rotate(deg, x, y, z); 
}

inline void srglLoadIdentity() {
  sora::MatrixStack::GetInstance().SetMatrixToIdentity(); 
}
inline void srglMultMatrix(float *m) {
  sora::MatrixStack::GetInstance().MultMatrix(m); 
}
inline void srglMatrixMode(int mode) {
  sora::MatrixStack::GetInstance().MatrixMode(mode); 
}
inline void srglPerspective(float fovy, float aspect, float zNear, float zFar) {
  sora::MatrixStack::GetInstance().Perspective(fovy, aspect, zNear, zFar); 
}
inline void srglLookAt(float eye_x, float eye_y, float eye_z,
  float target_x, float target_y, float target_z, 
  float up_x, float up_y, float up_z) {
  sora::MatrixStack::GetInstance().LookAt(eye_x, eye_y, eye_z,
    target_x, target_y, target_z, up_x, up_y, up_z); 
}
inline void srglOrtho(float left, float right, float bottom, float top, float near_val, float far_val) {
  sora::MatrixStack::GetInstance().Ortho(left, right, bottom, top, near_val, far_val);
}

#define SR_PROJECTION 1
#define SR_MODELVIEW  2

#undef glPushMatrix
#undef glPopMatrix
#undef glScalef
#undef glTranslatef
#undef glRotatef
#undef glLoadIdentity
#undef glMultMatrixf
#undef glMultMatrix
#undef glMatrixMode
#undef gluPerspective
#undef gluLookAt
#undef gluOrtho2D

#endif  // SORA_MATRIX_STACK_H_