// Ŭnicode please
#pragma once

#include <new>

///@file 메모리 사용을 효율적으로 하기 위해서 간단하게 만든 할당자 템플릿 클래스
namespace matsu
{
	///@brief 생성할수있는 객체의 최대갯수를 제한하는 할당자이다.
	///메모리를 미리 전부 획득해놓는다. 크기가 작고 잦은 생성을 필요로하며
	//시스템상에 동시에 존재하는 객체의 수가 적을때
	///사용하면 될듯. 이벤트 객체의 임시 정보와 같은 곳에서 쓰자
	///스택기반으로 구현했기 떄문에 new[]와 같이
	///연속적으로 생성하는것은 사실상 불가능하다(굳이 만든다고 하더라도
	///요청시 성공확률보다 실패확률이 높다
	template<typename T, int maxSize>
	class FixedSizeMemPool {
	public:
		FixedSizeMemPool();
		virtual ~FixedSizeMemPool();
		
		void* operator new(std::size_t size);
		void operator delete(void *ptr);
	private:
		//할당한 메모리 객체를 밀어넣을 스택
		static std::stack<void*> memStack_;
	};
}

//template impl
namespace matsu
{
	template<typename T, int maxSize>
	std::stack<void*> FixedSizeMemPool<T,maxSize>::memStack_;

	template<typename T, int maxSize>
	FixedSizeMemPool<T,maxSize>::FixedSizeMemPool()
	{
	}
	template<typename T, int maxSize>
	FixedSizeMemPool<T,maxSize>::~FixedSizeMemPool()
	{
	}
	
	template<typename T, int maxSize>
	void* FixedSizeMemPool<T,maxSize>::operator new(std::size_t size)
	{
		static char *pool = NULL;
		if(pool == NULL) 
		{
			MT_ASSERT(size % 8 == 0);
			pool = (char*)malloc(size * maxSize);
			for(int i = 0 ; i < maxSize ; i++) 
			{
				char *ptr = pool + (size * i);
				memStack_.push(ptr);
			}
		}
		MT_ASSERT(memStack_.empty() == false);
		void *result = memStack_.top();
		memStack_.pop();
		return result;
	}
	
	template<typename T, int maxSize>
	void FixedSizeMemPool<T,maxSize>::operator delete(void *ptr)
	{
		memStack_.push(ptr);		
	}
}