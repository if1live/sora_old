// Ŭnicode please
#include "../SoraTestStdAfx.h"
#include "matsu/MTFixedSizeMemPool.h"

using namespace matsu;

class MemPoolTestObj : public FixedSizeMemPool<MemPoolTestObj,5> {
public:
	MemPoolTestObj(int v) : value(v) {}
	virtual ~MemPoolTestObj() {}
	int value;
};

TEST(FixedSizeMemPool, new_delete)
{
	//클래스의 객체가 5개만 필요하다고 이미 선언했으니까 5개만 생성한다
	MemPoolTestObj *o1 = new MemPoolTestObj(1);
	MemPoolTestObj *o2 = new MemPoolTestObj(2);
	MemPoolTestObj *o3 = new MemPoolTestObj(3);
	MemPoolTestObj *o4 = new MemPoolTestObj(4);
	MemPoolTestObj *o5 = new MemPoolTestObj(5);

	//new가 아닌 걸로 만들면 문제없다
	MemPoolTestObj testobj(12);

	EXPECT_EQ(1, o1->value);
	EXPECT_EQ(2, o2->value);
	EXPECT_EQ(3, o3->value);
	EXPECT_EQ(4, o4->value);
	EXPECT_EQ(5, o5->value);

	delete(o1);
	delete(o2);
	//delete하는 순서는 보장되어있지 않다. 그러니까 그냥 스택처럼 
	//미리 할당받은 메모리를 반환하면 새로 생성할때 골치아프다

	//2개 지웠으니까 2개 추가할당 가능
	MemPoolTestObj *o6 = new MemPoolTestObj(6);
	MemPoolTestObj *o7 = new MemPoolTestObj(7);
	EXPECT_EQ(6, o6->value);
	EXPECT_EQ(7, o7->value);

	delete(o3);
	delete(o4);
	delete(o5);
	delete(o6);
	delete(o7);
}