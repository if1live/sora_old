// Ŭnicode please 
#pragma once
#include "karen/KRHeap.h"

namespace karen
{
	template<typename T>
	class ClassHeap {
	public:
		ClassHeap();
		virtual ~ClassHeap();

		void* operator new(size_t x);
		void operator delete(void *ptr);

	private:
		static Heap heap_;
	};
}

namespace karen
{
	template<typename T>
	ClassHeap<T>::ClassHeap()
	{
	}

	template<typename T>
	ClassHeap<T>::~ClassHeap()
	{
	}

	template<typename T>
	void* ClassHeap<T>::operator new(size_t x)
	{
		void *ptr = heap_.malloc(x);
		return x;
	}

	template<typename T>
	void ClassHeap<T>::operator delete(void *ptr)
	{
		heap_.free(ptr);
	}

	template<typename T>
	Heap ClassHeap<T>::heap_;
}
