/* Copyright (C) 2011-2012 by if1live */
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
// Ŭnicode please
#ifndef SORA_SORA_STDAFX_H_
#define SORA_SORA_STDAFX_H_
// 사실상 모든 소스에서 필요한건 미리 인클루드
#include "arch.h"
#include "assert_inc.h"
#include "mem.h"
#include "logger.h"

//disable warning
#if SR_WIN
//http://winapi.co.kr/clec/cpp2/18-3-3.htm
#pragma warning(disable:4996)
#endif

#if SR_USE_PCH
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <cerrno>
#include <ctime>
#include <iostream>

#include <type_traits> 

//#include <types.h>
//#include <stat.h>
#include <fcntl.h>
#if SR_WIN
// open, close...
#include <io.h>
#endif
// #include <unistd.h>

#include <sstream>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <map>
#include <stack>
#include <queue>
#include <list>
#include <functional>
#include <memory>

//#if SR_WIN
//#include <Windows.h>
//#include <tchar.h>
//#include <direct.h>
//#endif

#include "shared_ptr_inc.h"
#include "unordered_map_inc.h"
#include "gl_inc.h"
#include "tuple_inc.h"
#include "array_inc.h"

//boost
#include <boost/foreach.hpp>
#include <boost/utility.hpp>

#include "lodepng/lodepng.h"

//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#endif

#endif  // SORA_SORA_STDAFX_H_
