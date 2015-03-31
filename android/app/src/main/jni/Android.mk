LOCAL_PATH := $(call my-dir)
RENDERLIB_PATH := ../../../../../renderlib
PREBUILTLIB_PATH := ../3rdparty/lib/$(TARGET_ARCH_ABI)

include $(CLEAR_VARS)
LOCAL_MODULE := openctm
LOCAL_C_INCLUDES := $(RENDERLIB_PATH)/openctm/liblzma
LOCAL_SRC_FILES :=  $(wildcard $(RENDERLIB_PATH)/openctm/*.c) \
                    $(wildcard $(RENDERLIB_PATH)/openctm/liblzma/*.c)
                    
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := crypto
LOCAL_SRC_FILES := $(PREBUILTLIB_PATH)/libcrypto.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := ssl
LOCAL_SRC_FILES := $(PREBUILTLIB_PATH)/libssl.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := curl
LOCAL_SRC_FILES := $(PREBUILTLIB_PATH)/libcurl.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := renderlib
#LOCAL_CFLAGS := -Wall -Wextra
LOCAL_C_INCLUDES := $(RENDERLIB_PATH) $(RENDERLIB_PATH)/openctm
LOCAL_SRC_FILES :=  $(wildcard $(RENDERLIB_PATH)/*.cpp) \
                    JNIInterface.cpp
                    
LOCAL_STATIC_LIBRARIES := openctm
LOCAL_SHARED_LIBRARIES := crypto ssl curl
LOCAL_LDLIBS := -lGLESv2 -llog
LOCAL_CPP_FEATURES := exceptions
include $(BUILD_SHARED_LIBRARY)