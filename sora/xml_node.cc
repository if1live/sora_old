// Ŭnicode please
#include "sora_stdafx.h"
#include "xml_node.h"
#include "template_library.h"

namespace sora {;
XmlNode::XmlNode()
  : name_("ThisIsEmpty"), parent_(NULL) {
}
XmlNode::XmlNode(const char *name)
  : name_(name), parent_(NULL) {
}
XmlNode::XmlNode(const std::string &name)
  : name_(name), parent_(NULL) {
  SR_ASSERT(name.length() > 0 && "node must have name");
}
XmlNode::~XmlNode() {
  //자식 파기
  DestroyList(&children_); 
}

void XmlNode::SetAttribute(const std::string &key, const std::string &value) {
  SetAttribute(key.c_str(), value.c_str()); 
}
void XmlNode::SetAttribute(const char *key, const char *value) {
  attribute_[key] = value;
}
const std::string &XmlNode::GetAttribute(const char *key) const {
  XmlAttributeDictType::const_iterator it = attribute_.find(key);
  if(it == attribute_.end()) {
    static std::string emptyStr("");
    return emptyStr;
  } else {
    return it->second;
  }
}
bool XmlNode::HasAttribute(const char *key) const {
  XmlAttributeDictType::const_iterator it = attribute_.find(key);
  if(it == attribute_.end()) {
    return false;
  } else {
    return true;
  }
}

//child method
void XmlNode::AddChild(XmlNode *child) {
  child->set_parent(this);
  children_.push_back(child);
}

XmlNode* XmlNode::Child(int index) const {
  if (index >= 0 && (int)children_.size() < index) {
    return children_[index];
  } else {
    return NULL;
  }
}
XmlNode *XmlNode::FirstChild() const { 
  return this->Child(0); 
}
XmlNode *XmlNode::LastChild() const {
  return this->Child(ChildCount()-1); 
}

}
