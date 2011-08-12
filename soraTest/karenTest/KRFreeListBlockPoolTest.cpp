// Ŭnicode please 
#include "../SoraTestStdAfx.h"
#include "karen/KRFreeListBlockPool.h"
#include <cmath>

using namespace karen;

class KarenBlockTestObj {
public:
	KarenBlockTestObj() : a(1) { cnt++; }
	~KarenBlockTestObj() { cnt--;}
public:
	int a;
	static int cnt;
};
int KarenBlockTestObj::cnt = 0;

TEST(KRFreeListBlockPool, test)
{
	FreeListBlockPool pool(3, sizeof(KarenBlockTestObj));
	EXPECT_EQ(0, KarenBlockTestObj::cnt);	//constructor is not called
	void *ptr1 = pool.malloc();
	EXPECT_TRUE(true, ptr1 != NULL);
	void *ptr2 = pool.malloc();
	EXPECT_TRUE(true, ptr2 != NULL);
	void *ptr3 = pool.malloc();
	EXPECT_TRUE(true, ptr3 != NULL);
	//KarenTestObj *ptr4 = pool.malloc();	//dead point (too may request)

	pool.free(ptr2);
	pool.free(ptr3);

	void *ptr4 = pool.malloc();
	EXPECT_TRUE(true, ptr4 != NULL);
	EXPECT_EQ(ptr4, ptr3);	//free list management is stack based;
	void *ptr5 = pool.malloc();
	EXPECT_TRUE(true, ptr5 != NULL);
	EXPECT_EQ(ptr5, ptr2);
}

TEST(KRFreeListBlockPool, blockSize)
{
	FreeListBlockPool pool(3, sizeof(KarenBlockTestObj));
	void *ptr1 = pool.malloc();
	void *ptr2 = pool.malloc();
	EXPECT_EQ(sizeof(KarenBlockTestObj), abs((int)ptr1 - (int)ptr2));
}
