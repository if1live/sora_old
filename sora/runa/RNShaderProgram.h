// Ŭnicode please
#pragma once
#include "runa/RNTypedef.h"

namespace runa
{
	class ShaderProgram : boost::noncopyable {
	public:
		ShaderProgram(VertexShader *vertShader, FragmentShader *fragShader);
		ShaderProgram(const std::string &vertSrc, const std::string &fragSrc);
		virtual ~ShaderProgram();

		bool validate();
		bool link();
		GLuint getHandle() const;

		void bindAttribLocation(GLuint index, const GLchar* name) const { glBindAttribLocation(program_, index, name); }
		GLuint getAttribLocation(const GLchar* name) const { return glGetAttribLocation(program_, name); }
		GLuint getUniformLocation(const GLchar* name) const { return glGetUniformLocation(program_, name); }

		void use();
		///@brief same with use
		void operator()();

		std::vector<ShaderLocation> getUniformLocationList();
		std::vector<ShaderLocation> getAttributeLocationList();

		static ShaderProgram &getLastUsedProgram();

	private:
		bool init();

		///do not change shader in runtime
		///(if you need to use new shader, create new shader program)
		void setShader(VertexShader *vertShader);
		void setShader(FragmentShader *fragShader);
		bool setShader(VertexShader *vertShader, FragmentShader *fragShader);

		GLuint program_;
		std::auto_ptr<VertexShader> vShader_;
		std::auto_ptr<FragmentShader> fShader_;

		static ShaderProgram *lastUsedProg_;
	};
}