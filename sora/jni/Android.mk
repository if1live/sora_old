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
	../sdk/libpng	\
	../sdk/tinyxml	\
	../sdk/libzip/jni

LOCAL_MODULE    := sora
LOCAL_CFLAGS    := -Werror 
LOCAL_LDLIBS    := 
LOCAL_SRC_FILES := \
	../logger.cpp	\
	../arch.cpp	\
	../clock.cpp	\
	../common_string.cpp	\
	../sys_locale.cpp	\
	../math_helper.cpp	\
	../mem.cpp	\
	
LOCAL_SRC_FILES +=	\
	../io/memory_file.cpp	\
	../io/filesystem.cpp	\
	../io/xml_node.cpp	\
	../io/xml_reader.cpp	\
	../io/low_level_c_file.cpp	\
	../io/zip_stream_file.cpp
	
LOCAL_SRC_FILES +=	\
	../render/matrix_stack.cpp	\
	../render/gl_helper.cpp	\
	../render/gl_inc.cpp	\
	../render/gl_window.cpp	\
	../render/basic_shader.cpp	\
	../render/immediate_mode_emulator.cpp	\
	../render/scene_manager.cpp	\
	../render/shader.cpp	\
	../render/sprite_sheet_manager.cpp	\
	../render/texture.cpp	\
	../render/texture_atlas.cpp	\
	../render/texture_info.cpp	\
	../render/texture_manager.cpp	\
	../render/texture_helper.cpp
	

#LOCAL_SRC_FILES +=	\
#	../ui/button.cpp	\
#	../ui/image_label.cpp	\
#	../ui/touch.cpp	\
#	../ui/ui_component.cpp	\
#	../ui/ui_container.cpp	\
#	../ui/ui_drawer.cpp	\

include $(BUILD_STATIC_LIBRARY)



include $(CLEAR_VARS)
LOCAL_MODULE    := sora-tmp
LOCAL_SRC_FILES := 
LOCAL_STATIC_LIBRARIES := sora
include $(BUILD_SHARED_LIBRARY)

#$(call import-module,boost)
#include $(START_PATH)/../../sdk/tinyxml/jni/Android.mk
#include $(START_PATH)/../../sdk/libpng/jni/Android.mk
#include $(START_PATH)/../../sdk/libzip/jni/Android.mk