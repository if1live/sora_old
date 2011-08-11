// Ŭnicode please
#include "../SoraTestStdAfx.h"
#include "runa/RNShader.h"

using namespace std;
using namespace runa;

TEST(RNVertexShader, test)
{
	VertexShader vsh;
	EXPECT_EQ(0, vsh.getHandle());	//not yet created
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
	vsh.load(src);
	EXPECT_EQ(true, vsh.getHandle() > 0);
}

TEST(RNFragmentShader, test)
{
	FragmentShader fsh;
	EXPECT_EQ(0, fsh.getHandle());	//not yet created
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
	fsh.load(src);
	EXPECT_EQ(true, fsh.getHandle() > 0);
}