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

#include "model.h"

namespace F3D {
    /**
     * Model class for all games using F3D.
     */

    Model::Model() :
            m_vertices(NULL),
            m_normals(NULL),
            m_uvs(NULL),
            m_colors(NULL),
            m_indices(NULL),
            m_position(NULL),
            m_rotate(NULL),
            m_textureId(-1),
            m_triangleNums(0),
            m_enabled(GL_FALSE) {
        //
    }

    Model::~Model() {
#ifdef DEBUG
        printf("Model destructor...\n");
#endif
        if (m_vertices != NULL)
            free(m_vertices);
        if (m_normals != NULL)
            free(m_normals);
        if (m_uvs != NULL)
            free(m_uvs);
        if (m_colors != NULL)
            free(m_colors);
        if (m_indices != NULL)
            free(m_indices);
    }

    void Model::initGlCmds() {
        if (m_vertices != NULL)
            glEnableClientState(GL_VERTEX_ARRAY);

        if (m_normals != NULL)
            glEnableClientState(GL_NORMAL_ARRAY);

        if (m_uvs != NULL)
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        if (m_colors != NULL)
            glEnableClientState(GL_COLOR_ARRAY);
    }

    void Model::setVertices(GLfloat *vertices) {
        m_vertices = vertices;
        setEnabled(GL_TRUE);
    }

    void Model::setNormals(GLfloat *normals) {
        m_normals = normals;
    }

    void Model::setUvs(GLfloat *uvs) {
        m_uvs = uvs;
    }

    void Model::setColors(GLubyte *colors) {
        m_colors = colors;
    }

    void Model::setIndices(GLshort *indices) {
        m_indices = indices;
    }

    void Model::setTextureId(GLint textureId) {
        m_textureId = textureId;
    }

    void Model::setTriangleNums(GLint triangleNums) {
        m_triangleNums = triangleNums;
    }

    void Model::prepareFrame() {
#ifdef DEBUG
        static bool isFirst = true;
        if (isFirst) {
            printf("Call Model::prepareFrame()...\n");
            isFirst = false;
        }
#endif
    }

    void Model::setEnabled(GLboolean enabled) {
        m_enabled = enabled;
    }

    void Model::setPosition(GLfloat x, GLfloat y, GLfloat z) {
        if (m_position == NULL) {
            m_position = (float *) malloc(3 * sizeof(float));
        }

        if (m_position != NULL) {
            m_position[0] = x;
            m_position[1] = y;
            m_position[2] = z;
        }
    }

    void Model::setRotate(GLfloat x, GLfloat y, GLfloat z) {
        if (m_rotate == NULL) {
            m_rotate = (float *) malloc(3 * sizeof(float));
        }

        if (m_rotate != NULL) {
            m_rotate[0] = x;
            m_rotate[1] = y;
            m_rotate[2] = z;
        }
    }

    void Model::renderModel() {
        if (!m_enabled)
            return;

        //save current matrix
        glPushMatrix();
        //prepare frame to render
        prepareFrame();
        //enable or disable gl command status
        initGlCmds();

        if (m_position != NULL) {
            glTranslatef(m_position[0], m_position[1], m_position[2]);
        }

        if (m_rotate != NULL) {
            //ratate x axis
            if (m_rotate[0] != 0.0f)
                glRotatef(m_rotate[0], 1.0f, 0.0f, 0.0f);
            //ratate y axis
            if (m_rotate[1] != 0.0f)
                glRotatef(m_rotate[1], 0.0f, 1.0f, 0.0f);
            //ratate z axis
            if (m_rotate[2] != 0.0f)
                glRotatef(m_rotate[2], 0.0f, 0.0f, 1.0f);
        }


        if (m_vertices != NULL)
            glVertexPointer(3, GL_FLOAT, 0, m_vertices);
        else {
#ifdef DEBUG
            printf("No vertices, exit render!\n");
#endif
            return;
        }

        if (m_normals != NULL)
            glNormalPointer(GL_FLOAT, 0, m_normals);

        if (m_uvs != NULL && m_textureId != -1) {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, m_textureId);
            glTexCoordPointer(2, GL_FLOAT, 0, m_uvs);
        } else {
            if ((m_uvs != NULL && m_textureId == -1) ||
                    (m_uvs == NULL && m_textureId != -1) ) {
#ifdef DEBUG
                printf("UVs, exit render!\n");
#endif
            }
        }

        if (m_colors != NULL)
            glColorPointer(4, GL_UNSIGNED_BYTE, 0, m_colors);

        if (m_indices != NULL)
            glDrawElements(GL_TRIANGLES, m_triangleNums * 3, GL_UNSIGNED_SHORT, m_indices);
        else
            glDrawArrays(GL_TRIANGLES, 0, m_triangleNums * 3);
        //restore matrix
        glPopMatrix();
    }

}
