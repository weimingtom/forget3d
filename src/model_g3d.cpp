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

#include "model_g3d.h"

namespace F3D {
    /**
     * ModelG3D class for all games using F3D.
     * G3D is glest(http://www.glest.org/) 3D model
     */

    ModelG3D::ModelG3D() :
            m_g3dMeshs(NULL),
            m_frameIdx(0),
            m_frameCount(0),
            m_isPrepared(GL_FALSE) {
#ifdef DEBUG
        printf("ModelG3D constructor...\n");
#endif
    }

    ModelG3D::~ModelG3D() {
        delete [] m_g3dMeshs;
#ifdef DEBUG
        printf("ModelG3D destructor...\n");
#endif
    }

    bool ModelG3D::loadModel(const char *filename) {
        FILE *file;

        file = fopen (filename, "rb");
        if (!file)
            return false;

        /* initialize model and read header */
        fread (&m_header, sizeof (g3d_header_t), 1, file);
#ifdef DEBUG
        printf ("magic:%s\n", m_header.magic);
        printf ("version:%d\n", m_header.version);
        printf ("meshCount:%d\n", m_header.meshCount);
        printf ("type:%d\n", m_header.type);
#endif

        if (!strcmp((char *)m_header.magic, "G3D") || m_header.version != 4) {
            printf("Invalid G3D model or unsupport version, magic:[%s], version[%d]...\n",
                   m_header.magic, m_header.version);
            fclose(file);

            return false;
        }

        setMeshCount(m_header.meshCount);
        m_g3dMeshs = new g3d_mesh_t[m_meshCount];

        for (uint16 i = 0; i < m_header.meshCount; i++) {
            fread(&m_g3dMeshs[i].header, sizeof(g3d_mesh_header_t), 1, file);
#ifdef DEBUG
            printf ("Mesh[%d] name:%s\n", i, m_g3dMeshs[i].header.name);
            printf ("Mesh[%d] frameCount:%d\n", i, m_g3dMeshs[i].header.frameCount);
            printf ("Mesh[%d] vertexCount:%d\n", i, m_g3dMeshs[i].header.vertexCount);
            printf ("Mesh[%d] indexCount:%d\n", i, m_g3dMeshs[i].header.indexCount);
            printf ("Mesh[%d] textures:%d\n", i, m_g3dMeshs[i].header.textures);
#endif
            //read textures
            for (uint16 j = 0; j < m_g3dMeshs[i].header.textures; j++) {
                char texName[NAME_SIZE];
                fread(texName, NAME_SIZE, 1, file);
#ifdef DEBUG
                printf ("Mesh[%d] textures[%d]:%s\n", i, j, texName);
#endif
            }

            int frameCount = m_g3dMeshs[i].header.frameCount;
            int vertexCount = m_g3dMeshs[i].header.vertexCount;
            int indicesCount = m_g3dMeshs[i].header.indexCount;
            int v_size = frameCount * vertexCount * sizeof(Vec3f);
            int u_size = vertexCount * sizeof(Vec2f);
            int i_size = indicesCount * sizeof(unsigned int);
            //read mesh all frame vertices
            m_g3dMeshs[i].vertices = new Vec3f[frameCount * vertexCount];
            fread(&m_g3dMeshs[i].vertices[0], v_size, 1, file);
            //read mesh all frame normals
            m_g3dMeshs[i].normals = new Vec3f[frameCount * vertexCount];
            fread(&m_g3dMeshs[i].normals[0], v_size, 1, file);
            //read all uvs if model have textures
            if (m_g3dMeshs[i].header.textures > 0) {
                m_g3dMeshs[i].uvs = new Vec2f[vertexCount];
                fread(&m_g3dMeshs[i].uvs[0], u_size, 1, file);
            }
            //read mesh all frame indices
            m_g3dMeshs[i].indices = new unsigned int[indicesCount];
            fread(&m_g3dMeshs[i].indices[0], i_size, 1, file);

            setTriangleNums(indicesCount / 3, i);
        }

        fclose(file);

        m_frameCount = m_g3dMeshs[0].header.frameCount;
        m_frameIdx = 0;
        //set uvs
        for (int i = 0; i < m_meshCount; i++) {
            int indicesCount = m_g3dMeshs[i].header.indexCount;
            int u_size = indicesCount * sizeof(Vec2f);

            float *uvs = (float *) malloc(u_size);
            int u_idx = 0;

            for (int j = 0; j < indicesCount; j++) {
                int i_idx = m_g3dMeshs[i].indices[j];

                uvs[u_idx++] = m_g3dMeshs[i].uvs[i_idx].s;
                uvs[u_idx++] = m_g3dMeshs[i].uvs[i_idx].t;
            }

            setUvs(uvs, u_size, i);
        }

        return true;
    }

    void ModelG3D::prepareFrame() {
#ifdef DEBUG
        static bool isFirst = true;
        if (isFirst) {
            printf("Call ModelG3D::prepareFrame()...\n");
            isFirst = false;
        }
#endif
        if (m_frameCount == 1 && m_isPrepared) {
#ifdef DEBUG
            printf("Only one frame, don't need prepare again...\n");
#endif
            return;
        }

        if (!m_isPrepared)
            m_isPrepared = GL_FALSE;

        //read all g3d meshs and copy data to model meshs
        for (int i = 0; i < m_meshCount; i++) {
            int vertexCount = m_g3dMeshs[i].header.vertexCount;
            int indicesCount = m_g3dMeshs[i].header.indexCount;
            int v_size = indicesCount * sizeof(Vec3f);
            float *vertices = (float *) malloc(v_size);
            float *normals = (float *) malloc(v_size);
            int v_idx = 0, n_idx = 0;

            for (int j = 0; j < indicesCount; j++) {
                int i_idx = vertexCount * m_frameIdx + m_g3dMeshs[i].indices[j];

                vertices[v_idx++] = m_g3dMeshs[i].vertices[i_idx].x;
                vertices[v_idx++] = m_g3dMeshs[i].vertices[i_idx].y;
                vertices[v_idx++] = m_g3dMeshs[i].vertices[i_idx].z;

                normals[n_idx++] = m_g3dMeshs[i].normals[i_idx].x;
                normals[n_idx++] = m_g3dMeshs[i].normals[i_idx].y;
                normals[n_idx++] = m_g3dMeshs[i].normals[i_idx].z;
            }

            setVertices(vertices, v_size, i);
            setNormals(normals, v_size, i);
        }

        m_frameIdx++;
        if (m_frameIdx >= m_frameCount)
            m_frameIdx = 0;
    }

}
