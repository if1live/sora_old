#pragma once

namespace sora {;
class XmlNode;
class XmlNodeList;

typedef std::vector<XmlNode*> XmlNodeListType;
typedef XmlNodeListType::iterator XmlNodeListIterator;
typedef XmlNodeListType::const_iterator XmlNodeListConstIterator;
typedef std::map<std::string, std::string> XmlAttributeDictType;

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
