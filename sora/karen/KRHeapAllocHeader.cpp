// Ŭnicode please 
#include "../SoraStdAfx.h"
#include "karen/KRHeapAllocHeader.h"

namespace karen
{
	HeapAllocHeader::HeapAllocHeader()
		: signature_(0),
		size_(0),
		heap_(NULL)
	{
	}
	HeapAllocHeader::~HeapAllocHeader()
	{
	}
	void HeapAllocHeader::reset(int signature, int size, Heap *heap)
	{
		signature_ = signature;
		size_ = size;
		heap_ = heap;
	}
	int HeapAllocHeader::getSignature() const
	{
		return signature_;
	}
	int HeapAllocHeader::getSize() const
	{
		return size_;
	}
	Heap *HeapAllocHeader::getHeap() const
	{
		return heap_;
	}
}