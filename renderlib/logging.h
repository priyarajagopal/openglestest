#pragma once

#ifdef __ANDROID_API__
#include <android/log.h>

#define APPNAME "renderLib"
#define  LOG(...)  __android_log_print(ANDROID_LOG_INFO, APPNAME, __VA_ARGS__)
#define  LOGW(...)  __android_log_write(ANDROID_LOG_INFO, APPNAME, __VA_ARGS__)
#else
#include <stdio.h>

#define  LOG(...) fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n");
#define  LOGW(...) fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n");
#endif