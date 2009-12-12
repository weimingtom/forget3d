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

#ifndef F3D_MODELG3D_H_
#define F3D_MODELG3D_H_

#include "model.h"
#include "f3d.h"

#define NAME_SIZE   64

namespace F3D {

#ifdef ANDROID
#define F3D_PACKED __attribute__((packed))
#elif (defined(WIN32) || defined(_WIN32_WCE))
#ifndef F3D_PACKED
#pragma pack( push, packing )
#pragma pack( 1 )
#define F3D_PACKED
#endif
#endif

    /**
     * G3D model structs.
     */

    typedef struct {
        byte    magic[3];
        byte    version;
        uint16  meshCount;
        byte    type;
    } F3D_PACKED g3d_header_t;

    typedef struct {
        char    name[NAME_SIZE];
        int     frameCount;
        int     vertexCount;
        int     indexCount;
        GLfloat diffuseColor[3];
        GLfloat specularColor[3];
        GLfloat specularPower;
        GLfloat opacity;
        int     properties;
        int     textures;
    } F3D_PACKED g3d_mesh_header_t;

    typedef struct {
        g3d_mesh_header_t   header;
        Vec3f               *vertices;
        Vec3f               *normals;
        Vec2f               *uvs;
        unsigned int        *indices;
    } F3D_PACKED g3d_mesh_t;

#if (defined(WIN32) || defined(_WIN32_WCE))
#pragma pack( pop, packing )
#endif
#undef F3D_PACKED

    /**
     * ModelG3D class for all games using F3D.
     */
    class ModelG3D : public Model {
    private:
        g3d_header_t    m_header;
        //mesh data
        g3d_mesh_t      *m_g3dMeshs;
        int             m_frameIdx;
        int             m_frameCount;
        GLboolean       m_isPrepared;
        //private functions
        void prepareFrame();
    public:
        /**
        * Constructor
        */
        ModelG3D();

        /**
         * Destructor
         */
        virtual ~ModelG3D();

        bool loadModel(const char *filename);
    };

}

#endif /* F3D_MODELG3D_H_ */
