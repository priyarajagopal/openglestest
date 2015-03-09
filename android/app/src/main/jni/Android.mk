LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := renderlib
LOCAL_CFLAGS    := -Wall -Wextra
LOCAL_SRC_FILES := ../../../../../renderlib/renderlib.cpp JNIInterface.cpp
LOCAL_LDLIBS := -lGLESv2

include $(BUILD_SHARED_LIBRARY)