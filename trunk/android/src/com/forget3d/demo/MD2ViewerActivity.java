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

public class MD2ViewerActivity extends Activity {
	private F3DMS2ViewerGLSurfaceView mGLView;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
        // fullscreen mode
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                   WindowManager.LayoutParams.FLAG_FULLSCREEN);		
		
		mGLView = new F3DMS2ViewerGLSurfaceView(this);
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

class F3DMS2ViewerGLSurfaceView extends GLSurfaceView {
	private F3DMD2ViewerRenderer mRenderer;
	
	public F3DMS2ViewerGLSurfaceView(Context context) {
		super(context);
		
		mRenderer = new F3DMD2ViewerRenderer();
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

class F3DMD2ViewerRenderer implements GLSurfaceView.Renderer {
	
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		f3dMD2ViewerInit();
	}

	public void onSurfaceChanged(GL10 gl, int w, int h) {
		f3dMD2ViewerResize(w, h);
	}

	public void onDrawFrame(GL10 gl) {
		f3dMD2ViewerRender();
	}
	
	public void quit() {
		f3dMD2ViewerDone();
	}

	public void touch() {
		f3dMD2ViewerTouch();
	}
	
	public void pause() {
		f3dMD2ViewerPause();
	}
	
	private static native void f3dMD2ViewerInit();

	private static native void f3dMD2ViewerResize(int w, int h);

	private static native void f3dMD2ViewerRender();

	private static native void f3dMD2ViewerDone();
	
	private static native void f3dMD2ViewerPause();
	
	private static native void f3dMD2ViewerTouch();
}
