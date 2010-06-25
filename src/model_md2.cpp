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

#include "model_md2.h"

float NORMALS_TABLE[NUMVERTEXNORMALS][3] = {
#include "anorms.h"
};

namespace F3D {
    /**
     * ModelMD2 class for all games using F3D.
     */

    ModelMD2::ModelMD2() :
            m_skins(NULL),
            m_texCoords(NULL),
            m_triangles(NULL),
            m_frames(NULL),
            m_actions(NULL),
            m_textureId(-1),
            m_frameIdx(0),
            m_actionIdx(0),
            m_actionCount(0) {
#ifdef DEBUG
        printf("ModelMD2 constructor...\n");
#endif
        setMeshCount(1);
    }

    ModelMD2::~ModelMD2() {
#ifdef DEBUG
        printf("ModelMD2 destructor...\n");
#endif
        FREEANDNULL(m_skins);
        FREEANDNULL(m_texCoords);
        FREEANDNULL(m_triangles);
        FREEANDNULL(m_frames);
        FREEANDNULL(m_actions);
    }

    bool ModelMD2::loadModel(const char *filename, GLboolean is_absPath) {
        FILE *file;
        GLubyte buffer[MD2_MAX_FRAMESIZE];
        int i;

        file = fopen(Utils::getFileName(filename, is_absPath), "rb");

        if (!file)
            return false;

        /* initialize model and read header */
        fread (&m_header, sizeof (md2_header_t), 1, file);
#if DEBUG
        printf ("magic:\t\t%d\n", m_header.magic);
        printf ("version:\t\t%d\n", m_header.version);
        printf ("skinWidth:\t\t%d\n", m_header.skinWidth);
        printf ("skinHeight:\t\t%d\n", m_header.skinHeight);
        printf ("frameSize:\t\t%d\n", m_header.frameSize);
        printf ("numSkins:\t\t%d\n", m_header.numSkins);
        printf ("numVertices:\t\t%d\n", m_header.numVertices);
        printf ("numTexCoords:\t\t%d\n", m_header.numTexCoords);
        printf ("numTriangles:\t\t%d\n", m_header.numTriangles);
        printf ("numGlCommands:\t\t%d\n", m_header.numGlCommands);
        printf ("numFrames:\t\t%d\n", m_header.numFrames);
        printf ("offsetSkins:\t\t%d\n", m_header.offsetSkins);
        printf ("offsetTexCoords:\t%d\n", m_header.offsetTexCoords);
        printf ("offsetTriangles:\t%d\n", m_header.offsetTriangles);
        printf ("offsetFrames:\t\t%d\n", m_header.offsetFrames);
        printf ("offsetGlCommands:\t%d\n", m_header.offsetGlCommands);
        printf ("offsetEnd:\t\t%d\n", m_header.offsetEnd);
#endif

        if (m_header.magic != (int) (('2' << 24) + ('P' << 16) + ('D' << 8) + 'I')) {
            fclose(file);
            return false;
        }

        /* read skins */
        fseek (file, m_header.offsetSkins, SEEK_SET);
        if (m_header.numSkins > 0) {
            m_skins = (md2_skin_t *) malloc(sizeof(md2_skin_t) * m_header.numSkins);
            if (!m_skins) {
                //md2_freeModel (model);
                return false;
            }

            for (i = 0; i < m_header.numSkins; i++)
                fread (&m_skins[i], sizeof(md2_skin_t), 1, file);
        }

        /* read texture coordinates */
        fseek (file, m_header.offsetTexCoords, SEEK_SET);
        if (m_header.numTexCoords > 0) {
            m_texCoords = (md2_textureCoordinate_t *) malloc (sizeof (md2_textureCoordinate_t) * m_header.numTexCoords);
            if (!m_texCoords) {
                //md2_freeModel (model);
                return false;
            }

            for (i = 0; i < m_header.numTexCoords; i++)
                fread (&m_texCoords[i], sizeof (md2_textureCoordinate_t), 1, file);
        }

        /* read triangles */
        fseek (file, m_header.offsetTriangles, SEEK_SET);
        if (m_header.numTriangles > 0) {
            m_triangles = (md2_triangle_t *) malloc(sizeof(md2_triangle_t) * m_header.numTriangles);
            if (!m_triangles) {
                //md2_freeModel (model);
                return false;
            }

            for (i = 0; i < m_header.numTriangles; i++)
                fread (&m_triangles[i], sizeof (md2_triangle_t), 1, file);
        }

        /* read alias frames */
        fseek (file, m_header.offsetFrames, SEEK_SET);
        if (m_header.numFrames > 0) {
            m_frames = (md2_frame_t *) malloc (sizeof (md2_frame_t) * m_header.numFrames);
            if (!m_frames) {
                //md2_freeModel (model);
                return false;
            }

            for (i = 0; i < m_header.numFrames; i++) {
                md2_alias_frame_t *frame = (md2_alias_frame_t *) buffer;
                int j;

                m_frames[i].vertices = (md2_triangleVertex_t *) malloc (sizeof (md2_triangleVertex_t) * m_header.numVertices);
                if (!m_frames[i].vertices) {
                    //md2_freeModel (model);
                    return false;
                }

                fread (frame, 1, m_header.frameSize, file);
                strcpy (m_frames[i].name, frame->name);
                for (j = 0; j < m_header.numVertices; j++) {
                    m_frames[i].vertices[j].vertex[0] = (float) ((int) frame->alias_vertices[j].vertex[0]) * frame->scale[0] + frame->translate[0];
                    m_frames[i].vertices[j].vertex[2] = -1* ((float) ((int) frame->alias_vertices[j].vertex[1]) * frame->scale[1] + frame->translate[1]);
                    m_frames[i].vertices[j].vertex[1] = (float) ((int) frame->alias_vertices[j].vertex[2]) * frame->scale[2] + frame->translate[2];

                    m_frames[i].vertices[j].normal[0] = NORMALS_TABLE[frame->alias_vertices[j].lightNormalIndex][0];
                    m_frames[i].vertices[j].normal[1] = NORMALS_TABLE[frame->alias_vertices[j].lightNormalIndex][1];
                    m_frames[i].vertices[j].normal[2] = NORMALS_TABLE[frame->alias_vertices[j].lightNormalIndex][2];
                    //model->frames[i].vertices[j].lightNormalIndex = frame->alias_vertices[j].lightNormalIndex;
                }
            }
        }
        //ignore gl commands reading

        fclose (file);

        setTriangleNums(m_header.numTriangles);

        m_actionCount = getAnimationCount();
#if DEBUG
        printf("getAnimationCount:\t%d\n", m_actionCount);
#endif
        m_actions = (md2_action_t *) malloc(m_actionCount * sizeof(md2_action_t));
        for (i = 0; i < (int) m_actionCount; i++) {
            m_actions[i].index = i;
            strcpy (m_actions[i].name, getAnimationName(i));
            getAnimationFrames(i, &(m_actions[i].min_idx), &(m_actions[i].max_idx ));
#if DEBUG
            printf("m_actions[%d]:\t\t%s [%d, %d]\n", i, m_actions[i].name, m_actions[i].min_idx, m_actions[i].max_idx);
#endif
        }

        return true;
    }

    void ModelMD2::prepareFrame() {
#ifdef DEBUG
        static bool isFirst = true;
        if (isFirst) {
            printf("Call ModelMD2::prepareFrame()...\n");
            isFirst = false;
        }
#endif
        if (m_actionIdx == -1)
            return;

        float *uvs = (float *) malloc(m_header.numTriangles * 6 * sizeof(float));
        float *vertices = (float *) malloc(m_header.numTriangles * 9 * sizeof(float));
        float *normals = (float *) malloc(m_header.numTriangles * 9 * sizeof(float));
        int i, j, uvIdx = 0, vertexIdx = 0, normalIdx = 0;

        md2_frame_t *f = &m_frames[m_frameIdx];

        for (i = 0; i < m_header.numTriangles; i++) {
            md2_triangle_t *t = &m_triangles[i];
            for (j = 0; j < 3; j++) {
                //set uvs
                uvs[uvIdx++] = (float) m_texCoords[t->textureIndices[j]].s / (float) m_header.skinWidth;
                uvs[uvIdx++] = 1.0f - (float) m_texCoords[t->textureIndices[j]].t / (float) m_header.skinHeight;
                //set vertices & normals
                vertices[vertexIdx++] = f->vertices[t->vertexIndices[j]].vertex[0];
                vertices[vertexIdx++] = f->vertices[t->vertexIndices[j]].vertex[1];
                vertices[vertexIdx++] = f->vertices[t->vertexIndices[j]].vertex[2];

                normals[normalIdx++] = f->vertices[t->vertexIndices[j]].normal[0];
                normals[normalIdx++] = f->vertices[t->vertexIndices[j]].normal[2];
                normals[normalIdx++] = f->vertices[t->vertexIndices[j]].normal[1];
            }
        }
        setVertices(vertices, m_header.numTriangles * 9 * sizeof(float));
        setNormals(normals, m_header.numTriangles * 9 * sizeof(float));
        setUvs(uvs, m_header.numTriangles * 6 * sizeof(float));

        FREEANDNULL(vertices);
        FREEANDNULL(normals);
        FREEANDNULL(uvs);

        m_frameIdx++;
        if (m_frameIdx > m_actions[m_actionIdx].max_idx)
            m_frameIdx = m_isLoop ? m_actions[m_actionIdx].min_idx : m_actions[m_actionIdx].max_idx;
    }

    int ModelMD2::getAnimationCount() {
        int i, j, pos;
        int count;
        int lastId;
        char name[16], last[16];

        strcpy (last, m_frames[0].name);
        pos = strlen (last) - 1;
        j = 0;
        while (last[pos] >= '0' && last[pos] <= '9' && j < 2) {
            pos--;
            j++;
        }
        last[pos + 1] = '\0';

        lastId = 0;
        count = 0;

        for (i = 0; i <= m_header.numFrames; i++) {
            if (i == m_header.numFrames)
                strcpy (name, ""); // some kind of a sentinel
            else
                strcpy (name, m_frames[i].name);
            pos = strlen (name) - 1;
            j = 0;
            while (name[pos] >= '0' && name[pos] <= '9' && j < 2) {
                pos--;
                j++;
            }
            name[pos + 1] = '\0';

            if (strcmp (last, name)) {
                strcpy (last, name);
                count++;
            }
        }

        return count;
    }

    const char* ModelMD2::getAnimationName (int animation) {
        int i, j, pos;
        int count;
        int lastId;
        static char last[32];
        char name[32];

        strcpy (last, m_frames[0].name);
        pos = strlen (last) - 1;
        j = 0;
        while (last[pos] >= '0' && last[pos] <= '9' && j < 2) {
            pos--;
            j++;
        }
        last[pos + 1] = '\0';

        lastId = 0;
        count = 0;

        for (i = 0; i <= m_header.numFrames; i++) {
            if (i == m_header.numFrames)
                strcpy (name, ""); // some kind of a sentinel
            else
                strcpy (name, m_frames[i].name);
            pos = strlen (name) - 1;
            j = 0;
            while (name[pos] >= '0' && name[pos] <= '9' && j < 2) {
                pos--;
                j++;
            }
            name[pos + 1] = '\0';

            if (strcmp (last, name)) {
                if (count == animation)
                    return last;

                strcpy (last, name);
                count++;
            }
        }

        return 0;
    }

    void ModelMD2::getAnimationFrames(int animation, GLuint *startFrame, GLuint *endFrame) {
        int i, j, pos;
        int count, numFrames, frameCount;
        int lastId;
        char name[16], last[16];

        strcpy (last, m_frames[0].name);
        pos = strlen (last) - 1;
        j = 0;
        while (last[pos] >= '0' && last[pos] <= '9' && j < 2) {
            pos--;
            j++;
        }
        last[pos + 1] = '\0';

        lastId = 0;
        count = 0;
        numFrames = 0;
        frameCount = 0;

        for (i = 0; i <= m_header.numFrames; i++) {
            if (i == m_header.numFrames)
                strcpy (name, ""); // some kind of a sentinel
            else
                strcpy (name, m_frames[i].name);
            pos = strlen (name) - 1;
            j = 0;
            while (name[pos] >= '0' && name[pos] <= '9' && j < 2) {
                pos--;
                j++;
            }
            name[pos + 1] = '\0';

            if (strcmp (last, name)) {
                strcpy (last, name);

                if (count == animation) {
                    *startFrame = frameCount - numFrames;
                    *endFrame = frameCount - 1;
                    return;
                }

                count++;
                numFrames = 0;
            }
            frameCount++;
            numFrames++;
        }

        *startFrame = *endFrame = 0;
    }

    void ModelMD2::setActionIndex(int index) {
        if (index < m_actionCount) {
            m_actionIdx = index;
            m_frameIdx = m_actions[m_actionIdx].min_idx;
            setEnabled(GL_TRUE);
        } else {
            setEnabled(GL_FALSE);
            m_actionIdx = -1;
        }
    }

    const char* ModelMD2::getActionName(int index) {
        if (index < m_actionCount)
            return m_actions[index].name;
        else
            return NULL;
    }

    int ModelMD2::getActionCount() {
        return m_actionCount;
    }

}
