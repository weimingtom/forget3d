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

public class StarsActivity extends Activity {
	private F3DStarsGLSurfaceView mGLView;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
        // fullscreen mode
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                   WindowManager.LayoutParams.FLAG_FULLSCREEN);		
		
		mGLView = new F3DStarsGLSurfaceView(this);
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

class F3DStarsGLSurfaceView extends GLSurfaceView {
	private F3DStarsRenderer mRenderer;
	
	public F3DStarsGLSurfaceView(Context context) {
		super(context);
		
		mRenderer = new F3DStarsRenderer();
		setRenderer(mRenderer);
	}

	public boolean onTouchEvent(final MotionEvent event) {
		if (event.getAction() == MotionEvent.ACTION_DOWN) {
			mRenderer.twinkle();
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

class F3DStarsRenderer implements GLSurfaceView.Renderer {
	
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		f3dStarsInit();
	}

	public void onSurfaceChanged(GL10 gl, int w, int h) {
		f3dStarsResize(w, h);
	}

	public void onDrawFrame(GL10 gl) {
		f3dStarsRender();
	}
	
	public void quit() {
		f3dStarsDone();
	}

	public void twinkle() {
		f3dStarsTwinkled();
	}
	
	public void pause() {
		f3dStarsPause();
	}
	
	private static native void f3dStarsInit();

	private static native void f3dStarsResize(int w, int h);

	private static native void f3dStarsRender();

	private static native void f3dStarsDone();
	
	private static native void f3dStarsPause();
	
	private static native void f3dStarsTwinkled();
}
