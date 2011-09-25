#pragma once

//platform 정보를 얻을수있는 매크로
#undef SR_IOS
#undef SR_WIN
#undef SR_ANDROID
#define SR_IOS 0
#define SR_WIN 0
#define SR_ANDROID 0

//platform check macro
#if __APPLE__
//iphone check
#import <Availability.h>
#if __IPHONE_2_0
#undef SR_IOS
#define SR_IOS 1
#endif
#endif

//android check
#ifdef __ANDROID__
#undef SR_ANDROID
#define SR_ANDROID 1
#endif

#if (defined _WIN32) || (defined _W64)
#undef SR_WIN
#define SR_WIN 1
#endif

