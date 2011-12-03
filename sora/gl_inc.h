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
#ifndef SORA_GL_INC_H_
#define SORA_GL_INC_H_

#if SR_WIN
#include <GL/glew.h>
#include <GL/glfw.h>
#else
#error "not support gl"
#endif

// gl함수 전부 갈아치울수 있도록 하기
// 이름만 다르게 쓸수있도록해놓으면 나중에 함수로 교체하든지하는 편법이 가능
// gl함수 썡으로 쓰면 확장하기 힘들다(원본 함수와 구분이 안되나까)
#define srglClear         glClear
#define srglEnable        glEnable
#define srglViewport      glViewport

#define srglDrawElements    glDrawElements
#define srglDrawArrays    glDrawArrays

// 텍스쳐 관련
#define srglBindTexture   glBindTexture
#define srglGenTextures   glGenTextures
#define srglDeleteTextures  glDeleteTextures
#define srglPixelStorei   glPixelStorei
#define srglTexParameteri glTexParameteri
#define srglTexImage2D    glTexImage2D

// shader 관련
#define srglDeleteShader  glDeleteShader
#define srglShaderSource  glShaderSource
#define srglCompileShader glCompileShader
#define srglDeleteProgram glDeleteProgram
#define srglCreateProgram glCreateProgram
#define srglAttachShader  glAttachShader
#define srglLinkProgram   glLinkProgram
#define srglCreateShader  glCreateShader
#define srglGetShaderiv   glGetShaderiv
#define srglGetShaderInfoLog  glGetShaderInfoLog
#define srglGetProgramiv  glGetProgramiv
#define srglGetProgramInfoLog glGetProgramInfoLog
#define srglValidateProgram glValidateProgram
#define srglUseProgram    glUseProgram

#define srglGetUniformLocation  glGetUniformLocation
#define srglGetAttribLocation   glGetAttribLocation
#define srglVertexAttribPointer glVertexAttribPointer
#define srglEnableVertexAttribArray glEnableVertexAttribArray
#define srglUniformMatrix4fv    glUniformMatrix4fv
#endif  // SORA_GL_INC_H_