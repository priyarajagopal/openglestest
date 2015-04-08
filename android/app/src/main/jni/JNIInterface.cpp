#include "com_invicara_androidtest_JNIInterface.h"
#include "viewer.h"

using namespace renderlib;

Viewer* viewer = 0;

void throwJavaException(JNIEnv* env, const char* msg)
{
    jclass exClass = env->FindClass("java/lang/Error");
    if (exClass != NULL)
        env->ThrowNew(exClass, msg);
}

JNIEXPORT void JNICALL Java_com_invicara_androidtest_JNIInterface_on_1surface_1created(JNIEnv* env, jclass) 
{
    try {
        if (viewer)
            delete viewer;
        viewer = new Viewer();
        viewer->init();
        viewer->load_model("http://localhost:8081/models/SampleHouseTest/geomInfo.json");
    } catch (const std::exception& e) {
        throwJavaException(env, e.what());
    } catch (...) {
        throwJavaException(env, "Unhandled exception from renderLib");
    }    
}

JNIEXPORT void JNICALL Java_com_invicara_androidtest_JNIInterface_on_1surface_1changed(JNIEnv* env, jclass, jint width, jint height)
{
    try {
        if (viewer)
            viewer->set_viewport(0, 0, width, height);
    } catch (const std::exception& e) {
        throwJavaException(env, e.what());
    } catch (...) {
        throwJavaException(env, "Unhandled exception from renderLib");
    }            
}

JNIEXPORT void JNICALL Java_com_invicara_androidtest_JNIInterface_on_1draw_1frame(JNIEnv* env, jclass) 
{
    try {
        if (viewer)
            viewer->draw();
    } catch (const std::exception& e) {
        throwJavaException(env, e.what());
    } catch (...) {
        throwJavaException(env, "Unhandled exception from renderLib");
    }            
}
