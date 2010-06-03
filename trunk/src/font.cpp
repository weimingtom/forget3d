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

#include "font.h"

namespace F3D {
    /**
     * Font class for all games using F3D.
     */
    Font::Font(GLuint charWidth,
               GLuint charHeight,
               const char *texName) :
            m_texture(NULL),
            m_color(NULL) {
        createFont(charWidth, charHeight, charWidth, charHeight, texName);
    }

    //another font constructor with real draw font width,height
    Font::Font(GLuint charWidth,
               GLuint charHeight,
               GLuint fontWidth,
               GLuint fontHeight,
               const char *texName) :
            m_texture(NULL),
            m_color(NULL){
        createFont(charWidth, charHeight, fontWidth, fontHeight, texName);
    }

    void Font::createFont(GLuint charWidth,
               GLuint charHeight,
               GLuint fontWidth,
               GLuint fontHeight,
               const char *texName) {
#ifdef DEBUG
        printf("Font constructor...\n");
#endif
        m_charWidth = charWidth;
        m_charHeight = charHeight;
#ifdef DEBUG
        printf("m_charWidth: %d, m_charHeight: %d\n", m_charWidth, m_charHeight);
#endif
        m_fontWidth = fontWidth;
        m_fontHeight = fontHeight;
#ifdef DEBUG
        printf("m_fontWidth: %d, m_fontHeight: %d\n", m_fontWidth, m_fontHeight);
#endif
        m_texture = Image::loadTexture(texName);

        m_colCount = m_texture->width / m_charWidth;
        m_rowCount = m_texture->height / m_charHeight - 1;
#ifdef DEBUG
        printf("m_colCount: %d, m_rowCount: %d\n", m_colCount, m_rowCount);
#endif

        m_color = (Color4f*) malloc(sizeof(Color4f));
        m_color->red = 1.0f;
        m_color->green = 1.0f;
        m_color->blue = 1.0f;
        m_color->alpha = 1.0f;
    }

    Font::~Font() {
#ifdef DEBUG
        printf("Font destructor...\n");
#endif
        DELETEANDNULL(m_color, false);
        DELETEANDNULL(m_texture, false);
    }

    GLuint Font::getFonWidth() {
        return m_fontWidth;
    }

    GLuint Font::getFonHeight() {
        return m_fontHeight;
    }

    void Font::drawString(int x, int y, const char *str) {
        drawString(x, y, m_fontWidth, m_fontHeight, str);
    }

    void Font::drawString(int x, int y, int fontWidth, int fontHeight, const char *str) {
#ifndef GL_OES_draw_texture
#ifdef DEBUG
        printf("Unsupport GL_OES_draw_texture extension...\n");
#endif
#else
        glPushMatrix();

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_FOG);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE); //GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_texture->textureId);

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        glColor4f(m_color->red, m_color->green, m_color->blue, m_color->alpha);

        for (GLuint i = 0; i < strlen(str); i++) {
            int index = (int)(str[i] - 32);
#if (defined(DEBUG) && defined(SHOW_FONT_POS))
            printf("str[%d]: %c, index: %d\n", i, str[i], index);
#endif
            GLint crop[4] = { 0, 0, m_charWidth, m_charHeight };

            int x_idx = index % m_colCount;
            int y_idx = (int)(index / m_colCount);
            crop[0] = x_idx * m_charWidth;
            crop[1] = (m_rowCount - y_idx) * m_charHeight;
#if (defined(DEBUG) && defined(SHOW_FONT_POS))
            printf("x_idx: %d, y_idx: %d, crop[0]: %d, crop[1]: %d\n", x_idx, y_idx, crop[0], crop[1]);
#endif

            glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, crop);
            glDrawTexiOES(x + i * fontWidth, y, 0, fontWidth, fontHeight);
        }

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);

		glPopMatrix();
#endif
    }

    void Font::setFontColor(Color4f* color) {
        m_color->red = color->red;
        m_color->green = color->green;
        m_color->blue = color->blue;
        m_color->alpha = color->alpha;
    }

    Color4f* Font::getFontColor() {
        return m_color;
    }

}
