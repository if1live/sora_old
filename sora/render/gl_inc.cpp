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
#include "sora_stdafx.h"
#include "gl_inc.h"

void srglBindTexture(GLenum target, GLuint texture) {
  GLuint prev_tex_2d = 0;
  if (target == GL_TEXTURE_2D) {
    static bool first_run = false;
    if (first_run == false) {
      // texture handle는 unsigned int니까 0으로 기본값쓰고
      // 최초 시행시 무조건 bind
      first_run = true;
      glBindTexture(target, texture);
      prev_tex_2d = texture;
    } else if (prev_tex_2d != texture) {
      glBindTexture(target, texture);
      prev_tex_2d = texture;
    }
  } else {
    glBindTexture(target, texture);
  }
}

void srglClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) {
  glClearColor(red, green, blue, alpha);
}
void srglClear(	GLbitfield  	mask) {
  glClear(mask);
}

void srglEnable(	GLenum  	cap) { glEnable(cap); }
void srglDisable(	GLenum  	cap) { glDisable(cap); }

void srglViewport (GLint x, GLint y, GLsizei width, GLsizei height) {
  glViewport(x, y, width, height);
}
void srglDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices) {
  glDrawElements(mode, count, type, indices);
}
void srglDrawArrays (GLenum mode, GLint first, GLsizei count) {
  glDrawArrays(mode, first, count);
}
void srglBlendFunc (GLenum sfactor, GLenum dfactor) {
  glBlendFunc(sfactor, dfactor);
}

void srglGenTextures (GLsizei n, GLuint *textures) {
  glGenTextures(n, textures);
}
void srglDeleteTextures (GLsizei n, const GLuint *textures) {
  glDeleteTextures(n, textures);
}
void srglPixelStorei (GLenum pname, GLint param) {
  glPixelStorei(pname, param);
}
void srglTexParameteri (GLenum target, GLenum pname, GLint param) {
  glTexParameteri(target, pname, param);
}
void srglTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels) {
  glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
}

