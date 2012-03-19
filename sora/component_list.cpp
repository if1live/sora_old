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
#include "component_list.h"
#include "component.h"
#include "template_lib.h"

namespace sora {;
ComponentList::ComponentList() {
}
ComponentList::~ComponentList() {
  SR_ASSERT(comp_list_.size() == 0);
  sora::DestroyList(&comp_list_);  
}
bool ComponentList::IsExist(Component *comp) const {
  ConstIterator found = std::find(comp_list_.begin(), comp_list_.end(), comp);
  if (found == comp_list_.end()) {
    return false;
  } else {
    return true;
  }
}
bool ComponentList::Add(Component *comp) {
  if (comp == NULL) {
    return false;
  }
  // 넣는것은 맨 앞에 넣자. 왜냐하면 보통 나중에 넣은 컴포넌트가 빠질 확률이 높으니까
  // 중복삽입을 막기 위해서 검사를 하자
  Iterator found = std::find(comp_list_.begin(), comp_list_.end(), comp);
  if (found == comp_list_.end()) {
    comp_list_.push_front(comp);
    return true;
  } else {
    SR_ASSERT(!"alreay added");
    return false;
  }
}
bool ComponentList::Remove(Component *comp) {
  // 나중에 넣은 컴포넌트일수록 사라질 확률이 높으니까 앞부터 검색
  using std::find;
  Iterator found = find(comp_list_.begin(), comp_list_.end(), comp);
  if (found == comp_list_.end()) {
    return false;
  } else {
    comp_list_.erase(found);
    return true;
  }
}
}
