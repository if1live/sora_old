/*  Copyright (C) 2011-2012 by if1live */
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
#include "gl_frame_buffer.h"

#include "renderer/renderer_env.h"

namespace sora {;
namespace gl {
  void GLFrameBuffer::CreateDepthTex(int w, int h, Texture *depth_tex) {
    GLuint tex_id = 0;
    //깊이를 텍스쳐에 연결
    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
    SR_CHECK_ERROR("DepthTexture");

    ImageDesc desc;
    desc.width = w;
    desc.height = h;
    depth_tex->Init(tex_id, desc, true);
  }
  void GLFrameBuffer::CreateNormalRGBAColorTex(int w, int h, Texture *color_tex) {
    //색은 그냥 버퍼로 쓰자
    /*
    glGenRenderbuffers(1, &color_rb);
    glBindRenderbuffer(GL_RENDERBUFFER, color_rb);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA4, w, h);
    SR_CHECK_ERROR("1");
    */
    GLuint tex_id;
    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    SR_CHECK_ERROR("ColorTexture");

    ImageDesc desc;
    desc.width = w;
    desc.height = h;
    color_tex->Init(tex_id, desc, true);
  }

  void GLFrameBuffer::InitAsDepthTex(HandleType *handle, int w, int h, Texture *color_tex, Texture *depth_tex) {
    SR_ASSERT(*handle == 0);
    CreateDepthTex(w, h, depth_tex);
    CreateNormalRGBAColorTex(w, h, color_tex);

    //fbo생성
    {
      GLuint fbo = 0;
      glGenFramebuffers(1, &fbo);
      glBindFramebuffer(GL_FRAMEBUFFER, fbo);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_tex->handle(), 0);
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_tex->handle(), 0);
      //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, color_rb);
      SR_CHECK_FRAMEBUFFER("fb");
      SR_CHECK_ERROR("Create FB");
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      *handle = fbo;
    }
  }
  void GLFrameBuffer::Deinit(HandleType *handle) {
    if(*handle != 0) {
      glDeleteFramebuffers(1, handle);
      *handle = 0;
    }
  }
  bool GLFrameBuffer::IsInit(HandleType handle) {
    if(handle == 0) {
      return false;
    } else {
      return true;
    }
  }
  void GLFrameBuffer::Bind(HandleType handle) {
    glBindFramebuffer(GL_FRAMEBUFFER, handle);
  }
  void GLFrameBuffer::Unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
} //namespace gl
} //namespace sora
