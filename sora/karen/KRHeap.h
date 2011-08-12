// Ŭnicode please 
#pragma once

#include "karen/KRTypedef.h"

namespace karen
{
	class Heap {
		typedef unsigned char byte;
	public:
		Heap();
		Heap(const std::string &name);
		~Heap();
		const std::string &getName();
		int getId() const;

		void* malloc(size_t size);
		void free(void *ptr);

		static Heap *getSharedHeap();

		void addAllocation(size_t size);
		void removeAllocation(size_t size);
		int getAllocatedMemSize() const;

		template<typename T>
		static HeapAllocHeader *getHeader(const T *ptr);
	private:
		int id_;
		std::string name_;
		int allocatedSize_;
		static int nextId_;
	};
}

namespace karen
{
	template<typename T>
	HeapAllocHeader *Heap::getHeader(const T *ptr)
	{
		HeapAllocHeader *header = (HeapAllocHeader*)((byte*)ptr - sizeof(HeapAllocHeader));
		return header;
	}
}