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
#elif SR_ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#error "not support gl"
#endif

// gl함수 전부 갈아치울수 있도록 하기
// 이름만 다르게 쓸수있도록해놓으면 나중에 함수로 교체하든지하는 편법이 가능
// gl함수 썡으로 쓰면 확장하기 힘들다(원본 함수와 구분이 안되나까)
void srglClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void srglClear(	GLbitfield  	mask);

void srglEnable(	GLenum  	cap);
void srglDisable(	GLenum  	cap);

void srglViewport (GLint x, GLint y, GLsizei width, GLsizei height);
void srglDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
void srglDrawArrays (GLenum mode, GLint first, GLsizei count);
void srglBlendFunc (GLenum sfactor, GLenum dfactor);

// 텍스쳐 관련
// #define srglBindTexture   glBindTexture
// 텍스쳐 bind는 최적화를 위해서 함수로 처리. 소프트웨어적으로 중복해결함
void srglBindTexture(GLenum target, GLuint texture);

void srglGenTextures (GLsizei n, GLuint *textures);
void srglDeleteTextures (GLsizei n, const GLuint *textures);
void srglPixelStorei (GLenum pname, GLint param);
void srglTexParameteri (GLenum target, GLenum pname, GLint param);
void srglTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);

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

//기본gl함수 전부 블럭.sr계열을 쓰게 강요하기
#undef glClearColor
#undef glClear
#undef glEnable
#undef glDisable
#undef glViewport

#undef glBindTexture

#undef glDrawElements
#undef glDrawArrays

#undef glBlendFunc

#undef glGenTextures
#undef glDeleteTextures
#undef glPixelStorei
#undef glTexParameteri
#undef glTexImage2D

#endif  // SORA_GL_INC_H_