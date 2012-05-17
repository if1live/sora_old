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
#include "gbuffer.h"
#include "texture.h"
#include "renderer_env.h"
#include "template_lib.h"

namespace sora {;
GBuffer::GBuffer()
  : fbo_(0), width_(2), height_(2) {
    for(size_t i = 0 ; i < tex_list_.size() ; i++) {
      tex_list_[i] = 0;
    }
}

GBuffer::~GBuffer() {

}
bool GBuffer::Init(int w, int h) {
  SR_ASSERT(IsInit() == false);

  glGenTextures(kTexCount, &tex_list_[0]);
  width_ = w;
  height_ = h;

  {
    //깊이를 텍스쳐에 연결
    TextureHandle &tex_id = tex_list_[kDepthTex];
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
    SR_CHECK_ERROR("DepthTexture");
  }
  {
    //diffuse tex
    TextureHandle &tex_id = tex_list_[kDiffuseTex];
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    SR_CHECK_ERROR("DiffuseTexture");
  }
  {
    //normal tex
    TextureHandle &tex_id = tex_list_[kNormalTex];
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_FLOAT, NULL);
    SR_CHECK_ERROR("NormalTexture");
  }
  {
    //specular tex
    TextureHandle &tex_id = tex_list_[kSpecularTex];
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    SR_CHECK_ERROR("SpecularTexture");
  }
  //fbo생성
  {
    glGenFramebuffers(1, &fbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex_list_[kDepthTex], 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_list_[kNormalTex], 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, tex_list_[kDiffuseTex], 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, tex_list_[kSpecularTex], 0);
    //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, color_rb);
    SR_CHECK_FRAMEBUFFER("fb");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
  return true;
}

void GBuffer::Deinit() {
  if(fbo_ != 0) {
    glDeleteTextures(kTexCount, &tex_list_[0]);
    for(size_t i = 0 ; i < tex_list_.size() ; i++) {
      tex_list_[i] = 0;
    }
    glDeleteFramebuffers(1, &fbo_);
    fbo_ = 0;
    width_ = 2;
    height_ = 2;
  }
}

bool GBuffer::IsInit() const {
  return (fbo_ != 0);
}
void GBuffer::Bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
  GLenum buffers[] = { 
    GL_COLOR_ATTACHMENT0, 
    GL_COLOR_ATTACHMENT1,
    GL_COLOR_ATTACHMENT2,
  };
  int buffer_size = GetArraySize(buffers);
  glDrawBuffers(buffer_size, buffers);
}
void GBuffer::Unbind() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Texture GBuffer::DepthTex() const {
  Texture tex;
  ImageDesc img_desc;
  img_desc.width = width_;
  img_desc.height = height_;
  tex.Init(tex_list_[kDepthTex], img_desc, true);
  return tex;
}
Texture GBuffer::NormalTex() const {
  Texture tex;
  ImageDesc img_desc;
  img_desc.width = width_;
  img_desc.height = height_;
  tex.Init(tex_list_[kNormalTex], img_desc, true);
  return tex;
}
Texture GBuffer::DiffuseTex() const {
  Texture tex;
  ImageDesc img_desc;
  img_desc.width = width_;
  img_desc.height = height_;
  tex.Init(tex_list_[kDiffuseTex], img_desc, true);
  return tex;
}

Texture GBuffer::SpecularTex() const {
  Texture tex;
  ImageDesc img_desc;
  img_desc.width = width_;
  img_desc.height = height_;
  tex.Init(tex_list_[kSpecularTex], img_desc, true);
  return tex;
}

} //namespace sora