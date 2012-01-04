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
#include "sora_stdafx.h"
#include "texture_manager.h"
#include "texture.h"
#include "filesystem.h"

namespace sora {;
bool TextureManagerThreadRunner::run_thread = true;
void TextureManagerThreadRunner::operator()() {
  //텍스쳐 로딩작업이 그렇게 자주 발생하는것도 아니니까 가끔씩만 확인하자
  boost::posix_time::time_duration td = boost::posix_time::milliseconds(100);
  while(run_thread) {
    sora::TextureManager::GetInstance().ProcessRequest();
    boost::this_thread::sleep(td);
    //printf("texture thread run...\n");
  }
  printf("texture thread exit...\n");
}

void TextureManager::PushRequest(const TextureLoadRequest &request) {
  request_stack_.push_back(request);
}
boolean TextureManager::IsResponseExist() const {
  if (response_stack_.empty()) {
    return false;
  } else {
    return true;
  }
}
TextureLoadResponse TextureManager::PopResponse() {
  TextureLoadResponse response = response_stack_.back();
  response_stack_.pop_back();
  return response;
}

void TextureManager::ProcessRequest() {
  using std::string;
  RequestStackType::iterator it = request_stack_.begin();
  RequestStackType::iterator endit = request_stack_.end();
  for ( ; it != endit ; it++) {
    const TextureLoadRequest &request = *it;
    string fullpath = Filesystem::GetAppPath(request.filename);
    TexFormat fmt;
    TextureHeader tex_header;
    void *data = Texture::LoadPNG(fullpath.c_str(), &fmt, &tex_header);
    
    TextureLoadResponse response;
    response.filename = request.filename;
    response.tex = request.tex;
    response.fmt = fmt;
    response.tex_header = tex_header;
    response.param = request.param;
    response.data = data;

    //TODO dict에는 어떻게 저장?

    response_stack_.push_back(response);
  }
  request_stack_.clear();
}

void TextureManager::ProcessResponse() {
  ResponseStackType::iterator it = response_stack_.begin();
  ResponseStackType::iterator endit = response_stack_.end();
  for ( ; it != endit ; it++) {
    const TextureLoadResponse &response = *it;

    response.tex->Init(response.fmt, response.tex_header, response.param, response.data);
    delete[](response.data);
  }
  response_stack_.clear();
}

TextureManager::TextureManager() {
}
TextureManager::~TextureManager() {
  TextureDictType::iterator it = tex_dict_.begin();
  TextureDictType::iterator endit = tex_dict_.end();
  for ( ; it != endit ; it++) {
    Texture *tex = it->second;
    delete(tex);
  }
}

}