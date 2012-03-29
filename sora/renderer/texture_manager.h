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
#ifndef SORA_TEXTURE_MANAGER_H_
#define SORA_TEXTURE_MANAGER_H_

#if SR_USE_PCH == 0
#include <string>
#include "core/unordered_map_inc.h"
#include "core/shared_ptr_inc.h"
#endif

namespace sora {;
class Texture;
typedef std::tr1::shared_ptr<Texture> TexturePtr;

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

#endif  // SORA_TEXTURE_MANAGER_H_
