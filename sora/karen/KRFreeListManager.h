// Ŭnicode please 
#pragma once

#include "karen/KRFreeListBlockPool.h"

namespace karen
{
	template<typename T,int N>
	class FreeListManager {
	public:
		void *operator new(size_t x);
		void operator delete(void *ptr);
	private:
		static FreeListBlockPool pool_;
	};
}

namespace karen
{
	template<typename T,int N>
	void *FreeListManager<T,N>::operator new(size_t x)
	{
		void *ptr = pool_.malloc();
		return ptr;
	}

	template<typename T,int N>
	void FreeListManager<T,N>::operator delete(void *ptr)
	{
		pool_.free(ptr);
	}
	
	template<typename T,int N>
	FreeListBlockPool FreeListManager<T,N>::pool_(N, sizeof(T));
}