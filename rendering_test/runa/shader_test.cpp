// Ŭnicode please
#include "precompile.h"
#include "runa/shader.h"

using namespace std;
using namespace runa;

TEST(VertexShader, test)
{
	VertexShader vsh;
	EXPECT_EQ(0, vsh.get_handle());	//not yet created
	EXPECT_EQ(GL_VERTEX_SHADER, vsh.Type);
	EXPECT_EQ(GL_VERTEX_SHADER, VertexShader::Type);

	//load sample shader
	string src = "	\
				 uniform mat4 u_mvpMatrix;	\
				 attribute vec4 a_position;	\
				 attribute vec4 a_color;	\
				 varying vec4 v_color;	\
				 void main()	\
				 {	\
				 v_color = a_color;	\
				 gl_Position = u_mvpMatrix * a_position;	\
				 }	\
				 ";
	vsh.Load(src);
	EXPECT_EQ(true, vsh.get_handle() > 0);
}

TEST(FragmentShader, test)
{
	FragmentShader fsh;
	EXPECT_EQ(0, fsh.get_handle());	//not yet created
	EXPECT_EQ(GL_FRAGMENT_SHADER, fsh.Type);
	EXPECT_EQ(GL_FRAGMENT_SHADER, FragmentShader::Type);
	string src = "	\
				 precision mediump float;	\
				 varying vec4 v_color;	\
				 void main()	\
				 {	\
				 gl_FragColor = v_color;	\
				 }	\
				 ";
	fsh.Load(src);
	EXPECT_EQ(true, fsh.get_handle() > 0);
}