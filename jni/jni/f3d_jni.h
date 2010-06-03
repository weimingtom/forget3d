/*****************************************************************************
 * Copyright (C) 2009 The Forget3D Project by Martin Foo (i25ffz@hotmail.com)
 * ALL RIGHTS RESERVED
 *
 * License I
 * Permission to use, copy, modify, and distribute this software for
 * any purpose and WITHOUT a fee is granted under following requirements:
 * - You make no money using this software.
 * - The authors and/or this software is credited in your software or any
 *   work based on this software.
 *
 * Licence II
 * Permission to use, copy, modify, and distribute this software for
 * any purpose and WITH a fee is granted under following requirements:
 * - As soon as you make money using this software, you have to pay a
 *   licence fee. Until this point of time, you can use this software
 *   without a fee.
 * 	 Please contact Martin Foo (i25ffz@hotmail.com) for further details.
 * - The authors and/or this software is credited in your software or any
 *   work based on this software.
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL THE AUTHORS
 * BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT, SPECIAL, INCIDENTAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER,
 * INCLUDING WITHOUT LIMITATION, LOSS OF PROFIT, LOSS OF USE, SAVINGS OR
 * REVENUE, OR THE CLAIMS OF THIRD PARTIES, WHETHER OR NOT THE AUTHORS HAVE
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 *****************************************************************************/

#ifndef F3D_JNI_H_
#define F3D_JNI_H_

/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_forget3d_demo_F3DRenderer */

#include <sys/time.h>
#include <android/log.h>

#include "f3d.h"
#include "world.h"
#include "camera.h"
#include "font.h"
#include "light.h"

using namespace F3D;

//F3D variables
static World* 	world = NULL;
static Camera*	camera = NULL;
static Font*	font = NULL;
static Image*	image = NULL;
static Texture*	texture = NULL;
static Light*   light = NULL;

static char	strFps[16];
static int	fps = 0;
static int  is_done = 0;
static int  interval = 0;
static int  i_time = 0;
static struct timeval timeNow;
static int	width = 320;
static int	height = 480;
static int  is_initialized = false;
static int  is_twinkled = 1;
static GLfloat zoom = -15.0f;   /* Viewing Distance Away From Stars */
static GLfloat tilt = 90.0f;    /* Tilt The View */
static int is_lighted = 1;
static GLfloat rotation = 0.0f;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     com_forget3d_demo_F3DStarsRenderer
 * Method:    f3dStarsInit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_forget3d_demo_F3DStarsRenderer_f3dStarsInit
  (JNIEnv *, jclass);

/*
 * Class:     com_forget3d_demo_F3DStarsRenderer
 * Method:    f3dStarsResize
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_forget3d_demo_F3DStarsRenderer_f3dStarsResize
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     com_forget3d_demo_F3DStarsRenderer
 * Method:    f3dStarsRender
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_forget3d_demo_F3DStarsRenderer_f3dStarsRender
  (JNIEnv *, jclass);

/*
 * Class:     com_forget3d_demo_F3DStarsRenderer
 * Method:    f3dStarsDone
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_forget3d_demo_F3DStarsRenderer_f3dStarsDone
  (JNIEnv *, jclass);

/*
 * Class:     com_forget3d_demo_F3DStarsRenderer
 * Method:    f3dStarsPause
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_forget3d_demo_F3DStarsRenderer_f3dStarsPause
  (JNIEnv *, jclass);

/*
 * Class:     com_forget3d_demo_F3DStarsRenderer
 * Method:    f3dStarsTwinkled
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_forget3d_demo_F3DStarsRenderer_f3dStarsTwinkled
  (JNIEnv *, jclass);

/*
 * Class:     com_forget3d_demo_F3DLightsRenderer
 * Method:    f3dLightsInit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_forget3d_demo_F3DLightsRenderer_f3dLightsInit
  (JNIEnv *, jclass);

/*
 * Class:     com_forget3d_demo_F3DLightsRenderer
 * Method:    f3dLightsResize
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_forget3d_demo_F3DLightsRenderer_f3dLightsResize
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     com_forget3d_demo_F3DLightsRenderer
 * Method:    f3dLightsRender
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_forget3d_demo_F3DLightsRenderer_f3dLightsRender
  (JNIEnv *, jclass);

/*
 * Class:     com_forget3d_demo_F3DLightsRenderer
 * Method:    f3dLightsDone
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_forget3d_demo_F3DLightsRenderer_f3dLightsDone
  (JNIEnv *, jclass);

/*
 * Class:     com_forget3d_demo_F3DLightsRenderer
 * Method:    f3dLightsPause
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_forget3d_demo_F3DLightsRenderer_f3dLightsPause
  (JNIEnv *, jclass);

/*
 * Class:     com_forget3d_demo_F3DLightsRenderer
 * Method:    f3dLightsTouch
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_forget3d_demo_F3DLightsRenderer_f3dLightsTouch
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif

#endif // !F3D_JNI_H_
