// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

//disable warning
//http://winapi.co.kr/clec/cpp2/18-3-3.htm
#pragma warning(disable:4996)
#pragma warning(disable:4793)
#pragma warning(disable:4099)

#include "core/arch.h"

#include "sora/sora_stdafx.h"

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <climits>
#include <cmath>

#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <set>
#include <stack>
#include <queue>

#include "renderer/gl_inc.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>