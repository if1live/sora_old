// Ŭnicode please
#pragma once

#include "matsu/MTMacro.h"

namespace matsu
{
	template<typename T>
	class SharedObject {
	public:
		static T& getInstance()
		{
			if(ctx_ == NULL)
			{
				ctx_ = new T;
			}
			return *ctx_;
		}
		static void destoryInstance()
		{
			if(ctx_ != NULL)
			{
				delete(ctx_)
				ctx_ = NULL;
			}
		}
		static bool isCreated()
		{
			return (ctx_ != NULL);
		}
		SharedObject() {}
		virtual ~SharedObject() {}

	private:
		static T *ctx_;
	};

	template<typename T>
	T *SharedObject<T>::ctx_ = NULL;
}