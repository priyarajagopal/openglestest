LOCAL_PATH := $(call my-dir)
RENDERLIB_PATH := ../../../../../renderlib

include $(CLEAR_VARS)
LOCAL_MODULE := libcrypto
LOCAL_SRC_FILES := ../3rdparty/lib/$(TARGET_ARCH_ABI)/libcrypto.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libssl
LOCAL_SRC_FILES := ../3rdparty/lib/$(TARGET_ARCH_ABI)/libssl.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libcurl
LOCAL_SRC_FILES := ../3rdparty/lib/$(TARGET_ARCH_ABI)/libcurl.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := renderlib
#LOCAL_CFLAGS    := -Wall -Wextra
LOCAL_C_INCLUDES := $(RENDERLIB_PATH)
LOCAL_SRC_FILES := $(wildcard $(RENDERLIB_PATH)/*.cpp) \
                    JNIInterface.cpp
LOCAL_SHARED_LIBRARIES := libcrypto libssl libcurl
LOCAL_LDLIBS := -lGLESv2 -llog
LOCAL_CPP_FEATURES := exceptions

include $(BUILD_SHARED_LIBRARY)