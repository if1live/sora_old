# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)
START_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := \
	..	\
	.	\
	../sora	\
	../libzip/jni	\
	
LOCAL_CFLAGS := 

LOCAL_MODULE    := sora
LOCAL_SRC_FILES := \
	../arch.cpp	\
	../common_string.cpp	\
	../timer.cpp	\
	../math_helper.cpp	\
	../matrix.cpp	\
	../logger.cpp	\
	../mem.cpp	\
	../vector.cpp	\
	../gl_helper.cpp	\
	../gl_inc.cpp	\
	../sys_locale.cpp	\
	../shader.cpp	\
	../matrix_stack.cpp	\
	../filesystem.cpp	\
	../low_level_c_file.cpp	\
	../memory_file.cpp	\
	../zip_stream_file.cpp	\
	../sora_main.cpp

#include $(BUILD_SHARED_LIBRARY)
include $(BUILD_STATIC_LIBRARY)

#########################
include $(CLEAR_VARS)
LOCAL_MODULE    := sora-tmp
LOCAL_SRC_FILES := 
LOCAL_STATIC_LIBRARIES := sora
include $(BUILD_SHARED_LIBRARY)


include $(START_PATH)/../../libzip/jni/Android.mk