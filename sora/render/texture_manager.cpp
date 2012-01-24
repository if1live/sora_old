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
#include "sora/io/filesystem.h"

#if SR_USE_PCH == 0
#include <algorithm>
#include <png.h>
#include <boost/thread.hpp>
#endif

#define USE_TEX_LOG 0
#if USE_TEX_LOG
#define TEX_LOG(...)  LOGI(__VA_ARGS__)
#else
#define TEX_LOG(...)
#endif

namespace sora {;

// texture manager에는 쓰레드가 들어가고 공유 영역이 있으니 락을 쓰자
boost::mutex request_stack_lock;
boost::mutex response_stack_lock;

bool TextureManagerThreadRunner::run_thread = true;
void TextureManagerThreadRunner::operator()() {
  //텍스쳐 로딩작업이 그렇게 자주 발생하는것도 아니니까 가끔씩만 확인하자
  boost::posix_time::time_duration td = boost::posix_time::milliseconds(100);

  TextureManager &mgr = TextureManager::GetInstance();

  while(run_thread) {
    if (mgr.IsRequestExist()) {
      mgr.ProcessRequest();
    } else {
      boost::this_thread::sleep(td);
    }
    //printf("texture thread run...\n");
  }
  //printf("texture thread exit...\n");
}


void TextureManager::AsyncLoad(const TexturePtr &request) {
  request_stack_lock.lock();
  request_stack_.push_back(request);
  request_stack_lock.unlock();
}
boolean TextureManager::IsRequestExist() const {
  //락없이 그냥써도 큰 문제 없을듯?
  return !request_stack_.empty();
}
void TextureManager::CancelAsyncLoad(const TexturePtr &request) {
  using namespace std;
  //아직 요청이 진입하지 않았으면 목록에서 제거
  request_stack_lock.lock();
  RequestStackType::iterator found = find(request_stack_.begin(),
    request_stack_.end(), request);
  if (found != request_stack_.end()) {
    TEX_LOG("Texture %s Load Cancelled", request->filename().c_str());
    request_stack_.erase(found);
  }
  request_stack_lock.unlock();

}
boolean TextureManager::IsResponseExist() const {
  response_stack_lock.lock();
  bool empty = response_stack_.empty();
  response_stack_lock.unlock();
  return !empty;
}
TextureLoadResponse TextureManager::PopResponse() {
  response_stack_lock.lock();
  TextureLoadResponse response = response_stack_.back();
  response_stack_.pop_back();
  response_stack_lock.unlock();
  return response;
}

void TextureManager::ProcessRequest() {
  using std::string;

  request_stack_lock.lock();
  TexturePtr request_tex;
  bool request_empty = request_stack_.empty();
  if (request_empty == false) {
    request_tex = request_stack_.back();
    request_stack_.pop_back();
  }
  request_stack_lock.unlock();
  
  if (request_empty == true) {
    return;
  }

  SR_ASSERT(!request_tex->filename().empty() && "no file defined?");
  string fullpath = Filesystem::GetAppPath(request_tex->filename());
  TexFormat fmt;
  TextureHeader tex_header;

  TEX_LOG("Texture %s Load Start", tex->filename().c_str());
  void *data = Texture::LoadPNG(fullpath.c_str(), &fmt, &tex_header);
    
  TextureLoadResponse response;
  response.tex = request_tex;
  response.fmt = fmt;
  response.tex_header = tex_header;
  response.data = data;

  response_stack_lock.lock();
  response_stack_.push_back(response);
  response_stack_lock.unlock();
}

void TextureManager::ProcessResponse() {
  response_stack_lock.lock();
  ResponseStackType cpy_response_stack_ = response_stack_;
  response_stack_.clear();
  response_stack_lock.unlock();

  ResponseStackType::iterator it = cpy_response_stack_.begin();
  ResponseStackType::iterator endit = cpy_response_stack_.end();
  for ( ; it != endit ; it++) {
    TextureLoadResponse &response = *it;
    TexturePtr &tex = response.tex;
    tex->Init(response.fmt, response.tex_header, tex->param(), response.data);
    TEX_LOG("Texture %s Load success", tex->filename().c_str());
    delete[]((unsigned char*)response.data);
  }
}

TextureManager::TextureManager() {
}
TextureManager::~TextureManager() {
}

TexturePtr TextureManager::Create(const std::string &name) {
  TexturePtr tex(new Texture());
  tex_dict_[name] = tex;
  return tex;
}

Texture *TextureManager::GetRaw(const std::string &name) {
  TexDictType::iterator found = tex_dict_.find(name);
  if (found == tex_dict_.end()) {
    return NULL;
  } else {
    return found->second.get();
  }
}
TexturePtr TextureManager::Get(const std::string &name) {
  static TexturePtr empty;
  TexDictType::iterator found = tex_dict_.find(name);
  if (found == tex_dict_.end()) {
    return empty;
  } else {
    return found->second;
  }
}
boolean TextureManager::IsExist(const std::string &name) const {
  TexDictType::const_iterator found = tex_dict_.find(name);
  if (found == tex_dict_.end()) {
    return false;
  } else {
    return true;
  }
}

boolean TextureManager::Remove(const std::string &name) {
  TexDictType::iterator found = tex_dict_.find(name);
  if (found != tex_dict_.end()) {
    tex_dict_.erase(found);
    return true;
  } else {
    return false;
  }
}
boolean TextureManager::Remove(const TexturePtr &tex) {
  TexDictType::iterator it = tex_dict_.begin();
  TexDictType::iterator endit = tex_dict_.end();
  for ( ; it != endit ; it++) {
    if (it->second == tex) {
      tex_dict_.erase(it);
      return true;
    }
  }
  return false;
}
}