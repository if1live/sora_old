// Ŭnicode please
//c library
#include <cstring>
#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <climits>
#include <sstream>
#include <ctime>

//device의존적 분기를 위한 코드 include
//tr1의 경우 플랫폼별 분기가 있어서 미리 인클루드한다
#include "matsu/MTMacro.h"

//stl
#include <vector>
#include <string>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <queue>

#if _IPHONE_
#include <tr1/memory> 
#include <tr1/unordered_map> 
#elif _WIN_ 
#include <unordered_map> 
#endif

#include <memory>
#include <algorithm>

#include <exception>
#include <stdexcept>

#if _ES2_
	#if _WIN_
		//opengl
		//#include "esutil/esUtil.h"
		//#include <GLES2/gl2.h>
		//#include <GLES2/gl2platform.h>
		//#include <EGL/egl.h>
		#include "GL/glew.h"
		#include "GL/glfw.h"
	#elif _IPHONE_
		#import <OpenGLES/ES2/gl.h>
		#import <OpenGLES/ES2/glext.h>
	#endif

#define SORA_USE_SHADER 1
#elif _GLFW_
	#include "GL/glew.h"
	#include "GL/glfw.h"
#define SORA_USE_SHADER 0
#define _ES1_ 1
#endif

//boost
#include <boost/utility.hpp>
#include <boost/foreach.hpp>
#include <boost/static_assert.hpp>
//#include <boost/thread.hpp>

//http://stackoverflow.com/questions/4738987/iterator-debug-level-error-in-visual-studio
#if _ITERATOR_DEBUG_LEVEL == 0 && _SECURE_SCL != 0 
#error _SECURE_SCL != 0 while _ITERATOR_DEBUG_LEVEL == 0 
#endif

//sora library중에서 사실상 고정된 안정적인것은 precompile header에 떄려박자(치기 귀찮거든..)
#include "matsu/MTMacro.h"