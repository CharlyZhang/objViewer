package com.charlyzhang.gl2jni;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Environment;

import java.io.File;

/**
 * Created by charlyzhang on 16/3/2.
 */
public class BitmapService {
    public static Bitmap getImageFromSD(String path) {
        Bitmap bitmap = null;

        if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) {
            File mFile = new File(path);
            if (mFile.exists()) {
                try {
                    bitmap = BitmapFactory.decodeFile(path);
                } catch (Exception e) {

                }
            }
        }

        return bitmap;
    }
}
