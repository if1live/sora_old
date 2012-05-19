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
#pragma once

#include "texture.h"

namespace sora {;
namespace gl {
  class GLFrameBuffer {
  public:
    typedef unsigned int HandleType;
  public:
    static void Init(HandleType *handle, int w, int h, Texture *color_tex, Texture *depth_tex);
    static void InitWithoutDepth(HandleType *handle, int w, int h, Texture *color_tex);

    static void Deinit(HandleType *handle);
    static bool IsInit(HandleType handle);

    static void Bind(HandleType handle);
    static void Unbind();
  protected:
    ~GLFrameBuffer() {}
  private:
    static void CreateDepthTex(int w, int h, Texture *depth_tex);
    //unsigned byte + rgba texture
    static void CreateNormalRGBAColorTex(int w, int h, Texture *color_tex);
  };
} //namespace gl
} //namespace sora