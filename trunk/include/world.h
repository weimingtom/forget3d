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

#ifndef F3D_WORLD_H_
#define F3D_WORLD_H_

#include "f3d.h"
#include "camera.h"
#include "fog.h"
#include "light.h"
#include "utils.h"

namespace F3D {

    /**
     * World class for all games using F3D.
     */
    class World {
    private:
        //egl variables
#ifndef ANDROID_NDK
        EGLDisplay  m_display;
        EGLContext  m_context;
        EGLSurface  m_surface;
#endif
        int         m_width;
        int         m_height;
        GLfloat     m_bgred;
        GLfloat     m_bggreen;
        GLfloat     m_bgblue;
        GLfloat     m_bgalpha;
        GLfloat     m_fovy;
        GLfloat     m_znear;
        GLfloat     m_zfar;
        //multiple camera support
        int         m_cameraCount;
        int         m_cameraIndex;
        Camera      *m_cameras;
        Fog         *m_fog;
        Light       *m_light;
#if (defined(WIN32) || defined(_WIN32_WCE))
		HWND        m_hwnd;
#endif

        //private functions
        void gluPerspective();
		bool initEGL();
        bool initGL();
        void deinitEGL();
        bool checkEglError(const char *name);
    public:
        /**
        * Constructor
        */
        World();

        /**
         * Destructor
         */
        virtual ~World();

        // camera functions
        void setCameraCount(int cameraCount);
        int getCameraCount();
        Camera *getCamera(int cameraIndex);
        void setActiveCamera(int cameraIndex);
        Camera *getActiveCamera();

        void setFog(Fog* fog);
        void setLight(Light* light);
        void setBgColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
#if (defined(WIN32) || defined(_WIN32_WCE))
		bool init(HWND hWnd);
#else
		bool init();
#endif
        void setSize(int width, int height);
		void resize(int width, int height);
        void setPerspective(GLfloat fovy, GLfloat znear, GLfloat zfar);
        void prepareRender();
        void finishRender();
        int getWidth();
        int getHeight();
    };

}

#endif /* F3D_WORLD_H_ */
