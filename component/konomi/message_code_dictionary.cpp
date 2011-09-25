#include "precompile.h"
#include "konomi/message_code_dictionary.h"

namespace konomi
{
  MessageCodeDescription::MessageCodeDescription()
    : code(0)
  {
  }
  MessageCodeDescription::MessageCodeDescription(MessageCode code, const char *file, int line)
    : code(code),
    file(file),
    line(line)
  {
  }
  MessageCodeDescription::~MessageCodeDescription()
  {
  }

  bool MessageCodeDescription::operator==(const MessageCodeDescription &o) const
  {
    return (code == o.code && file == o.file && line == o.line);
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

  bool MessageCodeDictionary::IsExist(MessageCode code) const
  {
    BOOST_FOREACH(const MessageCodeDescription &desc, code_desc_list_) {
      if (desc.code == code) {
        return true;
      }
    }
    return false;
  }
  void MessageCodeDictionary::Add(const MessageCodeDescription &desc)
  {
    SR_ASSERT(IsExist(desc.code) == false);
    code_desc_list_.push_back(desc);
  }
  const MessageCodeDescription* MessageCodeDictionary::Get(MessageCode code) const
  {
    BOOST_FOREACH(const MessageCodeDescription &desc, code_desc_list_) {
      if (desc.code == code) {
        return &desc;
      }
    }
    return NULL;
  }

  bool MessageCodeDictionary::RegisterCode(MessageCode code, const char *file, int line)
  {
    if(IsExist(code) == false) {
      const MessageCodeDescription desc(code, file, line);
      Add(desc);
      return true;
    } else {
      const MessageCodeDescription *prev = Get(code);
      const MessageCodeDescription desc(code, file, line);
      if(*prev != desc) {
        char msgbuf[512];
        sprintf(msgbuf, "%d is defined in %s::%d", code, prev->file.c_str(), prev->line);
        SR_ASSERT(false && msgbuf);
        return false;
      }
      else
      {
        return true;
      }
    }
  }
}