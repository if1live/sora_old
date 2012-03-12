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
#include "sora/shader.h"

TEST(Shader, InitVertexShader) {
  using sora::Shader;
  using std::string;
  Shader vsh;
  EXPECT_EQ(0, vsh.handle);  // not yet created

  // load sample shader
  string src = ""
    "uniform mat4 u_mvpMatrix;  "
    "attribute vec4 a_position;  "
    "attribute vec4 a_color;  "
    "varying vec4 v_color;  "
    "void main()  "
    "{  "
    "v_color = a_color; "
    "gl_Position = u_mvpMatrix * a_position;"
    "}";
  vsh.InitVertexShader(src);
  EXPECT_EQ(true, vsh.handle > 0);
  EXPECT_EQ(GL_VERTEX_SHADER, vsh.type);
}
TEST(Shader, InitFragmentShader) {
  using sora::Shader;
  using std::string;

  Shader fsh;
  EXPECT_EQ(0, fsh.handle);  // not yet created  
  EXPECT_EQ(false, fsh.IsInit());
  string src = ""
    "precision mediump float;  "
    "varying vec4 v_color;  "
    "void main()  "
    "{  "
    "gl_FragColor = v_color;  "
    "}  ";
  fsh.InitFragmentShader(src);
  EXPECT_EQ(true, fsh.handle > 0);
  EXPECT_EQ(GL_FRAGMENT_SHADER, fsh.type);
  EXPECT_EQ(true, fsh.IsInit());
}

TEST(Program, program) {
  using sora::Program;
  using std::string;
  Program prog;

  EXPECT_EQ(false, prog.IsInit());
  string v_src = ""
    "uniform mat4 u_mvpMatrix;  "
    "attribute vec4 a_position;  "
    "attribute vec4 a_color;  "
    "varying vec4 v_color;  "
    "void main()  "
    "{  "
    "v_color = a_color; "
    "gl_Position = u_mvpMatrix * a_position;"
    "}";
  string f_src = ""
    "precision mediump float;  "
    "varying vec4 v_color;  "
    "void main()  "
    "{  "
    "gl_FragColor = v_color;  "
    "}  ";
  prog.Init(v_src, f_src);
  EXPECT_EQ(true, prog.IsInit());
  EXPECT_EQ(true, prog.Validate(prog.prog));
}