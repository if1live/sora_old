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
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "logic_interface.h"

#include "sora/render/texture_manager.h"
#include "sora/render/scene_manager.h"
#include "sora/render/scene.h"
#include "sora/render/gl_helper.h"
#include "sora/render/texture.h"
#include "sora/render/gl_window.h"
#include "sora/render/immediate_mode_emulator.h"
#include "sora/render/gl_inc.h"
#include "main_scene.h"
#include <boost/thread.hpp>

using namespace sora;
using namespace yukino;

bool setupGraphics(int w, int h) {
	LOGI("setupGraphics(%d, %d)", w, h);
	sora::GLWindow win(w, h, sora::kWinModeWindow, 1);
	win.Init();
	sora::ImmediateModeEmulator::GetInstance().Init();
	srglViewport(0, 0, w, h);  
	
	sora::TextureManagerThreadRunner texture_thd_runner;
	boost::thread texture_thd(texture_thd_runner);
	sora::GLHelper::CheckError("1");	//	
	Scene *scene = new MainScene();
	sora::GLHelper::CheckError("112");	//
	SceneManager::GetInstance().Push(scene);
	
	sora::GLHelper::CheckError("12");	//
	
	return true;
}

void renderFrame() {
	sora::GLWindow &win = sora::GLWindow::GetInstance();
	LOGI("setupGraphics(%d, %d)", win.width(), win.height());  

	SceneManager::GetInstance().Update(1/30.0f);
	SceneManager::GetInstance().Draw();		
	
	sora::TextureManager::GetInstance().ProcessResponse();
	sora::GLHelper::CheckError("DrawEnd");
/*
const GLfloat gTriangleVertices[] = { 0.0f, 0.5f, -0.5f, -0.5f,
        0.5f, -0.5f };
    static float grey;
    grey += 0.01f;
    if (grey > 1.0f) {
        grey = 0.0f;
    }
    glClearColor(grey, grey, grey, 1.0f);
    checkGlError("glClearColor");
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");

    glUseProgram(gProgram);
    checkGlError("glUseProgram");

    glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, gTriangleVertices);
    checkGlError("glVertexAttribPointer");
    glEnableVertexAttribArray(gvPositionHandle);
    checkGlError("glEnableVertexAttribArray");
    glDrawArrays(GL_TRIANGLES, 0, 3);
    checkGlError("glDrawArrays");
	*/
}

extern "C" {
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_step(JNIEnv * env, jobject obj);	
	JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_onResume(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_onSuspend(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_onInit(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_onCleanup(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setApkPath(JNIEnv * env, jobject obj, jstring apkPath);
	JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_moveNextPage(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_movePrevPage(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_resetGyro(JNIEnv * env, jobject obj);
	JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_resetAccel(JNIEnv * env, jobject obj);
	JNIEXPORT jboolean JNICALL Java_com_android_gl2jni_GL2JNILib_isNextPageExist(JNIEnv * env, jobject obj);
	JNIEXPORT jboolean JNICALL Java_com_android_gl2jni_GL2JNILib_isPrevPageExist(JNIEnv * env, jobject obj);

};

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
    setupGraphics(width, height);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_step(JNIEnv * env, jobject obj)
{
    renderFrame();
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_onResume(JNIEnv * env, jobject obj) {
	LOGE("resume");
}
JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_onSuspend(JNIEnv * env, jobject obj) {
	LOGE("suspend");
}
JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_onInit(JNIEnv * env, jobject obj) {
	LOGE("on init");
}
JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_onCleanup(JNIEnv * env, jobject obj) {
	LOGE("on cleanup");
}
JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setApkPath(JNIEnv * env, jobject obj, jstring apkPath) {
	//LOGE("apk set called");
	const char* str;
	jboolean isCopy;
	str = env->GetStringUTFChars(apkPath, &isCopy);
	sora_set_apk_file_path(str);
}
JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_moveNextPage(JNIEnv * env, jobject obj) {
	LOGE("move next");
	sora_next_page();
}
JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_movePrevPage(JNIEnv * env, jobject obj) {
	LOGE("move prev");
	sora_prev_page();
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_resetGyro(JNIEnv * env, jobject obj) {
	LOGE("reset gyro");
	sora_reset_gyro();
}
JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_resetAccel(JNIEnv * env, jobject obj) {
	LOGE("reset accel");
	sora_reset_accel();
}

JNIEXPORT jboolean JNICALL Java_com_android_gl2jni_GL2JNILib_isNextPageExist(JNIEnv * env, jobject obj) {
	LOGE("is next page exist");
	if(sora_is_next_page_exist() == true) {
		return JNI_TRUE;
	} else {
		return JNI_FALSE;
	}
}
JNIEXPORT jboolean JNICALL Java_com_android_gl2jni_GL2JNILib_isPrevPageExist(JNIEnv * env, jobject obj) {
	LOGE("is prev page exist");
	if(sora_is_prev_page_exist() == true) {
		return JNI_TRUE;
	} else {
		return JNI_FALSE;
	}
}