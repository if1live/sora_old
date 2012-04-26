﻿/*  Copyright (C) 2011-2012 by if1live */
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
#include "renderer/shader.h"

using namespace sora;

const char *sample_vert_src = ""
  "uniform mat4 u_mvpMatrix;  "
  "attribute vec4 a_position;  "
  "attribute vec4 a_color;  "
  "varying vec4 v_color;  "
  "void main()  "
  "{  "
  "v_color = a_color; "
  "gl_Position = u_mvpMatrix * a_position;"
  "}";

const char *sample_frag_src = ""
  "precision mediump float;  "
  "varying vec4 v_color;  "
  "uniform vec3 sample_color;"
  "void main()  "
  "{  "
  "gl_FragColor = vec4(sample_color, 0.5);  "
  "}  ";

TEST(Shader, basic) {
  Shader shader;
  EXPECT_EQ(true, shader.Init(sample_vert_src, sample_frag_src));

  //validate
  EXPECT_EQ(true, shader.Validate());

  glm::mat4 m1;
  EXPECT_EQ(true, shader.SetMatrix("u_mvpMatrix", m1));
  glm::vec3 v1;
  EXPECT_EQ(true, shader.SetVector("sample_color", v1));
  shader.Deinit(); 
}