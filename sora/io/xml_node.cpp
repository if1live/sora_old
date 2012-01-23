/*  Copyright (C) 2011 by if1live */
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// Ŭnicode please
#include "sora_stdafx.h"
#include "xml_node.h"
#include "sora/template_library.h"

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
  if (index >= 0 && (int)children_.size() > index) {
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
