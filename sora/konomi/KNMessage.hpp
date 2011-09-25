// Ŭnicode please
namespace konomi
{
	template<unsigned int N>
	Message<N>::Message()
		: code_(0), numParam_(0)
	{
	}
	template<unsigned int N>
	Message<N>::Message(MessageCode code)
		: code_(code), numParam_(0)
	{
	}
	
	template<unsigned int N>
	template<unsigned int N2>
	Message<N>::Message(const Message<N2> &o)
		: code_(o.getCode()), numParam_(0)
	{
		SR_ASSERT(o.getNumParam() <= N);
		for(int i = 0 ; i < o.getNumParam() ; i++)
		{
			addParam(o.getParam(i));
		}
		numParam_ = o.getNumParam();
	}

	template<unsigned int N>
	Message<N>::~Message()
	{
	}

	template<unsigned int N>
	MessageCode Message<N>::getCode() const
	{
		return code_;
	}

	template<unsigned int N>
	int Message<N>::getNumParam() const
	{
		return numParam_;
	}

	template<unsigned int N>
	void Message<N>::addParam(const Variant &param)
	{
		SR_ASSERT(numParam_ < MaxVariantNumber);
		paramList_[numParam_] = param;
		numParam_++;
	}

	template<unsigned int N>
	const Variant& Message<N>::getParam(int index) const
	{
		SR_ASSERT(index >= 0 && index < N);	//배열 범위만 체그
		return paramList_[index];	//범위벗어난것은 널객체가 반환될것이다
	}
	template<unsigned int N>
	template<int index>
	const Variant& Message<N>::getParam() const
	{
		BOOST_STATIC_ASSERT(index >= 0 && index < N);
		return getParam(index);
	}

	template<unsigned int N>
	template<unsigned int N2>
	bool Message<N>::operator==(const Message<N2> &o) const
	{
		if(getCode() != o.getCode())
		{
			return false;
		}
		if(getNumParam() != o.getNumParam())
		{
			return false;
		}

		for(int i = 0 ; i < getNumParam() ; i++)
		{
			if(getParam(i) != o.getParam(i))
			{
				return false;
			}
		}
		return true;
	}

	template<unsigned int N>
	template<unsigned int N2>
	bool Message<N>::operator!=(const Message<N2> &o) const
	{
		return !(*this == o);
	}

	template<unsigned int N>
	void Message<N>::clear()
	{
		numParam_ = 0;
		code_ = 0;
	}
	template<unsigned int N>
	void Message<N>::clearParam()
	{
		numParam_ = 0;
	}

	template<unsigned int N>
	bool Message<N>::canAddParam() const
	{
		return (N > numParam_);
	}

	template<unsigned int N>
	template<int num>
	void Message<N>::setNumParam()
	{
		return setNumParam(num);
	}
	
	template<unsigned int N>
	void Message<N>::setNumParam(int num)
	{
		for(int i = 0 ; i < N ; i++)
		{
			Variant &param = paramList_[i];
			if(i < num)
			{
				if(param.isNull())
				{
					//int + 0을 기본값으로 적절히 설정한다
					param.set(static_cast<int>(0));
				}
			}
			else
			{
				param.setNull();
			}
		}
		numParam_ = num;
	}

	template<unsigned int N>
	template<int index>
	void Message<N>::setParam(const Variant &param)
	{
		BOOST_STATIC_ASSERT(index >= 0 && index < N);
		setParam(index, param);
	}
	template<unsigned int N>
	void Message<N>::setParam(int index, const Variant &param)
	{
		SR_ASSERT(index >= 0 && index < numParam_);
		paramList_[index] = param;
	}
}