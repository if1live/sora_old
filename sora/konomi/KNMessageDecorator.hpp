
//template impl
namespace konomi
{
	template<unsigned int N>
	MessageDecorator<N>::MessageDecorator(MessageType *msg)
		: msg_(msg)
	{
		//decorator를 거쳐서 사용할 물건은 전부 쓸수있도록바꾸자
		//이렇게해도 기능상이나 안전성에는 큰 문제가 없을듯하다 
		msg_->setNumParam(N);
	}
	template<unsigned int N>
	MessageDecorator<N>::MessageDecorator(MessageType &msg)
		: msg_(&msg)
	{
		//decorator를 거쳐서 사용할 물건은 전부 쓸수있도록바꾸자
		//이렇게해도 기능상이나 안전성에는 큰 문제가 없을듯하다 
		msg_->setNumParam(N);
	}
	template<unsigned int N>
	MessageDecorator<N>::~MessageDecorator()
	{
	}

	template<unsigned int N>
	void MessageDecorator<N>::setMessage(MessageType *msg)
	{
		msg_ = msg;
	}
	template<unsigned int N>
	void MessageDecorator<N>::setMessage(MessageType &msg)
	{
		msg_ = &msg;
	}
	template<unsigned int N>
	template<typename T, int index>
	T MessageDecorator<N>::getPrimitiveParam() const
	{
		BOOST_STATIC_ASSERT(index >= 0 && index < N);
		const Variant &v = msg_->getParam<index>();
		return (T)(v);
	}

	template<unsigned int N>
	template<typename T, int index>
	void MessageDecorator<N>::setPrimitiveParam(T value)
	{
		BOOST_STATIC_ASSERT(index >= 0 && index < N);
		msg_->setParam<index>(Variant(value));
	}

	template<unsigned int N>
	template<typename T>
	void MessageDecorator<N>::setPrimitiveParma(int index, T value)
	{
		msg_->setParam(index, Variant(value));
	}

	///////////////////////
	template<unsigned int N, MessageCode code>
	CodeCheckMessageDecorator<N,code>::CodeCheckMessageDecorator(MessageType *msg)
		: SuperDecoratorType(msg)
	{
		MT_ASSERT(msg->getCode() == code);
	}

	template<unsigned int N, MessageCode code>
	CodeCheckMessageDecorator<N,code>::CodeCheckMessageDecorator(MessageType &msg)
		: SuperDecoratorType(msg)
	{
		MT_ASSERT(msg.getCode() == code);
	}
	
	template<unsigned int N, MessageCode code>
	CodeCheckMessageDecorator<N,code>::~CodeCheckMessageDecorator()
	{
	}
}