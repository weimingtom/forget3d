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

#include "light.h"

namespace F3D {
    /**
     * Light class for all games using F3D.
     */

    Light::Light() :
            m_position(NULL),
            m_ambient(NULL),
            m_diffuse(NULL),
            m_specular(NULL),
            m_emission(NULL) {
        //
    }

    Light::~Light() {
#ifdef DEBUG
        printf("Light destructor...\n");
#endif
    }

    void Light::setPosition(GLfloat* position) {
        m_position = position;
    }

    void Light::setAmbient(GLfloat* ambient) {
        m_ambient = ambient;
    }

    void Light::setDiffuse(GLfloat* diffuse) {
        m_diffuse = diffuse;
    }

    void Light::setSpecular(GLfloat* specular) {
        m_specular = specular;
    }

    void Light::setEmission(GLfloat* emission) {
        m_emission = emission;
    }

    void Light::initLight() {
        glEnable(GL_LIGHT0);

        if (m_position != NULL)
            glLightfv(GL_LIGHT0, GL_POSITION, m_position);

        if (m_ambient != NULL)
            glLightfv(GL_LIGHT0, GL_AMBIENT, m_ambient);

        if (m_diffuse != NULL)
            glLightfv(GL_LIGHT0, GL_DIFFUSE, m_diffuse);

        if (m_specular != NULL)
            glLightfv(GL_LIGHT0, GL_SPECULAR, m_specular);

        if (m_emission != NULL)
            glLightfv(GL_LIGHT0, GL_EMISSION, m_emission);
    }

}
