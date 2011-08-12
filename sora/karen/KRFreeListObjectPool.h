// Ŭnicode please 
#pragma once

namespace karen
{
	///@brief reference GPG 4. free list memory manager
	///this class use new/delete to requset/destory memory
	///if constructor call in pool initializion is not important, use this class
	///@param N max pool size
	template<typename T>
	class FreeListObjectPool {
	public:
		FreeListObjectPool(int maxSize);
		~FreeListObjectPool();
		void reset(int maxSize);
		T *malloc();
		void free(T *ptr);
	private:
		T *objList_;
		T **freeObjList_;
		int maxSize_;
		int top_;
	};
}

namespace karen
{
	template<typename T>
	FreeListObjectPool<T>::FreeListObjectPool(int maxSize)
		: objList_(NULL),
		freeObjList_(NULL),
		maxSize_(0),
		top_(0)
	{
		reset(maxSize);
	}
	template<typename T>
	FreeListObjectPool<T>::~FreeListObjectPool()
	{
		//release all obj at once
		//MT_ASSERT(maxSize_ == freeListSize_);
		delete[](objList_);
		delete[](freeObjList_);
		objList_ = NULL;
		freeObjList_ = NULL;
	}

	template<typename T>
	void FreeListObjectPool<T>::reset(int maxSize)
	{
		if(objList_ != NULL)
		{
			//all previous allocated block must be unused
			MT_ASSERT(maxSize_ == top_+1);
			delete[](objList_);
			delete[](freeObjList_);
		}
		//reallocate
		maxSize_ = maxSize;

		objList_ = new T[maxSize];
		freeObjList_ = new T*[maxSize];

		for(int i = 0 ; i < maxSize_ ; i++)
		{
			freeObjList_[i] = &(objList_[i]);
		}
		top_ = maxSize_-1;
	}
	template<typename T>
	T *FreeListObjectPool<T>::malloc()
	{
		MT_ASSERT(top_ >= 0 && "no free block exist");
		T *ptr = freeObjList_[top_];
		top_--;
		return ptr;
	}
	template<typename T>
	void FreeListObjectPool<T>::free(T *ptr)
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
		freeObjList_[top_] = ptr;
	}
}