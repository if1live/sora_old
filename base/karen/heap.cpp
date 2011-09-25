// Ŭnicode please 
#include "karen/heap.h"
#include "karen/heap_alloc_header.h"
#include "sora/macro.h"

namespace karen
{
  int Heap::next_id_ = 1;
  Heap::Heap()
    : name_(""), allocated_size_(0), id_(next_id_++)
  {
  }
  Heap::Heap(const std::string &name)
    : name_(name), allocated_size_(0), id_(next_id_++)
  {
  }
  Heap::~Heap()
  {
  }
  const std::string &Heap::GetName()
  {
    return name_;
  }
  int Heap::GetId() const
  {
    return id_;
  }
  Heap *Heap::GetSharedHeap()
  {
    using namespace std;
    static Heap h(string("default"));
    return &h;
  }
  void* Heap::Malloc(size_t size)
  {
#if DEBUG
    int requestSize = size + sizeof(HeapAllocHeader);
    byte *mem = (byte*)::malloc(requestSize);
    HeapAllocHeader *header = (HeapAllocHeader*)mem;
    header->Reset(kAllocatedMemorySignature, size, this);
    this->AddAllocation(size);

    void *startBlock = mem + sizeof(HeapAllocHeader);
    return startBlock;
#else
    byte *mem = (byte*)::malloc(size);
    return mem;
#endif
  }
  void Heap::Free(void *ptr)
  {
#if DEBUG
    HeapAllocHeader *header = (HeapAllocHeader*)((byte*)ptr - sizeof(HeapAllocHeader));
    header->heap->RemoveAllocation(header->size);
    SR_ASSERT(header->signature == kAllocatedMemorySignature);
    ::free(header);
#else
    ::free(ptr);
#endif
  }

  void Heap::AddAllocation(size_t size)
  {
    allocated_size_ += size;
  }
  void Heap::RemoveAllocation(size_t size)
  {
    allocated_size_ -= size;
  }
  int Heap::GetAllocatedMemSize() const
  {
    return allocated_size_;
  }
}