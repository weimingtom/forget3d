LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := f3d_jni

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../include
LOCAL_CFLAGS := -DANDROID_NDK

LOCAL_SRC_FILES := stars.cpp lights.cpp md2viewer.cpp

LOCAL_STATIC_LIBRARIES := f3d_ndk

LOCAL_LDLIBS := -lGLESv1_CM -llog

include $(BUILD_SHARED_LIBRARY)
