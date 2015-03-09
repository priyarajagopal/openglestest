package com.invicara.androidtest;

import android.opengl.GLSurfaceView.Renderer;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Created by INVSG010 on 3/5/2015.
 */
public class NativeRenderer implements Renderer {
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        JNIInterface.on_surface_created();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        JNIInterface.on_surface_changed(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        JNIInterface.on_draw_frame();
    }
}
