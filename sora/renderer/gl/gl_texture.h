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
  public:
    GLTexture(const std::string &name, uint policy = 0);
    ~GLTexture();

    void Deinit();
    bool Init();
    //외부에서 생성된 GL텍스쳐를 sora텍스쳐로 사용하기. 근데 쓰나?
    bool Init(uint tex_id, int width, int height, bool has_alpha, bool is_rtt);
    bool Loaded() const;

    uint handle() const { return handle_; }
    const std::string &name() const { return name_; }
    bool has_alpha() const { return has_alpha_; }
    bool is_render_to_texture() const { return is_render_to_texture_; }

    //raw image에서 텍스쳐 생성하기
    bool LoadTexture(unsigned char *image, int w, int h, TexFormatType format, const TextureParam &param);
    bool LoadTexture(const Image &image);

  private:
    uint handle_;
    //텍스쳐 고유이름을 둬서 디버깅이나 검색이나 뭐 기타 용도로 쓰자
    std::string name_;

    int tex_width_;
    int tex_height_;
    int src_width_;
    int src_height_;
    bool has_alpha_;

    uint policy_;

    //render to texture?
    bool is_render_to_texture_;

  private:
    //압축이 풀린 데이터로부터 적절히 텍스쳐 생성하기
    bool LoadTexture(GLuint tex_id, unsigned char *image, int width, int height, GLenum format);
    bool LoadTexture(GLuint tex_id, const Image &image);
  };
} //namespace gl
} //namespace sora

#endif  // SORA_TEXTURE_H_