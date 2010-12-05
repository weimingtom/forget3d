package com.forget3d.demo;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.app.Activity;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.Window;
import android.view.WindowManager;

public class LightsActivity extends Activity {
	private F3DLightsGLSurfaceView mGLView;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
        // fullscreen mode
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                   WindowManager.LayoutParams.FLAG_FULLSCREEN);		
		
		mGLView = new F3DLightsGLSurfaceView(this);
		setContentView(mGLView);
	}

	@Override
	protected void onPause() {
		super.onPause();
		mGLView.onPause();
		mGLView.pause();
	}

	@Override
	protected void onResume() {
		super.onResume();
		mGLView.onResume();
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		mGLView.onResume();
	}
	
}

class F3DLightsGLSurfaceView extends GLSurfaceView {
	private F3DLightsRenderer mRenderer;
	
	public F3DLightsGLSurfaceView(Context context) {
		super(context);
		
		mRenderer = new F3DLightsRenderer();
		setRenderer(mRenderer);
	}

	public boolean onTouchEvent(final MotionEvent event) {
		if (event.getAction() == MotionEvent.ACTION_DOWN) {
			mRenderer.touch();
		}
		
		return true;
	}
	
	public void quit() {
		mRenderer.quit();
	}
	
	public void pause() {
		mRenderer.pause();
	}
	
}

class F3DLightsRenderer implements GLSurfaceView.Renderer {
	
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		f3dLightsInit();
	}

	public void onSurfaceChanged(GL10 gl, int w, int h) {
		f3dLightsResize(w, h);
	}

	public void onDrawFrame(GL10 gl) {
		f3dLightsRender();
	}
	
	public void quit() {
		f3dLightsDone();
	}

	public void touch() {
		f3dLightsTouch();
	}
	
	public void pause() {
		f3dLightsPause();
	}
	
	private static native void f3dLightsInit();

	private static native void f3dLightsResize(int w, int h);

	private static native void f3dLightsRender();

	private static native void f3dLightsDone();
	
	private static native void f3dLightsPause();
	
	private static native void f3dLightsTouch();
}
