#pragma once

#include "konomi/konomi_enum.h"
#include "sora/singleton.h"
#include <string>
#include <vector>

//�޼��� ���� ���Ǵ� ��ũ��. �̰��� �̿��ؼ� �޼����� ����ϸ� 
//�ߺ�Ȯ���� ����
#define KONOMI_MSG_CODE_REGISTER(ENUM_NAME, ENUM_VALUE)	\
	enum {	\
		ENUM_NAME = ENUM_VALUE,	\
	};	\
	static bool ENUM_NAME##_##ENUM_VALUE = konomi::MessageCodeDictionary::getInstance().	\
	RegisterCode(ENUM_VALUE, __FILE__, __LINE__);	\

namespace konomi
{
	class MessageCodeDictionary;
	struct MessageCodeDescription;

	struct MessageCodeDescription {
	public:
		MessageCodeDescription();
		MessageCodeDescription(MessageCode code, const char *file, int line);
		~MessageCodeDescription();

		bool operator==(const MessageCodeDescription &o) const;
		bool operator!=(const MessageCodeDescription &o) const;

  public:
		MessageCode code;
		std::string file;
		int line;
	};
	
	///@brief message code�� �ߺ� ����� ���� ���ؼ� �޼����ڵ��� ���� ���������� ���� 
	///�����Ҽ��ִ� Ŭ����
	class MessageCodeDictionary : public sora::Singleton<MessageCodeDictionary> {
	public:
		MessageCodeDictionary();
		virtual ~MessageCodeDictionary();

		///@brief macro�� ���ؼ� �����ϴ� ����
		bool RegisterCode(MessageCode code, const char *file, int line);
		bool IsExist(MessageCode code) const;
		void Add(const MessageCodeDescription &desc);
		const MessageCodeDescription* Get(MessageCode code) const;
	private:
		std::vector<MessageCodeDescription> code_desc_list_;
	};
}