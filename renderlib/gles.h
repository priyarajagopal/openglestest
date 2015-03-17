#pragma once

#ifdef __ANDROID_API__
#include <GLES2/gl2.h>
#elif defined(OPENGL_OPENGLES2)
#include <OpenGLES/ES2/gl.h>
#endif