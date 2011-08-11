// Ŭnicode please
#pragma once

//외부에서 사용할경우 이것만 인클루드해서 쓸수있도록 통쨰로 분리해놓자

///base library, debug(include assert) + custom type
#include "matsu/MTMacro.h"
#include "matsu/MTType.h"

///math library
#include "matsu/MTMath.h"
#include "matsu/MTVector.h"
#include "matsu/MTVectorTpl.h"

#include "matsu/MTMatrix.h"
#include "matsu/MTMatrixTpl.h"

#include "matsu/MTQuaternion.h"

//좌표계 관련
#include "matsu/MTCoordinate.h"

///for string library
#include "matsu/MTString.h"
#include "matsu/MTStringUtil.h"

//template library
#include "matsu/MTTemplateLib.h"
#include "matsu/MTSingleton.h"
#include "matsu/MTSharedObject.h"

//mem pool
#include "matsu/MTFixedSizeMemPool.h"
#include "matsu/MTUnfixedSizeMemPool.h"