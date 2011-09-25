// Ŭnicode please
#include "../SoraStdAfx.h"
#include "runa/RNShaderProgram.h"
#include "runa/RNShader.h"
#include "runa/RNShaderLocation.h"

using namespace std;

namespace runa
{
	ShaderProgram *ShaderProgram::lastUsedProg_ = NULL;

	ShaderProgram::ShaderProgram(VertexShader *vertShader, FragmentShader *fragShader)
		: program_(-1), vShader_(vertShader), fShader_(fragShader)
	{
		assert(vShader_.get() != NULL);
		assert(fShader_.get() != NULL);

		init();
	}
	ShaderProgram::ShaderProgram(const std::string &vertSrc, const std::string &fragSrc)
		: program_(-1), 
		vShader_(new VertexShader(vertSrc)),
		fShader_(new FragmentShader(fragSrc))
	{
		init();
	}

	bool ShaderProgram::init()
	{
		//create shader program
		program_ = glCreateProgram();

		//attach shader to program
		glAttachShader(program_, vShader_->getHandle());
		glAttachShader(program_, fShader_->getHandle());

		// Link program.
		if(link() == false)
		{
			return false;
		}
		return true;
	}

	bool ShaderProgram::setShader(VertexShader *vertShader, FragmentShader *fragShader)
	{
		assert(vertShader != NULL && fragShader != NULL);
		glDetachShader(program_, vShader_->getHandle());
		glDetachShader(program_, fShader_->getHandle());

		vShader_ = auto_ptr<VertexShader>(vertShader);
		fShader_ = auto_ptr<FragmentShader>(fragShader);

		glAttachShader(program_, vShader_->getHandle());
		glAttachShader(program_, fShader_->getHandle());

		return link();
	}

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(program_);
		program_ = -1;

		///지금 파괴하는 쉐이더 프로그램이 가장 마지막에 쓰인 경우
		///마지막에 쓰인 쉐이더 정보를 파괴
		if(lastUsedProg_ == this)
		{
			lastUsedProg_ = NULL;
		}
	}
	
	ShaderProgram &ShaderProgram::getLastUsedProgram()
	{
		SR_ASSERT(lastUsedProg_ != NULL && "not used shader program");
		return *lastUsedProg_;
	}

	void ShaderProgram::setShader(VertexShader *vertShader)
	{
		assert(vertShader != NULL);
		glDetachShader(program_, vShader_->getHandle());
		vShader_ = auto_ptr<VertexShader>(vertShader);
		glAttachShader(program_, vShader_->getHandle());
	}
	void ShaderProgram::setShader(FragmentShader *fragShader)
	{
		assert(fragShader != NULL);
		glDetachShader(program_, fShader_->getHandle());
		fShader_ = auto_ptr<FragmentShader>(fragShader);
		glAttachShader(program_, fShader_->getHandle());
	}
	void ShaderProgram::use()
	{
		glUseProgram(program_); 
		lastUsedProg_ = this;
	}
	void ShaderProgram::operator()()
	{
		use();
	}

	GLuint ShaderProgram::getHandle() const 
	{
		return program_; 
	}
	bool ShaderProgram::validate()
	{
		GLint logLength, status;

		glValidateProgram(program_);
		glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0)
		{
			GLchar *log = (GLchar *)malloc(logLength);
			glGetProgramInfoLog(program_, logLength, &logLength, log);

			MT_LOG("Program validate log: %s", log);
			free(log);
			SR_ASSERT(false);
		}

		glGetProgramiv(program_, GL_VALIDATE_STATUS, &status);
		if (status == 0)
			return false;

		return true;
	}
	bool ShaderProgram::link()
	{
		//link program
		glLinkProgram(program_);

		GLint status;
		glGetProgramiv(program_, GL_LINK_STATUS, &status);
		if(status == GL_FALSE)
		{
			GLchar msg[256];
			glGetProgramInfoLog(program_, sizeof(msg), 0, &msg[0]);
			MT_LINE_LOG(msg);
			return false;
		}
		return true;
	}
	
	std::vector<ShaderLocation> ShaderProgram::getUniformLocationList()
	{
		vector<ShaderLocation> list;

		if(this != lastUsedProg_)
		{
			this->use();
		}
		GLint maxUniformLen;
		GLint numUniform;
		
		glGetProgramiv(program_, GL_ACTIVE_UNIFORMS, &numUniform);
		glGetProgramiv(program_, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformLen);

		const int MaxNameLength = 1024;
		char uniformName[MaxNameLength];
		SR_ASSERT(maxUniformLen < MaxNameLength);
		for(int i = 0 ; i < numUniform ; i++)
		{
			GLint size;
			GLenum type;
			//get uniform information
			glGetActiveUniform(program_, i, maxUniformLen, NULL, &size, &type, uniformName);
			GLint location = glGetUniformLocation(program_, uniformName);

			ShaderLocation sl(uniformName, location, size, type);
			list.push_back(sl);
		}

		return list;
	}
	
	std::vector<ShaderLocation> ShaderProgram::getAttributeLocationList()
	{
		vector<ShaderLocation> list;

		if(this != lastUsedProg_)
		{
			this->use();
		}
		GLint maxAttributeLen;
		GLint numAttribute;		
		glGetProgramiv(program_, GL_ACTIVE_ATTRIBUTES, &numAttribute);
		glGetProgramiv(program_, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttributeLen);

		const int MaxNameLength = 1024;
		char attributeName[MaxNameLength];
		SR_ASSERT(maxAttributeLen < maxAttributeLen);
		for(int i = 0 ; i < numAttribute ; i++)
		{
			GLint size;
			GLenum type;
			glGetActiveAttrib(program_, i, maxAttributeLen, NULL, &size, &type, attributeName);
			GLint location = glGetAttribLocation(program_, attributeName);

			ShaderLocation sl(attributeName, location, size, type);
			list.push_back(sl);
		}
		return list;
	}
}