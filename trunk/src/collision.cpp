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

#include "collision.h"

namespace F3D {
    /**
     * BoundingBox class for all games using F3D.
     */

    BoundingBox::BoundingBox() {
    #ifdef DEBUG
        printf("BoundingBox constructor...\n");
    #endif

        m_minEdge = (Vec3f*) malloc(sizeof(Vec3f));
        m_maxEdge = (Vec3f*) malloc(sizeof(Vec3f));
        m_center = (Vec3f*) malloc(sizeof(Vec3f));
    }

    BoundingBox::~BoundingBox() {
    #ifdef DEBUG
        printf("BoundingBox destructor...\n");
    #endif

        FREEANDNULL(m_minEdge);
        FREEANDNULL(m_maxEdge);
        FREEANDNULL(m_center);
    }

    void BoundingBox::setMinEdge(Vec3f vec3f) {
        m_minEdge->x = vec3f.x;
        m_minEdge->y = vec3f.y;
        m_minEdge->z = vec3f.z;
    }

    Vec3f* BoundingBox::getMinEdge() {
        return m_minEdge;
    }

    void BoundingBox::setMaxEdge(Vec3f vec3f) {
        m_maxEdge->x = vec3f.x;
        m_maxEdge->y = vec3f.y;
        m_maxEdge->z = vec3f.z;
    }

    Vec3f* BoundingBox::getMaxEdge() {
        return m_maxEdge;
    }

    Vec3f *BoundingBox::getCenter() {
        m_center->x = (m_minEdge->x + m_maxEdge->x) / 2;
        m_center->y = (m_minEdge->y + m_maxEdge->y) / 2;
        m_center->z = (m_minEdge->z + m_maxEdge->z) / 2;

        return m_center;
    }

    GLboolean BoundingBox::isCollided(BoundingBox *other) {
        if ( ( (other->getMinEdge()->x >= m_minEdge->x && other->getMinEdge()->x <= m_maxEdge->x) ||
                (other->getMaxEdge()->x >= m_minEdge->x && other->getMaxEdge()->x <= m_maxEdge->x) ) &&
                ( (other->getMinEdge()->y >= m_minEdge->y && other->getMinEdge()->y <= m_maxEdge->y) ||
                  (other->getMaxEdge()->y >= m_minEdge->y && other->getMaxEdge()->y <= m_maxEdge->y) ) &&
                ( (other->getMinEdge()->y >= m_minEdge->y && other->getMinEdge()->y <= m_maxEdge->y) ||
                  (other->getMaxEdge()->y >= m_minEdge->y && other->getMaxEdge()->y <= m_maxEdge->y) ) )
            return GL_TRUE;
        else
            return GL_FALSE;
    }

}
