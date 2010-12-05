LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := f3d_jni

LOCAL_CFLAGS := -DANDROID_NDK -I../../include
#-I/cygdrive/d/android-ndk-r4/samples/forget3d/jni/include

LOCAL_SRC_FILES := stars.cpp lights.cpp md2viewer.cpp

LOCAL_LDLIBS := -L../../lib -lf3d_ndk -lc -lGLESv1_CM -llog

include $(BUILD_SHARED_LIBRARY)
