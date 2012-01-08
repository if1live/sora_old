/*  Copyright (C) 2011 by if1live */
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// Ŭnicode please
#include "sora_test_stdafx.h"
#include "sora/handle.h"
#include "sora/handle_manager.h"

class SampleClass {
public:
  SampleClass() : a(0) { obj_count++; }
  SampleClass(int a) : a(a) { obj_count++; }
  ~SampleClass() { obj_count--; }
  int a;
  static int obj_count;
};
int SampleClass::obj_count = 0;

TEST(HandleManager, test) {
  using namespace sora;
  
  Handle<SampleClass> handle_1;
  EXPECT_EQ(true, handle_1.IsNull());
  EXPECT_EQ(0, handle_1.handle());
  
  handle_1.Init(1);
  EXPECT_EQ(false, handle_1.IsNull());

  Handle<SampleClass> handle_2(2);
  Handle<SampleClass> handle_3(1);
  
  EXPECT_EQ(true, handle_1 != handle_3);  //인덱스 생성값이 같아도 magic이 다를테니 다른 객체
  EXPECT_EQ(true, handle_1 != handle_2);
}

TEST(DynamicHandleManager, Acquire) {
  using namespace sora;
  EXPECT_EQ(0, SampleClass::obj_count);

  typedef Handle<SampleClass> SampleHandle;
  typedef DynamicHandleManager<SampleClass, SampleHandle> SampleHandleManager;
  SampleHandleManager &mgr = SampleHandleManager::GetInstance();

  // 최초에는 핸들로 아무 객체도 존재하지 않는다
  EXPECT_EQ(0, mgr.GetUsedHandleCount());

  // 새로운 객체를 1개 생성하기
  SampleHandle handle1;
  SampleClass *obj1 = mgr.Acquire(handle1);
  ASSERT_EQ(true, obj1 != NULL);
  EXPECT_EQ(1, mgr.GetUsedHandleCount());
  EXPECT_EQ(0, handle1.res_index());

  SampleClass *obj2 = mgr.Acquire(handle1);
  ASSERT_EQ(true, obj2 != NULL);
  EXPECT_EQ(obj1, obj2);

  //새로운 객체 1개 더
  SampleHandle handle2;
  SampleClass *obj3 = mgr.Acquire(handle2);
  ASSERT_NE(obj1, obj3);
  ASSERT_EQ(true, NULL != obj3);
  SampleHandle handle3;
  SampleClass *obj4 = mgr.Acquire(handle3);
  ASSERT_NE(obj3, obj4);
  ASSERT_EQ(true, NULL != obj4);
  EXPECT_EQ(3, mgr.GetUsedHandleCount());

  //하나 지우기. 핸들 관리자는 내부를 벡터로 처리하니 지울때 아마 인덱스가 꼬일거다
  //핸들을 기반으로 다시 찾아낼수 있어야한다
  mgr.Remove(handle2);  //중간에 낀거 삭제
  EXPECT_EQ(2, mgr.GetUsedHandleCount());
  EXPECT_EQ(NULL, mgr.Acquire(handle2));

  SampleClass *obj6 = mgr.Acquire(handle3);
  ASSERT_EQ(obj4, obj6);
  EXPECT_EQ(2, mgr.GetUsedHandleCount());

  mgr.Remove(handle1);  //처음꺼 삭제
  SampleClass *obj7 = mgr.Acquire(handle3);
  ASSERT_EQ(obj4, obj7);
  EXPECT_EQ(1, mgr.GetUsedHandleCount());

  mgr.DestroyInstance();
  EXPECT_EQ(0, SampleClass::obj_count);
}

TEST(DynamicHandleManager, Get) {
  using namespace sora;
  EXPECT_EQ(0, SampleClass::obj_count);

  typedef Handle<SampleClass> SampleHandle;
  typedef DynamicHandleManager<SampleClass, SampleHandle> SampleHandleManager;
  SampleHandleManager &mgr = SampleHandleManager::GetInstance();

  // 최초에는 핸들로 아무 객체도 존재하지 않는다
  EXPECT_EQ(0, mgr.GetUsedHandleCount());

  // 새로운 객체를 1개 생성하기
  SampleHandle handle1;
  SampleClass *obj1 = mgr.Acquire(handle1);
  ASSERT_EQ(true, obj1 != NULL);
  EXPECT_EQ(1, mgr.GetUsedHandleCount());
  EXPECT_EQ(0, handle1.res_index());

  SampleClass *obj2 = mgr.Get(handle1);
  ASSERT_EQ(true, obj2 != NULL);
  EXPECT_EQ(obj1, obj2);

  //새로운 객체 1개 더
  SampleHandle handle2;
  SampleClass *obj3 = mgr.Acquire(handle2);
  ASSERT_NE(obj1, obj3);
  ASSERT_EQ(true, NULL != obj3);

  SampleHandle handle3;
  SampleClass *obj4 = mgr.Get(handle3);
  ASSERT_EQ(NULL, obj4);
  EXPECT_EQ(2, mgr.GetUsedHandleCount());

  obj4 = mgr.Acquire(handle3);
  EXPECT_EQ(3, mgr.GetUsedHandleCount());
  
  //하나 지우기. 핸들 관리자는 내부를 벡터로 처리하니 지울때 아마 인덱스가 꼬일거다
  //핸들을 기반으로 다시 찾아낼수 있어야한다
  mgr.Remove(handle2);  //중간에 낀거 삭제
  EXPECT_EQ(2, mgr.GetUsedHandleCount());
  EXPECT_EQ(NULL, mgr.Get(handle2));

  SampleClass *obj6 = mgr.Get(handle3);
  ASSERT_EQ(obj4, obj6);
  EXPECT_EQ(2, mgr.GetUsedHandleCount());

  mgr.Remove(handle1);  //처음꺼 삭제
  ASSERT_EQ(NULL, mgr.Get(handle1));
  ASSERT_EQ(obj4, mgr.Get(handle3));
  EXPECT_EQ(1, mgr.GetUsedHandleCount());
  
  mgr.DestroyInstance();
  EXPECT_EQ(0, SampleClass::obj_count);
}