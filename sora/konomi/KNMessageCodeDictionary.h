#pragma once

#include "konomi/KNTypedef.h"
#include "matsu/MTSingleton.h"

//�޼��� ���� ���Ǵ� ��ũ��. �̰��� �̿��ؼ� �޼����� ����ϸ� 
//�ߺ�Ȯ���� ����
#define KONOMI_MSG_CODE_REGISTER(ENUM_NAME, ENUM_VALUE)	\
	enum {	\
		ENUM_NAME = ENUM_VALUE,	\
	};	\
	static bool ENUM_NAME##_##ENUM_VALUE = konomi::MessageCodeDictionary::getInstance().	\
	registerCode(ENUM_VALUE, __FILE__, __LINE__);	\

namespace konomi
{
	class MessageCodeDictionary;
	class MessageCodeDescription;

	class MessageCodeDescription {
	public:
		MessageCodeDescription();
		MessageCodeDescription(MessageCode code, const char *file, int line);
		~MessageCodeDescription();

		MessageCode getCode() const;
		const std::string &getFile() const;
		const int getLine() const;

		bool operator==(const MessageCodeDescription &o) const;
		bool operator!=(const MessageCodeDescription &o) const;
	private:
		MessageCode code_;
		std::string file_;
		int line_;
	};
	
	///@brief message code�� �ߺ� ����� ���� ���ؼ� �޼����ڵ��� ���� ���������� ���� 
	///�����Ҽ��ִ� Ŭ����
	class MessageCodeDictionary : public matsu::Singleton<MessageCodeDictionary> {
	public:
		MessageCodeDictionary();
		virtual ~MessageCodeDictionary();

		///@brief macro�� ���ؼ� �����ϴ� ����
		bool registerCode(MessageCode code, const char *file, int line);
		bool isExist(MessageCode code) const;
		void add(const MessageCodeDescription &desc);
		const MessageCodeDescription* get(MessageCode code) const;
	private:
		std::vector<MessageCodeDescription> codeDescList_;
	};
}