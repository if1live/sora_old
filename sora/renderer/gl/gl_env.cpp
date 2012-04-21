﻿/*  Copyright (C) 2011 by if1live */
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

#include "core/common_string.h"
#include "gl_env.h"

#if SR_ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

namespace sora {;
namespace gl {
  bool GLEnv::CheckFrameBufferStatus(const char *name) {
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(status == GL_FRAMEBUFFER_COMPLETE) {
      return true;
    }
    //else,,
    const char *error_msg = NULL;
    switch(status) {
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
      error_msg = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
      break;
#if SR_WIN && (SR_GLES == 0)
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
#else
    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
#endif
      error_msg = "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS";
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
      error_msg = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
      break;
    case GL_FRAMEBUFFER_UNSUPPORTED:
      error_msg = "GL_FRAMEBUFFER_UNSUPPORTED";
      break;
#if SR_WIN && (SR_GLES == 0)
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER :
      error_msg = "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
      break;
#endif
    default:
      CheckError(name);
      error_msg = "Unknown Error";
      break;  
    }
    LOGE("FrameBuffer Error [%s] : %s", name, error_msg);
    //SR_ASSERT(false);

    return false;
  }

  bool GLEnv::CheckError(const char *name) {
    int error = glGetError();
    if (error != GL_NO_ERROR) {
      const char *error_msg;
      switch (error) {
      case GL_INVALID_ENUM:
        error_msg = "GL_INVALID_ENUM";
        break;
      case GL_INVALID_VALUE:
        error_msg = "GL_INVALID_VALUE";
        break;
      case GL_INVALID_OPERATION:
        error_msg = "GL_INVALID_OPERATION";
        break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        error_msg = "GL_INVALID_FRAMEBUFFER_OPERATION";
        break;
      case GL_OUT_OF_MEMORY:
        error_msg = "GL_OUT_OF_MEMORY";
        break;
      default:
        error_msg = "UNKNOWN";
        break;
      }

      LOGE("OpenGL Error [%s] : %s", name, error_msg);
#if SR_ANDROID
      exit(-1);
#endif
      return false;
    }
    return true;
  }
  const std::vector<std::string> &GLEnv::GetExtensionList() const {
    using std::vector;
    using std::string;

    static vector<string> ext_list;
    static bool run = false;
    if (run == false) {
      run = true;
      const char *str = (const char*)glGetString(GL_EXTENSIONS);
      string ext_str = str;

      Split(ext_str, ' ', &ext_list);
    }
    return ext_list;
  }

  const std::string &GLEnv::GetVersion() const {
    static bool run = false;
    static std::string version;
    if (run == false) {
      run = true;
      const char *str = (const char *)glGetString(GL_VERSION);
      version = str;
    }
    return version;
  }
  const std::string &GLEnv::GetVender() const {
    static bool run = false;
    static std::string version;
    if (run == false) {
      run = true;
      const char *str = (const char *)glGetString(GL_VENDOR);
      version = str;
    }
    return version;
  }
  const std::string &GLEnv::GetRenderer() const {
    static bool run = false;
    static std::string version;
    if (run == false) {
      run = true;
      const char *str = (const char *)glGetString(GL_RENDERER);
      version = str;
    }
    return version;
  }
  GLenum GLEnv::DrawTypeToGLEnum(DrawType type) {
    static bool run = false;
    static std::array<GLenum, 10> enum_list;
    if(run == false) {
      run = true;
      enum_list[kDrawPoints] = GL_POINTS;
      enum_list[kDrawLines]  = GL_LINES;
      enum_list[kDrawLineStrip] = GL_LINE_STRIP;
      enum_list[kDrawLineLoop] = GL_LINE_LOOP;
      enum_list[kDrawTriangles] = GL_TRIANGLES;
      enum_list[kDrawTriangleStrip] = GL_TRIANGLE_STRIP;
      enum_list[kDrawTriangleFan] = GL_TRIANGLE_FAN;
    }
    return enum_list[(int)type];
  }
  GLenum GLEnv::TexFormatToGLEnum(TexFormatType type) {
    static bool run = false;
    static std::array<GLenum, 10> enum_list;
    if(run == false) {
      run = true;
      enum_list[kTexFormatAlpha] = GL_ALPHA;
      enum_list[kTexFormatLumianceAlpha] = GL_LUMINANCE_ALPHA;
      enum_list[kTexFormatLumiance] = GL_LUMINANCE;
      enum_list[kTexFormatRGB] = GL_RGB;
      enum_list[kTexFormatRGBA] = GL_RGBA;
    }
    return enum_list[(int)type];
  }
  GLenum GLEnv::TexMagFilterToGLEnum(TexMagFilter type) {
    static bool run = false;
    static std::array<GLenum, 10> enum_list;
    if(run == false) {
      run = true;
      enum_list[kTexMagLinear] = GL_LINEAR;
      enum_list[kTexMagNearest] = GL_NEAREST;
    }
    return enum_list[(int)type];
  }
  GLenum GLEnv::TexMinFilterToGLEnum(TexMinFilter type) {
    static bool run = false;
    static std::array<GLenum, 10> enum_list;
    if(run == false) {
      run = true;
      enum_list[kTexMinLinear] = GL_LINEAR;
      enum_list[kTexMinNearest] = GL_NEAREST;
      enum_list[kTexMinNearestMipmapNearest] = GL_NEAREST_MIPMAP_NEAREST;
      enum_list[kTexMinNearestMipmapLinear] = GL_NEAREST_MIPMAP_LINEAR;
      enum_list[kTexMinLinearMipmapNearest] = GL_LINEAR_MIPMAP_NEAREST;
      enum_list[kTexMinLinearMipmapLinear] = GL_LINEAR_MIPMAP_LINEAR;
    }
    return enum_list[(int)type];
  }

} //namespace gl
} //namespace sora
