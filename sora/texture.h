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
#ifndef SORA_TEXTURE_H_
#define SORA_TEXTURE_H_

#include "gl_texture.h"
#include "shared_ptr_inc.h"
#include "unordered_map_inc.h"
#include "globals.h"

namespace sora {;

template<typename PolicyType>
class TextureT : public PolicyType {
public:
  typedef PolicyType Policy;
  typedef TextureHandle HandleType;
public:

  TextureT(unsigned int policy = 0)
    : is_render_to_texture_(false), 
    tex_policy_(policy),
    handle_(0) {}

  TextureT(const std::string &name, unsigned int policy = 0)
  : name_(name), 
  is_render_to_texture_(false), 
  tex_policy_(policy),
  handle_(0) {}
  ~TextureT() {}

  //외부에서 생성된 GL텍스쳐를 sora텍스쳐로 사용하기
  void Deinit() { Policy::Deinit(&handle_); }
  void Init() { Policy::Init(&handle_); }
  //외부에서 생성된 텍스쳐를 직접 찔러넣기. renter texture나 외부 라이브러리에서 생성된 텍스쳐에서 쓴다
  bool Init(HandleType handle, const ImageDesc &img_desc, bool is_rtt) {
    if(handle_ != 0) {
      Deinit();
    }
    handle_ = handle;
    img_desc_ = img_desc;
    is_render_to_texture_ = is_rtt;
    return true;
  }

  bool Loaded() const { return Policy::Loaded(handle_); }
  const std::string &name() const { return name_; }
  
  bool is_alpha() const { return img_desc_.is_alpha; }
  bool is_grayscale() const { return img_desc_.is_grayscale; }
  bool is_render_to_texture() const { return is_render_to_texture_; }

  bool LoadTexture(const Image &img) {
    img_desc_ = img.desc();
    return Policy::LoadTexture(handle_, img, tex_policy_);
  }

  //압축 풀린 데이터를 올리는 경우
  bool LoadTexture(unsigned char *image, int w, int h, TexFormatType format, const TextureParam &param) {
    img_desc_ = CreateImageDesc(w, h, format);
    bool result = Policy::LoadTexture(handle_, image, img_desc_, tex_policy_);
    Policy::ApplyTextureParam(handle_, param);
    return result;
  }
  const HandleType &handle() const { return handle_; }

  static ImageDesc CreateImageDesc(int w, int h, TexFormatType format);

private:
  std::string name_;
  HandleType handle_;

  //texture's image data
  ImageDesc img_desc_;
  bool is_render_to_texture_;
  unsigned int tex_policy_;
};

//텍스쳐 관리까지 한군데에 전부 떠넘기자
class TextureManager {
public:
  TextureManager();
  ~TextureManager();

  //텍스쳐 파일정보와 메모리를 설정한 다음에 텍스쳐를 넣으면
  //복사해서 적절히 생성함
  bool Add(const Texture &tex);

  bool IsExist(const std::string &name) const;
  TexturePtr Get(const std::string &name);
  Texture *Get_ptr(const std::string &name) { return Get(name).get(); }

private:
  typedef std::tr1::unordered_map<std::string, TexturePtr> TexDictType;

  TexDictType tex_dict_;
};
}
#endif  // SORA_TEXTURE_H_