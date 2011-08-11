#include "../SoraTestStdAfx.h"

int main(int argc, char *argv[]) 
{
	//init gl
#if _GLFW_
	if (glfwInit() != GL_TRUE) {
		return -1;
	}
	glfwOpenWindow(320, 240, 5, 6, 5, 0, 0, 0, GLFW_WINDOW);

	//init glew
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
#endif

	::testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();

	//cleanup gl
#if _GLFW_
	glfwTerminate();
#endif

	getchar();
	return result;
}