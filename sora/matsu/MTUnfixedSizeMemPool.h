// Ŭnicode please
#pragma once

#include <new>

namespace matsu
{
	///@brief FixedSizeMemPool과 비슷하지만 그 최대크기가 고정되지 않은 메모리 할당자이다
	///미리 할당한것을 전부 사용한경우 추가 요청을 해서 더 얻는다
	template<typename T, int initCapacity>
	class UnfixedSizeMemPool {
	public:
		UnfixedSizeMemPool();
		virtual ~UnfixedSizeMemPool();

		void *operator new(std::size_t size);
		void operator delete(void *ptr);
	private:
		//할당한 메모리 객체를 밀어넣을 스택
		static std::stack<void*> memStack_;
		//지금까지 할당받은 메모리 블럭을 보관하는 스택. 이것이 있어야 나중에 flush같은짓이라도 할수있다
		static std::vector<void*> allocatedBlockStack_;
	};
}

//template impl
namespace matsu
{
	template<typename T, int initCapacity>
	std::stack<void*> UnfixedSizeMemPool<T,initCapacity>::memStack_;
	
	template<typename T, int initCapacity>
	std::vector<void*> UnfixedSizeMemPool<T,initCapacity>::allocatedBlockStack_;
	
	template<typename T, int initCapacity>
	UnfixedSizeMemPool<T,initCapacity>::UnfixedSizeMemPool()
	{
	}

	template<typename T, int initCapacity>
	UnfixedSizeMemPool<T,initCapacity>::~UnfixedSizeMemPool()
	{
	}

	template<typename T, int initCapacity>
	void *UnfixedSizeMemPool<T,initCapacity>::operator new(std::size_t size)
	{
		if(memStack_.empty() == true)
		{
			//새로운 블럭 획득
			MT_ASSERT(size % 8 == 0 && "for architecture");
			int requestCapacity = initCapacity * pow(2, allocatedBlockStack_.size());
			char *block = (char*)malloc(size * requestCapacity);
			allocatedBlockStack_.push_back(block);

			//메모리 조각을 stack에 넣기
			for(int i = 0 ; i < requestCapacity ;i++)
			{
				char *ptr = block + (i * size);
				memStack_.push(ptr);
			}
		}
		void *result = memStack_.top();
		memStack_.pop();
		return result;
	}

	template<typename T, int initCapacity>
	void UnfixedSizeMemPool<T,initCapacity>::operator delete(void *ptr)
	{
		memStack_.push(ptr);
	}
}