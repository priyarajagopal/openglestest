package com.invicara.androidtest;

/**
 * Created by INVSG010 on 3/9/2015.
 */
public class JNIInterface {
    static {
        System.loadLibrary("renderlib");
    }

    public static native void on_surface_created();

    public static native void on_surface_changed(int width, int height);

    public static native void on_draw_frame();
}
