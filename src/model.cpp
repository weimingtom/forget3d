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
     * Mesh class for all games using F3D.
     */

    Mesh::Mesh() :
            m_vertices(NULL),
            m_normals(NULL),
            m_uvs(NULL),
            m_colors(NULL),
            m_indices(NULL),
            m_position(NULL),
            m_rotate(NULL),
            m_scale(NULL),
            m_textureId(-1),
            m_triangleNums(0),
            m_enabled(GL_FALSE) {
#ifdef DEBUG
        printf("Mesh constructor...\n");
#endif
    }

    Mesh::~Mesh() {
#ifdef DEBUG
        printf("Start mesh destructor...\n");
#endif
        FREEANDNULL(m_vertices);
        FREEANDNULL(m_normals);
        FREEANDNULL(m_uvs);
        FREEANDNULL(m_colors);
        FREEANDNULL(m_indices);
        FREEANDNULL(m_position);
        FREEANDNULL(m_rotate);
        FREEANDNULL(m_scale);
#ifdef DEBUG
        printf("Mesh destructor OK!\n");
#endif
    }

    void Mesh::setVertices(GLfloat *vertices, int size) {
        FREEANDNULL(m_vertices);
        m_vertices = (GLfloat *) malloc(size);
        memcpy(m_vertices, vertices, size);

        setEnabled(GL_TRUE);
    }

    void Mesh::setNormals(GLfloat *normals, int size) {
        FREEANDNULL(m_normals);
        m_normals = (GLfloat *) malloc(size);
        memcpy(m_normals, normals, size);
    }

    void Mesh::setUvs(GLfloat *uvs, int size) {
        FREEANDNULL(m_uvs);
        m_uvs = (GLfloat *) malloc(size);
        memcpy(m_uvs, uvs, size);
    }

    void Mesh::setColors(GLubyte *colors, int size) {
        FREEANDNULL(m_colors);
        m_colors = (GLubyte *) malloc(size);
        memcpy(m_colors, colors, size);
    }

    void Mesh::setIndices(GLshort *indices, int size) {
        FREEANDNULL(m_indices);
        m_indices = (GLshort *) malloc(size);
        memcpy(m_indices, indices, size);
    }

    void Mesh::setTextureId(GLint textureId) {
        m_textureId = textureId;
    }

    void Mesh::setTriangleNums(GLint triangleNums) {
        m_triangleNums = triangleNums;
    }

    void Mesh::setEnabled(GLboolean enabled) {
        m_enabled = enabled;
    }

    void Mesh::setPosition(GLfloat x, GLfloat y, GLfloat z) {
        if (m_position == NULL) {
            m_position = (GLfloat *) malloc(3 * sizeof(GLfloat));
        }

        m_position[0] = x;
        m_position[1] = y;
        m_position[2] = z;
    }

    void Mesh::setRotate(GLfloat x, GLfloat y, GLfloat z) {
        if (m_rotate == NULL) {
            m_rotate = (GLfloat *) malloc(3 * sizeof(GLfloat));
        }

        m_rotate[0] = x;
        m_rotate[1] = y;
        m_rotate[2] = z;
    }

    void Mesh::setScale(GLfloat x, GLfloat y, GLfloat z) {
        if (m_scale == NULL) {
            m_scale = (GLfloat *) malloc(3 * sizeof(GLfloat));
        }

        m_scale[0] = x;
        m_scale[1] = y;
        m_scale[2] = z;
    }

    void Mesh::initGlCmds() {
        if (m_vertices != NULL)
            glEnableClientState(GL_VERTEX_ARRAY);

        if (m_normals != NULL)
            glEnableClientState(GL_NORMAL_ARRAY);

        if (m_uvs != NULL)
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        if (m_colors != NULL)
            glEnableClientState(GL_COLOR_ARRAY);
    }

    void Mesh::renderMesh() {
        if (!m_enabled)
            return;

        //save current matrix
        glPushMatrix();

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

        if (m_scale != NULL)
            glScalef(m_scale[0], m_scale[1], m_scale[2]);

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
                glDisable(GL_TEXTURE_2D);
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

    /**
     * Model class for all games using F3D.
     */

    Model::Model() :
            m_meshs(NULL),
            m_meshCount(0) {
#ifdef DEBUG
        printf("Model constructor...\n");
#endif
    }

    Model::~Model() {
#ifdef DEBUG
        printf("Start model destructor...\n");
#endif
        delete [] m_meshs;
#ifdef DEBUG
        printf("Model destructor OK!\n");
#endif
    }

    void Model::setVertices(GLfloat *vertices, int size, int meshIndex) {
        m_meshs[meshIndex].setVertices(vertices, size);
    }

    void Model::setNormals(GLfloat *normals, int size, int meshIndex) {
        m_meshs[meshIndex].setNormals(normals, size);
    }

    void Model::setUvs(GLfloat *uvs, int size, int meshIndex) {
        m_meshs[meshIndex].setUvs(uvs, size);
    }

    void Model::setColors(GLubyte *colors, int size, int meshIndex) {
        m_meshs[meshIndex].setColors(colors, size);
    }

    void Model::setIndices(GLshort *indices, int size, int meshIndex) {
        m_meshs[meshIndex].setIndices(indices, size);
    }

    void Model::setTriangleNums(GLint triangleNums, int meshIndex) {
        m_meshs[meshIndex].setTriangleNums(triangleNums);
    }

    void Model::setEnabled(GLboolean enabled, int meshIndex) {
        m_meshs[meshIndex].setEnabled(enabled);
    }

    void Model::setTextureId(GLint textureId, int meshIndex) {
        m_meshs[meshIndex].setTextureId(textureId);
    }

    void Model::setPosition(GLfloat x, GLfloat y, GLfloat z) {
        for (int i = 0; i < m_meshCount; i++) {
            m_meshs[i].setPosition(x, y, z);
        }
    }

    void Model::setRotate(GLfloat x, GLfloat y, GLfloat z) {
        for (int i = 0; i < m_meshCount; i++) {
            m_meshs[i].setRotate(x, y, z);
        }
    }

    void Model::setScale(GLfloat x, GLfloat y, GLfloat z) {
        for (int i = 0; i < m_meshCount; i++) {
            m_meshs[i].setScale(x, y, z);
        }
    }

    void Model::setMeshCount(int meshCount) {
        if (this->m_meshs != NULL) {
            delete[] m_meshs;
            m_meshs = NULL;
        }

        if (meshCount > 0) {
#ifdef DEBUG
            printf("Model create [%d] meshs ...\n", meshCount);
#endif
            m_meshCount = meshCount;
            // create meshs
            m_meshs = new Mesh[m_meshCount];
        }
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

    void Model::renderModel() {
        //enable or disable gl command status
        prepareFrame();

        for (int i = 0; i < m_meshCount; i++)
            m_meshs[i].renderMesh();
    }

}
