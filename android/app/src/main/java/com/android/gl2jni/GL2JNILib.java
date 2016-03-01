/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.gl2jni;

// Wrapper for native library

import android.content.res.AssetManager;

public class GL2JNILib {

    static {
        System.loadLibrary("Application3D");
    }


    public static native void init(AssetManager assetManager);
    public static native void loadObjModel(String filename, boolean quickLoad);
    public static native void setRenderBufferSize(int w, int h);
    public static native void frame();
    public static native void reset();

    public static native void setDocDirectory(String docDir);

    // control
    //	/note : (deltaX,deltaY) is in the screen coordinate system
    public static native void rotate(float deltaX, float deltaY);
    public static native void translate(float deltaX, float deltaY);
    public static native void scale(float s);

    // custom config
    public static native void setBackgroundColor(float r, float g, float b, float a);
    public static native void setModelColor(float r, float g, float b, float a);

    // camera
    public static native void setCameraPosition(float x, float y, float z);

    // light
    public static native void setLightPosition(float x, float y, float z);   ///< TO DEPRECATED
    public static native void setLigthDirection(float x, float y, float z);
    public static native void setAmbientColor(boolean r, boolean g, boolean b);
    public static native void setDiffuseColor(boolean r, boolean g, boolean b);

    // texture
    public static native void enableTexture(boolean flag);

}
