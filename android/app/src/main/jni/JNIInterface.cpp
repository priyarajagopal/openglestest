#include "com_invicara_androidtest_JNIInterface.h"
#include "viewer.h"

using namespace renderlib;

Viewer* viewer = 0;

JNIEXPORT void JNICALL Java_com_invicara_androidtest_JNIInterface_on_1surface_1created(JNIEnv *, jclass) 
{
    if (viewer)
        delete viewer;
    viewer = new Viewer();
    viewer->init();
}

JNIEXPORT void JNICALL Java_com_invicara_androidtest_JNIInterface_on_1surface_1changed(JNIEnv *, jclass, jint width, jint height)
{
    if (viewer)
        viewer->set_viewport(0, 0, width, height);
}

JNIEXPORT void JNICALL Java_com_invicara_androidtest_JNIInterface_on_1draw_1frame(JNIEnv *, jclass) 
{
    if (viewer)
        viewer->draw();
}
