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
#ifndef ANDROID_NDK
            m_display(EGL_NO_DISPLAY),
            m_context(EGL_NO_CONTEXT),
            m_surface(EGL_NO_SURFACE),
#endif
			m_width(0),
			m_height(0),
            m_bgred(0.0f),
            m_bggreen(0.0f),
            m_bgblue(0.0f),
            m_bgalpha(0.5f),
            m_fovy(60.0f),
            m_znear(1.0f),
            m_zfar(1000.0f),
            m_cameras(NULL),
            m_fog(NULL),
            m_light(NULL) {
#ifdef DEBUG
        printf("World constructor...\n");
#endif
        setCameraCount(1);
        setActiveCamera(0);
    }

    World::~World() {
        deinitEGL();

        DELETEANDNULL(m_cameras, true);
        DELETEANDNULL(m_fog, false);
        DELETEANDNULL(m_light, false);
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

    // camera functions
    void World::setCameraCount(int cameraCount) {
        if (m_cameras != NULL) {
            delete[] m_cameras;
            m_cameras = NULL;
        }

        if (cameraCount > 0) {
#ifdef DEBUG
            printf("World create [%d] cameras ...\n", cameraCount);
#endif
            m_cameraCount = cameraCount;
            // create meshs
            m_cameras = new Camera[m_cameraCount];
        }
    }

    int World::getCameraCount() {
        return m_cameraCount;
    }

    Camera *World::getCamera(int cameraIndex) {
        return &m_cameras[cameraIndex];
    }

    void World::setActiveCamera(int cameraIndex) {
        m_cameraIndex = cameraIndex;
    }

    Camera *World::getActiveCamera() {
        return &m_cameras[m_cameraIndex];
    }

    void World::setFog(Fog* fog) {
        m_fog = fog;
    }

    void World::setLight(Light* light) {
        m_light = light;
    }

    bool World::initEGL() {
#ifndef ANDROID_NDK
        EGLint config_nums = -1;
        EGLint maj_ver;
        EGLint min_ver;
        EGLConfig config;
#ifdef _WIN32_WCE
		EGLint config_attribs[] = {
			EGL_RED_SIZE,		5,
			EGL_GREEN_SIZE, 	6,
			EGL_BLUE_SIZE,		5,
			EGL_DEPTH_SIZE,     16,
			EGL_BUFFER_SIZE,	16,
			EGL_ALPHA_SIZE,     EGL_DONT_CARE,
			EGL_STENCIL_SIZE,   EGL_DONT_CARE,
			EGL_SURFACE_TYPE,   EGL_WINDOW_BIT,
            EGL_NONE
        };
#elif defined(WIN32)
		EGLint config_attribs[] = {
			EGL_RED_SIZE,		8,
			EGL_GREEN_SIZE, 	8,
			EGL_BLUE_SIZE,		8,
			EGL_DEPTH_SIZE, 	16,
			EGL_ALPHA_SIZE, 	EGL_DONT_CARE,
			EGL_STENCIL_SIZE,	EGL_DONT_CARE,
			EGL_SURFACE_TYPE,	EGL_WINDOW_BIT,
            EGL_NONE
        };
#else
		EGLint config_attribs[] = {
            EGL_RED_SIZE,       5,
            EGL_GREEN_SIZE,     6,
            EGL_BLUE_SIZE,      5,
			EGL_DEPTH_SIZE,    16,
			EGL_STENCIL_SIZE,   0,
            EGL_NONE
        };
#endif

#ifdef USE_WRAPPER_GL
        int result = Utils::initGlWrapper();
        if (!result) {
#ifdef DEBUG
            MessageBox(0, TEXT("Import EGL & GL functions error!\nAnd ensure you have a GPU!"), TEXT("Error"), MB_OK);
#endif
            return false;
        }
#endif // !USE_WRAPPER_GL

        //start init EGL
#if (defined(WIN32) || defined(_WIN32_WCE)) && defined(IS_DC_EGL)
        HDC hdc = GetDC( m_hwnd );
        m_display = eglGetDisplay( (NativeDisplayType)hdc );
#else
        m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
#endif
#ifndef USE_VINCENT
        if (!checkEglError("eglGetDisplay"))
            return false;
#endif

        eglInitialize(m_display, &maj_ver, &min_ver);
#ifdef DEBUG
        printf("maj_ver: %d, min_ver: %d\n", maj_ver, min_ver);
	#if (defined(WIN32) || defined(_WIN32_WCE))
		TCHAR infoStr[1024];

		wsprintf(infoStr, TEXT("maj_ver: %d, min_ver: %d"), maj_ver, min_ver);
		MessageBox(m_hwnd, infoStr, TEXT("EGL Info"), MB_OK);
	#endif
#endif
        if (!checkEglError("eglInitialize"))
            return false;

        eglGetConfigs(m_display, NULL, 0, &config_nums);
#ifdef DEBUG
        printf("config_nums: %d\n", config_nums);
	#if (defined(WIN32) || defined(_WIN32_WCE))
		wsprintf(infoStr, TEXT("config_nums: %d"), config_nums);
		MessageBox(m_hwnd, infoStr, TEXT("EGL Info"), MB_OK);
	#endif
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

        eglQuerySurface(m_display, m_surface, EGL_WIDTH, &m_width);
        eglQuerySurface(m_display, m_surface, EGL_HEIGHT, &m_height);

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

	#if (defined(WIN32) || defined(_WIN32_WCE))
		wsprintf(infoStr, TEXT("m_width: %d, m_height: %d"), m_width, m_height);
		MessageBox(m_hwnd, infoStr, TEXT("EGL Info"), MB_OK);

        CHAR infoChr[1024];
        sprintf(infoChr, "EGL_VENDOR: %s\nEGL_VERSION: %s\nEGL_EXTENSIONS: %s",
            eglQueryString(m_display, EGL_VENDOR),
            eglQueryString(m_display, EGL_VERSION),
            eglQueryString(m_display, EGL_EXTENSIONS));

        Utils::asciiToWchar(infoStr, infoChr);
        MessageBox(m_hwnd, infoStr, TEXT("EGL  Info"), MB_OK);
	#endif
#endif

        checkEglError("eglAll");
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

        // set some init status
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glShadeModel(GL_SMOOTH);//GL_SMOOTH,GL_FLAT
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_FRONT);

        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //GL_FASTEST GL_NICEST

#ifdef DEBUG
	#if (defined(WIN32) || defined(_WIN32_WCE))
        TCHAR infoStr[1024];
        CHAR infoChr[1024];
        sprintf(infoChr, "GL_VENDOR: %s\nGL_RENDERER: %s\nGL_VERSION: %s\nGL_EXTENSIONS: %s",
            glGetString(GL_VENDOR),
            glGetString(GL_RENDERER),
            glGetString(GL_VERSION),
            glGetString(GL_EXTENSIONS));

        Utils::asciiToWchar(infoStr, infoChr);
        MessageBox(m_hwnd, infoStr, TEXT("GL Info"), MB_OK);
	#endif

        printf("********GL info********\n");
        printf("GL_VENDOR\t: %s\n", glGetString(GL_VENDOR));
        printf("GL_RENDERER\t: %s\n", glGetString(GL_RENDERER));
        printf("GL_VERSION\t: %s\n", glGetString(GL_VERSION));
        printf("GL_EXTENSIONS\t: %s\n", glGetString(GL_EXTENSIONS));
#endif
        return true;
    }

    void World::deinitEGL() {
#ifndef ANDROID_NDK
        eglMakeCurrent(m_display, NULL, NULL, NULL);
        eglDestroyContext(m_display, m_context);
        eglDestroySurface(m_display, m_surface);
        eglTerminate(m_display);
#endif

#ifdef USE_WRAPPER_GL
		Utils:: deinitGlWrapper();
#endif //USE_WRAPPER_GL
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

        #if (defined(WIN32) || defined(_WIN32_WCE))
            MessageBox(hwnd, TEXT("Init EGL error!"), TEXT("Error"), MB_OK);
        #endif
#endif // !DEBUG
            return false;
        }

#if defined(DEBUG) && (defined(WIN32) || defined(_WIN32_WCE))
		MessageBox(hwnd, TEXT("Init EGL ok, start init GL!"), TEXT("Info"), MB_OK);
#endif

        initGL();

        return true;
    }

    void World::setSize(int width, int height) {
        m_width = width;
        m_height = height;
    }

	void World::resize(int width, int height) {
		setSize(width, height);
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
#ifndef ANDROID_NDK
        unsigned err = eglGetError();

        if (err != EGL_SUCCESS) {
#ifdef DEBUG
            printf("%s() error: 0x%4x!\n", name, err);
	#if (defined(WIN32) || defined(_WIN32_WCE))
			TCHAR errorStr[512];

			wsprintf(errorStr, TEXT("%s() error: 0x%4x!"), name, err);
			MessageBox(m_hwnd, errorStr, TEXT("GL Info"), MB_OK);
	#endif /* defined(WIN32) || defined(_WIN32_WCE) */
#endif /* DEBUG */
            return false;
        } else {
            return true;
        }
#else
        return true;
#endif // !ANDROID_NDK
    }

    void World::prepareRender() {
        // Set the screen background color.
        glClearColor(m_bgred, m_bggreen, m_bgblue, m_bgalpha);
		glClearDepthf(1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glLoadIdentity();

        m_cameras[m_cameraIndex].gluLookAt();

        if (m_fog != NULL) {
            glEnable(GL_FOG);
            m_fog->initFog();
        } else
            glDisable(GL_FOG);

        if (m_light != NULL) {
            glEnable(GL_LIGHTING);
            glEnable(GL_NORMALIZE);
            glEnable(GL_COLOR_MATERIAL);
            m_light->initLight();
        } else {
            glDisable(GL_LIGHTING);
            glDisable(GL_NORMALIZE);
            glDisable(GL_COLOR_MATERIAL);
        }
    }

    void World::finishRender() {
#ifndef ANDROID_NDK
        eglSwapBuffers(m_display, m_surface);
#endif
    }

}
