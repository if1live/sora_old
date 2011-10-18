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
#include "precompile.h"
#include "sora/class_type.h"

class TypeTest {
  SR_SUPER_CLASS();
};
class TypeTest1 : public TypeTest {
  SR_CHILD_CLASS("type_test_1");
};
class TypeTest2 : public TypeTest {
  SR_CHILD_CLASS("type_test_2");
};

TEST(ClassType, test) {
  using std::string;

  TypeTest1 obj1;
  EXPECT_EQ(true, obj1.GetClassType() == TypeTest1::ClassType());
  EXPECT_EQ(true, obj1.GetClassName() == "type_test_1");
  EXPECT_EQ(true, obj1.GetClassName() == TypeTest1::ClassName());

  TypeTest2 obj2;
  const string &name1 = obj1.GetClassName();
  const string &name2 = obj2.GetClassName();
  int code1 = obj1.GetClassType();
  int code2 = obj2.GetClassType();
  EXPECT_EQ(true, name1 != name2);
  EXPECT_EQ(true, code1 != code2);

  const string &name3 = TypeTest1::ClassTypeToName(TypeTest2::ClassType());
  EXPECT_EQ(true, obj2.GetClassName() == name3);
  EXPECT_EQ(true, obj2.GetClassType() == TypeTest1::ClassNameToType(TypeTest2::ClassName()));
}

TEST(ClassType2, test) {
}