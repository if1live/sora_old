//
//  MioMacro.h
//  mio
//
//  Created by if1live on 11. 5. 21..
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "mio/MioConsole.h"

//디버깅용 메세지 매크로 사용하자
#define MIO_LOG(...) mio::Console::log(__VA_ARGS__)

#define MIO_TIMELOG(...) mio::Console::log(__VA_ARGS__)