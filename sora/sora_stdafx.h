/* Copyright (C) 2011 by if1live */
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
#include "sora/arch.h"
#include "sora/assert_inc.h"
#include "sora/mem.h"
#include "sora/logger.h"

#if SR_USE_PCH
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <cerrno>
#include <ctime>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#if SR_WIN
// open, close...
#include <io.h>
#endif
// #include <unistd.h>

#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <map>

// boost
#include <boost/noncopyable.hpp>
#include <boost/foreach.hpp>

#if SR_WIN
#include <Windows.h>
#include <tchar.h>
#include <direct.h>
#endif

#include "sora/shared_ptr_inc.h"
#include "sora/unordered_map_inc.h"
#include "sora/gl_inc.h"

#endif

#endif  // SORA_SORA_STDAFX_H_
