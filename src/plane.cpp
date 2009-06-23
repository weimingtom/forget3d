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

#include "plane.h"

namespace F3D {
    /**
     * Plane class for all games using F3D.
     */

    Plane::Plane(int width, int height, float scale) {
        int vt_idx = 0, nm_idx = 0, uv_idx = 0;

        m_vertices = (float *) malloc(width * height * 18 * sizeof(float));
        m_normals = (float *) malloc(width * height * 18 * sizeof(float));
        m_uvs = (float *) malloc(width * height * 12 * sizeof(float));
        setTriangleNums(width * height * 2);

        for (int x = 0; x < width; x++) {
            for (int z = 0; z < height; z++) {
                //triangle 1 first point
                m_vertices[vt_idx++] = x * scale;
                m_vertices[vt_idx++] = 0.0f;
                m_vertices[vt_idx++] = z * scale;

                m_normals[nm_idx++] = 0.0f;
                m_normals[nm_idx++] = 1.0f;
                m_normals[nm_idx++] = 0.0f;

                m_uvs[uv_idx++] = 0.0f;
                m_uvs[uv_idx++] = 0.0f;
                //second point
                m_vertices[vt_idx++] = (x + 1) * scale;
                m_vertices[vt_idx++] = 0.0f;
                m_vertices[vt_idx++] = z * scale;

                m_normals[nm_idx++] = 0.0f;
                m_normals[nm_idx++] = 1.0f;
                m_normals[nm_idx++] = 0.0f;

                m_uvs[uv_idx++] = 1.0f;
                m_uvs[uv_idx++] = 0.0f;
                //third point
                m_vertices[vt_idx++] = (x + 1) * scale;
                m_vertices[vt_idx++] = 0.0f;
                m_vertices[vt_idx++] = (z + 1) * scale;

                m_normals[nm_idx++] = 0.0f;
                m_normals[nm_idx++] = 1.0f;
                m_normals[nm_idx++] = 0.0f;

                m_uvs[uv_idx++] = 1.0f;
                m_uvs[uv_idx++] = 1.0f;
                //triangle 1 first point
                m_vertices[vt_idx++] = x * scale;
                m_vertices[vt_idx++] = 0.0f;
                m_vertices[vt_idx++] = z * scale;

                m_normals[nm_idx++] = 0.0f;
                m_normals[nm_idx++] = 1.0f;
                m_normals[nm_idx++] = 0.0f;

                m_uvs[uv_idx++] = 0.0f;
                m_uvs[uv_idx++] = 0.0f;
                //third point
                m_vertices[vt_idx++] = (x + 1) * scale;
                m_vertices[vt_idx++] = 0.0f;
                m_vertices[vt_idx++] = (z + 1) * scale;

                m_normals[nm_idx++] = 0.0f;
                m_normals[nm_idx++] = 1.0f;
                m_normals[nm_idx++] = 0.0f;

                m_uvs[uv_idx++] = 1.0f;
                m_uvs[uv_idx++] = 1.0f;
                //fourth pint
                m_vertices[vt_idx++] = x * scale;
                m_vertices[vt_idx++] = 0.0f;
                m_vertices[vt_idx++] = (z + 1) * scale;

                m_normals[nm_idx++] = 0.0f;
                m_normals[nm_idx++] = 1.0f;
                m_normals[nm_idx++] = 0.0f;

                m_uvs[uv_idx++] = 0.0f;
                m_uvs[uv_idx++] = 1.0f;
            }
        }
        //after set m_enabled to true, so model can render it
        setEnabled(GL_TRUE);
#ifdef DEBUG
        printf("Generate plane OK!\n");
#endif
    }

    Plane::~Plane() {
#ifdef DEBUG
        printf("Plane destructor...\n");
#endif
    }

}
