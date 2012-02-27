/*  Copyright (C) 2012 by if1live */
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
#include "gl_window.h"
#include "basic_shader.h"
#include "immediate_mode_emulator.h"

#include "sora/io/low_level_c_file.h"
#include "sora/render/shader.h"
#include "sora/render/matrix_stack.h"

#include "teapot.h"

using namespace yuriko;
using namespace sora;

void Draw();
void Init();

int main() {
  GLWindow &win = GLWindow::GetInstance();
  win.SetSize(640, 480);
  win.Init();

  Init();

  //적절히 렌더링하기...
  while(true) {
    Draw();
    //swap buffer
    win.SwapBuffer();
  }
  win.Deinit();
  return 0;
}

Program prog;

void Init() {
  const char simple_vsh[] = "simple.vsh";
  const char simple_fsh[] = "simple.fsh";
  ReadonlyCFile vsh_file(simple_vsh);
  ReadonlyCFile fsh_file(simple_fsh);
  vsh_file.Open();
  fsh_file.Open();
  const char *vsh_src = (const char*)vsh_file.GetBuffer();
  const char *fsh_src = (const char*)fsh_file.GetBuffer();  
  prog.Init(vsh_src, fsh_src);
}

void Draw() {
  srglClearColor(0.5, 0.5, 0.5, 1);
  srglClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  srglMatrixMode(SR_PROJECTION);
  srglLoadIdentity();
  //srglOrtho(-100, 100, -100, 100, -100, 100);
  srglPerspective(60.0, (GLfloat)640/(GLfloat)480, 1.0, 30000.0);
  srglLookAt(0, 0, 100, 0, 0, 0, 0, 1, 0);

  srglMatrixMode(SR_MODELVIEW);
  srglLoadIdentity();
  
  srglPushMatrix();
  {
    srglTranslatef(30, 0, 0);
    srglScalef(0.5, 0.5, 1);
    srglRotatef(15, 0, 0, 1);
    srglBegin(GL_QUADS, sora::kUseColor);
    srglColor3f(1, 0, 0); srglVertex2f(-50, -50);
    srglColor3f(0, 1, 0); srglVertex2f(50, -50);
    srglColor3f(0, 0, 1); srglVertex2f(50, 50);
    srglColor3f(1, 1, 1); srglVertex2f(-50, 50);
    srglEnd();
  }
  srglPopMatrix();

  //주전자 그리는것은 완전히 독립된 쉐이더를 사용한다
  static float angle = 0;
  angle += 0.01;
  srglPushMatrix();
  srglRotatef(angle, 1, 0, 0);
  srglRotatef(angle * 1.5, 0, 1, 0);
  srglRotatef(angle * 2, 0, 0, 1);

  prog.Use();
  
  //apply matrix
  MatrixStack &mat_stack = MatrixStack::GetInstance();
  const mat4 &projection = mat_stack.projection_mat();
  const mat4 &modelview = mat_stack.modelview_mat();
  int projection_location = prog.GetUniformLocation("u_projection");
  int modelview_location = prog.GetUniformLocation("u_modelview");
  glUniformMatrix4fv(projection_location, 1, GL_FALSE, projection.value);
  glUniformMatrix4fv(modelview_location, 1, GL_FALSE, modelview.value);

  //draw sample teapot
  int pos_location = prog.GetAttribLocation("a_position");
  glVertexAttribPointer(pos_location, 3, GL_FLOAT, GL_FALSE, 0, teapotVertices);
  glDrawElements(GL_TRIANGLES, NUM_TEAPOT_OBJECT_INDEX, GL_UNSIGNED_SHORT, teapotIndices);
}
