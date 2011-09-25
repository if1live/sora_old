// Ŭnicode please 
#pragma once

namespace karen
{
  ///@brief reference GPG 4. free list memory manager
  ///this class use malloc/free to requset/destory memory
  ///if you want to constructor not called, use this class
  ///for example, class operator new/delete overloading
  class FreeListBlockPool {
  private:
    typedef unsigned char byte;
  public:
    FreeListBlockPool(int max_size, int block_size);
    ~FreeListBlockPool();
    void Reset(int max_size, int block_size);
    void* Malloc();
    void Free(void *ptr);
  private:
    byte *object_list_;
    byte **free_object_list_;
    int max_size_;
    int block_size_;
    int top_;
  };
}