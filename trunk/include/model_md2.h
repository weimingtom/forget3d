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

#ifndef F3D_MODELMD2_H_
#define F3D_MODELMD2_H_

#include "model.h"

#define MD2_MAX_TRIANGLES		4096
#define MD2_MAX_VERTICES		2048
#define MD2_MAX_TEXCOORDS		2048
#define MD2_MAX_FRAMES			512
#define MD2_MAX_SKINS			32
#define MD2_MAX_FRAMESIZE		(MD2_MAX_VERTICES * 4 + 128)

#define NUMVERTEXNORMALS 162

namespace F3D {
    /**
     * MD2 model structs.
     */
    typedef struct {
        int magic;
        int version;
        int skinWidth;
        int skinHeight;
        int frameSize;
        int numSkins;
        int numVertices;
        int numTexCoords;
        int numTriangles;
        int numGlCommands;
        int numFrames;
        int offsetSkins;
        int offsetTexCoords;
        int offsetTriangles;
        int offsetFrames;
        int offsetGlCommands;
        int offsetEnd;
    } md2_header_t;

    typedef struct {
        byte vertex[3];
        byte lightNormalIndex;
    } md2_alias_triangleVertex_t;

    typedef struct {
        float vertex[3];
        float normal[3];
        //byte lightNormalIndex;
    } md2_triangleVertex_t;

    typedef struct {
        short vertexIndices[3];
        short textureIndices[3];
    } md2_triangle_t;

    typedef struct {
        short s, t;
    } md2_textureCoordinate_t;

    typedef struct {
        float scale[3];
        float translate[3];
        char name[16];
        md2_alias_triangleVertex_t alias_vertices[1];
    } md2_alias_frame_t;

    typedef struct {
        char name[16];
        md2_triangleVertex_t *vertices;
    } md2_frame_t;

    typedef char md2_skin_t[64];

    typedef struct {
        unsigned int index;
        char name[16];
        unsigned int min_idx;
        unsigned int max_idx;
    } md2_action_t;

    /**
     * ModelMD2 class for all games using F3D.
     */
    class ModelMD2 : public Model {
    private:
        md2_header_t			m_header;
        md2_skin_t				*m_skins;
        md2_textureCoordinate_t	*m_texCoords;
        md2_triangle_t			*m_triangles;
        md2_frame_t				*m_frames;
        md2_action_t            *m_actions;
        int                     m_textureId;
        unsigned int            m_frameIdx;
        int                     m_actionIdx;
        int                     m_actionCount;
        //private functions
        void prepareFrame();
        int getAnimationCount();
        const char* getAnimationName (int animation);
        void getAnimationFrames(int animation, unsigned int *startFrame, unsigned int *endFrame);
    public:
        /**
        * Constructor
        */
        ModelMD2();

        /**
         * Destructor
         */
        virtual ~ModelMD2();

        bool loadModel(const char *filename);
        void setActionIndex(int index);
        const char* getActionName(int index);
        int getActionCount();
    };

}

#endif /* F3D_MODELMD2_H_ */
