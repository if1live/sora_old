/*  Copyright (C) 2011 by if1live */
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
#include "sora_test_stdafx.h"

#include "sora/renderer/gl_inc.h"

//#include "sora/common/logger.h"
//#include "sora/common/assert_inc.h"

#if SR_WIN && (SR_GLES == 0)
int main(int argc, char *argv[]) {
  //SHOW_LINE();
  //LOGI("LOGI");
  //LOGE("LOGE %s", "args");
  //SR_ASSERT(true == false);

  if( !glfwInit() ) {
    exit( EXIT_FAILURE );
  }
  // Open an OpenGL window
  if( !glfwOpenWindow( 300,300, 0,0,0,0,0,0, GLFW_WINDOW ) ) {
    glfwTerminate();
    exit( EXIT_FAILURE );
  }
  // init glew
  glewInit();

  ::testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();

  glfwTerminate();
	getchar();
	return result;
}
#endif

#if SR_WIN && (SR_GLES == 1)
/*
int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();

	getchar();
	return result;
}*/

//
// Book:      OpenGL(R) ES 2.0 Programming Guide
// Authors:   Aaftab Munshi, Dan Ginsburg, Dave Shreiner
// ISBN-10:   0321502795
// ISBN-13:   9780321502797
// Publisher: Addison-Wesley Professional
// URLs:      http://safari.informit.com/9780321563835
//            http://www.opengles-book.com
//

// Hello_Triangle.c
//
//    This is a simple example that draws a single triangle with
//    a minimal vertex/fragment shader.  The purpose of this 
//    example is to demonstrate the basic concepts of 
//    OpenGL ES 2.0 rendering.
#include <stdlib.h>
#include "esUtil.h"

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext ) {
	int result = RUN_ALL_TESTS();
	getchar();
  exit(result);
}


int main ( int argc, char *argv[] ) {
  ::testing::InitGoogleTest(&argc, argv);

   ESContext esContext;
   esInitContext ( &esContext );

   esCreateWindow ( &esContext, "Hello Triangle", 320, 240, ES_WINDOW_RGB );
   esRegisterDrawFunc ( &esContext, Draw );   
   esMainLoop ( &esContext );
}


#endif