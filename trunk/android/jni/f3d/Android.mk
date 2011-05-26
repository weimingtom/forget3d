LOCAL_PATH := $(call my-dir)

###########################
#
# F3D static library
#
###########################

include $(CLEAR_VARS)

LOCAL_MODULE := f3d_ndk

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../include
LOCAL_CFLAGS := -DANDROID_NDK

LOCAL_SRC_FILES := \
	$(subst $(LOCAL_PATH)/,, \
	$(wildcard $(LOCAL_PATH)/../../../src/*.cpp))

include $(BUILD_STATIC_LIBRARY)
