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
#ifndef PLATFORM_MIO_READER_H_
#define PLATFORM_MIO_READER_H_

#include <vector>

namespace mio {;
namespace reader {;
	const int kDefaultBufferSize = 1024 * 128;	//128kb
	std::string Read(const std::string &file, int buffersize=kDefaultBufferSize);
  int Read(const std::string &file, std::string *target, int buffersize=kDefaultBufferSize);
	//std::vector<std::string> Readline(const std::string &file, int buffersize=kDefaultBufferSize);
}

class FileReader {
public:
	std::string Read(const std::string &file, int buffersize=reader::kDefaultBufferSize) const;
	//std::vector<std::string> Readline(const std::string &file, int buffersize=reader::kDefaultBufferSize) const;
};

class BufferFileReader {
public:
	BufferFileReader(const std::string &file, int buffersize=reader::kDefaultBufferSize);
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

#endif  // PLATFORM_MIO_READER_H_
