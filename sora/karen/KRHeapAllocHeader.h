// Ŭnicode please 
#pragma once

#include "karen/KRTypedef.h"
namespace karen
{
	class HeapAllocHeader {
	public:
		HeapAllocHeader();
		~HeapAllocHeader();
		void reset(int signature, int size, Heap *heap);
		int getSignature() const;
		int getSize() const;
		Heap *getHeap() const;
	private:
		int signature_;
		int size_;
		Heap *heap_;
	};
}