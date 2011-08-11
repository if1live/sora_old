// Ŭnicode please
#include "../SoraTestStdAfx.h"

#include <irina/IRComponent.h>
#include <irina/IREntity.h>
#include "IRComponentTest.h"

using namespace irina;
using namespace std;

class Test2ComponentHandler {
public:
	Test2ComponentHandler() { clear(); }
	void operator()(Test2Component *comp) { called = true; ptr = comp; }
	void clear() { called = false; ptr = NULL; }
	bool called;
	Test2Component *ptr;
};

TEST(Component, classtype)
{
	Test1Component c1;
	EXPECT_STREQ("test1", c1.getComponentType().c_str()); 
	EXPECT_STREQ("test1", c1.getType().c_str());
	Test2Component c2;
	EXPECT_STREQ("test2", c2.getComponentType().c_str());
	EXPECT_STREQ("test2", c2.getType().c_str());

	Component *c = new Test1Component();
	EXPECT_STREQ("test1", c->getType().c_str());
	//포인트로 받은 경우는 static 함수로 타입을 얻는것이 컴파일타임에 불가능하도록 함
	//왜냐하면 static를 시도하면 Component자체의 정적함수를 찾도록하는데 그것은 없으니까
	//그래서 아래의 한줄은 컴파일도 안된다
	//EXPECT_STREQ("test1", c->getComponentType().c_str());
	delete(c);
}

TEST(ComponentManager, size)
{
	//before
	ASSERT_EQ(0, (int)ComponentManager<Test1Component>::size());
	ASSERT_EQ(0, (int)ComponentManager<Test2Component>::size());

	//start
	Test1Component c1;
	EXPECT_EQ(0, (int)ComponentManager<Test1Component>::size());
	Test1Component c2;
	EXPECT_EQ(0, (int)ComponentManager<Test1Component>::size());

	Test2Component c3;
	EXPECT_EQ(1u, ComponentManager<Test2Component>::size());
	Test2Component c4;
	EXPECT_EQ(2u, ComponentManager<Test2Component>::size());

	Component *c = new Test2Component();
	EXPECT_EQ(3u, ComponentManager<Test2Component>::size());
	delete(c);
	EXPECT_EQ(2u, ComponentManager<Test2Component>::size());
}

TEST(ComponentManager, doAll)
{
	Test2Component c1;
	Test2ComponentHandler handler;
	ComponentManager<Test2Component>::doAll(handler);

	EXPECT_EQ(true, handler.called);
	EXPECT_EQ(&c1, handler.ptr);

}

