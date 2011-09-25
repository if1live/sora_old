// Ŭnicode please 
#include "karen/heap_alloc_header.h"
#include <cstdlib>

namespace karen
{
  HeapAllocHeader::HeapAllocHeader()
    : signature(0),
    size(0),
    heap(NULL)
  {
  }
  HeapAllocHeader::~HeapAllocHeader()
  {
  }
  void HeapAllocHeader::Reset(int signature, int size, Heap *heap)
  {
    this->signature = signature;
    this->size = size;
    this->heap = heap;
  }
}