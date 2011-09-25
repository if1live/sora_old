// Ŭnicode please
#include <cstdlib>
#include <cstring>
#include "karen/free_list_block_pool.h"
#include "sora/macro.h"

namespace karen
{
  FreeListBlockPool::FreeListBlockPool(int max_size, int block_size)
    : object_list_(NULL),
    free_object_list_(NULL),
    max_size_(max_size),
    top_(0),
    block_size_(block_size)
  {
    Reset(max_size, block_size);
  }

  FreeListBlockPool::~FreeListBlockPool()
  {
    //release all obj at once
    //SR_ASSERT(max_size_ == freeListSize_);
    ::free(free_object_list_);
    ::free(object_list_);
    object_list_ = NULL;
    free_object_list_ = NULL;
  }

  void FreeListBlockPool::Reset(int max_size, int block_size)
  {
    if(object_list_ != NULL) {
      //all previous allocated block must be unused
      SR_ASSERT(max_size_ == top_+1);
      ::free(free_object_list_);
      ::free(object_list_);
    }
    //reallocate
    max_size_ = max_size;

    object_list_ = (byte*)::malloc(sizeof(byte) * max_size * block_size);
    memset(object_list_, 0, sizeof(byte) * max_size * block_size);
    free_object_list_ = (byte**)::malloc(sizeof(byte*) * max_size);
    memset(free_object_list_, 0, sizeof(byte*) * max_size);

    for(int i = 0 ; i < max_size_ ; i++) {
      byte *ptr = (object_list_ + i*block_size);
      free_object_list_[i] = ptr;
    }
    top_ = max_size-1;
  }

  void *FreeListBlockPool::Malloc()
  {
    SR_ASSERT(top_ >= 0 && "no free block exist");
    byte *ptr = free_object_list_[top_];
    top_--;
    return (void*)ptr;
  }

  void FreeListBlockPool::Free(void *ptr)
  {
#if DEBUG
    bool found = false;
    //ptr must be member of object_list
    for(int i = 0 ; i < max_size_ ; i++) {
      if((object_list_ + i*block_size_) == ptr) {
        found = true;
      }
    }
    SR_ASSERT(found == true);
#endif
    top_++;
    free_object_list_[top_] = (byte*)ptr;
  }
}