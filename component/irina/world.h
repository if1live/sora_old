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
#ifndef COMPONENT_IRINA_WORLD_H_
#define COMPONENT_IRINA_WORLD_H_

#include "irina/irina_enum.h"
#include "sora/tr1_include.h"
#include "irina/component.h"

namespace irina {;
class World {
 public:
  typedef std::tr1::unordered_map<int, Entity*> EntityDictType;
  typedef std::tr1::unordered_map<std::string, Entity*> EntityNameDictType;
  typedef std::tr1::unordered_map<int, ComponentList*> ComponentListDictType;
 public:
  World();
  ~World();

  Entity *CreateEntity();
  Entity *CreateEntity(const std::string &name);
  bool DestroyEntity(Entity *entity);
  int EntityCount() const { return entity_dict_.size(); }

 public:
  bool RegisterComponent(Component *comp);
  bool UnregisterComponent(Component *comp);
  ComponentList &GetComponentList(int comp_type);
  template<typename T>  ComponentList &GetComponentList() {
    return GetComponentList(T::ClassType());
  }

 private:
  EntityDictType entity_dict_;
  EntityNameDictType entity_name_dict_;
  ComponentListDictType complist_dict_;
};
}

#endif  // COMPONENT_IRINA_WORLD_H_
