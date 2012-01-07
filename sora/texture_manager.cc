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

// texture manager에는 쓰레드가 들어가고 공유 영역이 있으니 락을 쓰자
boost::mutex request_stack_lock;
boost::mutex response_stack_lock;

bool TextureManagerThreadRunner::run_thread = true;
void TextureManagerThreadRunner::operator()() {
  //텍스쳐 로딩작업이 그렇게 자주 발생하는것도 아니니까 가끔씩만 확인하자
  boost::posix_time::time_duration td = boost::posix_time::milliseconds(100);
  while(run_thread) {
    sora::TextureManager::GetInstance().ProcessRequest();
    boost::this_thread::sleep(td);
    //printf("texture thread run...\n");
  }
  //printf("texture thread exit...\n");
}

void TextureManager::AsyncLoad(const TextureHandle &request) {
  request_stack_lock.lock();
  request_stack_.push_back(request);
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
  RequestStackType cpy_request_stack = request_stack_;
  request_stack_.clear();
  request_stack_lock.unlock();

  ResponseStackType created_request_list;
  RequestStackType::iterator it = cpy_request_stack.begin();
  RequestStackType::iterator endit = cpy_request_stack.end();
  for ( ; it != endit ; it++) {
    TextureHandle &handle = *it;
    Texture *tex = GetTexture(handle);
    SR_ASSERT(!tex->filename().empty() && "no file defined?");
    string fullpath = Filesystem::GetAppPath(tex->filename());
    TexFormat fmt;
    TextureHeader tex_header;
    void *data = Texture::LoadPNG(fullpath.c_str(), &fmt, &tex_header);
    
    TextureLoadResponse response;
    response.handle = handle;
    response.fmt = fmt;
    response.tex_header = tex_header;
    response.data = data;

    //락거는 횟수를 줄이기 위해서 임시목록에 넣었다가 한번에 완료목록에 등록하자
    created_request_list.push_back(response);
  }

  if (!created_request_list.empty()) {
    response_stack_lock.lock();
    size_t curr_response_stack_size = response_stack_.size();
    size_t target_size = curr_response_stack_size + created_request_list.size();
    if (response_stack_.capacity() < target_size) {
      response_stack_.resize(target_size);
    }
    ResponseStackType::iterator insert_it = response_stack_.begin();
    std::advance(insert_it, curr_response_stack_size);
    std::copy(created_request_list.begin(), created_request_list.end(), insert_it);
    response_stack_lock.unlock();
  }
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

    Texture *tex = GetTexture(response.handle);
    tex->Init(response.fmt, response.tex_header, tex->param(), response.data);
    delete[](response.data);
  }
}

TextureManager::TextureManager() {
}
TextureManager::~TextureManager() {
}

Texture *TextureManager::GetTexture(const std::string &name) {
  TextureHandle handle = FileNameToHandle(name);
  return GetTexture(handle);
}
boolean TextureManager::IsExist(const std::string &name) const {
  TextureHandle handle = FileNameToHandle(name);
  if (handle.IsNull()) {
    return false;
  } else {
    return true;
  }
}
boolean TextureManager::RemoveTexture(const std::string &name) {
  TextureHandle handle = FileNameToHandle(name);
  return RemoveTexture(handle);
}

boolean TextureManager::IsExist(TextureHandle &handle) {
  Texture *tex = handle_mgr_.Get(handle);
  return (tex != NULL);
}
Texture *TextureManager::GetTexture(TextureHandle &handle) {
  return handle_mgr_.Get(handle);
}
boolean TextureManager::RemoveTexture(TextureHandle &handle) {
  return handle_mgr_.Remove(handle);
}
Texture *TextureManager::CreateTexture(TextureHandle &handle) {
  if (handle.IsNull()) {
    return handle_mgr_.CreateOrGet(handle);
  } else {
    return NULL;
  }
}

TextureHandle TextureManager::FileNameToHandle(const std::string &name) const {
  HandleMgrType::DataListType::const_iterator it = handle_mgr_.Begin();
  HandleMgrType::DataListType::const_iterator endit = handle_mgr_.End();
  for ( ; it != endit ; it++) {
    const HandleMgrType::DataPairType &data_pair = *it;
    if (it->data->filename() == name) {
      u16 index = std::distance(handle_mgr_.Begin(), it);
      u16 magic = it->magic;
      TextureHandle handle(index, magic);
      return handle;
    }
  }
  TextureHandle null_handle;
  return null_handle;
}
}