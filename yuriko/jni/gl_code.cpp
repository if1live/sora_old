/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// OpenGL ES 2.0 code

#include <jni.h>

#include "sora/vector.h"
#include "sora/sora_main.h"
#include "sora/logger.h"

extern "C" {
    JNIEXPORT void JNICALL Java_so_libsora_yuriko_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_so_libsora_yuriko_GL2JNILib_step(JNIEnv * env, jobject obj);
	
	JNIEXPORT void JNICALL Java_so_libsora_yuriko_GL2JNILib_setApkPath(JNIEnv * env, jobject obj, jstring apkPath);
};

JNIEXPORT void JNICALL Java_so_libsora_yuriko_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
    //setupGraphics(width, height);
	SORA_setup_graphics(width, height);
}

JNIEXPORT void JNICALL Java_so_libsora_yuriko_GL2JNILib_step(JNIEnv * env, jobject obj)
{
    //renderFrame();
	SORA_set_cam_pos(0, 1.0f);
	SORA_draw_frame();
}

JNIEXPORT void JNICALL Java_so_libsora_yuriko_GL2JNILib_setApkPath(JNIEnv * env, jobject obj, jstring apkPath)
{
	//LOGE("apk set called");
	const char* str;
	jboolean isCopy;
	str = env->GetStringUTFChars(apkPath, &isCopy);
	SORA_set_apk_file_path(str);
}