// Ŭnicode please 
#include "sora_test_stdafx.h"
#include "free_list_block_pool.h"
#include <cmath>

using namespace sora;

class KarenBlockTestObj {
public:
  KarenBlockTestObj() : a(1) { cnt++; }
  ~KarenBlockTestObj() { cnt--;}
public:
  int a;
  static int cnt;
};
int KarenBlockTestObj::cnt = 0;

TEST(FreeListBlockPool, test)
{
  FreeListBlockPool pool(3, sizeof(KarenBlockTestObj));
  EXPECT_EQ(0, KarenBlockTestObj::cnt);	//constructor is not called
  void *ptr1 = pool.Malloc();
  EXPECT_TRUE(ptr1 != NULL);
  void *ptr2 = pool.Malloc();
  EXPECT_TRUE(ptr2 != NULL);
  void *ptr3 = pool.Malloc();
  EXPECT_TRUE(ptr3 != NULL);
  //KarenTestObj *ptr4 = pool.malloc();	//dead point (too may request)

  pool.Free(ptr2);
  pool.Free(ptr3);

  void *ptr4 = pool.Malloc();
  EXPECT_TRUE(ptr4 != NULL);
  EXPECT_EQ(ptr4, ptr3);	//free list management is stack based;
  void *ptr5 = pool.Malloc();
  EXPECT_TRUE(ptr5 != NULL);
  EXPECT_EQ(ptr5, ptr2);
}

TEST(FreeListBlockPool, blockSize)
{
  FreeListBlockPool pool(3, sizeof(KarenBlockTestObj));
  void *ptr1 = pool.Malloc();
  void *ptr2 = pool.Malloc();
  EXPECT_EQ(sizeof(KarenBlockTestObj), abs((int)ptr1 - (int)ptr2));
}
