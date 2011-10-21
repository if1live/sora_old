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
#include "sample_component.h"
#include "irina/component_list.h"

TEST(World, CreateEntity) {
  using irina::World;
  using irina::Entity;
  using std::string;
  World world;
  Entity *e1 = world.CreateEntity();
  Entity *e2 = world.CreateEntity(string("asdf"));
  EXPECT_EQ(true, e1->world() == &world);
  EXPECT_EQ(true, e2->world() == &world);
  EXPECT_EQ(NULL, e1->user_data);
  EXPECT_STREQ("asdf", e2->name().c_str());
}

TEST(World, DestroyEntity) {
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

TEST(World, Component) {
  using irina::World;
  
  World world;
  Sample1Component comp1;
  world.RegisterComponent(&comp1);
  Sample1Component comp2;
  world.RegisterComponent(&comp2);
  Sample1Component comp3;
  world.RegisterComponent(&comp3);

  Sample2Component comp4;
  world.RegisterComponent(&comp4);
  Sample2Component comp5;
  world.RegisterComponent(&comp5);

  EXPECT_EQ(3, world.GetComponentList<Sample1Component>().Count());
  EXPECT_EQ(2, world.GetComponentList<Sample2Component>().Count());

  world.UnregisterComponent(&comp1);
  world.UnregisterComponent(&comp2);
  world.UnregisterComponent(&comp3);
  world.UnregisterComponent(&comp4);
  world.UnregisterComponent(&comp5);

  EXPECT_EQ(0, world.GetComponentList<Sample1Component>().Count());
  EXPECT_EQ(0, world.GetComponentList<Sample2Component>().Count());
}

TEST(Entity, component) {
  using irina::World;
  using irina::Entity;

  World world;
  Entity *e1 = world.CreateEntity();
  EXPECT_EQ(0, e1->ComponentCount());
  EXPECT_EQ(false, e1->HasComponent<Sample1Component>());
  EXPECT_EQ(false, e1->RemoveComponent<Sample1Component>());
  EXPECT_EQ(true, NULL == e1->GetComponent<Sample1Component>());

  e1->AddComponent(new Sample1Component());
  EXPECT_EQ(1, e1->ComponentCount());
  EXPECT_EQ(true, e1->HasComponent<Sample1Component>());
  EXPECT_EQ(false, e1->HasComponent<Sample2Component>());
  EXPECT_EQ(false, e1->RemoveComponent<Sample2Component>());
  EXPECT_EQ(true, NULL != e1->GetComponent<Sample1Component>());

  e1->AddComponent(new Sample2Component());
  EXPECT_EQ(2, e1->ComponentCount());
  EXPECT_EQ(true, e1->HasComponent<Sample1Component>());
  EXPECT_EQ(true, e1->HasComponent<Sample2Component>());

  EXPECT_EQ(true, e1->RemoveComponent<Sample1Component>());
  EXPECT_EQ(1, e1->ComponentCount());
  EXPECT_EQ(false, e1->HasComponent<Sample1Component>());
  EXPECT_EQ(true, e1->HasComponent<Sample2Component>());
  EXPECT_EQ(false, e1->RemoveComponent<Sample1Component>());
}
