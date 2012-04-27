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
#ifndef SORA_GL_TEXTURE_H_
#define SORA_GL_TEXTURE_H_

#if SR_USE_PCH == 0
#include "core/shared_ptr_inc.h"
#include <string>
#include "gl_inc.h"
#endif
#include "renderer/globals.h"
#include "gl_env.h"

namespace sora {;
class Image;
namespace gl {
  class GLTexture {
  public:
    typedef TextureHandleType HandleType;
  protected:
    ~GLTexture() {}

  public:
    void Deinit(HandleType *handle);
    bool Init(HandleType *handle);
    bool Loaded(const HandleType &handle) const;

    bool LoadTexture(const HandleType &handle, const Image &image, uint policy);
    void ApplyTextureParam(HandleType handle, const TextureParam &param);
    bool LoadTexture(HandleType handle, unsigned char *image, const ImageDesc &img_desc, uint policy);
  public:
    bool LoadTexture(HandleType tex_id, unsigned char *image, int width, int height, GLenum format, uint policy);
  };
} //namespace gl
} //namespace sora

#endif  // SORA_TEXTURE_H_