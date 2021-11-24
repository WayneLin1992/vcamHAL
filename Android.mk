LOCAL_PATH := $(call my-dir)

v4l2_shared_libs := \
    liblog \
    libutils \
    libcutils \
    libcamera_client \
    libui \
    libjpeg \
    libcamera_metadata \
    libskia \
    libandroid_runtime

v4l2_static_libs := \
  libyuv_static \
  libjpeg_static_ndk \

v4l2_cflags := -fno-short-enums -Wall -Wextra -Werror -fvisibility=hidden -DHAVE_JPEG

v4l2_c_includes := $(call include-path-for, camera) \
  external/libyuv/files/include \

v4l2_src_files := \
    vcamHAL.cpp \
    camera.cpp \
    v4l2_camera.cpp \
    v4l2_wrapper.cpp \

include $(CLEAR_VARS)
LOCAL_MODULE := camera.$(TARGET_BOARD_PLATFORM)
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_CFLAGS += $(v4l2_cflags)
LOCAL_SHARED_LIBRARIES := $(v4l2_shared_libs)
LOCAL_HEADER_LIBRARIES := libgtest_prod_headers
LOCAL_STATIC_LIBRARIES := $(v4l2_static_libs)

LOCAL_C_INCLUDES += $(v4l2_c_includes)
LOCAL_SRC_FILES := $(v4l2_src_files)
include $(BUILD_SHARED_LIBRARY)