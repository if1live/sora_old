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
LOCAL_PATH:= $(call my-dir)
START_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := ../sdk/boost	\
	../sdk	\
	../	\
	../sora	\
	../yukino	\
	../sdk/libpng	\
	../sdk/tinyxml	\
	../sdk/libzip/jni

LOCAL_MODULE    := yukino
LOCAL_CFLAGS    := -Werror	-std=gnu++0x
LOCAL_LDLIBS    := -llog  -landroid -lEGL -lGLESv2 -lz
#-lGLESv1_CM
LOCAL_CPP_EXTENSION := .cpp .cc
LOCAL_STATIC_LIBRARIES := android_native_app_glue sora tinyxml libpng boost_thread libzip
LOCAL_SRC_FILES := gl_code.cc \
	../book.cpp	\
	../book_paper.cpp	\
	../book_paper_builder.cpp	\
	../book_scene.cpp	\
	../camera.cpp	\
	../glassless3d.cpp	\
	../lang_button_selector.cpp	\
	../logic_interface.cpp	\
	../main_scene.cpp	\
	
include $(BUILD_SHARED_LIBRARY)


include $(START_PATH)/../../sora/jni/Android.mk
include $(START_PATH)/../../sdk/libpng/jni/Android.mk
include $(START_PATH)/../../sdk/tinyxml/jni/Android.mk
include $(START_PATH)/../../sdk/libzip/jni/Android.mk

$(call import-module,boost)
