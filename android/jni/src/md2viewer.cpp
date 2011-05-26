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

#include "f3d_jni.h"

/*
 * Class:     com_forget3d_demo_F3DMD2ViewerRenderer
 * Method:    f3dMD2ViewerInit
 * Signature: ()V
 */
void Java_com_forget3d_demo_F3DMD2ViewerRenderer_f3dMD2ViewerInit(JNIEnv* env, jclass cls) {
	__android_log_print(ANDROID_LOG_INFO, "Forget3D", "world->init()...");

	World::release();
    world = World::getInstance();
    world->init();

    camera = world->getActiveCamera();
    camera->setEye(60.0f, 15.0f, 60.0f);

    float fogColor[] = { 0.9f, 0.9f, 0.9f, 1.0f };
    fog = new Fog();
    fog->setFogColor(fogColor);
    fog->setFogStart(-5.0f);
    fog->setFogEnd(5.0f);
    fog->setFogDensity(0.004f);

    world->setFog(fog);

    Texture* texture0 = Image::loadTexture("/sdcard/tris.bmp");
    Texture* texture1 = Image::loadTexture("/sdcard/weapon.bmp");
    Texture* texture2 = Image::loadTexture("/sdcard/floor.bmp");
    Texture* texture3 = Image::loadTexture("/sdcard/clouds.bmp");

    model = new ModelMD2();
    model->loadModel("/sdcard/tris.md2");
    model->setActionIndex(action_idx);
    if (texture0 != NULL)
        model->setTextureId(texture0->textureId);

    weapon = new ModelMD2();
    weapon->loadModel("/sdcard/weapon.md2");
    weapon->setActionIndex(action_idx);
    if (texture1 != NULL)
        weapon->setTextureId(texture1->textureId);

    plane = new Plane(4, 4, 128.0f);
    if (texture2 != NULL)
        plane->setTextureId(texture2->textureId);
    plane->setPosition(-256.0f, -28.0f, -256.0f);

    skydome = new Skydome(256, 30.0f, 10.0f);
    if (texture3 != NULL)
        skydome->setTextureId(texture3->textureId);
    skydome->setPosition(0.0f, (float)(-256 * sinf(DTOR * 10.0f)) - 28.0f, 0.0f);

    font = new Font(16, 16, 24, 36, "/sdcard/font.bmp");

	__android_log_print(ANDROID_LOG_INFO, "Forget3D", "model->getActionName(%d): %s\n", action_idx, model->getActionName(action_idx));

	__android_log_print(ANDROID_LOG_INFO, "Forget3D", "start loop...");
    is_done = 1;
    is_initialized = true;
    rotation = 0.0f;

    gettimeofday(&timeNow, NULL);
    i_time = CLOCK(timeNow);
    sprintf(strFps, "Fps:%.2f", 0.0f);
    __android_log_print(ANDROID_LOG_INFO, "Forget3D", "strFps: %s\n", strFps);
}

/*
 * Class:     com_forget3d_demo_F3DMD2ViewerRenderer
 * Method:    f3dMD2ViewerResize
 * Signature: (II)V
 */
void Java_com_forget3d_demo_F3DMD2ViewerRenderer_f3dMD2ViewerResize(JNIEnv* env, jclass cls, jint w, jint h) {
    __android_log_print(ANDROID_LOG_INFO, "Forget3D", "call f3dStarsResize...");

	width = w;
	height = h;

	if (is_initialized) {
		world->resize(width, height);
	}
}

/*
 * Class:     com_forget3d_demo_F3DMD2ViewerRenderer
 * Method:    f3dMD2ViewerRender
 * Signature: ()V
 */
void Java_com_forget3d_demo_F3DMD2ViewerRenderer_f3dMD2ViewerRender(JNIEnv* env, jclass cls) {
	if (!is_done)
		return;

    __android_log_print(ANDROID_LOG_INFO, "Forget3D", "call f3dStarsRender...");

    world->prepareRender();

    skydome->setRotate(-90.0f, 0.0f, rotation);
    skydome->renderModel();
    plane->renderModel();

    model->renderModel();
    weapon->renderModel();

    //printf("strFps: %s\n", strFps);
    font->drawString(4, 4, strFps);

    //draw action info
    int w_height = world->getHeight();
    int f_height = font->getFonHeight();
    font->drawString(4, w_height - f_height - 4, strAction);

    world->finishRender();

    fps++;
    rotation += 0.1f;

	gettimeofday(&timeNow, NULL);
	interval = CLOCK(timeNow) - i_time;

	if (((CLOCK(timeNow) - i_time) / 1000) % 2 == 0 && interval > 0) {
		sprintf(strFps, "Fps:%.2f", fps * 1000.0f / interval);
		__android_log_print(ANDROID_LOG_INFO, "Forget3D", "%s", strFps);
	}
}

/*
 * Class:     com_forget3d_demo_F3DMD2ViewerRenderer
 * Method:    f3dMD2ViewerDone
 * Signature: ()V
 */
void Java_com_forget3d_demo_F3DMD2ViewerRenderer_f3dMD2ViewerDone(JNIEnv* env, jclass cls) {
    __android_log_print(ANDROID_LOG_INFO, "Forget3D", "call f3dStarsDone...");

    DELETEANDNULL(model, false);
    DELETEANDNULL(weapon, false);
    DELETEANDNULL(font, false);
    DELETEANDNULL(plane, false);
    DELETEANDNULL(skydome, false);
    World::release();
}
/*
 * Class:     com_forget3d_demo_F3DMD2ViewerRenderer
 * Method:    f3dMD2ViewerPause
 * Signature: ()V
 */
void Java_com_forget3d_demo_F3DMD2ViewerRenderer_f3dMD2ViewerPause(JNIEnv* env, jclass cls) {
    __android_log_print(ANDROID_LOG_INFO, "Forget3D", "call f3dStarsPause...");

	is_done = !is_done;
}

/*
 * Class:     com_forget3d_demo_F3DMD2ViewerRenderer
 * Method:    f3dMD2ViewerTouch
 * Signature: ()V
 */
void Java_com_forget3d_demo_F3DMD2ViewerRenderer_f3dMD2ViewerTouch(JNIEnv* env, jclass cls) {
    __android_log_print(ANDROID_LOG_INFO, "Forget3D", "call f3dMD2ViewerTouch...");
    //change model action index
    action_idx++;
    if (action_idx >= (int)model->getActionCount())
        action_idx = 0;
    //go to next action
    model->setActionIndex(action_idx);
    weapon->setActionIndex(action_idx);
    //enable or disable fog
    is_foged = !is_foged;
    if (is_foged)
        world->setFog(fog);
    else
        world->setFog(NULL);
}
