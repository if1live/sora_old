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
#include "world.h"
#include "template_lib.h"
#include "entity.h"
#include "component_list.h"
#include "component.h"

namespace sora {;
World::World() {
}
World::~World() {
  sora::DestroyDict(&entity_dict_);
  entity_name_dict_.clear();

  sora::DestroyDict(&complist_dict_);
}
Entity *World::CreateEntity() {
  Entity *entity = new Entity(this);
  int id = entity->id();
  entity_dict_[id] = entity;
  return entity;
}
Entity *World::CreateEntity(const std::string &name) {
  Entity *entity = new Entity(this, name);
  int id = entity->id();
  entity_dict_[id] = entity;
  entity_name_dict_[name] = entity;
  return entity;
}
bool World::DestroyEntity(Entity *entity) {
  const std::string &name = entity->name();
  if(name.empty() == false) {
    EntityNameDictType::iterator name_it = entity_name_dict_.find(name);
    SR_ASSERT(name_it != entity_name_dict_.end());
    entity_name_dict_.erase(name_it);
  }

  EntityDictType::iterator it = entity_dict_.find(entity->id());
  if (it != entity_dict_.end()) {
    entity_dict_.erase(it);
    delete(entity);
    return true;
  } else {
    return false;
  }
}
ComponentList &World::GetComponentList(int comp_type) {
  ComponentListDictType::iterator found = complist_dict_.find(comp_type);
  ComponentList *complist = NULL;
  if (found == complist_dict_.end()) {
    complist = new ComponentList();
    complist_dict_[comp_type] = complist;
  } else {
    complist = found->second;
  }
  return *complist;
}
bool World::RegisterComponent(Component *comp) {
  ComponentList &complist = GetComponentList(comp->GetClassType());
  return complist.Add(comp);
}
bool World::UnregisterComponent(Component *comp) {
  ComponentList &complist = GetComponentList(comp->GetClassType());
  return complist.Remove(comp);
}
}
