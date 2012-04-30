// Ŭnicode please
#include "sora_test_stdafx.h"
#include "free_list_object_pool.h"

using namespace sora;

class KarenTestObj {
public:
  KarenTestObj() : a(1) { cnt++; }
  ~KarenTestObj() { cnt--;}
public:
  int a;
  static int cnt;
};
int KarenTestObj::cnt = 0;

TEST(FreeListPool, test) {
  FreeListObjectPool<KarenTestObj> pool(3);
  EXPECT_EQ(3, KarenTestObj::cnt);	//constructor called by pool initializion phase
  KarenTestObj *ptr1 = pool.Malloc();
  KarenTestObj *ptr2 = pool.Malloc();
  KarenTestObj *ptr3 = pool.Malloc();
  //KarenTestObj *ptr4 = pool.malloc();	//dead point (too may request)

  pool.Free(ptr2);
  pool.Free(ptr3);

  KarenTestObj *ptr4 = pool.Malloc();
  EXPECT_EQ(ptr4, ptr3);	//free list management is stack based;
  KarenTestObj *ptr5 = pool.Malloc();
  EXPECT_EQ(ptr5, ptr2);
}