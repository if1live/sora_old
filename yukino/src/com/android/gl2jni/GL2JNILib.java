/*
 * Copyright (C) 2007 The Android Open Source Project
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

package com.android.gl2jni;

// Wrapper for native library

public class GL2JNILib {

     static {
         System.loadLibrary("yukino");
     }

    /**
     * @param width the current view width
     * @param height the current view height
     */
     public static native void init(int width, int height);
     public static native void step();
     
     // sora scene으로 연결되는 함수
     public static native void onResume();
     public static native void onSuspend();
     public static native void onInit();
     public static native void onCleanup();
     public static native void setApkPath(String path);
     
     // sora book로 연결되는 함수
     //특정 페이지로의 이동은 안쓰더라
     //public static native void setBookPage(int index);
     public static native void moveNextPage();
     public static native void movePrevPage();
     public static native boolean isNextPageExist();
     public static native boolean isPrevPageExist();
     public static native void resetGyro();
     public static native void resetAccel();
}
