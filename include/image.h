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

#ifndef F3D_IMAGE_H_
#define F3D_IMAGE_H_

#include "f3d.h"
#include "utils.h"
#include "world.h"

namespace F3D {
/**
 * Image class for all games using F3D.
 */

class Image {
private:
    Texture* m_texture;
    Color4f* m_color;
    GLuint   m_width;
    GLuint   m_height;
    //private function
    static void fetchPallete(FILE *fd, Color pallete[], int count);
    static GLubyte *loadBMP(FILE *fd, Texture *texture);
    static GLubyte *loadTGA(FILE *fd, Texture *texture);
public:
    /**
    * Constructor
    */
    Image(const char *filename, GLboolean is_absPath = GL_FALSE);

    /**
     * Destructor
     */
    virtual ~Image();
    //static function loadTexture
    static Texture *loadTexture(const char *filename, GLboolean is_absPath = GL_FALSE);

    //darw image at (x, y)
    void drawImage(int x, int y, DrawAnchor anchor = BOTTOM_LEFT);
    //darw image at (x, y) with new size(width, height)
    void drawImage(int x, int y, int width, int height, DrawAnchor anchor = BOTTOM_LEFT);
    //darw image at (x, y) with crop image(crpX, crpY, crpWidth, crpHeight)
    void drawImage(int x, int y, int crpX, int crpY, int crpWidth, int crpHeight, DrawAnchor anchor = BOTTOM_LEFT);
    //darw image at (x, y) with crop image(crpX, crpY) and new size(width, height)
    void drawImage(int x, int y, int crpX, int crpY, int crpWidth, int crpHeight, int width, int height, DrawAnchor anchor = BOTTOM_LEFT);
    //get image width & height
    GLuint getWidth();
    GLuint getHeight();
    //image color
    void setImageColor(Color4f* color);
    Color4f* getImageColor();
};

}

#endif /* F3D_IMAGE_H_ */
