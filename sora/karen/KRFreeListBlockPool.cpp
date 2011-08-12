// Ŭnicode please
#include "../SoraStdAfx.h"
#include "karen/KRFreeListBlockPool.h"

namespace karen
{
	FreeListBlockPool::FreeListBlockPool(int maxSize, int blockSize)
		: objList_(NULL),
		freeObjList_(NULL),
		maxSize_(maxSize),
		top_(0),
		blockSize_(blockSize)
	{
		reset(maxSize, blockSize);
	}

	FreeListBlockPool::~FreeListBlockPool()
	{
		//release all obj at once
		//MT_ASSERT(maxSize_ == freeListSize_);
		::free(freeObjList_);
		::free(objList_);
		objList_ = NULL;
		freeObjList_ = NULL;
	}

	void FreeListBlockPool::reset(int maxSize, int blockSize)
	{
		if(objList_ != NULL)
		{
			//all previous allocated block must be unused
			MT_ASSERT(maxSize_ == top_+1);
			::free(freeObjList_);
			::free(objList_);
		}
		//reallocate
		maxSize_ = maxSize;

		objList_ = (byte*)::malloc(sizeof(byte) * maxSize * blockSize);
		memset(objList_, 0, sizeof(byte) * maxSize * blockSize);
		freeObjList_ = (byte**)::malloc(sizeof(byte*) * maxSize);
		memset(freeObjList_, 0, sizeof(byte*) * maxSize);

		for(int i = 0 ; i < maxSize_ ; i++)
		{
			byte *ptr = (objList_ + i*blockSize);
			freeObjList_[i] = ptr;
		}
		top_ = maxSize-1;
	}

	void *FreeListBlockPool::malloc()
	{
		MT_ASSERT(top_ >= 0 && "no free block exist");
		byte *ptr = freeObjList_[top_];
		top_--;
		return (void*)ptr;
	}

	void FreeListBlockPool::free(void *ptr)
	{
#if DEBUG
		bool found = false;
		//ptr must be member of objList
		for(int i = 0 ; i < maxSize_ ; i++)
		{
			if(objList_[i] == ptr)
			{
				found = true;
			}
		}
		MT_ASSERT(found == true);
#endif
		top_++;
		freeObjList_[top_] = (byte*)ptr;
	}
}