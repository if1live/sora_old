// Ŭnicode please
#include "../SoraTestStdAfx.h"
#include "konomi/KNVariant.h"

using namespace konomi;

typedef enum {
	TestEnum1,
} TestEnum;

TEST(KNVariant, construct)
{
	//int
	Variant v1(1);
	EXPECT_EQ(VariantTypeInt, v1.getType());
	EXPECT_EQ(1, (int)v1);
	
	//float
	Variant v2(2.0f);
	EXPECT_EQ(VariantTypeFloat, v2.getType());
	EXPECT_EQ(2.0f, (float)v2);
	
	//bool
	Variant v3(false);
	EXPECT_EQ(VariantTypeBool, v3.getType());
	EXPECT_EQ(false, (bool)v3);

	//pointer
	int a = 1;
	Variant v4(&a);
	EXPECT_EQ(VariantTypePointer, v4.getType());
	EXPECT_EQ(&a, v4.getAsPointer<int>());

	//enum
	Variant v5(TestEnum1);
	EXPECT_EQ(VariantTypeInt, v5.getType());
	EXPECT_EQ(TestEnum1, v5.getAsEnum<TestEnum>());
	EXPECT_EQ(false, v5.isNull());

	//null
	Variant v6;
	EXPECT_EQ(VariantTypeNull, v6.getType());
	EXPECT_EQ(true, v6.isNull());
}

TEST(KNVariant, set)
{
	Variant v1(1);
	
	v1.set(2.0f);
	EXPECT_EQ(VariantTypeFloat, v1.getType());
	EXPECT_EQ(2.0f, (float)v1);
	
	v1.set(false);
	EXPECT_EQ(VariantTypeBool, v1.getType());
	EXPECT_EQ(false, (bool)v1);
	
	v1.set(3);
	EXPECT_EQ(VariantTypeInt, v1.getType());
	EXPECT_EQ(3, (int)v1);

	int a = 1;
	v1.set(&a);
	EXPECT_EQ(VariantTypePointer, v1.getType());
	EXPECT_EQ(&a, v1.getAsPointer<int>());

	v1.set(TestEnum1);
	EXPECT_EQ(VariantTypeInt, v1.getType());
	EXPECT_EQ(TestEnum1, v1.getAsEnum<TestEnum>());
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