// Ŭnicode please
#pragma once

#include <tinyxml.h>
#include "xml_node.h"

namespace mio
{
	class XmlNode;
	class XmlReaderError;
	class XmlReader;
	
	class XmlReader {
	public:
		XmlReader();
		~XmlReader();
		XmlNodePtr read(const char *content);
		XmlNodePtr read(const std::string &content);
		bool isErrorOccur() const;
		XmlReaderError *getError();
	private:
		XmlNode *parse(TiXmlNode *node);
		std::auto_ptr<XmlReaderError> error_;
	};
	
	///@brief xml reader가 죽었을때 발생한 에러에 대한 정보를 담는 클래스
	class XmlReaderError {
	public:
		XmlReaderError(const char *msg, int row, int col);
		~XmlReaderError();
		const std::string &getMsg() const;
		int getRow() const;
		int getCol() const;
		///@brief error msg를 하나의 문자열로 구성한것
		std::string str() const;
	private:
		std::string msg_;
		int row_;
		int col_;
	};
}
