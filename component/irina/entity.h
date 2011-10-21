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
#ifndef COMPONENT_IRINA_ENTITY_H_
#define COMPONENT_IRINA_ENTITY_H_

#include "irina/irina_enum.h"
#include "sora/id_generator.h"

namespace irina {;
class Entity : public sora::IdGenerator<Entity> {
 public:
  Entity(World *world);
  Entity(World *world, const std::string &name);
  ~Entity();

  World *world();

 ///custom data
 public:
  template<typename T>
  T GetUserDataAs();
  void *user_data;

 public:
  const std::string &name() const;

 private:
  World *world_;
  std::string name_;
};
}

namespace irina {;
template<typename T>
T Entity::GetUserDataAs() {
  return reinterpret_cast<T>(user_data);
}
}

#endif  // COMPONENT_IRINA_ENTITY_H_
