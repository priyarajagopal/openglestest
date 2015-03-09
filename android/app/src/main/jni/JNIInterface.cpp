#include "com_invicara_androidtest_JNIInterface.h"
#include "../../../../../renderlib/renderlib.h"

JNIEXPORT void JNICALL Java_com_invicara_androidtest_JNIInterface_on_1surface_1created
  (JNIEnv *, jclass) {
    on_surface_created();
}

JNIEXPORT void JNICALL Java_com_invicara_androidtest_JNIInterface_on_1surface_1changed
  (JNIEnv *, jclass, jint width, jint height){
    on_surface_changed(width, height);
}

JNIEXPORT void JNICALL Java_com_invicara_androidtest_JNIInterface_on_1draw_1frame
  (JNIEnv *, jclass) {
    on_draw_frame();
}
