
//template impl
namespace konomi
{
	template<unsigned int N>
	MessageDecorator<N>::MessageDecorator(MessageType *msg)
		: msg_(msg)
	{
		//decorator�� ���ļ� ����� ������ ���� �����ֵ��Ϲٲ���
		//�̷����ص� ��ɻ��̳� ���������� ū ������ �������ϴ� 
		msg_->SetNumParam(N);
	}
	template<unsigned int N>
	MessageDecorator<N>::MessageDecorator(MessageType &msg)
		: msg_(&msg)
	{
		//decorator�� ���ļ� ����� ������ ���� �����ֵ��Ϲٲ���
		//�̷����ص� ��ɻ��̳� ���������� ū ������ �������ϴ� 
		msg_->SetNumParam(N);
	}
	template<unsigned int N>
	MessageDecorator<N>::~MessageDecorator()
	{
	}

	template<unsigned int N>
	void MessageDecorator<N>::SetMessage(MessageType *msg)
	{
		msg_ = msg;
	}
	template<unsigned int N>
	void MessageDecorator<N>::SetMessage(MessageType &msg)
	{
		msg_ = &msg;
	}
	template<unsigned int N>
	template<typename T, int index>
	T MessageDecorator<N>::GetPrimitiveParam() const
	{
		BOOST_STATIC_ASSERT(index >= 0 && index < N);
		const Variant &v = msg_->GetParam<index>();
		return (T)(v);
	}

	template<unsigned int N>
	template<typename T, int index>
	void MessageDecorator<N>::SetPrimitiveParam(T value)
	{
		BOOST_STATIC_ASSERT(index >= 0 && index < N);
		msg_->SetParam<index>(Variant(value));
	}

	template<unsigned int N>
	template<typename T>
	void MessageDecorator<N>::SetPrimitiveParam(int index, T value)
	{
		msg_->SetParam(index, Variant(value));
	}

	///////////////////////
	template<unsigned int N, MessageCode code>
	CodeCheckMessageDecorator<N,code>::CodeCheckMessageDecorator(MessageType *msg)
		: SuperDecoratorType(msg)
	{
		SR_ASSERT(msg->get_code() == code);
	}

	template<unsigned int N, MessageCode code>
	CodeCheckMessageDecorator<N,code>::CodeCheckMessageDecorator(MessageType &msg)
		: SuperDecoratorType(msg)
	{
		SR_ASSERT(msg.get_code() == code);
	}
	
	template<unsigned int N, MessageCode code>
	CodeCheckMessageDecorator<N,code>::~CodeCheckMessageDecorator()
	{
	}
}