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
#ifndef SORA_XML_NODE_H_
#define SORA_XML_NODE_H_

#if SR_USE_PCH == 0
#include <vector>
#include "unordered_map_inc.h"
#endif
namespace sora {;
class XmlNode;
class XmlNodeList;

typedef std::vector<XmlNode*> XmlNodeListType;
typedef XmlNodeListType::iterator XmlNodeListIterator;
typedef XmlNodeListType::const_iterator XmlNodeListConstIterator;
typedef std::tr1::unordered_map<std::string, std::string> XmlAttributeDictType;

class XmlNode {
public:
  XmlNode();
  XmlNode(const char *name);
  XmlNode(const std::string &name);
  ~XmlNode();

  //attribute
  void SetAttribute(const char *key, const char *value);
  void SetAttribute(const std::string &key, const std::string &value);
  const std::string &GetAttribute(const char *key) const;
  bool HasAttribute(const char *key) const;

  //children
  void AddChild(XmlNode *child);
  XmlNode *Child(int index) const;
  int ChildCount() const { return children_.size(); }
  XmlNode *FirstChild() const;
  XmlNode *LastChild() const;

  XmlNodeListIterator ChildBegin() { return children_.begin(); }
  XmlNodeListIterator ChildEnd() { return children_.end(); }
  XmlNodeListConstIterator ChildBegin() const { return children_.begin(); }
  XmlNodeListConstIterator ChildEnd() const { return children_.end(); }

  //getter + setter
  void set_content(const char *content) { content_ = content; }
  void set_content(const std::string &content) { content_ = content; }
  const std::string &content() const { return content_; }

  const std::string &name() const { return name_; }
  void set_name(const std::string &name) { name_ = name; }
  void set_name(const char *name) { name_ = name; }

  XmlNode *parent() const { return parent_; }

protected:
  //setter
  void set_parent(XmlNode *parent) { parent_ = parent; }

  std::string name_;
  XmlAttributeDictType attribute_;
  XmlNode *parent_;

  XmlNodeListType children_;
  std::string content_;
};
}

#endif  // SORA_XML_NODE_H_