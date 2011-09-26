#pragma once

#include "sora/platform.h"

#if SR_WIN
#include <memory>
#include <unordered_map>
#elif SR_IOS
#include <tr1/memory>
#include <tr1/unordered_map>
#else
#error "not support"
#endif