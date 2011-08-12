// Ŭnicode please 
#include "../SoraTestStdAfx.h"
#include "karen/KRHeap.h"
#include "karen/KRHeapAllocHeader.h"

using namespace karen;
using namespace std;

#if DEBUG
TEST(KRHeap, test)
{
	Heap heap(string("sample"));
	EXPECT_EQ(0, heap.getAllocatedMemSize());

	void *ptr = heap.malloc(10);
	EXPECT_EQ(10, heap.getAllocatedMemSize());

	void *ptr1 = heap.malloc(20);
	EXPECT_EQ(30, heap.getAllocatedMemSize());

	heap.free(ptr);
	EXPECT_EQ(20, heap.getAllocatedMemSize());

	heap.free(ptr1);
	EXPECT_EQ(0, heap.getAllocatedMemSize());
}

TEST(KRHeap, getHeader)
{
	Heap heap(string("sample"));
	EXPECT_EQ(0, heap.getAllocatedMemSize());

	void *ptr = heap.malloc(10);
	HeapAllocHeader *header = Heap::getHeader(ptr);
	EXPECT_EQ(AllocatedMemorySignature, header->getSignature());
	EXPECT_EQ(10, header->getSize());
	EXPECT_EQ(&heap, header->getHeap());
	heap.free(ptr);
}
#endif