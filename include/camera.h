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

#ifndef F3D_CAMERA_H_
#define F3D_CAMERA_H_

#include "f3d.h"

namespace F3D {
    /**
     * Camera class for all games using F3D.
     */
    class Camera {
    private:
        GLfloat m_eyex;
        GLfloat m_eyey;
        GLfloat m_eyez;
        GLfloat m_centerx;
        GLfloat m_centery;
        GLfloat m_centerz;
        GLfloat m_upx;
        GLfloat m_upy;
        GLfloat m_upz;
        //private functions
        void crossf(float v0[3], float v1[3], float r[3]);
        void normalizef(float v[3]);
        void identf(GLfloat m[]);
    public:
        /**
        * Constructor
        */
        Camera();

        /**
         * Destructor
         */
        virtual ~Camera();

        void setEye(GLfloat eyex, GLfloat eyey, GLfloat eyez);
        void setCenter(GLfloat centerx, GLfloat centery, GLfloat centerz);
        void setUp(GLfloat upx, GLfloat upy, GLfloat upz);
        void gluLookAt();
    };

}

#endif /* F3D_CAMERA_H_ */
