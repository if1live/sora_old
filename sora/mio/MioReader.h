// Ŭnicode please
#pragma once

#include <vector>

namespace mio
{
	namespace reader
	{
		const int DefaultBufferSize = 1024 * 128;	//128kb

		std::string read(const std::string &file, int buffersize=DefaultBufferSize);
		std::vector<std::string> readline(const std::string &file, int buffersize=DefaultBufferSize);
	}

	class FileReader {
	public:
		std::string read(const std::string &file, int buffersize=reader::DefaultBufferSize) const;
		std::vector<std::string> readline(const std::string &file, int buffersize=reader::DefaultBufferSize) const;
	};

	class BufferFileReader {
	public:
		BufferFileReader(const std::string &file, int buffersize=reader::DefaultBufferSize);
		~BufferFileReader();
		int filesize() const;
		unsigned char readChar();
		std::string readLine();
		std::string readWord();
		bool canRead() const;
	private:
		FILE *file_;
		int pos_;	//다음에 읽을 지점
		bool lastSegment_;
		const int buffersize_;
		unsigned char *buffer_;

		int filesize_;	//파일 사이즈는 읽기모드라서 안변한다
	};
}
