// Ŭnicode please 
#include "sora_test_stdafx.h"
#include "free_list_manager.h"

using namespace sora;
class FreeListManagerTest;

class FreeListManagerTest : public FreeListManager<FreeListManagerTest,3> {
public:
  FreeListManagerTest() : a(1) {}
  ~FreeListManagerTest() { printf("a\n"); }
  int a;
};

TEST(FreeListManager, test) {
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

TEST(FreeListManager, blockSize) {
  FreeListManagerTest *t1 = new FreeListManagerTest();
  FreeListManagerTest *t2 = new FreeListManagerTest();
  EXPECT_EQ(sizeof(FreeListManagerTest), abs((int)t1 - (int)t2));

  delete(t1);
  delete(t2);
}