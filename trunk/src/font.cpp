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
    //another font constructor with real draw font width,height
    Font::Font(unsigned int charWidth,
               unsigned int charHeight,
               unsigned int fontWidth,
               unsigned int fontHeight,
               const char *texName) {
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
        Image *image = new Image();
        Texture *texture = image->loadTexture(texName);

        m_colCount = texture->width / m_charWidth;
        m_rowCount = texture->height / m_charHeight - 1;
#ifdef DEBUG
        printf("m_colCount: %d, m_rowCount: %d\n", m_colCount, m_rowCount);
#endif
        m_textureId = texture->textureId;

        delete image;
    }

    Font::~Font() {
#ifdef DEBUG
        printf("Font destructor...\n");
#endif
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
//        glMatrixMode(GL_PROJECTION);
//        glPushMatrix();
//        glLoadIdentity();
//
//        glOrthof(0.0f, 640.0f, 0.0f, 480.0f, 0.0f, 1.0f);
//        glMatrixMode(GL_MODELVIEW);
//        glPushMatrix();
//        glLoadIdentity();

        glLoadIdentity();
        glPushMatrix();

        //glColor4f(0x10000, 0x10000, 0x10000, 0x10000);
        glColor4f(0.9f, 0.2f, 0.1f, 0.0f);

        glShadeModel(GL_FLAT);//GL_SMOOTH,GL_FLAT
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glEnable(GL_DITHER);
        glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_ONE); //GL_SRC_ALPHA

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_textureId);

        glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

        for (unsigned int i = 0; i < strlen(str); i++) {
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

        glPopMatrix();

        /* test glBlendFunc
        int i = 0, j = 0;
        GLint crop0[4] = { 0, 0, 16, 18 };
        //GLint crop1[4] = { 88, 0, 8, 16 };
        GLint funcs [8] = { GL_ZERO, GL_ONE, GL_SRC_COLOR,
                            GL_ONE_MINUS_SRC_COLOR, GL_SRC_ALPHA,
                            GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA,
                            GL_ONE_MINUS_DST_ALPHA
                          };
        for (j = 0; j < 8; j++) {
            for (i = 0; i < 8; i++) {
                //printf("i: %d, j: %d\n", i, j);
                if (i == 0 && (j == 2 || j == 3))
                    continue;

                glBlendFunc(funcs[j], funcs[i]); //GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR, GL_ONE_MINUS_DST_ALPHA
                //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
                crop0[0] = i * 8;

                glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, crop0);
                glDrawTexiOES(i * 16, 240 - (j + 1) * 18, 0, 16, 16);
            }
        }
        */

        glDisable(GL_TEXTURE_2D);
        glShadeModel(GL_SMOOTH);//GL_SMOOTH,GL_FLAT
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glDisable(GL_DITHER);
#endif
    }

}
