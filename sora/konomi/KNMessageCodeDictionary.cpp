#include "../SoraStdAfx.h"
#include "konomi/KNMessageCodeDictionary.h"

namespace konomi
{
	MessageCodeDescription::MessageCodeDescription()
		: code_(0)
	{
	}
	MessageCodeDescription::MessageCodeDescription(MessageCode code, const char *file, int line)
		: code_(code),
		file_(file),
		line_(line)
	{
	}
	MessageCodeDescription::~MessageCodeDescription()
	{
	}

	MessageCode MessageCodeDescription::getCode() const
	{
		return code_;
	}
	const std::string &MessageCodeDescription::getFile() const
	{
		return file_;
	}
	const int MessageCodeDescription::getLine() const
	{
		return line_;
	}

	bool MessageCodeDescription::operator==(const MessageCodeDescription &o) const
	{
		return (code_ == o.code_ && file_ == o.file_ && line_ == o.line_);
	}
	bool MessageCodeDescription::operator!=(const MessageCodeDescription &o) const
	{
		return !(*this == o);
	}


	MessageCodeDictionary::MessageCodeDictionary()
	{
	}
	MessageCodeDictionary::~MessageCodeDictionary()
	{
	}

	bool MessageCodeDictionary::isExist(MessageCode code) const
	{
		BOOST_FOREACH(const MessageCodeDescription &desc, codeDescList_)
		{
			if(desc.getCode() == code)
			{
				return true;
			}
		}
		return false;
	}
	void MessageCodeDictionary::add(const MessageCodeDescription &desc)
	{
		MT_ASSERT(isExist(desc.getCode()) == false);
		codeDescList_.push_back(desc);
	}
	const MessageCodeDescription* MessageCodeDictionary::get(MessageCode code) const
	{
		BOOST_FOREACH(const MessageCodeDescription &desc, codeDescList_)
		{
			if(desc.getCode() == code)
			{
				return &desc;
			}
		}
		return NULL;
	}

	bool MessageCodeDictionary::registerCode(MessageCode code, const char *file, int line)
	{
		if(isExist(code) == false)
		{
			const MessageCodeDescription desc(code, file, line);
			add(desc);
			return true;
		}
		else
		{
			const MessageCodeDescription *prev = get(code);
			const MessageCodeDescription desc(code, file, line);
			if(*prev != desc)
			{
				char msgbuf[512];
				sprintf(msgbuf, "%d is defined in %s::%d", code, prev->getFile().c_str(), prev->getLine());
				MT_ASSERT(false && msgbuf);
				return false;
			}
			else
			{
				return true;
			}
		}
	}
}