// Ŭnicode please
#pragma once

///@file vector의 사용을 한방으로 하기 위해서+하위 호환성을 위해서 남겼다
#include <cmath>
#include "matsu/vector_template.h"

namespace matsu
{
typedef VectorTemplate<int,2> ivec2;
typedef VectorTemplate<float,2> vec2;
	
typedef VectorTemplate<int,3> ivec3;
typedef VectorTemplate<float,3> vec3;
	
typedef VectorTemplate<int,4> ivec4;
typedef VectorTemplate<float,4> vec4;

}