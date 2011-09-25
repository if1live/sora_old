// Ŭnicode please
#pragma once

//외부에서 사용할경우 이것만 인클루드해서 쓸수있도록 통쨰로 분리해놓자

///base library, debug(include assert) + custom type
#include "sora/SRMacro.h"
#include "matsu/MTType.h"

///math library
#include "matsu/MTMath.h"
#include "matsu/MTVector.h"
#include "matsu/MTVectorTemplate.h"

#include "matsu/MTMatrix.h"
#include "matsu/MTMatrixTpl.h"

#include "matsu/MTQuaternion.h"

//좌표계 관련
#include "matsu/MTCoordinate.h"

///for string library
#include "matsu/MTString.h"
#include "sora/SRStringUtil.h"

//template library
#include "sora/SRTemplateLib.h"
#include "sora/SRSingleton.h"
#include "matsu/MTSharedObject.h"