#pragma once

namespace mio
{
	class XmlNode;
	class NullXmlNode;
	class XmlNodeList;

	//typedef
	typedef std::tr1::shared_ptr<XmlNode> XmlNodePtr;
	typedef std::vector<XmlNodePtr>::const_iterator XmlNodePtrIter;

	class XmlNode {
	public:
		XmlNode(const char *name);
		XmlNode(const std::string &name);
		virtual ~XmlNode();

		//attribute
		virtual void setAttribute(const char *key, const char *value);
		virtual void setAttribute(const std::string &key, const std::string &value);
		virtual const std::string &getAttribute(const char *key) const;
		virtual bool hasAttribute(const char *key) const;

		//children
		virtual void addChild(XmlNode *child);
		virtual XmlNodePtr child(int index) const;
		virtual int childSize() const;
		virtual XmlNodePtr firstChild() const;
		virtual XmlNodePtr lastChild() const;

		virtual XmlNodePtrIter childBegin() const;
		virtual XmlNodePtrIter childEnd() const;

		//getter + setter
		void setContent(const char *content);
		void setContent(const std::string &content);
		const std::string &getContent() const;

		const std::string &getName() const;
		XmlNode *parent() const;
		virtual bool isNull() const;

		//search
		XmlNodeList getElementsByName(const char *name) const;
		XmlNodePtr getElementById(const char *id) const;

	protected:
		//setter
		void setParent(XmlNode *parent);

		std::string name_;
		std::map<std::string, std::string> attribute_;
		XmlNode *parent_;
		std::auto_ptr<XmlNodeList> children_;
		std::string content_;
	};

	class NullXmlNode : public XmlNode {
	public:
		static XmlNode *nullPtr();
		static XmlNodePtr &null();
		virtual ~NullXmlNode();

		//attribute
		virtual void setAttribute(const char *key, const char *value);
		virtual const std::string &getAttribute(const char *key) const;
		virtual bool hasAttribute(const char *key) const;

		//children
		virtual void addChild(XmlNode *child);
		virtual XmlNodePtr child(int index) const;
		virtual int childSize() const;
		virtual XmlNodePtr firstChild() const;
		virtual XmlNodePtr lastChild() const;

		virtual bool isNull() const;

	private:
		NullXmlNode();
	};

	class XmlNodeList {
	public:
		XmlNodeList();
		void add(XmlNode *node);
		void add(XmlNodePtr nodeptr);
		XmlNodePtr get(int index) const;
		int size() const;
		std::vector<XmlNodePtr>::const_iterator begin() const;
		std::vector<XmlNodePtr>::const_iterator end() const;

	private:
		std::vector<XmlNodePtr> nodelist_;
	};
}
