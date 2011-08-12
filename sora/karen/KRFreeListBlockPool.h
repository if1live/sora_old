// Ŭnicode please 
#pragma once

namespace karen
{
	///@brief reference GPG 4. free list memory manager
	///this class use malloc/free to requset/destory memory
	///if you want to constructor not called, use this class
	///for example, class operator new/delete overloading
	class FreeListBlockPool {
		typedef unsigned char byte;
	public:
		FreeListBlockPool(int maxSize, int blockSize);
		~FreeListBlockPool();
		void reset(int maxSize, int blockSize);
		void* malloc();
		void free(void *ptr);
	private:
		byte *objList_;
		byte **freeObjList_;
		int maxSize_;
		int blockSize_;
		int top_;
	};
}