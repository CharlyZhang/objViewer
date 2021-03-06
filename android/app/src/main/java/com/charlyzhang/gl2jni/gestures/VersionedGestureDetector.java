package com.charlyzhang.gl2jni.gestures;

import android.content.Context;
import android.os.Build;

public final class VersionedGestureDetector {

    public static GestureDetector newInstance(Context context,
                                              OnGestureListener listener) {
        final int sdkVersion = Build.VERSION.SDK_INT;
        GestureDetector detector;

        if (sdkVersion < Build.VERSION_CODES.ECLAIR) {
            detector = new CupcakeGestureDetector(context);
        } else {
            detector = new EclairGestureDetector(context);
        }

        detector.setOnGestureListener(listener);

        return detector;
    }

}