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

#ifndef F3D_FONT_H_
#define F3D_FONT_H_

#include "f3d.h"
#include "image.h"

namespace F3D {
/**
 * Font class for all games using F3D.
 */
class Font {
private:
    GLuint      m_charWidth;
    GLuint      m_charHeight;
    GLuint      m_colCount;
    GLuint      m_rowCount;
    GLuint      m_fontWidth;
    GLuint      m_fontHeight;
    Texture*    m_texture;
    Color4f*    m_color;
    //private functions
    void createFont(GLuint charWidth,
                    GLuint charHeight,
                    GLuint fontWidth,
                    GLuint fontHeight,
                    const char *texName,
					GLboolean is_absPath = GL_FALSE);
public:
    /**
    * Constructor
    */
    Font(GLuint charWidth,
         GLuint charHeight,
         const char *texName,
		 GLboolean is_absPath = GL_FALSE);	// the texName is absolute path?
    //another font constructor with real draw font width,height
    Font(GLuint charWidth,
         GLuint charHeight,
         GLuint fontWidth,
         GLuint fontHeight,
         const char *texName,
		 GLboolean is_absPath = GL_FALSE);

    /**
     * Destructor
     */
    virtual ~Font();

    void drawString(int x, int y, const char *str, DrawAnchor anchor = BOTTOM_LEFT);
    void drawString(int x, int y, int fontWidth, int fontHeight, const char *str, DrawAnchor anchor = BOTTOM_LEFT);
    GLuint getFonWidth();
    GLuint getFonHeight();
    //font color
    void setFontColor(Color4f* color);
    Color4f* getFontColor();
};

}

#endif /* F3D_FONT_H_ */
