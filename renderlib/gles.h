#pragma once

#ifdef OPENGL_OPENGLES2
#include <OpenGLES/ES2/gl.h>
#endif

#ifdef __ANDROID_API__
#include <GLES2/gl2.h>
//#define GL_GLEXT_PROTOTYPES
//#include <GLES2/gl2ext.h>
#endif

#ifdef OPENGL_GLEW
#include <GL/glew.h>
#endif
