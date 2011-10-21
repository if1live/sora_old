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
#include "precompile.h"
#include "irina/entity.h"
#include "irina/world.h"
#include "irina/component.h"

namespace irina {;
class ComponentTypeFind {
public:
  ComponentTypeFind(int comp_type) : comp_type_(comp_type) {}
  bool operator()(Component *comp) {
    return (comp->GetClassType() == comp_type_);
  }
private:
  int comp_type_;
};

Entity::Entity(World *world)
  : world_(world), user_data(NULL) {
}
Entity::Entity(World *world, const std::string &name)
  : world_(world), user_data(NULL), name_(name) {
}
Entity::~Entity() {
  BOOST_FOREACH(Component *comp, comp_list_) {
    world()->UnregisterComponent(comp);
    delete(comp);
  }
  comp_list_.clear();
}
World *Entity::world() {
  return world_;
}
const std::string &Entity::name() const {
  return name_;
}
bool Entity::AddComponent(Component *comp) {
  if (comp == NULL) {
    return false;
  }
  using std::find;
  ComponentListType::iterator found = find(comp_list_.begin(),
    comp_list_.end(), comp);
  if (found == comp_list_.end()) {
    comp_list_.push_back(comp);
    world()->RegisterComponent(comp);
    return true;
  } else {
    SR_ASSERT(!"already exist");
    return false;
  }
}
bool Entity::HasComponent(int comp_type) const {
  ComponentTypeFind func(comp_type);
  using std::find_if;
  ComponentListType::const_iterator found = find_if(comp_list_.begin(),
    comp_list_.end(), func);
  return (found != comp_list_.end());
}
bool Entity::RemoveComponent(int comp_type) {
  ComponentTypeFind func(comp_type);
  using std::find_if;
  ComponentListType::iterator found = find_if(comp_list_.begin(),
    comp_list_.end(), func);
  if (found == comp_list_.end()) {
    return false;
  } else {
    Component *comp = *found;
    world()->UnregisterComponent(comp);
    comp_list_.erase(found);
    delete(comp);
    return true;
  }
}
Component *Entity::GetComponent(int comp_type) {
  ComponentTypeFind func(comp_type);
  using std::find_if;
  ComponentListType::iterator found = find_if(comp_list_.begin(),
    comp_list_.end(), func);
  if (found == comp_list_.end()) {
    return NULL;
  } else {
    Component *comp = *found;
    return comp;
  }
}
}