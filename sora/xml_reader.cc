// Ŭnicode please
#include "sora_stdafx.h"
#include "xml_reader.h"
#include "xml_node.h"
#include <iostream>

using namespace std;

namespace sora {;

// tinyxml based implementation
class XmlReaderImpl {
public:
  XmlReaderImpl() {}
  ~XmlReaderImpl() {}
  XmlNodePtr Read(const char *content);
  bool IsErrorOccur() const;
  XmlReaderError *GetError();
private:
  XmlNode *Parse(TiXmlNode *node);
  std::auto_ptr<XmlReaderError> error_;
};

bool XmlReaderImpl::IsErrorOccur() const {
  return (error_.get() != NULL);
}
XmlReaderError *XmlReaderImpl::GetError() {
  return error_.get();
}
XmlNodePtr XmlReaderImpl::Read(const char *content) {
  //error초기화
  error_.reset();

  TiXmlDocument doc;
  doc.Parse(content);

  if(doc.Error()) {
    //SR_ASSERT(!"Xml Error");
    //xml문법 에러로 프로그램이 그냥 죽어버리면 여러모로 골치아프다. 
    //그렇다고 예외를 던지면 받기가 미묘하다
    //널 노드를 반환하고 이후 적절히 받은곳에서 처리하도록하자

    const char *errorMsg = doc.ErrorDesc();
    int row = doc.ErrorRow();
    int col = doc.ErrorCol();
    error_ = auto_ptr<XmlReaderError>(new XmlReaderError(errorMsg, row, col));
    XmlNodePtr &null = NullXmlNode::null();
    return null;
  }

  //root + construct custom xml tree
  TiXmlNode *root = doc.RootElement();
  SR_ASSERT(root);

  XmlNode *result = this->Parse(root);
  return XmlNodePtr(result);
}
XmlNode *XmlReaderImpl::Parse(TiXmlNode *node) {
  if(node->Type() != TiXmlNode::TINYXML_ELEMENT) {
    SR_ASSERT(!"Not valid recursive call");
  }

  //casting
  TiXmlElement *elem = static_cast<TiXmlElement*>(node);
  string name(elem->Value());
  XmlNode *result = new XmlNode(name);

  //attribute
  TiXmlAttribute *attr = elem->FirstAttribute();
  for( ; attr != NULL ; attr = attr->Next()) {
    string key(attr->Name());
    string value(attr->Value());
    result->setAttribute(key, value);
  }

  //get child
  TiXmlNode *child = elem->FirstChildElement();
  for( ; child != NULL ; child = child->NextSibling()) {
    if(child->Type() == TiXmlNode::TINYXML_ELEMENT) {
      //일반 노드인 경우
      XmlNode *childnode = Parse(child);
      result->addChild(childnode);
    }

  }

  //content
  child = elem->FirstChild();
  for( ; child != NULL ; child = child->NextSibling()) {
    if(child->Type() == TiXmlNode::TINYXML_TEXT) {
      //text content인 경우 뺴내기
      string content(child->Value());
      result->setContent(content.c_str());
      break;	//text content는 1개 존재햐야한다
    }
  }
  return result;
}

/////////////////////////////////////////////
XmlReader::XmlReader()
: impl_(NULL) {
  impl_ = new XmlReaderImpl();
}
XmlReader::~XmlReader() {
  if (impl_ != NULL) {
    delete(impl_);
    impl_ = NULL;
  }
}
XmlNodePtr XmlReader::Read(const char *content) {
  return impl_->Read(content);
}
bool XmlReader::IsErrorOccur() const {
  return impl_->IsErrorOccur();
}
XmlReaderError *XmlReader::GetError() {
  return impl_->GetError();
}


//////////////////////////////////////////////

std::string XmlReaderError::str() const {
  ostringstream oss;
  oss << "[row=" << row << "/col=" << col << "]";
  oss << "Msg:" << msg;
  return oss.str();
}
}

