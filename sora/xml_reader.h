// Ŭnicode please
#pragma once

#include "xml_node.h"

namespace sora {;
class XmlNode;
class XmlReader;
struct XmlReaderError;
class XmlReaderImpl;

class XmlReader {
public:
  XmlReader();
  ~XmlReader();
  XmlNodePtr Read(const char *content);
  XmlNodePtr Read(const std::string &content) { return Read(content.c_str()); }
  bool IsErrorOccur() const;
  XmlReaderError *GetError();
private:
  XmlReaderImpl *impl_;
};

///@brief xml reader가 죽었을때 발생한 에러에 대한 정보를 담는 클래스
struct XmlReaderError {
  XmlReaderError(const char *msg, int row, int col)
    : msg(msg), row(row), col(col) {}
  ~XmlReaderError() {}
  ///@brief error msg를 하나의 문자열로 구성한것
  std::string str() const;

public:
  std::string msg;
  int row;
  int col;
};
}
