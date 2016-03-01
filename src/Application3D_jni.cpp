

#include <jni.h>
#include "Application3D.h"

Application3D app3d;

extern "C" {
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj,  jstring sceneFilename);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_loadObjModel(JNIEnv * env, jobject obj, jstring filename, jboolean quickLoad);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setRenderBufferSize(JNIEnv * env, jobject obj, jint w, jint h);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_frame(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_reset(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setDocDirectory(JNIEnv * env, jobject obj, jstring docDir);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setGLSLDirectory(JNIEnv * env, jobject obj, jstring glslDir);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_rotate(JNIEnv * env, jobject obj, jfloat deltaX, jfloat deltaY);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_translate(JNIEnv * env, jobject obj, jfloat deltaX, jfloat deltaY);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_scale(JNIEnv * env, jobject obj, jfloat s);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setBackgroundColor(JNIEnv * env, jobject obj, jfloat r, jfloat g, jfloat b, jfloat a);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setModelColor(JNIEnv * env, jobject obj, jfloat r, jfloat g, jfloat b, jfloat a);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setCameraPosition(JNIEnv * env, jobject obj, jfloat x, jfloat y, jfloat z);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setLightPosition(JNIEnv * env, jobject obj, jfloat x, jfloat y, jfloat z);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setLigthDirection(JNIEnv * env, jobject obj, jfloat x, jfloat y, jfloat z);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setAmbientColor(JNIEnv * env, jobject obj, jboolean r, jboolean g, jboolean b);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setDiffuseColor(JNIEnv * env, jobject obj, jboolean r, jboolean g, jboolean b);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_enableTexture(JNIEnv * env, jobject obj, jboolean flag);

};

char* jstringTostring(JNIEnv* env, jstring jstr)
{        
    char* rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("utf-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0)
    {
        rtn = (char*)malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->DeleteLocalRef(strencode);
    env->ReleaseByteArrayElements(barr, ba, 0);

    return rtn;
}

/////

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj,  jstring sceneFilename)
{
    char *sceneFile = jstringTostring(env, sceneFilename);
    app3d.init(sceneFile);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_loadObjModel(JNIEnv * env, jobject obj, jstring filename, jboolean quickLoad)
{
    char *file = jstringTostring(env, filename);
    bool b = quickLoad;
    app3d.loadObjModel(file, b);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setRenderBufferSize(JNIEnv * env, jobject obj, jint w, jint h)
{
    app3d.setRenderBufferSize(w,h);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_frame(JNIEnv * env, jobject obj)
{
    app3d.frame();
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_reset(JNIEnv * env, jobject obj)
{
    app3d.reset();
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setDocDirectory(JNIEnv * env, jobject obj, jstring docDir)
{
    char *doc = jstringTostring(env, docDir);
    app3d.setDocDirectory(doc);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setGLSLDirectory(JNIEnv * env, jobject obj, jstring glslDir)
{
    char *glsl = jstringTostring(env,glslDir);
    app3d.setGLSLDirectory(glsl);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_rotate(JNIEnv * env, jobject obj, jfloat deltaX, jfloat deltaY)
{
    app3d.rotate(deltaX,deltaY);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_translate(JNIEnv * env, jobject obj, jfloat deltaX, jfloat deltaY)
{
    app3d.translate(deltaX,deltaY);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_scale(JNIEnv * env, jobject obj, jfloat s)
{
    app3d.scale(s);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setBackgroundColor(JNIEnv * env, jobject obj, jfloat r, jfloat g, jfloat b, jfloat a)
{
    app3d.setBackgroundColor(r,g,b,a);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setModelColor(JNIEnv * env, jobject obj, jfloat r, jfloat g, jfloat b, jfloat a)
{
    app3d.setModelColor(r,g,b,a);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setCameraPosition(JNIEnv * env, jobject obj, jfloat x, jfloat y, jfloat z)
{
    app3d.setCameraPosition(x,y,z);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setLightPosition(JNIEnv * env, jobject obj, jfloat x, jfloat y, jfloat z)
{
    app3d.setLightPosition(x,y,z);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setLigthDirection(JNIEnv * env, jobject obj, jfloat x, jfloat y, jfloat z)
{
    app3d.setLigthDirection(x,y,z);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setAmbientColor(JNIEnv * env, jobject obj, jboolean r, jboolean g, jboolean b)
{
    app3d.setAmbientColor(r,g,b);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_setDiffuseColor(JNIEnv * env, jobject obj, jboolean r, jboolean g, jboolean b)
{
    app3d.setDiffuseColor(r,g,b);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_enableTexture(JNIEnv * env, jobject obj, jboolean flag)
{
}

