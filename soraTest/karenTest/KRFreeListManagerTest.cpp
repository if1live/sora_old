// Ŭnicode please 
#include "../SoraTestStdAfx.h"
#include "karen/KRFreeListManager.h"

using namespace karen;
class FreeListManagerTest;

class FreeListManagerTest : public FreeListManager<FreeListManagerTest,3> {
public:
	FreeListManagerTest() : a(1) {}
	virtual ~FreeListManagerTest() {}
	int a;
};

TEST(KRFreeListManager, test)
{
	FreeListManagerTest *t1 = new FreeListManagerTest();
	FreeListManagerTest *t2 = new FreeListManagerTest();
	FreeListManagerTest *t3 = new FreeListManagerTest();
	//FreeListManagerTest *t4 = new FreeListManagerTest();	//fail by assert

	delete(t2);
	delete(t3);

	FreeListManagerTest *t4 = new FreeListManagerTest();
	EXPECT_EQ(t4, t3);
	FreeListManagerTest *t5 = new FreeListManagerTest();
	EXPECT_EQ(t5, t2);

	//delete all
	delete(t1);
	delete(t4);
	delete(t5);
}

TEST(KRFreeListManager, blockSize)
{
	FreeListManagerTest *t1 = new FreeListManagerTest();
	FreeListManagerTest *t2 = new FreeListManagerTest();
	EXPECT_EQ(sizeof(FreeListManagerTest), abs((int)t1 - (int)t2));

	delete(t1);
	delete(t2);
}