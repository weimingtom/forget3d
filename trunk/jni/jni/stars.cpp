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

/* Define the star structure */
typedef struct
{
    int r, g, b;   /* Stars Color */
    GLfloat dist;  /* Stars Distance From Center */
    GLfloat angle; /* Stars Current Angle */
} star;

/* Number of stars */
#define NUM 50
star stars[NUM];       /* Make an array of size 'NUM' of stars */

// draw all stars
static void drawScene() {
    GLfloat spin = 0;

    GLfloat texcoords[4][2];
    GLfloat vertices[4][3];
    GLubyte indices[4] = {1, 0, 2, 3}; /* QUAD to TRIANGLE_STRIP conversion; */

    glEnable(GL_TEXTURE_2D);

	glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE ); //GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE

    /* Select Our Texture */
    glBindTexture(GL_TEXTURE_2D, texture->textureId);

	glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	/* Set pointers to vertices and texcoords */
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, texcoords);

    /* Loop Through All The Stars */
    for (int loop = 0; loop < NUM; loop++) {
        /* Reset The View Before We Draw Each Star */
        glLoadIdentity();
        /* Zoom Into The Screen (Using The Value In 'zoom') */
        glTranslatef(0.0f, 0.0f, zoom);

        /* Tilt The View (Using The Value In 'tilt') */
        glRotatef(tilt, 1.0f, 0.0f, 0.0f);
        /* Rotate To The Current Stars Angle */
        glRotatef(stars[loop].angle, 0.0f, 1.0f, 0.0f);

        /* Move Forward On The X Plane */
        glTranslatef(stars[loop].dist, 0.0f, 0.0f);

        /* Cancel The Current Stars Angle */
        glRotatef(-stars[loop].angle, 0.0f, 1.0f, 0.0f);
        /* Cancel The Screen Tilt */
        glRotatef(-tilt, 1.0f, 0.0f, 0.0f);

        /* Twinkling Stars Enabled */
        if (is_twinkled) {
            /* Assign A Color Using Bytes */
            glColor4f((GLfloat)stars[(NUM-loop)-1].r/255.0f,
                      (GLfloat)stars[(NUM-loop)-1].g/255.0f,
                      (GLfloat)stars[(NUM-loop)-1].b/255.0f, 0.8f);

            /* Fill texture and vertex arrays */
            texcoords[0][0]=0.0f; texcoords[0][1]=0.0f;
            vertices[0][0]=-1.0f; vertices[0][1]=-1.0f; vertices[0][2]=0.0f;
            texcoords[1][0]=1.0f; texcoords[1][1]=0.0f;
            vertices[1][0]=1.0f; vertices[1][1]=-1.0f; vertices[1][2]=0.0f;
            texcoords[2][0]=1.0f; texcoords[2][1]=1.0f;
            vertices[2][0]=1.0f; vertices[2][1]=1.0f; vertices[2][2]=0.0f;
            texcoords[3][0]=0.0f; texcoords[3][1]=1.0f;
            vertices[3][0]=-1.0f; vertices[3][1]=1.0f; vertices[3][2]=0.0f;

            /* Draw one textured plane using two stripped triangles */
            glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);
        }

        /* Rotate The Star On The Z Axis */
        glRotatef(spin, 0.0f, 0.0f, 1.0f);

        /* Assign A Color Using Bytes */
        glColor4f((GLfloat)stars[loop].r/255.0f,
                  (GLfloat)stars[loop].g/255.0f,
                  (GLfloat)stars[loop].b/255.0f, 0.8f);

        /* Fill texture and vertex arrays */
        texcoords[0][0]=0.0f; texcoords[0][1]=0.0f;
        vertices[0][0]=-1.0f; vertices[0][1]=-1.0f; vertices[0][2]=0.0f;
        texcoords[1][0]=1.0f; texcoords[1][1]=0.0f;
        vertices[1][0]=1.0f; vertices[1][1]=-1.0f; vertices[1][2]=0.0f;
        texcoords[2][0]=1.0f; texcoords[2][1]=1.0f;
        vertices[2][0]=1.0f; vertices[2][1]=1.0f; vertices[2][2]=0.0f;
        texcoords[3][0]=0.0f; texcoords[3][1]=1.0f;
        vertices[3][0]=-1.0f; vertices[3][1]=1.0f; vertices[3][2]=0.0f;

        /* Draw one textured plane using two stripped triangles */
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

        /* Used To Spin The Stars */
        spin += 0.01f;
        /* Changes The Angle Of A Star */
        stars[loop].angle+=(float)loop/NUM;
        /* Changes The Distance Of A Star */
        stars[loop].dist-=0.01f;

        /* Is The Star In The Middle Yet */
        if (stars[loop].dist<0.0f) {
            /* Move The Star 5 Units From The Center */
            stars[loop].dist+=5.0f;
            /* Give It A New Red Value */
            stars[loop].r=rand()%256;
            /* Give It A New Green Value */
            stars[loop].g=rand()%256;
            /* Give It A New Blue Value */
            stars[loop].b=rand()%256;
        }
    }

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

/*
 * Class:     com_forget3d_demo_F3DStarsRenderer
 * Method:    f3dStarsInit
 * Signature: ()V
 */
void JNICALL Java_com_forget3d_demo_F3DStarsRenderer_f3dStarsInit(JNIEnv* env, jclass cls) {
	__android_log_print(ANDROID_LOG_INFO, "Forget3D", "world->init()...");

	DELETEANDNULL(world, false);
    world = new World();
    world->init();

    texture = Image::loadTexture("/sdcard/star.bmp");
	__android_log_print(ANDROID_LOG_INFO, "Forget3D", "load texture: %s ...", "/sdcard/star.bmp");

	DELETEANDNULL(camera, false);
    camera = new Camera();
    camera->setEye(0.0f, 0.0f, 15.0f);

    world->setCamera(camera);

	/* Create A Loop That Goes Through All The Stars */
    for (int loop=0; loop < NUM; loop++) {
        /* Start All The Stars At Angle Zero */
        stars[loop].angle=0.0f;

        /* Calculate Distance From The Center */
        stars[loop].dist=((float)loop/NUM)*5.0f;
        /* Give star[loop] A Random Red Intensity */
        stars[loop].r=rand() % 256;
        /* Give star[loop] A Random Green Intensity */
        stars[loop].g=rand() % 256;
        /* Give star[loop] A Random Blue Intensity */
        stars[loop].b=rand() % 256;
    }

	DELETEANDNULL(font, false);
    font = new Font(16, 16, 24, 36, "/sdcard/font.bmp"); // /sdcard/game/forget3d/font.bmp, /dev/sample/font.bmp
	__android_log_print(ANDROID_LOG_INFO, "Forget3D", "create Font: %s ...", "/sdcard/font.bmp");

	is_initialized = true;
	is_done = 1;

	__android_log_print(ANDROID_LOG_INFO, "Forget3D", "start loop...");
    is_done = 1;

    gettimeofday(&timeNow, NULL);
    i_time = CLOCK(timeNow);
    sprintf(strFps, "Fps:%.2f", 0.0f);
    printf("strFps: %s\n", strFps);
}

/*
 * Class:     com_forget3d_demo_F3DStarsRenderer
 * Method:    f3dStarsResize
 * Signature: (II)V
 */
void JNICALL Java_com_forget3d_demo_F3DStarsRenderer_f3dStarsResize(JNIEnv* env, jclass cls, jint w, jint h) {
    __android_log_print(ANDROID_LOG_INFO, "Forget3D", "call f3dStarsResize...");
	width = w;
	height = h;

	if (is_initialized) {
		world->resize(width, height);
	}
}

/*
 * Class:     com_forget3d_demo_F3DStarsRenderer
 * Method:    f3dStarsRender
 * Signature: ()V
 */
void JNICALL Java_com_forget3d_demo_F3DStarsRenderer_f3dStarsRender(JNIEnv* env, jclass cls) {
    __android_log_print(ANDROID_LOG_INFO, "Forget3D", "call f3dStarsRender...");
	if (!is_done)
		return;

	world->prepareRender();

	drawScene();

	//printf("strFps: %s\n", strFps);
	font->drawString(4, 4, strFps);

//	world->finishRender();

	fps++;

	gettimeofday(&timeNow, NULL);
	interval = CLOCK(timeNow) - i_time;

	if (((CLOCK(timeNow) - i_time) / 1000) % 2 == 0 && interval > 0) {
		sprintf(strFps, "Fps:%.2f", fps * 1000.0f / interval);
		__android_log_print(ANDROID_LOG_INFO, "Forget3D", "%s", strFps);
	}
}

/*
 * Class:     com_forget3d_demo_F3DStarsRenderer
 * Method:    f3dStarsDone
 * Signature: ()V
 */
void JNICALL Java_com_forget3d_demo_F3DStarsRenderer_f3dStarsDone(JNIEnv* env, jclass cls) {
    __android_log_print(ANDROID_LOG_INFO, "Forget3D", "call f3dStarsDone...");

	if (font != NULL)
		delete font;

	if (world != NULL)
		delete world;
}

/*
 * Class:     com_forget3d_demo_F3DStarsRenderer
 * Method:    f3dStarsPause
 * Signature: ()V
 */
void JNICALL Java_com_forget3d_demo_F3DStarsRenderer_f3dStarsPause(JNIEnv* env, jclass cls) {
    __android_log_print(ANDROID_LOG_INFO, "Forget3D", "call f3dStarsPause...");

	is_done = !is_done;
}

/*
 * Class:     com_forget3d_demo_F3DStarsRenderer
 * Method:    f3dStarsTwinkled
 * Signature: ()V
 */
void JNICALL Java_com_forget3d_demo_F3DStarsRenderer_f3dStarsTwinkled(JNIEnv* env, jclass cls) {
    __android_log_print(ANDROID_LOG_INFO, "Forget3D", "call f3dStarsTwinkled...");

	is_twinkled = !is_twinkled;
}
