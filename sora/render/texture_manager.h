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
#ifndef SORA_TEXTURE_MANAGER_H_
#define SORA_TEXTURE_MANAGER_H_

#include "sora/template_library.h"
#include "sora/render/texture_info.h"
#include "sora/handle_manager.h"

#if SR_USE_PCH == 0
#include "sora/unordered_map_inc.h"
#include <vector>
#endif

namespace sora {;
class Texture;

struct TextureManagerThreadRunner {
  void operator()();
  static bool run_thread;
};

// png파일같은것을 읽은 결과 내용이다. 텍스쳐 생성은
// opengl을 생성한 쓰레드인 메인 쓰레드에서 작업해야하므로 이렇게 다시 넘기도록했다
struct TextureLoadResponse {
  TexturePtr tex;
  TexFormat fmt;
  TextureHeader tex_header;
  void *data;
};

class TextureManager : public Singleton<TextureManager> {
public:
  typedef std::vector<TexturePtr>   RequestStackType;
  typedef std::vector<TextureLoadResponse>  ResponseStackType;
  typedef std::tr1::unordered_map<std::string, TexturePtr> TexDictType;

public:
  void AsyncLoad(const TexturePtr &request);
  void CancelAsyncLoad(const TexturePtr &request);
  boolean IsResponseExist() const;
  boolean IsRequestExist() const;
  TextureLoadResponse PopResponse();
  void ProcessRequest();
  void ProcessResponse();

  TexturePtr Create(const std::string &name);
  Texture *GetRaw(const std::string &name);
  TexturePtr Get(const std::string &name);
  boolean IsExist(const std::string &name) const;
  boolean Remove(const std::string &name);
  boolean Remove(const TexturePtr &tex);

  TextureManager();
protected:
  ~TextureManager();

  TexDictType tex_dict_;
  RequestStackType    request_stack_;
  ResponseStackType   response_stack_;
};
}

#endif