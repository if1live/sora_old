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

include $(CLEAR_VARS)

LOCAL_C_INCLUDES := ../sdk/boost	\
	../sdk	\
	../	\
	../sdk/libpng	\
	../sdk/tinyxml

LOCAL_MODULE    := libgl2jni
LOCAL_CFLAGS    := -Werror	-std=gnu++0x
LOCAL_LDLIBS    := -llog -lGLESv2
LOCAL_CPP_EXTENSION := .cpp .cc
LOCAL_SRC_FILES := gl_code.cc	\
	../arch.cpp	\
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
	../logger.cpp	\
	../low_level_c_file.cpp	\
	../math_helper.cpp	\
	../matrix_stack.cpp	\
	../mem.cpp	\
	../memory_file.cpp	\
	../scene_manager.cpp	\
	../shader.cpp	\
	../sprite_sheet_manager.cpp	\
	../texture.cpp	\
	../texture_info.cpp	\
	../texture_manager.cpp	\
	../touch.cpp	\
	../ui_component.cpp	\
	../ui_container.cpp	\
	../ui_drawer.cpp	\
	../xml_node.cpp	\
	../xml_reader.cpp

include $(BUILD_SHARED_LIBRARY)



include $(CLEAR_VARS)
LOCAL_MODULE    := libgl2jni-tmp
LOCAL_SRC_FILES := 
LOCAL_LDLIBS += -landroid
LOCAL_STATIC_LIBRARIES := libgl2jni
include $(BUILD_SHARED_LIBRARY)