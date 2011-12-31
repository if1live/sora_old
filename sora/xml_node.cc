// Ŭnicode please
#include "sora_stdafx.h"
#include "xml_node.h"

using namespace std;

namespace sora {;
XmlNode::XmlNode(const char *name)
  : name_(name), parent_(NULL), children_(new XmlNodeList())
{
  SR_ASSERT(strlen(name) > 0 && "node must have name");
}
XmlNode::XmlNode(const std::string &name)
  : name_(name), parent_(NULL), children_(new XmlNodeList())
{
  SR_ASSERT(name.length() > 0 && "node must have name");
}
XmlNode::~XmlNode()
{
}

void XmlNode::setAttribute(const std::string &key, const std::string &value)
{
  setAttribute(key.c_str(), value.c_str()); 
}
void XmlNode::setAttribute(const char *key, const char *value)
{
  attribute_[key] = value;
}
const std::string &XmlNode::getAttribute(const char *key) const
{
  static string emptyStr("");
  map<string, string>::const_iterator it = attribute_.find(key);
  if(it == attribute_.end())
    return emptyStr;
  else
    return it->second;
}
bool XmlNode::hasAttribute(const char *key) const
{
  map<string, string>::const_iterator it = attribute_.find(key);
  if(it == attribute_.end())
    return false;
  else
    return true;
}

//child method
void XmlNode::addChild(XmlNode *child)
{
  children_->add(child);
  child->setParent(this);
}
XmlNodePtr XmlNode::child(int index) const { return children_->get(index); }
int XmlNode::childSize() const { return children_->size(); }
XmlNodePtr XmlNode::firstChild() const { return this->child(0); }
XmlNodePtr XmlNode::lastChild() const { return this->child(childSize()-1); }
XmlNodePtrIter XmlNode::childBegin() const { return children_->begin(); }
XmlNodePtrIter XmlNode::childEnd() const { return children_->end(); }

//search
XmlNodePtr XmlNode::getElementById(const char *id) const
{
  XmlNodePtr result = NullXmlNode::null();
  //자기 자신은 제외
  //if(this->getAttribute("id") == id)	return const_cast<XmlNode*>(this);
  //else..자식을 돌면서 확인
  vector<XmlNodePtr>::const_iterator iter = this->childBegin();
  for( ; iter != this->childEnd() ; iter++)
  {
    XmlNodePtr child = *iter;
    if(child->getAttribute("id") == id)
      return child;

    XmlNodePtr childresult = child->getElementById(id);
    if(childresult->isNull() == false)
      return childresult;
  }
  //else..못찾은 경우
  return result;
}

XmlNodeList XmlNode::getElementsByName(const char *name) const
{
  XmlNodeList result;
  //자기 자신은 제외
  //if(this->name() == name)	result.push_back(const_cast<XmlNode*>(this));

  XmlNodePtrIter iter = this->childBegin();
  for( ; iter != this->childEnd() ; iter++)
  {
    XmlNodePtr child = *iter;
    if(child->getName() == name)
      result.add(child);

    XmlNodeList childresult = child->getElementsByName(name);

    XmlNodePtrIter childIter;
    for(childIter = childresult.begin() ; childIter != childresult.end() ; childIter++)
    {
      result.add(*childIter);
    }
  }

  return result;
}

//getter + setter
const string &XmlNode::getName() const 
{
  return name_; 
}
XmlNode *XmlNode::parent() const 
{
  return parent_; 
}
void XmlNode::setParent(XmlNode *parent) 
{
  parent_ = parent; 
}
bool XmlNode::isNull() const 
{
  return false; 
}
void XmlNode::setContent(const char *content) 
{
  content_ = content; 
}
void XmlNode::setContent(const std::string &content)
{
  content_ = content;
}
const std::string &XmlNode::getContent() const 
{
  return content_; 
}

NullXmlNode::NullXmlNode()
  : XmlNode("NULL")
{
  parent_ = this;
}
NullXmlNode::~NullXmlNode()
{
}

XmlNode *NullXmlNode::nullPtr()
{
  XmlNodePtr &n = null();
  return n.get();
}

XmlNodePtr &NullXmlNode::null()
{
  static XmlNode *node = new NullXmlNode();
  static XmlNodePtr ptr(node);
  return ptr;
}

//attribute
void NullXmlNode::setAttribute(const char *key, const char *value) {}
const std::string &NullXmlNode::getAttribute(const char *key) const
{
  static string emptyStr("");
  return emptyStr;
}
bool NullXmlNode::hasAttribute(const char *key) const 
{
  return false;
}

//children
void NullXmlNode::addChild(XmlNode *child) {}
XmlNodePtr NullXmlNode::child(int index) const 
{
  return null(); 
}
int NullXmlNode::childSize() const 
{
  return 0; 
}
XmlNodePtr NullXmlNode::firstChild() const 
{
  return null(); 
}
XmlNodePtr NullXmlNode::lastChild() const 
{
  return null(); 
}

bool NullXmlNode::isNull() const 
{
  return true; 
}

XmlNodeList::XmlNodeList()
{
}
void XmlNodeList::add(XmlNode *node)
{
  XmlNodePtr nodeptr(node);
  nodelist_.push_back(nodeptr);
}
void XmlNodeList::add(XmlNodePtr nodeptr)
{
  nodelist_.push_back(nodeptr);
}
XmlNodePtr XmlNodeList::get(int index) const
{
  const int count = this->size();
  if(count == 0 || index < 0 || index >= count)
  {
    //잘못된 범위인 경우 + 없는 경우
    return NullXmlNode::null();
  }
  return nodelist_[index];
}
int XmlNodeList::size() const { return nodelist_.size(); }
std::vector<XmlNodePtr>::const_iterator XmlNodeList::begin() const { return nodelist_.begin(); }
std::vector<XmlNodePtr>::const_iterator XmlNodeList::end() const { return nodelist_.end(); }
}
