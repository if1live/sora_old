#include "sora/renderer/gl_inc.h"
#include <stdlib.h>
#include "esUtil.h"

#include "sora/sora_main.h"
#include "sora/sys/device.h"
#include "sora/core/timer.h"
#include "sora/shadow_map_main.h"
#include "sora/test_function.h"
#include "sora/selection_main.h"
#include "sora/celshading_main.h"
#include "sora/depth_map_main.h"

typedef struct
{
  // Handle to a program object
  GLuint programObject;

} UserData;

///
// Create a shader object, load the shader source, and
// compile the shader.
//
GLuint LoadShader ( GLenum type, const char *shaderSrc )
{
  GLuint shader;
  GLint compiled;

  // Create the shader object
  shader = glCreateShader ( type );

  if ( shader == 0 )
    return 0;

  // Load the shader source
  glShaderSource ( shader, 1, &shaderSrc, NULL );

  // Compile the shader
  glCompileShader ( shader );

  // Check the compile status
  glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

  if ( !compiled ) 
  {
    GLint infoLen = 0;

    glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );

    if ( infoLen > 1 )
    {
      char* infoLog = (char*)malloc (sizeof(char) * infoLen );

      glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
      esLogMessage ( "Error compiling shader:\n%s\n", infoLog );            

      free ( infoLog );
    }

    glDeleteShader ( shader );
    return 0;
  }

  return shader;

}

///
// Initialize the shader and program object
//
int Init ( ESContext *esContext )
{
  UserData *userData = (UserData *)esContext->userData;
  const char vShaderStr[] =  
    "attribute vec4 vPosition;    \n"
    "void main()                  \n"
    "{                            \n"
    "   gl_Position = vPosition;  \n"
    "}                            \n";

  const char fShaderStr[] =  
    "precision mediump float;\n"\
    "void main()                                  \n"
    "{                                            \n"
    "  gl_FragColor = vec4 ( 1.0, 0.0, 0.0, 1.0 );\n"
    "}                                            \n";

  GLuint vertexShader;
  GLuint fragmentShader;
  GLuint programObject;
  GLint linked;

  // Load the vertex/fragment shaders
  vertexShader = LoadShader ( GL_VERTEX_SHADER, vShaderStr );
  fragmentShader = LoadShader ( GL_FRAGMENT_SHADER, fShaderStr );

  // Create the program object
  programObject = glCreateProgram ( );

  if ( programObject == 0 )
    return 0;

  glAttachShader ( programObject, vertexShader );
  glAttachShader ( programObject, fragmentShader );

  // Bind vPosition to attribute 0   
  glBindAttribLocation ( programObject, 0, "vPosition" );

  // Link the program
  glLinkProgram ( programObject );

  // Check the link status
  glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

  if ( !linked ) 
  {
    GLint infoLen = 0;

    glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );

    if ( infoLen > 1 )
    {
      char* infoLog = (char*)malloc (sizeof(char) * infoLen );

      glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
      esLogMessage ( "Error linking program:\n%s\n", infoLog );            

      free ( infoLog );
    }

    glDeleteProgram ( programObject );
    return FALSE;
  }

  // Store the program object
  userData->programObject = programObject;

  glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
  return TRUE;
}

///
// Draw a triangle using the shader pair created in Init()
//
void Draw ( ESContext *esContext )
{
  UserData *userData = (UserData *)esContext->userData;
  GLfloat vVertices[] = {  0.0f,  0.5f, 0.0f, 
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f };

  // Set the viewport
  glViewport ( 0, 0, esContext->width, esContext->height );

  // Clear the color buffer
  glClear ( GL_COLOR_BUFFER_BIT );

  // Use the program object
  glUseProgram ( userData->programObject );

  // Load the vertex data
  glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, vVertices );
  glEnableVertexAttribArray ( 0 );

  glDrawArrays ( GL_TRIANGLES, 0, 3 );

  eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

sora::Device dev;

void MainDraw ( ESContext *esContext ) {
  sora::celshading::draw_frame(&dev);
  float curr_time = Timer_GetSecond();
  //float dt = curr_time - prev_time;
  float dt = 1.0f / 60.0f;
  sora::celshading::update_frame(&dev, dt);

  //prev_time = curr_time;
  //Timer_Tick();
  eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
}

int main ( int argc, char *argv[] )
{
  ESContext esContext;
  UserData  userData;

  esInitContext ( &esContext );
  esContext.userData = &userData;

  int kWinWidth = 480;
  int kWinHeight = 800;

  esCreateWindow ( &esContext, "Hello Triangle", kWinWidth, kWinHeight, ES_WINDOW_RGB );

  //if ( !Init ( &esContext ) )
  //   return 0;
  //esRegisterDrawFunc ( &esContext, Draw );

  sora::celshading::setup_graphics(&dev, kWinWidth, kWinHeight);
  esRegisterDrawFunc ( &esContext, MainDraw );

  esMainLoop ( &esContext );
}


#if 0
/******************************************************************************

@File         OGLES2IntroducingPVRShell.cpp

@Title        Introduces PVRShell

@Version      

@Copyright    Copyright (c) Imagination Technologies Limited.

@Platform     Independant

@Description  Shows how to use the PowerVR framework for initialization. This
framework allows platform abstraction so applications using it
will work on any MBX enabled device.

******************************************************************************/
#include <stdio.h>
#include <string.h>

#if defined(__APPLE__) && defined (TARGET_OS_IPHONE)
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#else
#if defined(__BADA__)
#include <FGraphicsOpengl2.h>

using namespace Osp::Graphics::Opengl;
#else
#include <GLES2/gl2.h>
#endif
#endif

#include "PVRShell.h"

/*
*	Lesson 3: The PowerVR Shell
*	===========================

The PowerVR shell handles all OS specific initialisation code, and is
extremely convenient for writing portable applications. It also has
several built in command line features, which allow you to specify
attributes like the backbuffer size, vsync and antialiasing modes.

The code is constructed around a "PVRShell" superclass. You must define
your app using a class which inherits from this, which should implement
the following five methods, (which at execution time are essentially
called in the order in which they are listed):


InitApplication:	This is called before any API initialisation has
taken place, and can be used to set up any application data which does
not require API calls, for example object positions, or arrays containing
vertex data, before they are uploaded.

InitView:	This is called after the API has initialised, and can be
used to do any remaining initialisation which requires API functionality.
In this app, it is used to upload the vertex data.

RenderScene:	This is called repeatedly to draw the geometry. Returning
false from this function instructs the app to enter the quit sequence:

ReleaseView:	This function is called before the API is released, and
is used to release any API resources. In this app, it releases the
vertex buffer.

QuitApplication:	This is called last of all, after the API has been
released, and can be used to free any leftover user allocated memory.


The shell framework starts the application by calling a "NewDemo" function,
which must return an instance of the PVRShell class you defined. We will
now use the shell to create a "Hello triangle" app, similar to the previous
one.
#if defined(__APPLE__)
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#else
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2extimg.h>
#endif
*/

/******************************************************************************
Defines
******************************************************************************/

// Index to bind the attributes to vertex shaders
#define VERTEX_ARRAY	0

/*!****************************************************************************
To use the shell, you have to inherit a class from PVRShell
and implement the five virtual functions which describe how your application
initializes, runs and releases the ressources.
******************************************************************************/
class OGLESIntroducingPVRShell : public PVRShell
{
  // The vertex and fragment shader OpenGL handles
  GLuint m_uiVertexShader, m_uiFragShader;

  // The program object containing the 2 shader objects
  GLuint m_uiProgramObject;

  // VBO handle
  GLuint m_ui32Vbo;

public:
  virtual bool InitApplication();
  virtual bool InitView();
  virtual bool ReleaseView();
  virtual bool QuitApplication();
  virtual bool RenderScene();
};


/*!****************************************************************************
@Function		InitApplication
@Return		bool		true if no error occured
@Description	Code in InitApplication() will be called by PVRShell once per
run, before the rendering context is created.
Used to initialize variables that are not dependant on it
(e.g. external modules, loading meshes, etc.)
If the rendering context is lost, InitApplication() will
not be called again.
******************************************************************************/
bool OGLESIntroducingPVRShell::InitApplication()
{
  return true;
}

/*!****************************************************************************
@Function		QuitApplication
@Return		bool		true if no error occured
@Description	Code in QuitApplication() will be called by PVRShell once per
run, just before exiting the program.
If the rendering context is lost, QuitApplication() will
not be called.
******************************************************************************/
bool OGLESIntroducingPVRShell::QuitApplication()
{
  return true;
}

/*!****************************************************************************
@Function		InitView
@Return		bool		true if no error occured
@Description	Code in InitView() will be called by PVRShell upon
initialization or after a change in the rendering context.
Used to initialize variables that are dependant on the rendering
context (e.g. textures, vertex buffers, etc.)
******************************************************************************/
bool OGLESIntroducingPVRShell::InitView()
{
  // Fragment and vertex shaders code
  const char* pszFragShader = "\
                              void main (void)\
                              {\
                              gl_FragColor = vec4(1.0, 1.0, 0.66  ,1.0);\
                              }";
  const char* pszVertShader = "\
                              attribute highp vec4	myVertex;\
                              uniform mediump mat4	myPMVMatrix;\
                              void main(void)\
                              {\
                              gl_Position = myPMVMatrix * myVertex;\
                              }";

  // Create the fragment shader object
  m_uiFragShader = glCreateShader(GL_FRAGMENT_SHADER);

  // Load the source code into it
  glShaderSource(m_uiFragShader, 1, (const char**)&pszFragShader, NULL);

  // Compile the source code
  glCompileShader(m_uiFragShader);

  // Check if compilation succeeded
  GLint bShaderCompiled;
  glGetShaderiv(m_uiFragShader, GL_COMPILE_STATUS, &bShaderCompiled);
  if (!bShaderCompiled)
  {
    // An error happened, first retrieve the length of the log message
    int i32InfoLogLength, i32CharsWritten;
    glGetShaderiv(m_uiFragShader, GL_INFO_LOG_LENGTH, &i32InfoLogLength);

    // Allocate enough space for the message and retrieve it
    char* pszInfoLog = new char[i32InfoLogLength];
    glGetShaderInfoLog(m_uiFragShader, i32InfoLogLength, &i32CharsWritten, pszInfoLog);

    /*
    Displays the message in a dialog box when the application quits
    using the shell PVRShellSet function with first parameter prefExitMessage.
    */
    char* pszMsg = new char[i32InfoLogLength+256];
    strcpy(pszMsg, "Failed to compile fragment shader: ");
    strcat(pszMsg, pszInfoLog);
    PVRShellSet(prefExitMessage, pszMsg);

    delete [] pszMsg;
    delete [] pszInfoLog;
    return false;
  }

  // Loads the vertex shader in the same way
  m_uiVertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(m_uiVertexShader, 1, (const char**)&pszVertShader, NULL);
  glCompileShader(m_uiVertexShader);
  glGetShaderiv(m_uiVertexShader, GL_COMPILE_STATUS, &bShaderCompiled);
  if (!bShaderCompiled)
  {
    int i32InfoLogLength, i32CharsWritten;
    glGetShaderiv(m_uiVertexShader, GL_INFO_LOG_LENGTH, &i32InfoLogLength);
    char* pszInfoLog = new char[i32InfoLogLength];
    glGetShaderInfoLog(m_uiVertexShader, i32InfoLogLength, &i32CharsWritten, pszInfoLog);
    char* pszMsg = new char[i32InfoLogLength+256];
    strcpy(pszMsg, "Failed to compile vertex shader: ");
    strcat(pszMsg, pszInfoLog);
    PVRShellSet(prefExitMessage, pszMsg);

    delete [] pszMsg;
    delete [] pszInfoLog;
    return false;
  }

  // Create the shader program
  m_uiProgramObject = glCreateProgram();

  // Attach the fragment and vertex shaders to it
  glAttachShader(m_uiProgramObject, m_uiFragShader);
  glAttachShader(m_uiProgramObject, m_uiVertexShader);

  // Bind the custom vertex attribute "myVertex" to location VERTEX_ARRAY
  glBindAttribLocation(m_uiProgramObject, VERTEX_ARRAY, "myVertex");

  // Link the program
  glLinkProgram(m_uiProgramObject);

  // Check if linking succeeded in the same way we checked for compilation success
  GLint bLinked;
  glGetProgramiv(m_uiProgramObject, GL_LINK_STATUS, &bLinked);
  if (!bLinked)
  {
    int i32InfoLogLength, i32CharsWritten;
    glGetProgramiv(m_uiProgramObject, GL_INFO_LOG_LENGTH, &i32InfoLogLength);
    char* pszInfoLog = new char[i32InfoLogLength];
    glGetProgramInfoLog(m_uiProgramObject, i32InfoLogLength, &i32CharsWritten, pszInfoLog);
    char* pszMsg = new char[i32InfoLogLength+256];
    strcpy(pszMsg, "Failed to link program: ");
    strcat(pszMsg, pszInfoLog);
    PVRShellSet(prefExitMessage, pszMsg);

    delete [] pszMsg;
    delete [] pszInfoLog;
    return false;
  }

  // Actually use the created program
  glUseProgram(m_uiProgramObject);

  // Sets the clear color
  glClearColor(0.6f, 0.8f, 1.0f, 1.0f);

  // Create VBO for the triangle from our data

  // Vertex data
  GLfloat afVertices[] = {-0.4f,-0.4f,0.0f,  0.4f,-0.4f,0.0f,   0.0f,0.4f,0.0f};

  // Gen VBO
  glGenBuffers(1, &m_ui32Vbo);

  // Bind the VBO
  glBindBuffer(GL_ARRAY_BUFFER, m_ui32Vbo);

  // Set the buffer's data
  glBufferData(GL_ARRAY_BUFFER, 3 * (3 * sizeof(GLfloat)) /* 3 Vertices of 3 floats in size */, afVertices, GL_STATIC_DRAW);

  // Unbind the VBO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return true;
}

/*!****************************************************************************
@Function		ReleaseView
@Return		bool		true if no error occured
@Description	Code in ReleaseView() will be called by PVRShell when the
application quits or before a change in the rendering context.
******************************************************************************/
bool OGLESIntroducingPVRShell::ReleaseView()
{
  // Release Vertex buffer object.
  glDeleteBuffers(1, &m_ui32Vbo);

  // Frees the OpenGL handles for the program and the 2 shaders
  glDeleteProgram(m_uiProgramObject);
  glDeleteShader(m_uiVertexShader);
  glDeleteShader(m_uiFragShader);
  return true;
}

/*!****************************************************************************
@Function		RenderScene
@Return		bool		true if no error occured
@Description	Main rendering loop function of the program. The shell will
call this function every frame.
eglSwapBuffers() will be performed by PVRShell automatically.
PVRShell will also manage important OS events.
Will also manage relevent OS events. The user has access to
these events through an abstraction layer provided by PVRShell.
******************************************************************************/
bool OGLESIntroducingPVRShell::RenderScene()
{
  // Matrix used for projection model view
  float afIdentity[] = {
    1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    0,0,0,1
  };

  /*
  Clears the color buffer.
  glClear() can also be used to clear the depth or stencil buffer
  (GL_DEPTH_BUFFER_BIT or GL_STENCIL_BUFFER_BIT)
  */
  glClear(GL_COLOR_BUFFER_BIT);

  /*
  Bind the projection model view matrix (PMVMatrix) to
  the associated uniform variable in the shader
  */

  // First gets the location of that variable in the shader using its name
  int i32Location = glGetUniformLocation(m_uiProgramObject, "myPMVMatrix");

  // Then passes the matrix to that variable
  glUniformMatrix4fv(i32Location, 1, GL_FALSE, afIdentity);

  // Bind the VBO
  glBindBuffer(GL_ARRAY_BUFFER, m_ui32Vbo);

  /*
  Enable the custom vertex attribute at index VERTEX_ARRAY.
  We previously binded that index to the variable in our shader "vec4 MyVertex;"
  */
  glEnableVertexAttribArray(VERTEX_ARRAY);

  // Points to the data for this vertex attribute
  glVertexAttribPointer(VERTEX_ARRAY, 3, GL_FLOAT, GL_FALSE, 0, 0);

  /*
  Draws a non-indexed triangle array from the pointers previously given.
  This function allows the use of other primitive types : triangle strips, lines, ...
  For indexed geometry, use the function glDrawElements() with an index list.
  */
  glDrawArrays(GL_TRIANGLES, 0, 3);

  // Unbind the VBO
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  return true;
}

/*!****************************************************************************
@Function		NewDemo
@Return		PVRShell*		The demo supplied by the user
@Description	This function must be implemented by the user of the shell.
The user should return its PVRShell object defining the
behaviour of the application.
******************************************************************************/
PVRShell* NewDemo()
{
  return new OGLESIntroducingPVRShell();
}

/******************************************************************************
End of file (OGLESIntroducingPVRShell.cpp)
******************************************************************************/
#endif