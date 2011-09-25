// Ŭnicode please 
#pragma once

#include "karen/karen_enum.h"

namespace karen
{
  struct HeapAllocHeader {
  public:
    HeapAllocHeader();
    ~HeapAllocHeader();
    void Reset(int signature, int size, Heap *heap);

  public:
    int signature;
    int size;
    Heap *heap;
  };
}