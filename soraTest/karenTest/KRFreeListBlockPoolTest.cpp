// Ŭnicode please 
#include "../SoraTestStdAfx.h"
#include "karen/KRFreeListBlockPool.h"

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
	FreeListBlockPool<sizeof(KarenBlockTestObj)> pool(3);
	EXPECT_EQ(0, KarenBlockTestObj::cnt);	//constructor is not called
	void *ptr1 = pool.malloc();
	void *ptr2 = pool.malloc();
	void *ptr3 = pool.malloc();
	//KarenTestObj *ptr4 = pool.malloc();	//dead point (too may request)

	pool.free(ptr2);
	pool.free(ptr3);

	void *ptr4 = pool.malloc();
	EXPECT_EQ(ptr4, ptr3);	//free list management is stack based;
	void *ptr5 = pool.malloc();
	EXPECT_EQ(ptr5, ptr2);
}