// Ŭnicode please 
#pragma once

#include <string>
#include "karen/karen_enum.h"

namespace karen
{
  class Heap {
    typedef unsigned char byte;
  public:
    Heap();
    Heap(const std::string &name);
    ~Heap();
    const std::string &GetName();
    int GetId() const;

    void* Malloc(size_t size);
    void Free(void *ptr);

    static Heap *GetSharedHeap();

    void AddAllocation(size_t size);
    void RemoveAllocation(size_t size);
    int GetAllocatedMemSize() const;

    template<typename T>
    static HeapAllocHeader *GetHeader(const T *ptr);
  private:
    int id_;
    std::string name_;
    int allocated_size_;
    static int next_id_;
  };
}

namespace karen
{
  template<typename T>
  HeapAllocHeader *Heap::GetHeader(const T *ptr)
  {
    HeapAllocHeader *header = (HeapAllocHeader*)((byte*)ptr - sizeof(HeapAllocHeader));
    return header;
  }
}