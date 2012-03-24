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

#include "sora/core/vector.h"
#include "sora/sora_main.h"
#include "sora/core/logger.h"

extern "C" {
    JNIEXPORT void JNICALL Java_so_libsora_yuriko_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_so_libsora_yuriko_GL2JNILib_step(JNIEnv * env, jobject obj);
	
	JNIEXPORT void JNICALL Java_so_libsora_yuriko_GL2JNILib_setApkPath(JNIEnv * env, jobject obj, jstring apkPath);
	
	JNIEXPORT void JNICALL Java_so_libsora_yuriko_GL2JNILib_touchBegan(JNIEnv * env, jobject obj, jint x, jint y, jint uid, jint tick_count, jfloat timestamp);
	JNIEXPORT void JNICALL Java_so_libsora_yuriko_GL2JNILib_touchMoved(JNIEnv * env, jobject obj, jint curr_x, jint curr_y, jint prev_x, jint prev_y, jint uid, jint tick_count, jfloat timestamp);
	JNIEXPORT void JNICALL Java_so_libsora_yuriko_GL2JNILib_touchEnded(JNIEnv * env, jobject obj, jint curr_x, jint curr_y, jint prev_x, jint prev_y, jint uid, jint tick_count, jfloat timestamp);
	JNIEXPORT void JNICALL Java_so_libsora_yuriko_GL2JNILib_touchCancelled(JNIEnv * env, jobject obj, jint curr_x, jint curr_y, jint prev_x, jint prev_y, jint uid, jint tick_count, jfloat timestamp);
};

JNIEXPORT void JNICALL Java_so_libsora_yuriko_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
    //setupGraphics(width, height);
	SORA_setup_graphics(width, height);
}

JNIEXPORT void JNICALL Java_so_libsora_yuriko_GL2JNILib_step(JNIEnv * env, jobject obj)
{
    //renderFrame();
	//SORA_set_cam_pos(0, 1.0f);
	SORA_update_frame(1.0f / 60.0f);
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

JNIEXPORT void JNICALL Java_so_libsora_yuriko_GL2JNILib_touchBegan(JNIEnv * env, jobject obj, jint x, jint y, jint uid, jint tick_count, jfloat timestamp)
{
	SORA_touch_began(x, y, uid, tick_count, timestamp);
}
JNIEXPORT void JNICALL Java_so_libsora_yuriko_GL2JNILib_touchMoved(JNIEnv * env, jobject obj, jint curr_x, jint curr_y, jint prev_x, jint prev_y, jint uid, jint tick_count, jfloat timestamp)
{
	SORA_touch_moved(curr_x, curr_y, prev_x, prev_y, uid, tick_count, timestamp);
}
JNIEXPORT void JNICALL Java_so_libsora_yuriko_GL2JNILib_touchEnded(JNIEnv * env, jobject obj, jint curr_x, jint curr_y, jint prev_x, jint prev_y, jint uid, jint tick_count, jfloat timestamp)
{
	SORA_touch_ended(curr_x, curr_y, prev_x, prev_y, uid, tick_count, timestamp);
}
JNIEXPORT void JNICALL Java_so_libsora_yuriko_GL2JNILib_touchCancelled(JNIEnv * env, jobject obj, jint curr_x, jint curr_y, jint prev_x, jint prev_y, jint uid, jint tick_count, jfloat timestamp)
{
	SORA_touch_cancelled(curr_x, curr_y, prev_x, prev_y, uid, tick_count, timestamp);
}