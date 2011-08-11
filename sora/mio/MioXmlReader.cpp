// Ŭnicode please
#include "../SoraStdAfx.h"
#include "mio/MioXmlReader.h"
#include "mio/MioXmlNode.h"
#include <iostream>

using namespace std;

namespace mio
{
	XmlReader::XmlReader()
	{
	}
	XmlReader::~XmlReader()
	{
	}
	bool XmlReader::isErrorOccur() const
	{
		if(error_.get() == NULL)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	XmlReaderError *XmlReader::getError()
	{
		return error_.get();
	}
	XmlNodePtr XmlReader::read(const char *content)
	{
		string str(content);
		return read(str);
	}
	XmlNodePtr XmlReader::read(const std::string &content)
	{
		//error초기화
		error_.reset();

		TiXmlDocument doc;
		doc.Parse(content.c_str());

		if(doc.Error())
		{
			//MT_ASSERT(!"Xml Error");
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
		MT_ASSERT(root);
		
		XmlNode *result = this->parse(root);
		return XmlNodePtr(result);
	}
	XmlNode *XmlReader::parse(TiXmlNode *node)
	{
		if(node->Type() != TiXmlNode::TINYXML_ELEMENT)
		{
			MT_ASSERT(!"Not valid recursive call");
		}
		//casting
		TiXmlElement *elem = static_cast<TiXmlElement*>(node);
		string name(elem->Value());
		XmlNode *result = new XmlNode(name);

		//attribute
		TiXmlAttribute *attr = elem->FirstAttribute();
		for( ; attr != NULL ; attr = attr->Next())
		{
			string key(attr->Name());
			string value(attr->Value());
			result->setAttribute(key, value);
		}

		//get child
		TiXmlNode *child = elem->FirstChildElement();
		for( ; child != NULL ; child = child->NextSibling())
		{
			if(child->Type() == TiXmlNode::TINYXML_ELEMENT)
			{
				//일반 노드인 경우
				XmlNode *childnode = parse(child);
				result->addChild(childnode);
			}
		
		}

		//content
		child = elem->FirstChild();
		for( ; child != NULL ; child = child->NextSibling())
		{
			if(child->Type() == TiXmlNode::TINYXML_TEXT)
			{
				//text content인 경우 뺴내기
				string content(child->Value());
				result->setContent(content.c_str());
				break;	//text content는 1개 존재햐야한다
			}
		}
		return result;
	}

	//////////////////////////////////////////////
	XmlReaderError::XmlReaderError(const char *msg, int row, int col)
		: msg_(msg), row_(row), col_(col)
	{
	}
	XmlReaderError::~XmlReaderError()
	{
	}
	const std::string &XmlReaderError::getMsg() const
	{
		return msg_;
	}
	int XmlReaderError::getRow() const
	{
		return row_;
	}
	int XmlReaderError::getCol() const
	{
		return col_;
	}
	std::string XmlReaderError::str() const
	{
		ostringstream oss;
		oss << "[row=" << row_ << "/col=" << col_ << "]";
		oss << "Msg:" << msg_;
		return oss.str();
	}
}

