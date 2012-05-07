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
#include "sora_stdafx.h"
#include "buffer_object.h"

using namespace std;

namespace sora {;

IndexBufferObject::IndexBufferObject()
  : size_(0) { 
    BasePolicy::Reset(&handle_); 
}
IndexBufferObject::~IndexBufferObject() {
}

bool IndexBufferObject::Loaded() const { 
  return BasePolicy::Loaded(handle_); 
}
void IndexBufferObject::Deinit() {
  BasePolicy::Deinit(&handle_); 
}

bool IndexBufferObject::Load(const std::vector<unsigned short> &index_list) { 
  SR_ASSERT(Loaded() == true);
  if(index_list.empty()) {
    return false;
  }
  BufferUsageType usage = kBufferUsageStatic;
  int size = index_list.size() * sizeof(index_list[0]);
  BasePolicy::Load(handle_, size, (void*)&index_list[0], usage);
  size_ = index_list.size();
  return true;
}

bool IndexBufferObject::Init(const std::vector<unsigned short> &index_list) {
  //typedef IndexContainer::value_type IndexElemType;
  //static_assert(std::is_same<unsigned short, IndexElemType>::value, "not unsigned short index");

  SR_ASSERT(Loaded() == false);
  if(index_list.empty()) {
    return false;
  }
  BasePolicy::Init(&handle_);
  return Load(index_list);
}

} //namespace sora
