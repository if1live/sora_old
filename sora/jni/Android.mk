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
	../sdk/libzip/jni	\
	../sdk/glm-0.9.3.1	\
	../../boost_1_49_0	\
	../sdk/vincent
	
LOCAL_CFLAGS := 
LOCAL_CXXFLAGS := --std=gnu++0x

LOCAL_MODULE    := sora
LOCAL_SRC_FILES := \
	../arch.cpp	\
	../common_string.cpp	\
	../timer.cpp	\
	../math_helper.cpp	\
	../logger.cpp	\
	../mem.cpp	\
	../vector.cpp	\
	../gl_helper.cpp	\
	../gl_inc.cpp	\
	../sys_locale.cpp	\
	../shader.cpp	\
	../filesystem.cpp	\
	../low_level_c_file.cpp	\
	../memory_file.cpp	\
	../zip_stream_file.cpp	\
	../sora_main.cpp	\
	../renderer.cpp	\
	../obj_loader.cpp	\
	../obj_model.cpp	\
	../primitive_model.cpp	\
	../texture.cpp	\
	../lodepng/lodepng.cpp	\
	../gl_buffer_object.cpp	\
	../material_manager.cpp	\
	../texture_manager.cpp	\
	../matrix_stack.cpp	\
	../font.cpp	\
	../shader_variable.cpp	\
	../camera.cpp	\
	../component.cpp	\
	../component_list.cpp	\
	../entity.cpp	\
	../mesh_component.cpp	\
	../world.cpp	\
	../uber_shader.cpp

#include $(BUILD_SHARED_LIBRARY)
include $(BUILD_STATIC_LIBRARY)

#########################
include $(CLEAR_VARS)
LOCAL_MODULE    := sora-tmp
LOCAL_SRC_FILES := 
LOCAL_STATIC_LIBRARIES := sora
include $(BUILD_SHARED_LIBRARY)


include $(START_PATH)/../../sdk/libzip/jni/Android.mk