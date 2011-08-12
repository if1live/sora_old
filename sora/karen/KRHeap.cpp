// Ŭnicode please 
#include "../SoraStdAfx.h"
#include "karen/KRHeap.h"
#include "karen/KRHeapAllocHeader.h"

using namespace std;

namespace karen
{
	int Heap::nextId_ = 1;
	Heap::Heap()
		: name_(""), allocatedSize_(0), id_(nextId_++)
	{
	}
	Heap::Heap(const std::string &name)
		: name_(name), allocatedSize_(0), id_(nextId_++)
	{
	}
	Heap::~Heap()
	{
	}
	const std::string &Heap::getName()
	{
		return name_;
	}
	int Heap::getId() const
	{
		return id_;
	}
	Heap *Heap::getSharedHeap()
	{
		static Heap h(string("default"));
		return &h;
	}
	void* Heap::malloc(size_t size)
	{
#if DEBUG
		int requestSize = size + sizeof(HeapAllocHeader);
		byte *mem = (byte*)::malloc(requestSize);
		HeapAllocHeader *header = (HeapAllocHeader*)mem;
		header->reset(AllocatedMemorySignature, size, this);
		this->addAllocation(size);

		void *startBlock = mem + sizeof(HeapAllocHeader);
		return startBlock;
#else
		byte *mem = (byte*)::malloc(size);
		return mem;
#endif
	}
	void Heap::free(void *ptr)
	{
#if DEBUG
		HeapAllocHeader *header = (HeapAllocHeader*)((byte*)ptr - sizeof(HeapAllocHeader));
		header->getHeap()->removeAllocation(header->getSize());
		MT_ASSERT(header->getSignature() == AllocatedMemorySignature);
		::free(header);
#else
		::free(ptr);
#endif
	}

	void Heap::addAllocation(size_t size)
	{
		allocatedSize_ += size;
	}
	void Heap::removeAllocation(size_t size)
	{
		allocatedSize_ -= size;
	}
	int Heap::getAllocatedMemSize() const
	{
		return allocatedSize_;
	}
}