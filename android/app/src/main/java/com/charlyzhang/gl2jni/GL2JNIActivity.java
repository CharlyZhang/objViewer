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

package com.charlyzhang.gl2jni;

import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.FrameLayout;

public class GL2JNIActivity extends Activity {

    Button addButton;
    GL2JNIView gl2jniView;
    FrameLayout mFrameLayout;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);
        addButton = (Button) this.findViewById(R.id.main_act_btn);
        mFrameLayout = (FrameLayout) this.findViewById(R.id.content);
        // 初始化GL2JNIView
        gl2jniView = new GL2JNIView(GL2JNIActivity.this);

        // 设置点击监听
        addButton.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                // 点击按钮时将GL2JNIView添加到布局并开始加载3D
                mFrameLayout.addView(gl2jniView);
                gl2jniView.startGLESView();
                gl2jniView.setRenderModeDirty();
            }
        });
    }

}
