#include "viewer.h"

using namespace renderlib;

#include <android/log.h>

#define APPNAME "renderLib"

Viewer::Viewer()
{
}


Viewer::~Viewer()
{
}

void Viewer::init()
{
	__android_log_write(ANDROID_LOG_ERROR, APPNAME, "init");
	renderer_.init();
}

void Viewer::set_viewport(int x, int y, int w, int h)
{
	__android_log_print(ANDROID_LOG_ERROR, APPNAME, "set_viewport %d %d %d %d", x, y, w, h);
	viewport_x = x;
	viewport_y = y;
	viewport_w = w;
	viewport_h = h;
	glViewport(viewport_x, viewport_y, viewport_w, viewport_h);
	camera_.set_aspect(viewport_w, viewport_h);
}

void Viewer::draw()
{
	renderer_.render(camera_);
}