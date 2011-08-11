#include "../SoraTestStdAfx.h"

#include <irina/IRComponent.h>
#include <irina/IREntity.h>
#include "IRComponentTest.h"

using namespace matsu;
using namespace irina;

TEST(Entity, addRemoveHasComponent)
{
	Entity e1;

	e1.addComponent(new Test1Component());
	EXPECT_TRUE(e1.hasComponent<Test1Component>());
	EXPECT_FALSE(e1.hasComponent<Test2Component>());
	
	e1.addComponent(new Test2Component());
	EXPECT_TRUE(e1.hasComponent<Test1Component>());
	EXPECT_TRUE(e1.hasComponent<Test2Component>());

	e1.removeComponent<Test1Component>();
	EXPECT_FALSE(e1.hasComponent<Test1Component>());
	EXPECT_TRUE(e1.hasComponent<Test2Component>());

	e1.removeComponent<Test2Component>();
	EXPECT_FALSE(e1.hasComponent<Test1Component>());
	EXPECT_FALSE(e1.hasComponent<Test2Component>());

	//component 있는 상태에서 파기
	e1.addComponent(new Test1Component());
	e1.addComponent(new Test2Component());
}

TEST(Entity, getComponent)
{
	Entity e;
	e.addComponent(new Test1Component());

	EXPECT_STREQ(e.getComponent<Test1Component>()->getComponentType().c_str(), "test1");
	EXPECT_TRUE(e.getComponent<Test2Component>() == NULL);
}

TEST(Entity, getId)
{
	Entity e1;
	Entity e2;
	EXPECT_TRUE(e1.getId() > 0);
	EXPECT_TRUE(e2.getId() > 0);
	EXPECT_TRUE(e1.getId() + 1 == e2.getId());
}

TEST(EntityManager, get)
{
	Entity e1;
	EntityId e1id = e1.getId();
	EXPECT_EQ(&e1, EntityManager::getInstance().get(e1id));
	EXPECT_EQ((Entity*)NULL, EntityManager::getInstance().get(e1id-1));
}
