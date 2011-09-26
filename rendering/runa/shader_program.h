// Ŭnicode please
#pragma once
#include "runa/runa_enum.h"

namespace runa
{
	class ShaderProgram : boost::noncopyable {
	public:
		ShaderProgram(VertexShader *vert_shader, FragmentShader *frag_shader);
		ShaderProgram(const std::string &vert_src, const std::string &frag_src);
		virtual ~ShaderProgram();

		bool Validate();
		bool Link();
		GLuint get_handle() const;

		void bindAttribLocation(GLuint index, const GLchar* name) const { glBindAttribLocation(program_, index, name); }
		GLuint GetAttribLocation(const GLchar* name) const { return glGetAttribLocation(program_, name); }
		GLuint GetUniformLocation(const GLchar* name) const { return glGetUniformLocation(program_, name); }

		void Use();
		///@brief same with use
		void operator()();

		std::vector<ShaderLocation> GetUniformLocationList();
		std::vector<ShaderLocation> GetAttributeLocationList();

		static ShaderProgram &GetLastUsedProgram();

	private:
		bool Init();

		///do not change shader in runtime
		///(if you need to use new shader, create new shader program)
		void SetShader(VertexShader *vert_shader);
		void SetShader(FragmentShader *frag_shader);
		bool SetShader(VertexShader *vert_shader, FragmentShader *frag_shader);

		GLuint program_;
		std::auto_ptr<VertexShader> vertex_shader_;
		std::auto_ptr<FragmentShader> fragment_shader_;

		static ShaderProgram *last_used_prog_;
	};
}