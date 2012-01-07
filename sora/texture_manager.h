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

#include "template_library.h"
#include "texture_info.h"
#include "handle_manager.h"

namespace sora {;
class Texture;

struct TextureManagerThreadRunner {
  void operator()();
  static bool run_thread;
};

// png파일같은것을 읽은 결과 내용이다. 텍스쳐 생성은
// opengl을 생성한 쓰레드인 메인 쓰레드에서 작업해야하므로 이렇게 다시 넘기도록했다
struct TextureLoadResponse {
  TextureHandle handle;

  TexFormat fmt;
  TextureHeader tex_header;
  void *data;
};

class TextureManager : public Singleton<TextureManager> {
public:
  typedef std::vector<TextureHandle>   RequestStackType;
  typedef std::vector<TextureLoadResponse>  ResponseStackType;
  typedef DynamicHandleManager<Texture, TextureHandle> HandleMgrType;

public:
  void AsyncLoad(const TextureHandle &request);
  boolean IsResponseExist() const;
  TextureLoadResponse PopResponse();
  void ProcessRequest();
  void ProcessResponse();
  
  // filename based
  boolean IsExist(const std::string &name) const;
  Texture *GetTexture(const std::string &name);
  boolean RemoveTexture(const std::string &name);

  // handle based
  boolean IsExist(TextureHandle &handle);
  Texture *GetTexture(TextureHandle &handle);
  boolean RemoveTexture(TextureHandle &handle);

  Texture *CreateTexture(TextureHandle &handle);

  // 게임 내부의 구현은 핸들에 의존하는데 외부에서 읽은 파일은
  // 파일명에 의존하니 중간 변환을 할수있도록한다
  TextureHandle FileNameToHandle(const std::string &name) const;

  TextureManager();
protected:
  ~TextureManager();

  HandleMgrType handle_mgr_;
  RequestStackType    request_stack_;
  ResponseStackType   response_stack_;

};
}

#endif