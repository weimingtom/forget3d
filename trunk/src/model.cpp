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
            m_meshCount(0),
            m_meshs(NULL),
            m_aabbs(NULL),
            m_isLooped(GL_TRUE),
            m_isChkCollision(GL_FALSE) {
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

        if (m_isChkCollision) {
            // start to set bounding box, min, max vector
            Vec3f vec_min = {-0.0f, -0.0f, -0.0f};
            Vec3f vec_max = {0.0f, 0.0f, 0.0f};
            for (GLuint i = 0; i < size / sizeof(GLfloat); ) {
                if (vertices[i] < vec_min.x)
                    vec_min.x = vertices[i];

                if (vertices[i + 1] < vec_min.y)
                    vec_min.y = vertices[i + 1];

                if (vertices[i + 2] < vec_min.z)
                    vec_min.z = vertices[i + 2];

                if (vertices[i] > vec_max.x)
                    vec_max.x = vertices[i];

                if (vertices[i + 1] > vec_max.y)
                    vec_max.y = vertices[i + 1];

                if (vertices[i + 2] > vec_max.z)
                    vec_max.z = vertices[i + 2];

                i += 3;
            }
#ifdef DEBUG
            printf("setMinEdge: {%.4f, %.4f, %.4f}\n", vec_min.x, vec_min.y, vec_min.z);
            printf("setMaxEdge: {%.4f, %.4f, %.4f}\n", vec_max.x, vec_max.y, vec_max.z);
#endif
            m_aabbs[meshIndex].setMinEdge(vec_min);
            m_aabbs[meshIndex].setMaxEdge(vec_max);
        }
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
        //if textureId = 0, set texture to all meshes
        if (meshIndex == 0) {
            for (int i = 1; i < m_meshCount; i++)
                m_meshs[i].setTextureId(textureId);
        }
    }

    void Model::setPosition(GLfloat x, GLfloat y, GLfloat z) {
        for (int i = 0; i < m_meshCount; i++) {
            m_meshs[i].setPosition(x, y, z);
            m_aabbs[i].setPosition(x, y, z);
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
            m_aabbs[i].setScale(x, y, z);
        }
    }

    void Model::setMeshCount(int meshCount) {
        DELETEANDNULL(m_meshs, true);
        DELETEANDNULL(m_aabbs, true);

        if (meshCount > 0) {
#ifdef DEBUG
            printf("Model create [%d] meshs ...\n", meshCount);
#endif
            m_meshCount = meshCount;
            // create meshs
            m_meshs = new Mesh[m_meshCount];
            m_aabbs = new BoundingBox[m_meshCount];
        }
    }

    int Model::getMeshCount() {
        return m_meshCount;
    }

    Mesh* Model::getMesh(int index) {
        return &m_meshs[index];
    }

    BoundingBox* Model::getBoundingBox(int index){
        return &m_aabbs[index];
    }

    GLboolean Model::isCollided(Model* other) {
        for (int i = 0; i < m_meshCount; i++) {
            for (int j = 0; j < other->getMeshCount(); j ++) {
                if (getBoundingBox(i)->isCollided(other->getBoundingBox(j)))
                    return GL_TRUE;
            }
        }

        return GL_FALSE;
    }

    //looped flag
    void Model::setLooped(GLboolean isLooped) {
        m_isLooped = isLooped;
    }

    GLboolean Model::isLooped() {
        return m_isLooped;
    }

    //check collision flag
    void Model::setChkCollision(GLboolean isChkCollision) {
        m_isChkCollision = isChkCollision;
    }

    GLboolean Model::isChkCollision() {
        return m_isChkCollision;
    }

    void Model::renderModel() {
        //enable or disable gl command status
        prepareFrame();

        for (int i = 0; i < m_meshCount; i++)
            m_meshs[i].renderMesh();
    }

}
