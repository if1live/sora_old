// Ŭnicode please
#pragma once
namespace runa
{
	template<GLenum>
	class Shader;

	typedef Shader<GL_VERTEX_SHADER> VertexShader;
	typedef Shader<GL_FRAGMENT_SHADER> FragmentShader;

	typedef std::tr1::shared_ptr<VertexShader> VertexShaderPtr;
	typedef std::tr1::shared_ptr<FragmentShader> FragmentShaderPtr;

	class ShaderProgram;
	typedef std::tr1::shared_ptr<ShaderProgram> ShaderProgramPtr;

	class ShaderLocation;
}