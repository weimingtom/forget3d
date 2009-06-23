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

#include "fog.h"

namespace F3D {
    /**
     * Fog class for all games using F3D.
     */

    Fog::Fog() :
            m_fogMode(GL_EXP),
            m_fogDensity(1.0f),
            m_fogStart(1.0f),
            m_fogEnd(1.0f),
            m_fogColor(NULL) {
        //
    }

    Fog::Fog(GLfloat fogMode) :
            m_fogDensity(1.0f),
            m_fogStart(1.0f),
            m_fogEnd(1.0f),
            m_fogColor(NULL) {
        setFogMode(fogMode);
    }

    Fog::~Fog() {
#ifdef DEBUG
        printf("Fog destructor...\n");
#endif
    }

    void Fog::setFogMode(GLfloat fogMode) {
        m_fogMode = fogMode;
    }

    void Fog::setFogDensity(GLfloat fogDensity) {
        m_fogDensity = fogDensity;
    }

    void Fog::setFogStart(GLfloat fogStart) {
        m_fogStart = fogStart;
    }

    void Fog::setFogEnd(GLfloat fogEnd) {
        m_fogEnd = fogEnd;
    }

    void Fog::setFogColor(GLfloat *fogColor) {
        m_fogColor = fogColor;
    }

    void Fog::initFog() {
        glFogf(GL_FOG_MODE, m_fogMode);
        if (m_fogColor != NULL)
            glFogfv(GL_FOG_COLOR, m_fogColor);
        glFogf(GL_FOG_DENSITY, m_fogDensity);
        glFogf(GL_FOG_START, m_fogStart);
        glFogf(GL_FOG_END, m_fogEnd);

        glHint(GL_FOG_HINT, GL_FASTEST);
    }

}
