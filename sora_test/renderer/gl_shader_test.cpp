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
#include "sora_test_stdafx.h"
#include "sora/renderer/gl/gl_shader.h"

const char *vert_src = ""
  "uniform mat4 u_mvpMatrix;  "
  "attribute vec4 a_position;  "
  "attribute vec4 a_color;  "
  "varying vec4 v_color;  "
  "void main()  "
  "{  "
  "v_color = a_color; "
  "gl_Position = u_mvpMatrix * a_position;"
  "}";

const char *frag_src = ""
  "precision mediump float;  "
  "varying vec4 v_color;  "
  "void main()  "
  "{  "
  "gl_FragColor = v_color;  "
  "}  ";

TEST(GLShader, InitVertexShader) {
  using sora::gl::GLShader;
  using std::string;
  GLShader vsh;
  EXPECT_EQ(0, vsh.handle);  // not yet created

  // load sample shader

  vsh.InitVertexShader(vert_src);
  EXPECT_EQ(true, vsh.handle > 0);
  EXPECT_EQ(GL_VERTEX_SHADER, vsh.type);

  //쉐이더 해제는 소멸자로 하지 않도록 고쳐서
  //쉐이더를 복사해도 문제가 생기지 않도록햇다.
  vsh.Deinit();
}
TEST(GLShader, InitFragmentGLShader) {
  using sora::gl::GLShader;
  using std::string;

  GLShader fsh;
  EXPECT_EQ(0, fsh.handle);  // not yet created  
  EXPECT_EQ(false, fsh.IsInit());

  fsh.InitFragmentShader(frag_src);
  EXPECT_EQ(true, fsh.handle > 0);
  EXPECT_EQ(GL_FRAGMENT_SHADER, fsh.type);
  EXPECT_EQ(true, fsh.IsInit());

  //쉐이더 해제는 소멸자로 하지 않도록 고쳐서
  //쉐이더를 복사해도 문제가 생기지 않도록햇다.
  fsh.Deinit();
}

TEST(GLProgram, GLProgram) {
  using sora::gl::GLProgram;
  using std::string;
  GLProgram prog;

  EXPECT_EQ(false, prog.IsInit());
  prog.Init(vert_src, frag_src);
  EXPECT_EQ(true, prog.IsInit());
  EXPECT_EQ(true, prog.Validate(prog.prog));

  //쉐이더 해제는 소멸자로 하지 않도록 고쳐서
  //쉐이더를 복사해도 문제가 생기지 않도록햇다.
  prog.Deinit();
}

TEST(GLProgram, SetValueEtc) {
  using sora::gl::GLProgram;
  using std::string;
  GLProgram prog;
  prog.Init(vert_src, frag_src);

  float a;

  glm::mat4 m1;
  glm::vec3 m2;

  //쉐이더 해제는 소멸자로 하지 않도록 고쳐서
  //쉐이더를 복사해도 문제가 생기지 않도록햇다.
  prog.Deinit();
}
