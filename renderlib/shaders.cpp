#include "shaders.h"

const char VERTEX_SHADER[] = 
"uniform mat4 uMVPMatrix;"
"attribute vec3 aPosition;"
"attribute vec4 aColor;"
"" 
"varying vec4 vColor;"
"\n"
"void main()"
"{"
"	vColor = aColor;"
"	gl_Position = uMVPMatrix * vec4(aPosition, 1.0);"
"}";

const char FRAGMENT_SHADER[] =
"varying vec4 vColor;"
"\n"
"void main()"
"{"
"	gl_FragColor = vColor;"
"}";
