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
	../sdk/libpng	\
	../sdk/tinyxml

LOCAL_MODULE    := sora
LOCAL_CFLAGS    := -Werror	-std=gnu++0x
LOCAL_LDLIBS    := 
LOCAL_SRC_FILES := \
	../logger.cpp	\
	../arch.cpp	\
	../xml_node.cpp	\
	../xml_reader.cpp	\
	../basic_shader.cpp	\
	../button.cpp	\
	../clock.cpp	\
	../common_string.cpp	\
	../filesystem.cpp	\
	../gl_helper.cpp	\
	../gl_inc.cpp	\
	../gl_window.cpp	\
	../image_label.cpp	\
	../immediate_mode_emulator.cpp	\
	../locale.cpp	\
	../low_level_c_file.cpp	\
	../math_helper.cpp	\
	../matrix_stack.cpp	\
	../mem.cpp	\
	../memory_file.cpp	\
	../scene_manager.cpp	\
	../shader.cpp	\
	../sprite_sheet_manager.cpp	\
	../texture.cpp	\
	../texture_atlas.cpp	\
	../texture_info.cpp	\
	../texture_manager.cpp	\
	../touch.cpp	\
	../ui_component.cpp	\
	../ui_container.cpp	\
	../ui_drawer.cpp	\

include $(BUILD_STATIC_LIBRARY)



include $(CLEAR_VARS)
LOCAL_MODULE    := sora-tmp
LOCAL_SRC_FILES := 
LOCAL_STATIC_LIBRARIES := sora
include $(BUILD_SHARED_LIBRARY)

#$(call import-module,boost)
#include $(START_PATH)/../../sdk/tinyxml/jni/Android.mk
#include $(START_PATH)/../../sdk/libpng/jni/Android.mk