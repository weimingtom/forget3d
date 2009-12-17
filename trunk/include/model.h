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

#ifndef F3D_MODEL_H_
#define F3D_MODEL_H_

#include "f3d.h"

namespace F3D {
    /**
     * Mesh class for all games using F3D.
     */

    class Mesh {
    protected:
        GLfloat     *m_vertices;
        GLfloat     *m_normals;
        GLfloat     *m_uvs;
        GLubyte     *m_colors;
        GLshort     *m_indices;
        GLfloat     *m_position;
        GLfloat     *m_rotate;
        GLfloat     *m_scale;
        GLint       m_textureId;
        GLint       m_triangleNums;
        GLboolean   m_enabled;
        //private functions
        void initGlCmds();
    public:
        /**
        * Constructor
        */
        Mesh();

        /**
         * Destructor
         */
        virtual ~Mesh();

        void setVertices(GLfloat *vertices, int size);
        void setNormals(GLfloat *normals, int size);
        void setUvs(GLfloat *uvs, int size);
        void setColors(GLubyte *colors, int size);
        void setIndices(GLshort *indices, int size);
        void setTextureId(GLint textureId);
        void setTriangleNums(GLint triangleNums);
        void setEnabled(GLboolean enabled);
        void setPosition(GLfloat x, GLfloat y, GLfloat z);
        void setRotate(GLfloat x, GLfloat y, GLfloat z);
        void setScale(GLfloat x, GLfloat y, GLfloat z);

        /**
         * Render mesh
         */
        void renderMesh();
    };

    /**
     * Model class for all games using F3D.
     */

    class Model {
    protected:
        Mesh    *m_meshs;
        int     m_meshCount;
        //private functions
        virtual void prepareFrame();
    public:
        /**
        * Constructor
        */
        Model();

        /**
         * Destructor
         */
        virtual ~Model();

        void setVertices(GLfloat *vertices, int size, int meshIndex = 0);
        void setNormals(GLfloat *normals, int size, int meshIndex = 0);
        void setUvs(GLfloat *uvs, int size, int meshIndex = 0);
        void setColors(GLubyte *colors, int size, int meshIndex = 0);
        void setIndices(GLshort *indices, int size, int meshIndex = 0);
        void setTextureId(GLint textureId, int meshIndex = 0);
        void setTriangleNums(GLint triangleNums, int meshIndex = 0);
        void setEnabled(GLboolean enabled, int meshIndex = 0);
        void setPosition(GLfloat x, GLfloat y, GLfloat z);
        void setRotate(GLfloat x, GLfloat y, GLfloat z);
        void setScale(GLfloat x, GLfloat y, GLfloat z);
        void setMeshCount(int meshCount);
        int getMeshCount();
        /**
         * Render model
         */
        void renderModel();
    };

}

#endif /* F3D_MODEL_H_ */
