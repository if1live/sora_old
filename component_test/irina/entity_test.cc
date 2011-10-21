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

TEST(Entity, CreateEntity) {
  using irina::World;
  using irina::Entity;
  using std::string;
  World world;
  Entity *e1 = world.CreateEntity();
  Entity *e2 = world.CreateEntity(string("asdf"));
  EXPECT_EQ(true, e1->world() == &world);
  EXPECT_EQ(true, e2->world() == &world);
  EXPECT_STREQ("asdf", e2->name().c_str());
}

TEST(Entity, DestroyEntity) {
  using irina::World;
  using irina::Entity;
  using std::string;

  World world;
  EXPECT_EQ(0, world.EntityCount());
  
  Entity *e1 = world.CreateEntity();
  EXPECT_EQ(1, world.EntityCount());

  Entity *e2 = world.CreateEntity(string("asdf"));
  EXPECT_EQ(2, world.EntityCount());

  e2->world()->DestroyEntity(e2);
  EXPECT_EQ(1, world.EntityCount());

  world.DestroyEntity(e1);
  EXPECT_EQ(0, world.EntityCount());
}