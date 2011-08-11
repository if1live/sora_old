// Ŭnicode please
#include "../SoraStdAfx.h"
#include "mio/MioReader.h"
#include "matsu/MTStringUtil.h"

using std::string;
using std::vector;

namespace mio
{
	namespace reader
	{
		std::string read(const std::string &file, int buffersize)
		{
			assert(buffersize >= 1);
			FILE *f = fopen(file.c_str(), "rb");
			if(f == NULL)
			{
				string empty;
				return empty;
			}
			string result;
			//create buffer
			char *buf = new char[buffersize + 1];	//마지막 1은 null을 위한 영역
			memset(buf, 0, sizeof(char) * (buffersize+1));	//0으로 채우기
			while (true)
			{
				memset(buf, 0, sizeof(char) * (buffersize+1));	//0으로 채우기
				int readsize = fread(buf, sizeof(char), buffersize, f);	//버퍼보다 1작게 읽으면 마지막 1글자가 null역할한다
				result += buf;

				if(readsize != buffersize)
					break;
			}
			delete[](buf);
			fclose(f);
			return result;
		}
		std::vector<std::string> readline(const std::string &file, int buffersize)
		{
			vector<string> result;
			string rawdata(read(file, buffersize));
			matsu::StringUtil::split(rawdata, result, "\r\n");
			return result;
		}
	}

	string FileReader::read(const std::string &file, int buffersize) const
	{
		return mio::reader::read(file, buffersize);
	}
	vector<string> FileReader::readline(const std::string &file, int buffersize) const
	{
		return mio::reader::readline(file, buffersize);
	}

//#pragma mark buffer file reader
	BufferFileReader::BufferFileReader(const std::string &file, int buffersize)
	: buffersize_(buffersize), file_(NULL), pos_(0), lastSegment_(false)
	{
		buffer_ = new unsigned char[buffersize];
		memset(buffer_, 0, sizeof(unsigned char) * (buffersize));
		file_ = fopen(file.c_str(), "rb");
		//TODO: 파일을 읽지 못한 경우?

		//파일 크기를 읽어서 저장해놓는다 후에 로직에서 편하게 우려먹는다
		fseek(file_, 0, SEEK_END);
		filesize_ = ftell(file_);	//파일 크기 얻기
		fseek(file_, 0, SEEK_SET);

		//파일크기=0이면 읽기가 안됨
		if(filesize() == 0)
			lastSegment_ = true;

	}
	BufferFileReader::~BufferFileReader()
	{
		delete[](buffer_);
		fclose(file_);
	}
	int BufferFileReader::filesize() const
	{
		return filesize_;
	}
	unsigned char BufferFileReader::readChar()
	{
		unsigned char result = '\0';
		int readtarget = pos_ % buffersize_;

		//새로 읽어야되는 경우
		if(readtarget == 0)
		{
			if(lastSegment_ == false)
			{
				//아직 읽을것이 남아있는 경우
				memset(buffer_, 0, sizeof(unsigned char) * (buffersize_));	//버퍼의 내용 폭파
				int readsize = fread(buffer_, sizeof(unsigned char), buffersize_, file_);
				if(readsize != buffersize_)
					lastSegment_ = true;
				//우연히 파일크기가 buffer의 배수면 n번 읽기로 딱 떨어져서 reader, buffersize비교로
				//파일을 다 읽었나 확인할수 없을수 있다. 지금까지 읽은 크기=파일크기로 비교
				if(filesize() == readsize + pos_)
					lastSegment_ = true;
			}
			else
			{
				//더이상 읽을수 없는 경우 널문자 반환
				return result;
			}
		}

		//버퍼에서 읽기
		result = buffer_[readtarget];
		if(result != '\0')
			pos_++;	//null문자 발생=더이상 읽기 불가능
		return result;
	}
	std::string BufferFileReader::readLine()
	{
		string result;
		const string delim("\n");
		while(canRead() == true)
		{
			unsigned char ch = readChar();

			//\n직전까지 읽는다
			int n = delim.find_first_of(ch);
			if(n == std::string::npos)
				result.push_back(ch);
			else
				break;
		}
		return result;
	}
	std::string BufferFileReader::readWord()
	{
		const string &delim = " \r\t\n";
		string result;
		while(canRead() == true)
		{
			unsigned char ch = readChar();
			int n = delim.find_first_of(ch);
			if(n == std::string::npos)
				result.push_back(ch);
			else
				break;
		}
		return result;

	}

	bool BufferFileReader::canRead() const
	{
		int readtarget = pos_ % buffersize_;

		if(lastSegment_ == true)
		{
			if(readtarget == 0)
				return false;	//버퍼에 있는것은 다 읽었고 읽을게 더이상 없으면
			if(buffer_[readtarget] == '\0')
				return false;	//널은 파일의 끝에만 존재한다
		}

		//else
		return true;
	}
}
