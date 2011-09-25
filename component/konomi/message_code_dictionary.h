#pragma once

#include "konomi/konomi_enum.h"
#include "sora/singleton.h"
#include <string>
#include <vector>

//메세지 선언에 사용되는 매크로. 이것을 이요해서 메세지를 사용하면 
//중복확인을 지원
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
	
	///@brief message code의 중복 등록을 막기 위해서 메세지코드의 값과 기초정보를 같이 
	///보관할수있는 클래스
	class MessageCodeDictionary : public sora::Singleton<MessageCodeDictionary> {
	public:
		MessageCodeDictionary();
		virtual ~MessageCodeDictionary();

		///@brief macro에 의해서 진입하는 지점
		bool RegisterCode(MessageCode code, const char *file, int line);
		bool IsExist(MessageCode code) const;
		void Add(const MessageCodeDescription &desc);
		const MessageCodeDescription* Get(MessageCode code) const;
	private:
		std::vector<MessageCodeDescription> code_desc_list_;
	};
}