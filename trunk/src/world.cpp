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

#include "world.h"

namespace F3D {
    /**
     * World class for all games using F3D.
     */

    World::World() :
            m_bgred(0.0f),
            m_bggreen(0.0f),
            m_bgblue(0.0f),
            m_bgalpha(0.0f),
            m_fovy(60.0f),
            m_znear(1.0f),
            m_zfar(1000.0f),
            m_camera(NULL),
            m_fog(NULL),
            m_light(NULL),
            m_display(EGL_NO_DISPLAY),
            m_context(EGL_NO_CONTEXT),
            m_surface(EGL_NO_SURFACE) {
        //
    }

    World::~World() {
        deinitEGL();

        delete m_camera;
        delete m_fog;
#ifdef DEBUG
        printf("World destructor...\n");
#endif
    }

    void World::setBgColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
        m_bgred = red;
        m_bggreen = green;
        m_bgblue = blue;
        m_bgalpha = alpha;
    }

    void World::setCamera(Camera* camera) {
        m_camera = camera;
    }

    void World::setFog(Fog* fog) {
        m_fog = fog;
    }

    void World::setLight(Light* light) {
        m_light = light;
    }

    bool World::initEGL() {
        EGLint config_nums = -1;
        EGLint maj_ver;
        EGLint min_ver;
        EGLConfig config;
        EGLint config_attribs[] = {
#if (defined(WIN32) || defined(_WIN32_WCE))
			EGL_RED_SIZE,		8,
			EGL_GREEN_SIZE, 	8,
			EGL_BLUE_SIZE,		8,
			EGL_DEPTH_SIZE, 	16,
			EGL_ALPHA_SIZE, 	EGL_DONT_CARE,
			EGL_STENCIL_SIZE,	EGL_DONT_CARE,
			EGL_SURFACE_TYPE,	EGL_WINDOW_BIT,
#else
            EGL_RED_SIZE,       5,
            EGL_GREEN_SIZE,     6,
            EGL_BLUE_SIZE,      5,
			EGL_DEPTH_SIZE,    16,
			EGL_STENCIL_SIZE,   0,
#endif
            EGL_NONE
        };

        //start init EGL
        m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
#ifndef USE_VINCENT
        if (!checkEglError("eglGetDisplay"))
            return false;
#endif

        eglInitialize(m_display, &maj_ver, &min_ver);
#ifdef DEBUG
        printf("maj_ver: %d, min_ver: %d\n", maj_ver, min_ver);
#endif
        if (!checkEglError("eglInitialize"))
            return false;

        eglGetConfigs(m_display, NULL, 0, &config_nums);
#ifdef DEBUG
        printf("config_nums: %d\n", config_nums);
#endif
        if (!checkEglError("eglGetConfigs"))
            return false;

        eglChooseConfig(m_display, config_attribs, &config, 1, &config_nums);
        if (!checkEglError("eglChooseConfig"))
            return false;
#if (defined(WIN32) || defined(_WIN32_WCE))
        m_surface = eglCreateWindowSurface(m_display, config, m_hwnd, NULL);
#else
        m_surface = eglCreateWindowSurface(m_display, config,
                                           android_createDisplaySurface(), NULL);
#endif
        if (!checkEglError("eglCreateWindowSurface"))
            return false;

        m_context = eglCreateContext(m_display, config, NULL, NULL);
        if (!checkEglError("eglCreateContext"))
            return false;

        eglMakeCurrent(m_display, m_surface, m_surface, m_context);
        if (!checkEglError("eglMakeCurrent"))
            return false;

#if !(defined(WIN32) || defined(_WIN32_WCE))
        eglQuerySurface(m_display, m_surface, EGL_WIDTH, &m_width);
        eglQuerySurface(m_display, m_surface, EGL_HEIGHT, &m_height);
#endif

#ifdef DEBUG
        printf("********EGL info********\n");
        printf("EGL_VENDOR\t: %s\n", eglQueryString(m_display, EGL_VENDOR));
        printf("EGL_VERSION\t: %s\n", eglQueryString(m_display, EGL_VERSION));
        printf("EGL_EXTENSIONS\t: %s\n", eglQueryString(m_display, EGL_EXTENSIONS));
#ifndef USE_VINCENT
        printf("EGL_CLIENT_APIS\t: %s\n", eglQueryString(m_display, EGL_CLIENT_APIS));
#endif

        printf("m_width\t\t: %d\n", m_width);
        printf("m_height\t: %d\n", m_height);
#endif

        return true;
    }

    bool World::initGL() {
        // Initialize viewport and projection.
        glViewport( 0, 0, m_width, m_height );

        //set the matrix mode
        gluPerspective();

        //reset the matrix mode
        glMatrixMode(GL_MODELVIEW); //GL_PROJECTION,GL_MODELVIEW
        glLoadIdentity();

        // Set the screen background color.
        glClearColor(m_bgred, m_bggreen, m_bgblue, m_bgalpha);

        // set some init status
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glShadeModel(GL_SMOOTH);//GL_SMOOTH,GL_FLAT
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_FRONT);

        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //GL_FASTEST GL_NICEST

#ifdef DEBUG
        printf("********GL info********\n");
        printf("GL_VENDOR\t: %s\n", glGetString(GL_VENDOR));
        printf("GL_RENDERER\t: %s\n", glGetString(GL_RENDERER));
        printf("GL_VERSION\t: %s\n", glGetString(GL_VERSION));
        printf("GL_EXTENSIONS\t: %s\n", glGetString(GL_EXTENSIONS));
#endif
        return true;
    }

    void World::deinitEGL() {
        eglMakeCurrent(m_display, NULL, NULL, NULL);
        eglDestroyContext(m_display, m_context);
        eglDestroySurface(m_display, m_surface);
        eglTerminate(m_display);
    }

#if (defined(WIN32) || defined(_WIN32_WCE))
    bool World::init(HWND hwnd) {
		m_hwnd = hwnd;
#else
    bool World::init() {
#endif
        if (!initEGL()) {
#ifdef DEBUG
            printf("initEGL() error!\n");
    #if (!defined(ANDROID) && defined(_WIN32_WCE))
            MessageBox(hwnd, _T("Init EGL error!"), _T("Error"), MB_OK);
    #elif (!defined(ANDROID) && defined(WIN32))
            MessageBox(hwnd, "Init EGL error!", "Error", MB_OK);
    #endif

#endif
            return false;
        }

        initGL();

        return true;
    }

    void World::setSize(int width, int height) {
        m_width = width;
        m_height = height;
		//after resize screen, call initGL() again
		initGL();
    }

    int World::getWidth() {
        return m_width;
    }

    int World::getHeight() {
        return m_height;
    }

    void World::setPerspective(GLfloat fovy, GLfloat znear, GLfloat zfar) {
        m_fovy = fovy;
        m_znear = znear;
        m_zfar = zfar;
    }

    void World::gluPerspective() {
        // Start in projection mode.
        GLfloat xmin, xmax, ymin, ymax;
        GLfloat aspect = (GLfloat)m_width / (GLfloat)m_height;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        ymax = (float) (m_znear * tan(m_fovy * M_PI / 360.0));
        ymin = -ymax;
        xmin = ymin * aspect;
        xmax = ymax * aspect;

#ifdef DEBUG
        printf("glFrustumf()\t: %.3f, %.3f, %.3f, %.3f, %.3f, %.3f\n", xmin, xmax, ymin, ymax, m_znear, m_zfar);
#endif
        glFrustumf(xmin, xmax, ymin, ymax, m_znear, m_zfar);
    }

    bool World::checkEglError(const char *name) {
        unsigned err = eglGetError();

        if (err != EGL_SUCCESS) {
#ifdef DEBUG
            printf("%s() error: 0x%4x!\n", name, err);
	#if (defined(WIN32) || defined(_WIN32_WCE))
			TCHAR errWStr[32];

			wsprintf (errWStr, TEXT("%s() error: 0x%4x!\n"), name, err);
			MessageBox (m_hwnd, errWStr, TEXT("Error"), MB_OK);
	#endif /* defined(WIN32) || defined(_WIN32_WCE) */
#endif /* DEBUG */
            return false;
        } else {
            return true;
        }
    }

    void World::prepareRender() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        m_camera->gluLookAt();

        if (m_fog != NULL) {
            glEnable(GL_FOG);
            m_fog->initFog();
        }

        if (m_light != NULL) {
            glEnable(GL_LIGHTING);
            m_light->initLight();
        }
    }

    void World::finishRender() {
        eglSwapBuffers(m_display, m_surface);
    }

}