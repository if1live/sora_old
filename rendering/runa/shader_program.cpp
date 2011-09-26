// Ŭnicode please
#include "precompile.h"
#include "runa/shader_program.h"
#include "runa/shader.h"
#include "runa/shader_location.h"

using namespace std;

namespace runa
{
	ShaderProgram *ShaderProgram::last_used_prog_ = NULL;

	ShaderProgram::ShaderProgram(VertexShader *vert_shader, FragmentShader *frag_shader)
		: program_(-1), vertex_shader_(vert_shader), fragment_shader_(frag_shader)
	{
		assert(vertex_shader_.get() != NULL);
		assert(fragment_shader_.get() != NULL);

		Init();
	}
	ShaderProgram::ShaderProgram(const std::string &vert_src, const std::string &frag_src)
		: program_(-1), 
		vertex_shader_(new VertexShader(vert_src)),
		fragment_shader_(new FragmentShader(frag_src))
	{
		Init();
	}

	bool ShaderProgram::Init()
	{
		//create shader program
		program_ = glCreateProgram();

		//attach shader to program
		glAttachShader(program_, vertex_shader_->get_handle());
		glAttachShader(program_, fragment_shader_->get_handle());

		// Link program.
		if(Link() == false)
		{
			return false;
		}
		return true;
	}

	bool ShaderProgram::SetShader(VertexShader *vert_shader, FragmentShader *frag_shader)
	{
		assert(vert_shader != NULL && frag_shader != NULL);
		glDetachShader(program_, vertex_shader_->get_handle());
		glDetachShader(program_, fragment_shader_->get_handle());

		vertex_shader_ = auto_ptr<VertexShader>(vert_shader);
		fragment_shader_ = auto_ptr<FragmentShader>(frag_shader);

		glAttachShader(program_, vertex_shader_->get_handle());
		glAttachShader(program_, fragment_shader_->get_handle());

		return Link();
	}

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(program_);
		program_ = -1;

		///지금 파괴하는 쉐이더 프로그램이 가장 마지막에 쓰인 경우
		///마지막에 쓰인 쉐이더 정보를 파괴
		if(last_used_prog_ == this)
		{
			last_used_prog_ = NULL;
		}
	}
	
	ShaderProgram &ShaderProgram::GetLastUsedProgram()
	{
		SR_ASSERT(last_used_prog_ != NULL && "not used shader program");
		return *last_used_prog_;
	}

	void ShaderProgram::SetShader(VertexShader *vert_shader)
	{
		assert(vert_shader != NULL);
		glDetachShader(program_, vertex_shader_->get_handle());
		vertex_shader_ = auto_ptr<VertexShader>(vert_shader);
		glAttachShader(program_, vertex_shader_->get_handle());
	}
	void ShaderProgram::SetShader(FragmentShader *frag_shader)
	{
		assert(frag_shader != NULL);
		glDetachShader(program_, fragment_shader_->get_handle());
		fragment_shader_ = auto_ptr<FragmentShader>(frag_shader);
		glAttachShader(program_, fragment_shader_->get_handle());
	}
	void ShaderProgram::Use()
	{
		glUseProgram(program_); 
		last_used_prog_ = this;
	}
	void ShaderProgram::operator()()
	{
		Use();
	}

	GLuint ShaderProgram::get_handle() const 
	{
		return program_; 
	}
	bool ShaderProgram::Validate()
	{
		GLint logLength, status;

		glValidateProgram(program_);
		glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &logLength);
		if (logLength > 0)
		{
			GLchar *log = (GLchar *)malloc(logLength);
			glGetProgramInfoLog(program_, logLength, &logLength, log);

			MT_LOG("Program Validate log: %s", log);
			free(log);
			SR_ASSERT(false);
		}

		glGetProgramiv(program_, GL_VALIDATE_STATUS, &status);
		if (status == 0)
			return false;

		return true;
	}
	bool ShaderProgram::Link()
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
	
	std::vector<ShaderLocation> ShaderProgram::GetUniformLocationList()
	{
		vector<ShaderLocation> list;

		if(this != last_used_prog_)
		{
			this->Use();
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
	
	std::vector<ShaderLocation> ShaderProgram::GetAttributeLocationList()
	{
		vector<ShaderLocation> list;

		if(this != last_used_prog_)
		{
			this->Use();
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