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
#ifndef SORA_TEXTURE_MANAGER_H_
#define SORA_TEXTURE_MANAGER_H_

#include "template_library.h"
#include "texture_info.h"
#include "handle.h"
#include "handle_manager.h"

namespace sora {;
class Texture;

struct TextureTag { };
typedef Handle<TextureTag> TextureHandle;

struct TextureManagerThreadRunner {
  void operator()();
  static bool run_thread;
};

// 쓰레드를 써서 텍스쳐를 로딩하기 위해서 이와 같은 방식을 사용함
struct TextureLoadRequest {
  std::string filename;
  TextureHandle handle;
  TextureParameter param;
  // 텍스쳐 매니저에 등록할것인가. 동적할당일때만 사용가능하다
  boolean register_to_manager;
};

// png파일같은것을 읽은 결과 내용이다. 텍스쳐 생성은
// opengl을 생성한 쓰레드인 메인 쓰레드에서 작업해야하므로 이렇게 다시 넘기도록했다
struct TextureLoadResponse {
  std::string filename;
  TextureHandle handle;

  TexFormat fmt;
  TextureHeader tex_header;
  TextureParameter param;
  void *data;
};

class TextureManager : public Singleton<TextureManager> {
public:
  typedef std::vector<TextureLoadRequest>   RequestStackType;
  typedef std::vector<TextureLoadResponse>  ResponseStackType;
  typedef DynamicHandleManager<Texture, TextureHandle> HandleMgrType;

public:
  void PushRequest(const TextureLoadRequest &request);
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

  TextureManager();
protected:
  ~TextureManager();

  HandleMgrType handle_mgr_;
  RequestStackType    request_stack_;
  ResponseStackType   response_stack_;

  TextureHandle CreateHandle(const std::string &name) const;
};
}

#endif