package com.android.gl2jni.gestures;

public interface OnGestureListener {

    void onDrag(boolean single, float dx, float dy);

    void onScale(float scaleFactor, float focusX, float focusY);

    // 双手指双击
    void onDoubleTapUp();

    void isScale(boolean isScale);

    void hasSetIsScale(boolean b);
}