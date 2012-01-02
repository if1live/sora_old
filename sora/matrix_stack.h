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

#include "matrix.h"

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

  const mat4 &modelview_mat() const { return modelview_mat_; }
  const mat4 &projection_mat() const { return projection_mat_; }

public:
  // stack method

  static void StaticMultMatrix(float *m);
  static void StaticMatrixMode(int matrix_mode);

  static void StaticUseProjectionMatrixMode();
  static void StaticUseModelviewMatrixMode();
 
  static void StaticLookAt(float eye_x, float eye_y, float eye_z,
    float target_x, float target_y, float target_z,
    float up_x, float up_y, float up_z);
  static void StaticPerspective(float fovy, float aspect, float zNear, float zFar);

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

#define srglPushMatrix()  \
{ sora::MatrixStack::GetInstance().PushMatrix(); }
#define srglPopMatrix()   \
{ sora::MatrixStack::GetInstance().PopMatrix(); }
#define srglScalef(X, Y, Z)      \
{ sora::MatrixStack::GetInstance().Scale(X, Y, Z); }
#define srglTranslatef(X, Y, Z)  \
{ sora::MatrixStack::GetInstance().Translate(X, Y, Z); }
#define srglRotatef(DEG, X, Y, Z)   \
{ sora::MatrixStack::GetInstance().Rotate(DEG, X, Y, Z); }

#define srglLoadIdentity()  \
{ sora::MatrixStack::GetInstance().SetMatrixToIdentity(); }
#define srglMultMatrix(M) \
{ sora::MatrixStack::GetInstance().MultMatrix(M); }
#define srglMatrixMode(MODE)  \
{ sora::MatrixStack::GetInstance().MatrixMode(MODE); }
#define srglPerspective(fovy, aspect, zNear, zFar)  \
{ sora::MatrixStack::GetInstance().Perspective(fovy, aspect, zNear, zFar); }
#define srglLookAt(eye_x, eye_y, eye_z, target_x, target_y, target_z, up_x, up_y, up_z) \
{ sora::MatrixStack::GetInstance().LookAt(eye_x, eye_y, eye_z, target_x, target_y, target_z, up_x, up_y, up_z); }

#define SR_PROJECTION 1
#define SR_MODELVIEW  2

#endif  // SORA_MATRIX_STACK_H_