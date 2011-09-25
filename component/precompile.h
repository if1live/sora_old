#include "sora/macro.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>

#include <string>
#include <vector>

#include <boost/foreach.hpp>

//tr1
#if SR_WIN
#include <memory>
#include <unordered_map>
#elif SR_IOS
#include <tr1/memory>
#include <tr1/unordered_map>
#else
#error "not support this platform"
#endif