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
LOCAL_CXXFLAGS := --std=gnu++0x -W

LOCAL_MODULE    := sora

LOCAL_SRC_FILES := \
	../cbes/component.cpp	\
	../cbes/component_list.cpp	\
	../cbes/entity.cpp	\
	../cbes/world.cpp
	
LOCAL_SRC_FILES += 	\
	../core/arch.cpp	\
	../core/common_string.cpp	\
	../core/free_list_block_pool.cpp	\
	../core/heap.cpp	\
	../core/mem.cpp	\
	../core/timer.cpp	\
	../core/math_helper.cpp	\
	../core/logger.cpp	\
	../core/string_hash.cpp
	
LOCAL_SRC_FILES += 	\
	../event/touch_device.cpp	\
	../event/touch_event.cpp
	
LOCAL_SRC_FILES += \
	../lodepng/lodepng.cpp	\
	
LOCAL_SRC_FILES += \
	../soil/image_DXT.cpp	\
	../soil/image_helper.cpp	\
	../soil/SOIL.cpp	\
	../soil/stb_image_aug.cpp

LOCAL_SRC_FILES += \
	../renderer/gl_helper.cpp	\
	../renderer/gl_inc.cpp	\
	../renderer/shader.cpp	\
	../renderer/renderer.cpp	\
	../renderer/obj_loader.cpp	\
	../renderer/obj_model.cpp	\
	../renderer/primitive_model.cpp	\
	../renderer/texture.cpp	\
	../renderer/gl_buffer_object.cpp	\
	../renderer/material_manager.cpp	\
	../renderer/material.cpp	\
	../renderer/texture_manager.cpp	\
	../renderer/matrix_stack.cpp	\
	../renderer/font.cpp	\
	../renderer/shader_variable.cpp	\
	../renderer/camera.cpp	\
	../renderer/uber_shader.cpp	\
	../renderer/shader_bind_policy.cpp	\
	../renderer/mesh.cpp	\
	../renderer/mesh_manager.cpp	\
	../renderer/parametric_surface.cpp	\
	../renderer/render_state.cpp	\
	../renderer/selection_buffer.cpp
	
LOCAL_SRC_FILES += \
	../sys/sys_locale.cpp	\
	../sys/filesystem.cpp	\
	../sys/low_level_c_file.cpp	\
	../sys/memory_file.cpp	\
	../sys/zip_stream_file.cpp	\
	../sys/device.cpp
	
LOCAL_SRC_FILES += \
	../sora_main.cpp	\
	../depth_map_main.cpp	\
	../shadow_map_main.cpp	\
	../selection_main.cpp	\
	../celshading_main.cpp	\
	

#include $(BUILD_SHARED_LIBRARY)
include $(BUILD_STATIC_LIBRARY)

#########################
include $(CLEAR_VARS)
LOCAL_MODULE    := sora-tmp
LOCAL_SRC_FILES := 
LOCAL_STATIC_LIBRARIES := sora
include $(BUILD_SHARED_LIBRARY)


include $(START_PATH)/../../sdk/libzip/jni/Android.mk