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
#ifndef SORA_COMPONENT_LIST_H_
#define SORA_COMPONENT_LIST_H_

namespace sora {;
class Component;

class ComponentList {
public:
  // 리스트를 쓴것은 넣고뺴고가 자주 발생해서이다
  typedef std::list<Component*> ListType;
  typedef ListType::iterator Iterator;
  typedef ListType::const_iterator ConstIterator;

public:
  ComponentList();
  ~ComponentList();

  bool Add(Component *comp);
  bool Remove(Component *comp);
  bool IsExist(Component *comp) const;
  int Count() const { return comp_list_.size(); }

  Iterator Begin() { return comp_list_.begin(); }
  Iterator End() { return comp_list_.end(); }
  ConstIterator Begin() const { return comp_list_.begin(); }
  ConstIterator End() const { return comp_list_.end(); }
private:
  ListType comp_list_;
};
}

#endif  // SORA_COMPONENT_LIST_H_
