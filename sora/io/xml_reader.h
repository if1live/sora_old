// Ŭnicode please
#ifndef SORA_XML_READER_H_
#define SORA_XML_READER_H_

// tinyxml wrapper 수준으로 간단하게 xml 파일을 읽어서
// 내용을 볼수 있도록 만든 라이브러리이다.
// 아주 간단한 구조를 사용해서
// 메모리 관리같은것을 전혀 신경쓰지 않고 사용할수 있도록하자
namespace sora {;
class XmlNode;
class XmlReader;
struct XmlReaderError;
class XmlReaderImpl;

class XmlReader {
public:
  XmlReader();
  ~XmlReader();
  // 어떤 노드에 내용을 채울지를 밖에서 결정하도록 하여
  // 메모리 관리 시점이나 typedef를 알기 위해서 include 떡칠하는 문제를 벗어나도록함
  bool Read(XmlNode *root, const char *content);
  bool Read(XmlNode *root, const unsigned char *content) {
    return Read(root, (const char *)content);
  }
  bool Read(XmlNode *root, const std::string &content) {
    return Read(root, content.c_str());
  }
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

#endif  // SORA_XML_READER_H_