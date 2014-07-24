LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := ElysiumEngine
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../src/ $(LOCAL_PATH)/../../src/Engine
FILES := $(wildcard $(LOCAL_PATH)/../wee/*.cpp) $(wildcard $(LOCAL_PATH)/../../src/TinyXML2/*.cpp) $(wildcard $(LOCAL_PATH)/../../src/Engine/*.cpp) $(wildcard $(LOCAL_PATH)/../../src/Maths/*.cpp) $(wildcard $(LOCAL_PATH)/../../src/InputSystem/*.cpp) $(wildcard $(LOCAL_PATH)/../../src/WindowingSystem/*.cpp) $(wildcard $(LOCAL_PATH)/../../src/PhysicsSystem/*.cpp)  $(wildcard $(LOCAL_PATH)/../../src/GraphicsSystem/*.cpp)
LOCAL_SRC_FILES := $(FILES:$(LOCAL_PATH)/%=%)

include $(BUILD_SHARED_LIBRARY)
