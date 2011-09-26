#include <cstdlib>
#include <cstdio>
#include <sstream>

#include <vector>
#include <string>

#include "sora/platform.h"
#include "sora/macro.h"
#include "sora/tr1_include.h"

//gl code
#include <gl/glew.h>
#if SR_WIN
#include <gl/glfw.h>
#else
#error "not support"
#endif

//boost
#include <boost/utility.hpp>
#include <boost/foreach.hpp>