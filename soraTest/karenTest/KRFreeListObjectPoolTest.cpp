// Ŭnicode please
#include "../SoraTestStdAfx.h"
#include "karen/KRFreeListObjectPool.h"

using namespace karen;

class KarenTestObj {
public:
	KarenTestObj() : a(1) { cnt++; }
	~KarenTestObj() { cnt--;}
public:
	int a;
	static int cnt;
};
int KarenTestObj::cnt = 0;

TEST(KRFreeListPool, test)
{
	FreeListObjectPool<KarenTestObj> pool(3);
	EXPECT_EQ(3, KarenTestObj::cnt);	//constructor called by pool initializion phase
	KarenTestObj *ptr1 = pool.malloc();
	KarenTestObj *ptr2 = pool.malloc();
	KarenTestObj *ptr3 = pool.malloc();
	//KarenTestObj *ptr4 = pool.malloc();	//dead point (too may request)

	pool.free(ptr2);
	pool.free(ptr3);

	KarenTestObj *ptr4 = pool.malloc();
	EXPECT_EQ(ptr4, ptr3);	//free list management is stack based;
	KarenTestObj *ptr5 = pool.malloc();
	EXPECT_EQ(ptr5, ptr2);
}