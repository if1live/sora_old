// Ŭnicode please
#pragma once

#include "matsu/MTMacro.h"

namespace matsu
{
	template<typename T>
	class Singleton {
	public:
		static T& getInstance()
		{
			if(ctx_ == NULL)
			{
				ctx_ = new T;
			}
			return *ctx_;
		}

		static void destroyInstance()
		{
			if(ctx_ != NULL)
			{
				delete(ctx_);
				ctx_ = NULL;
			}
		}
		static bool isCreated()
		{
			if(ctx_ == NULL)
			{
				return false;
			}
			else
			{
				return true;
			}

		}
	protected:
		Singleton() 
		{
			MT_ASSERT(ctx_ == NULL);
			long offset = (long)(T*)1 - (long)(Singleton*)(T*)1;
			ctx_ = (T*)(long)(this + offset);
		}
		virtual ~Singleton()
		{
			MT_ASSERT(ctx_ != NULL);
			ctx_ = NULL;
		}
		Singleton(const Singleton &o);
		Singleton operator=(const Singleton &o);
		
		static T *ctx_;
	};

	template<typename T>
		T* Singleton<T>::ctx_ = NULL;
}
