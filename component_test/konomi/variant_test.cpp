// Ŭnicode please
#include "precompile.h"
#include "konomi/variant.h"

using namespace konomi;

typedef enum {
	TestEnum1,
} TestEnum;

TEST(KNVariant, construct)
{
	//int
	Variant v1(1);
	EXPECT_EQ(kVariantTypeInt, v1.get_type());
	EXPECT_EQ(1, (int)v1);
	
	//float
	Variant v2(2.0f);
	EXPECT_EQ(kVariantTypeFloat, v2.get_type());
	EXPECT_EQ(2.0f, (float)v2);
	
	//bool
	Variant v3(false);
	EXPECT_EQ(kVariantTypeBool, v3.get_type());
	EXPECT_EQ(false, (bool)v3);

	//pointer
	int a = 1;
	Variant v4(&a);
	EXPECT_EQ(kVariantTypePointer, v4.get_type());
	EXPECT_EQ(&a, v4.GetAsPointer<int>());

	//enum
	Variant v5(TestEnum1);
	EXPECT_EQ(kVariantTypeInt, v5.get_type());
	EXPECT_EQ(TestEnum1, v5.GetAsEnum<TestEnum>());
	EXPECT_EQ(false, v5.IsNull());

	//null
	Variant v6;
	EXPECT_EQ(kVariantTypeNull, v6.get_type());
	EXPECT_EQ(true, v6.IsNull());
}

TEST(KNVariant, set)
{
	Variant v1(1);
	
	v1 = 2.0f;
	EXPECT_EQ(kVariantTypeFloat, v1.get_type());
	EXPECT_EQ(2.0f, (float)v1);
	
	v1 = false;
	EXPECT_EQ(kVariantTypeBool, v1.get_type());
	EXPECT_EQ(false, (bool)v1);
	
	v1 = 3;
	EXPECT_EQ(kVariantTypeInt, v1.get_type());
	EXPECT_EQ(3, (int)v1);

	int a = 1;
	v1 = &a;
	EXPECT_EQ(kVariantTypePointer, v1.get_type());
	EXPECT_EQ(&a, v1.GetAsPointer<int>());

	v1 = TestEnum1;
	EXPECT_EQ(kVariantTypeInt, v1.get_type());
	EXPECT_EQ(TestEnum1, v1.GetAsEnum<TestEnum>());
}

TEST(KNVariant, op)
{
	int a = 1;
	Variant v1(1);
	Variant v2(1);
	Variant v3(true);
	Variant v4(2);
	EXPECT_EQ(true, v1 == v2);
	EXPECT_EQ(false, v1 == v3);
	EXPECT_EQ(false, v1 == v4);
}