// Ŭnicode please
#pragma once

#include <string>

namespace mio
{

	class Path;
	class PathCore;

	//namespace 함수/클래스 동시지원(클래스를 살린건 크로스 플랫폼으로 갈 경우 정리를 편하게 하기 위해서)
	class Path {
	public:
		static std::string docPath(const char *name, const char *ext);
		static std::string docPath(const char *file);
		static std::string docPath(const std::string &file);

		static std::string appPath(const char *name, const char *ext);
		static std::string appPath(const char *file);
		static std::string appPath(const std::string &file);

		static bool isFileExist(const std::string &file);

		static PathCore& path();	//path객체를 얻는 함수. 플랫폼별로 적절히 구현하기
	};

	class PathCore {
	public:
		virtual std::string docPath(const std::string &file) = 0;
		///@return 해당경로에 아무것도 없으면 빈 문자열
		virtual std::string appPath(const std::string &file) = 0;
		virtual bool isFileExist(const std::string &file) = 0;
	};

	class PathCoreWin : public PathCore {
	public:
		PathCoreWin();
		std::string docPath(const std::string &file);
		std::string appPath(const std::string &file);
		bool isFileExist(const std::string &file);
	private:
		std::string rootPath_;
	};

	
	class File {
	public:
		static bool copy(const std::string &src, const std::string &dst);
		static bool copy(const char *src, const char *dst);
		
		static int filesize(const char *file);
		static int filesize(const std::string &src);
	};
}
