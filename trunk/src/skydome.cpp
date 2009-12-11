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

#include "skydome.h"

#define CALC_UVS(x, y, z, u, v) \
        vx = x; \
        vy = y; \
        vz = z; \
        mag = (float)sqrt(SQR(vx)+SQR(vy)+SQR(vz)); \
        vx /= mag;  \
        vy /= mag;  \
        vz /= mag;  \
        u = (float)(atan2(vx, vz)/(M_PI * 2)) + 0.5f;    \
        v = (float)(asinf(vy) / M_PI) + 0.5f;

namespace F3D {
    /**
     * Skydome class for all games using F3D.
     */

    Skydome::Skydome(int radius, float dtheta, float dphi) {
        int vt_idx = 0, uv_idx = 0;
        float vx, vy, vz, mag;

        //calc triangle counts
        int num_vertices = (int)((360 / dtheta) * ((int)(90 / dphi) + 1) * 2);
#ifdef DEBUG
        printf("Plane constructor...\n");
        printf("skydome::num_vertices = %d\n", num_vertices);
#endif
        setMeshCount(1);

        float *vertices0 = (float *) malloc(num_vertices * 9 * sizeof(float));
        float *uvs0 = (float *) malloc(num_vertices * 6 * sizeof(float));

        setTriangleNums(num_vertices);

        for (float phi = 0.0f; phi <= 90.0f; phi += dphi) {
            for (float theta = -180.0f; theta < 180.0f; theta += dtheta) {
                float vertices[12];
                float uvs[8];
                float phi0 = phi + dphi;
                float theta0 = theta + dtheta;

                //point 0
                vertices[0] = (float) (radius * sinf(DTOR * theta) * cosf(DTOR * phi));
                vertices[1] = (float) (radius * cosf(DTOR * theta) * cosf(DTOR * phi));
                vertices[2] = (float) (radius * sinf(DTOR * phi));

                CALC_UVS(vertices[0], vertices[1], vertices[2], uvs[0], uvs[1]);

                //point 1
                vertices[3] = (float) (radius * sinf(DTOR * theta) * cosf(DTOR * (phi + dphi)));
                vertices[4] = (float) (radius * cosf(DTOR * theta) * cosf(DTOR * (phi + dphi)));
                vertices[5] = (float) (radius * sinf(DTOR * (phi + dphi)));

                CALC_UVS(vertices[3], vertices[4], vertices[5], uvs[2], uvs[3]);

                //point 2
                vertices[6] = (float) (radius * sinf(DTOR * theta0) * cosf(DTOR * phi0));
                vertices[7] = (float) (radius * cosf(DTOR * theta0) * cosf(DTOR * phi0));
                vertices[8] = (float) (radius * sinf(DTOR * phi0));

                CALC_UVS(vertices[6], vertices[7], vertices[8], uvs[4], uvs[5]);

                //point 3
                vertices[9] = (float) (radius * sinf(DTOR * theta0) * cosf(DTOR * (phi0 + dphi)));
                vertices[10] = (float) (radius * cosf(DTOR * theta0) * cosf(DTOR * (phi0 + dphi)));
                vertices[11] = (float) (radius * sinf(DTOR * (phi0 + dphi)));

                CALC_UVS(vertices[9], vertices[10], vertices[11], uvs[6], uvs[7]);
                //printf("uvs:%.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f\n", uvs[0], uvs[1], uvs[2], uvs[3], uvs[4], uvs[5], uvs[6], uvs[7]);
                //triangle 0 vertices & uvs of F3D skydome {0, 1, 2}
                vertices0[vt_idx++] = vertices[0];
                vertices0[vt_idx++] = vertices[1];
                vertices0[vt_idx++] = vertices[2];

                uvs0[uv_idx++] = uvs[0];
                uvs0[uv_idx++] = uvs[1];

                vertices0[vt_idx++] = vertices[3];
                vertices0[vt_idx++] = vertices[4];
                vertices0[vt_idx++] = vertices[5];

                uvs0[uv_idx++] = uvs[2];
                uvs0[uv_idx++] = uvs[3];

                vertices0[vt_idx++] = vertices[6];
                vertices0[vt_idx++] = vertices[7];
                vertices0[vt_idx++] = vertices[8];

                uvs0[uv_idx++] = uvs[4];
                uvs0[uv_idx++] = uvs[5];

                //triangle 0 vertices & uvs of F3D skydome {1, 2, 3}
                vertices0[vt_idx++] = vertices[3];
                vertices0[vt_idx++] = vertices[4];
                vertices0[vt_idx++] = vertices[5];

                uvs0[uv_idx++] = uvs[2];
                uvs0[uv_idx++] = uvs[3];

                vertices0[vt_idx++] = vertices[6];
                vertices0[vt_idx++] = vertices[7];
                vertices0[vt_idx++] = vertices[8];

                uvs0[uv_idx++] = uvs[4];
                uvs0[uv_idx++] = uvs[5];

                vertices0[vt_idx++] = vertices[9];
                vertices0[vt_idx++] = vertices[10];
                vertices0[vt_idx++] = vertices[11];

                uvs0[uv_idx++] = uvs[6];
                uvs0[uv_idx++] = uvs[7];
            }
        }

        setVertices(vertices0, num_vertices * 9 * sizeof(float));
        setUvs(uvs0, num_vertices * 6 * sizeof(float));

        FREEANDNULL(vertices0);
        FREEANDNULL(uvs0);
#ifdef DEBUG
        printf("skydome::uv_idx = %d\n", uv_idx);
#endif
        //after set m_enabled to true, so model can render it
		setPosition(0.0f, (float)(-radius * sinf(DTOR * dphi)), 0.0f);
        setRotate(-90.0f, 0.0f, 0.0f);
#ifdef DEBUG
        printf("Generate Skydome OK!\n");
#endif
    }

    Skydome::~Skydome() {
#ifdef DEBUG
        printf("Skydome destructor...\n");
#endif
    }

}
