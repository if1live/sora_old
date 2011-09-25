#pragma once

#include "konomi/KNTypedef.h"
#include "sora/SRSingleton.h"

//메세지 선언에 사용되는 매크로. 이것을 이요해서 메세지를 사용하면 
//중복확인을 지원
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
	
	///@brief message code의 중복 등록을 막기 위해서 메세지코드의 값과 기초정보를 같이 
	///보관할수있는 클래스
	class MessageCodeDictionary : public sora::Singleton<MessageCodeDictionary> {
	public:
		MessageCodeDictionary();
		virtual ~MessageCodeDictionary();

		///@brief macro에 의해서 진입하는 지점
		bool registerCode(MessageCode code, const char *file, int line);
		bool isExist(MessageCode code) const;
		void add(const MessageCodeDescription &desc);
		const MessageCodeDescription* get(MessageCode code) const;
	private:
		std::vector<MessageCodeDescription> codeDescList_;
	};
}