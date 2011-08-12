// Ŭnicode please 
#pragma once

namespace karen
{
	///@brief reference GPG 4. free list memory manager
	///this class use malloc/free to requset/destory memory
	///if you want to constructor not called, use this class
	///for example, class operator new/delete overloading
	///@param N max pool size
	template<int blockSize>
	class FreeListBlockPool {
		typedef unsigned char byte;
		enum {
			BlockSize = blockSize,
		};
	public:
		FreeListBlockPool(int maxSize);
		~FreeListBlockPool();
		void reset(int maxSize);
		void* malloc();
		void free(void *ptr);
	private:
		byte *objList_;
		byte **freeObjList_;
		int maxSize_;
		int freeListSize_;
	};
}

namespace karen
{
	template<int blockSize>
	FreeListBlockPool<blockSize>::FreeListBlockPool(int maxSize)
		: objList_(NULL),
		freeObjList_(NULL),
		maxSize_(0),
		freeListSize_(0)
	{
		reset(maxSize);
	}

	template<int blockSize>
	FreeListBlockPool<blockSize>::~FreeListBlockPool()
	{
		//release all obj at once
		//MT_ASSERT(maxSize_ == freeListSize_);
		::free(freeObjList_);
		::free(objList_);
		objList_ = NULL;
		freeObjList_ = NULL;
	}

	template<int blockSize>
	void FreeListBlockPool<blockSize>::reset(int maxSize)
	{
		if(objList_ != NULL)
		{
			//all previous allocated block must be unused
			MT_ASSERT(maxSize_ == freeListSize_);
			::free(freeObjList_);
			::free(objList_);
		}
		//reallocate
		maxSize_ = maxSize;
		freeListSize_ = maxSize;

		objList_ = (byte*)::malloc(sizeof(byte) * maxSize * blockSize);
		memset(objList_, 0, sizeof(byte) * maxSize * blockSize);
		freeObjList_ = (byte**)::malloc(sizeof(byte*) * maxSize);

		for(int i = 0 ; i < maxSize_ ; i++)
		{
			freeObjList_[i] = &(objList_[i]);
		}
	}

	template<int blockSize>
	void *FreeListBlockPool<blockSize>::malloc()
	{
		MT_ASSERT(freeListSize_ >= 1 && "no free block exist");
		byte *ptr = freeObjList_[freeListSize_];
		freeListSize_--;
		return (void*)ptr;
	}

	template<int blockSize>
	void FreeListBlockPool<blockSize>::free(void *ptr)
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
		freeListSize_++;
		freeObjList_[freeListSize_] = (byte*)ptr;
	}
}