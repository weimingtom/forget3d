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

#include "model_ms3d.h"


void applyRotationToMatrix(const float angles[3], float matrix[3][4] ) {
	float angle;
	float sr, sp, sy, cr, cp, cy;

	angle = angles[2];
	sy = sin(angle);
	cy = cos(angle);
	angle = angles[1];
	sp = sin(angle);
	cp = cos(angle);
	angle = angles[0];
	sr = sin(angle);
	cr = cos(angle);

	// matrix = (Z * Y) * X
	matrix[0][0] = cp*cy;
	matrix[1][0] = cp*sy;
	matrix[2][0] = -sp;
	matrix[0][1] = sr*sp*cy+cr*-sy;
	matrix[1][1] = sr*sp*sy+cr*cy;
	matrix[2][1] = sr*cp;
	matrix[0][2] = (cr*sp*cy+-sr*-sy);
	matrix[1][2] = (cr*sp*sy+-sr*cy);
	matrix[2][2] = cr*cp;
	matrix[0][3] = 0.0;
	matrix[1][3] = 0.0;
	matrix[2][3] = 0.0;
}

//multiply matrix to current matrix
void multMatrices (const float mul[3][4], float cur[3][4]) {
    float tmp[3][4];
    //copy cur to tmp matrix
    memcpy(tmp, cur, sizeof(12 * sizeof(float)));
    //multiply martrices
	cur[0][0] = mul[0][0] * tmp[0][0] + mul[0][1] * tmp[1][0] +
				mul[0][2] * tmp[2][0];
	cur[0][1] = mul[0][0] * tmp[0][1] + mul[0][1] * tmp[1][1] +
				mul[0][2] * tmp[2][1];
	cur[0][2] = mul[0][0] * tmp[0][2] + mul[0][1] * tmp[1][2] +
				mul[0][2] * tmp[2][2];
	cur[0][3] = mul[0][0] * tmp[0][3] + mul[0][1] * tmp[1][3] +
				mul[0][2] * tmp[2][3] + mul[0][3];
	cur[1][0] = mul[1][0] * tmp[0][0] + mul[1][1] * tmp[1][0] +
				mul[1][2] * tmp[2][0];
	cur[1][1] = mul[1][0] * tmp[0][1] + mul[1][1] * tmp[1][1] +
				mul[1][2] * tmp[2][1];
	cur[1][2] = mul[1][0] * tmp[0][2] + mul[1][1] * tmp[1][2] +
				mul[1][2] * tmp[2][2];
	cur[1][3] = mul[1][0] * tmp[0][3] + mul[1][1] * tmp[1][3] +
				mul[1][2] * tmp[2][3] + mul[1][3];
	cur[2][0] = mul[2][0] * tmp[0][0] + mul[2][1] * tmp[1][0] +
				mul[2][2] * tmp[2][0];
	cur[2][1] = mul[2][0] * tmp[0][1] + mul[2][1] * tmp[1][1] +
				mul[2][2] * tmp[2][1];
	cur[2][2] = mul[2][0] * tmp[0][2] + mul[2][1] * tmp[1][2] +
				mul[2][2] * tmp[2][2];
	cur[2][3] = mul[2][0] * tmp[0][3] + mul[2][1] * tmp[1][3] +
				mul[2][2] * tmp[2][3] + mul[2][3];
}

namespace F3D {
    /**
     * ModelMS3D class for all games using F3D.
     */

    ModelMS3D::ModelMS3D() :
            m_vertices(NULL),
            m_triangles(NULL),
            m_groups(NULL),
            m_materials(NULL),
            m_joints(NULL),
            m_frameCount(-1),
            m_frameIdx(-1),
            m_isPrepared(GL_FALSE) {
#ifdef DEBUG
        printf("ModelMS3D constructor...\n");
#endif
    }

    ModelMS3D::~ModelMS3D() {
#ifdef DEBUG
        printf("ModelMS3D destructor...\n");
#endif
        int i;
        FREEANDNULL(m_vertices);
        FREEANDNULL(m_triangles);
        //free group's triangle indices & group
        for (i = 0; i < m_groupsCount; i++)
            FREEANDNULL(m_groups[i].triangleIndices);
        FREEANDNULL(m_groups);

        FREEANDNULL(m_materials);
        for (i = 0; i < m_groupsCount; i++) {
            FREEANDNULL(m_joints[i].keyFramesRot);
            FREEANDNULL(m_joints[i].keyFramesTrans);
        }
        FREEANDNULL(m_joints);
    }

    bool ModelMS3D::loadModel(const char *filename) {
        FILE *file;
        int i, j;

        file = fopen (filename, "rb");
        if (!file) {
            printf("Open MS3D model:%s failure!\n", filename);

            return false;
        }

        /* initialize model and read header */
        fread (&m_header, sizeof (ms3d_header_t), 1, file);
#ifdef DEBUG
        printf("id:%s\n", m_header.id);
        printf("version:%d\n", m_header.version);
#endif

        if (!strcmp((char *)m_header.id, "MS3D000000") || m_header.version != 4) {
            printf("Invalid MS3D model or unsupport version, id:[%s], version[%d]...\n",
                    m_header.id, m_header.version);
            fclose(file);

            return false;
        }

        //read vertices
        fread(&m_verticesCount, sizeof(uint16), 1, file);
#ifdef DEBUG
        printf("m_verticesCount:%d\n", m_verticesCount);
#endif
        m_vertices = new ms3d_vertex_t[m_verticesCount];
        fread(m_vertices, sizeof(ms3d_vertex_t), m_verticesCount, file);

        //read triangles
        fread(&m_trianglesCount, sizeof(uint16), 1, file);
#ifdef DEBUG
        printf("m_trianglesCount:%d\n", m_trianglesCount);
#endif
        m_triangles = new ms3d_triangle_t[m_trianglesCount];
        fread(m_triangles, sizeof(ms3d_triangle_t), m_trianglesCount, file);

        //read groups
        fread(&m_groupsCount, sizeof(uint16), 1, file);
#ifdef DEBUG
        printf("m_groupsCount:%d\n", m_groupsCount);
#endif
        m_groups = new ms3d_group_t[m_groupsCount];
        for (i = 0; i < m_groupsCount; i++) {
            fread(&m_groups[i].flags, sizeof(byte), 1, file);
            fread(&m_groups[i].name, MS3D_NAME_SIZE, 1, file);
            fread(&m_groups[i].numTriangles, sizeof(uint16), 1, file);
            //after read triangles count, read triangle indices
            m_groups[i].triangleIndices = new uint16[m_groups[i].numTriangles];
            fread(&m_groups[i].triangleIndices[0], sizeof(uint16), m_groups[i].numTriangles, file);
            //read material index
            fread(&m_groups[i].materialIndex, sizeof(char), 1, file);
#ifdef DEBUG
            printf("m_groups[%d] name:%s\n", i, m_groups[i].name);
            printf("m_groups[%d] numTriangles:%d\n", i, m_groups[i].numTriangles);
            printf("m_groups[%d] materialIndex:%d\n", i, m_groups[i].materialIndex);
#endif
        }
        //set model mesh count to group count to init model meshs.
        setMeshCount(m_groupsCount);

        //read materials
        fread(&m_materialsCount, sizeof(uint16), 1, file);
#ifdef DEBUG
        printf("m_materialsCount:%d\n", m_materialsCount);
#endif
        if (m_materialsCount > 0) {
            m_materials = new ms3d_material_t[m_materialsCount];
            fread(m_materials, sizeof(ms3d_material_t), m_materialsCount, file);
#ifdef DEBUG
            for (i = 0; i < m_materialsCount; i++) {
                printf("m_materials[%d].name:%s\n", i, m_materials[i].name);
                printf("m_materials[%d].texture:%s\n", i, m_materials[i].texture);
            }
#endif
        }

        //read animation fps, current time, total frames
        fread(&m_animationFPS, sizeof(float), 1, file);
        fread(&m_currentTime, sizeof(float), 1, file);
        fread(&m_totalFrames, sizeof(int), 1, file);
        //set frame count & frame index to 0
        m_frameCount = m_totalFrames - 1; // totleFrames start with "1", not "0"
        m_frameIdx = 0;
#ifdef DEBUG
        printf("m_animationFPS:%.4f\n", m_animationFPS);
        printf("m_currentTime:%.4f\n", m_currentTime);
        printf("m_totalFrames:%d\n", m_totalFrames);
#endif
        //read joints
        fread(&m_jointsCount, sizeof(uint16), 1, file);
#ifdef DEBUG
        printf("m_jointsCount:%d\n", m_jointsCount);
#endif
        if (m_jointsCount > 0) {
            m_joints = new ms3d_joint_t[m_jointsCount];
            for (i = 0; i < m_jointsCount; i++) {
                fread(&m_joints[i].header, sizeof(ms3d_joint_header_t), 1, file);
                //read frame rots
                m_joints[i].keyFramesRot = new ms3d_keyframe_rot_t[m_joints[i].header.numKeyFramesRot];
                fread(&m_joints[i].keyFramesRot[0], sizeof(ms3d_keyframe_rot_t),
                    m_joints[i].header.numKeyFramesRot, file);

                //read frame trans
                m_joints[i].keyFramesTrans = new ms3d_keyframe_pos_t[m_joints[i].header.numKeyFramesTrans];
                fread(&m_joints[i].keyFramesTrans[0], sizeof(ms3d_keyframe_pos_t),
                    m_joints[i].header.numKeyFramesTrans, file);

                //get parent index by parentName
                m_joints[i].parentJointIndex = -1;
                if (strlen(m_joints[i].header.parentName) > 0) {
                    for (j = 0; j < i; j++) {
                        if (strcmp(m_joints[i].header.parentName, m_joints[j].header.name) == 0) {
                            m_joints[i].parentJointIndex = j;
                            //find the parent index by name, break!
                            break;
                        }
                    }
                }

                //setup joints
                applyRotationToMatrix(m_joints[i].header.rotation, m_joints[i].relMatrix);
                m_joints[i].relMatrix[0][3]= m_joints[i].header.position[0];
                m_joints[i].relMatrix[1][3]= m_joints[i].header.position[1];
                m_joints[i].relMatrix[2][3]= m_joints[i].header.position[2];
                //apply parent joint matrix to current joint
                if (m_joints[i].parentJointIndex != -1) {
                    multMatrices(m_joints[m_joints[i].parentJointIndex].absMatrix, m_joints[i].absMatrix);
                } else {
                    //if no parent, copy relMatrix to absMatrix
                    memcpy(m_joints[i].absMatrix, m_joints[i].relMatrix, sizeof(12 * sizeof(float)));
                }
#ifdef DEBUG
                printf("================================================\n");
                printf("m_joints[%d] name:%s\n", i, m_joints[i].header.name);
                printf("m_joints[%d] parentName:%s\n", i, m_joints[i].header.parentName);
                printf("m_joints[%d] numKeyFramesRot:%d\n", i, m_joints[i].header.numKeyFramesRot);
                printf("m_joints[%d] numKeyFramesTrans:%d\n", i, m_joints[i].header.numKeyFramesTrans);
                printf("m_joints[%d] parentJointIndex:%d\n", i, m_joints[i].parentJointIndex);
#endif
            }
        }

        //ignore other sections, such as sub version, comments, etc...
        fclose(file);

        return true;
    }

    void ModelMS3D::prepareFrame() {
#ifdef DEBUG
        static bool isFirst = true;
        if (isFirst) {
            printf("Call ModelMS3D::prepareFrame()...\n");
            isFirst = false;
        }
#endif
        if ((m_frameCount < 0 || m_frameIdx < 0 || (m_joints == NULL)) && m_isPrepared) {
#ifdef DEBUG
            printf("No joint or frame, don't need prepare again...\n");
#endif
            return;
        }

        for (int i = 0; i < m_groupsCount; i++) {
            int v_idx = 0, n_idx = 0, u_idx = 0;
            float *vertices = new float[m_groups[i].numTriangles * 9];
            float *normals = new float[m_groups[i].numTriangles * 9];
            float *uvs = NULL;

            if (m_materials != NULL && !m_isPrepared)
                uvs = new float[m_groups[i].numTriangles * 6];
            //read all group triangles
            for (int j = 0; j < m_groups[i].numTriangles; j++) {
                //get the triangle & vertex indices
                ms3d_triangle_t triangle = m_triangles[m_groups[i].triangleIndices[j]];
                for (int m = 0; m < 3; m++) {
                    //get the vertex, and apply joint data to it
                    ms3d_vertex_t vertex = m_vertices[triangle.vertexIndices[m]];
                    if (vertex.boneId != -1) {
                        //
                    }
                    //copy vertex data to mesh
                    for (int n = 0; n < 3; n++) {
                        vertices[v_idx++] = m_vertices[triangle.vertexIndices[m]].vertex[n]; // copy vertices
                        normals[n_idx++] = triangle.vertexNormals[m][n]; // copy normals
                    }
                    if (m_materials != NULL && !m_isPrepared) {
                        uvs[u_idx++] = triangle.s[m];
                        uvs[u_idx++] = 1.0f - triangle.t[m];
                    }
                }
            }
            //set data to mesh
            setVertices(vertices, m_groups[i].numTriangles * 9 * sizeof(float), i);
            setNormals(normals, m_groups[i].numTriangles * 9 * sizeof(float), i);
            if (m_materials != NULL && !m_isPrepared) {
                setUvs(uvs, m_groups[i].numTriangles * 6 * sizeof(float), i);
            }
            //set mesh triangle count
            setTriangleNums(m_groups[i].numTriangles, i);
        }

        //after prepare frame, set m_isPrepared to true
        if (!m_isPrepared)
            m_isPrepared = GL_TRUE;
    }

}
