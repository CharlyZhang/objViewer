package com.android.gl2jni.gestures;

import android.content.Context;
import android.os.SystemClock;
import android.util.FloatMath;
import android.util.Log;
import android.view.MotionEvent;
import android.view.VelocityTracker;
import android.view.ViewConfiguration;

import java.util.Scanner;

public class CupcakeGestureDetector implements GestureDetector {

    protected boolean DEBUG = false;
    protected OnGestureListener mListener;
    private static final String LOG_TAG = "CupcakeGestureDetector";
    float mLastTouchX;
    float mLastTouchY;
    float mLastTouchX1;
    float mLastTouchY1;
    final float mTouchSlop;
    final float mMinimumVelocity;
    /** 手指按下的个数 */
    protected int mFingerNum;
    long[] mHits = new long[4];
    /** 双击最小时间间隔 */
    private long exitMinTime =500;
    /** 两个手指按下时,两个手指的距离 */
    private float mOldDis = 0.0f;

    float[] mScales = new float[6];

    @Override
    public void setOnGestureListener(OnGestureListener listener) {
        this.mListener = listener;
    }

    public CupcakeGestureDetector(Context context) {
        final ViewConfiguration configuration = ViewConfiguration
                .get(context);
        mMinimumVelocity = configuration.getScaledMinimumFlingVelocity();
        mTouchSlop = configuration.getScaledTouchSlop();
    }

    private boolean mIsDragging;

    float getActiveX(MotionEvent ev) {
        return ev.getX();
    }

    float getActiveY(MotionEvent ev) {
        return ev.getY();
    }

    public boolean isScaling() {
        return false;
    }

    @Override
    public boolean onTouchEvent(MotionEvent ev) {
        switch (ev.getAction() & MotionEvent.ACTION_MASK) {
            case MotionEvent.ACTION_DOWN: {
                // 一根手指按下
                mLastTouchX = getActiveX(ev);
                mLastTouchY = getActiveY(ev);
                mIsDragging = false;
                mFingerNum = 1;
                break;
            }

            case MotionEvent.ACTION_MOVE: {
                final float x = getActiveX(ev);
                final float y = getActiveY(ev);
                final float dx = x - mLastTouchX, dy = y - mLastTouchY;

                if(mFingerNum == 2) {
                    float newDist = spacing(ev);

                    System.arraycopy(mScales, 1, mScales, 0, mScales.length - 1);
                    mScales[mScales.length - 1] = newDist - mOldDis;
                    if(mScales[0] != 0) {
                        float sum = 0;
                        for(int i = 0; i<mScales.length; i++){
                            sum += mScales[i];
                        }
                        float ave = sum / mScales.length;
                        if(DEBUG) {
                            Log.i(LOG_TAG, "ACTION_MOVE: ave:" + ave);
                        }
                        if(Math.abs(ave) > 4) {
                            mListener.isScale(true);
                        } else {
                            mListener.isScale(false);
                        }
                        mListener.hasSetIsScale(true);
                    }

                    mOldDis = newDist;
                }

                if (!mIsDragging) {
                    // Use Pythagoras to see if drag length is larger than
                    // touch slop
                    mIsDragging = FloatMath.sqrt((dx * dx) + (dy * dy)) >= mTouchSlop;
                }

                if (mIsDragging) {

                	if(mFingerNum == 2) {
                        final float x1 = ev.getX(1);
                        final float y1 = ev.getY(1);
                        final float dx1 = x1 - mLastTouchX1, dy1 = y1 - mLastTouchY1;

                		mListener.onDrag(false, (dx+dx1)/2, (dy+dy1)/2);
                        mLastTouchX1 = ev.getX(1);
                        mLastTouchY1 = ev.getY(1);
                	} else {
                		mListener.onDrag(true, dx, dy);
                	}
                    mLastTouchX = x;
                    mLastTouchY = y;
                }
                break;
            }

            case MotionEvent.ACTION_UP: {
                // 所有的手指抬起
                mFingerNum = 0;
                mListener.hasSetIsScale(false);
                mListener.isScale(false);
                mScales = new float[6];
                break;
            }
            case MotionEvent.ACTION_POINTER_UP: {
                // 多个手指按下时一个抬起

                // 识别连续的两次点击
                System.arraycopy(mHits, 1, mHits, 0, mHits.length - 1);
                mHits[mHits.length - 1] = SystemClock.uptimeMillis();
                if (mHits[0] >= (SystemClock.uptimeMillis() - exitMinTime)) {
                    mListener.onDoubleTapUp();
                }
                mFingerNum -= 1;
                break;
            }
            case MotionEvent.ACTION_POINTER_DOWN: {
                // 多个手指按下
                mLastTouchX1 = ev.getX(1);
                mLastTouchY1 = ev.getY(1);
                mOldDis = spacing(ev);
                mFingerNum++;
                System.arraycopy(mHits, 1, mHits, 0, mHits.length - 1);
                mHits[mHits.length - 1] = SystemClock.uptimeMillis();
                break;
            }
        }

        if(DEBUG) {
            Log.i(LOG_TAG, "ev.getAction():" + (ev.getAction() & MotionEvent.ACTION_MASK));
        }
        return true;
    }

    /**
     * 计算两个手指之间的距离
     * @param event
     * @return
     */
    private float spacing(MotionEvent event) {
        float x = event.getX(0) - event.getX(1);
        float y = event.getY(0) - event.getY(1);
        return (float)Math.sqrt(x * x + y * y);
    }
}
