#include "../SoraTestStdAfx.h"

int main(int argc, char *argv[]) 
{
	//init gl
#if _GLFW_
	if (glfwInit() != GL_TRUE) {
		return -1;
	}
	glfwOpenWindow(320, 240, 5, 6, 5, 0, 0, 0, GLFW_WINDOW);
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