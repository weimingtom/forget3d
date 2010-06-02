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

#include "image.h"

namespace F3D {
    /**
     * Image class for all games using F3D.
     */

    Image::Image() {
#ifdef DEBUG
        printf("Image constructor...\n");
#endif
    }

    Image::~Image() {
#ifdef DEBUG
        printf("Image destructor...\n");
#endif
    }

    void Image::fetchPallete(FILE *fd, Color pallete[], int count) {
        unsigned char buff[4];
        int i;

        fseek(fd, BMP_COLOR_OFFSET, SEEK_SET);
        for (i=0; i<count; i++) {
            //read(fd, buff, 4);
            fread (buff, 4, 1, fd);
            pallete[i].red = buff[2];
            pallete[i].green = buff[1];
            pallete[i].blue = buff[0];
            pallete[i].alpha = buff[3];
        }
        return;
    }

    Texture *Image::loadTexture(const char *filename) {
        FILE *fd;
        int  bpp, raster, i, j, skip, compression, index = 0;
        Texture *texture = (Texture *) malloc(sizeof(Texture));
        unsigned char buff[4];
        unsigned char id[2];
        //unsigned char *buffer = [width * height * 3];
        unsigned char *buffer;
        Color pallete[256];

#ifdef _WIN32_WCE
		fd = fopen(Utils::getFileName(filename), "rb");
#else
		fd = fopen(filename, "rb");
#endif
        if (!fd) {
#ifdef DEBUG
            printf("Open %s error!\n", filename);
        #if defined(WIN32) || defined(_WIN32_WCE)
			TCHAR errorStr[512];
			#ifdef _WIN32_WCE
			wsprintf(errorStr, TEXT("open image:[%s] error!"), Utils::getFileName(filename));
			#else
			wsprintf(errorStr, TEXT("open image:[%s] error!"), filename);
            #endif

			MessageBox(0, errorStr, TEXT("Image"), MB_OK);
        #endif
#endif
            return NULL;
        }
        //check is real bmp file
        fread (id, 2, 1, fd);

        if ( !(id[0]=='B' && id[1]=='M') ) {
            return NULL;
        }
#ifdef DEBUG
        printf("Load %s success!\n", filename);
#endif
        if (fseek(fd, BMP_TORASTER_OFFSET, SEEK_SET) == -1) {
            return NULL;
        }
        //read raster
        fread (buff, 4, 1, fd);
        raster = buff[0] + (buff[1]<<8) + (buff[2]<<16) + (buff[3]<<24);

        if (fseek(fd, BMP_SIZE_OFFSET, SEEK_SET) == -1) {
            return NULL;
        }
        //read width
        fread (buff, 4, 1, fd);
        m_width = buff[0] + (buff[1]<<8) + (buff[2]<<16) + (buff[3]<<24);
        //read height
        fread (buff, 4, 1, fd);
        m_height = buff[0] + (buff[1]<<8) + (buff[2]<<16) + (buff[3]<<24);
#ifdef DEBUG
        printf("Image width: %d, height: %d.\n", m_width, m_height);
#endif
        texture->width = m_width;
        texture->height = m_height;

        //read compression
        if (fseek(fd, BMP_RLE_OFFSET, SEEK_SET) == -1) {
            return NULL;
        }
        fread (buff, 4, 1, fd);
        compression = buff[0] + (buff[1]<<8) + (buff[2]<<16) + (buff[3]<<24);

        if (compression != 0) {
#ifdef DEBUG
			printf("Oly uncompressed bitmap is supported\n");
#endif
			return NULL;
		}

        //read bpp
        if (fseek(fd, BMP_BPP_OFFSET, SEEK_SET) == -1) {
            return NULL;
        }
        //read(fd, buff, 2);
        fread (buff, 2, 1, fd);
        bpp = buff[0] + (buff[1]<<8);
#ifdef DEBUG
        printf("Image bpp: %d.\n", bpp);
#endif
        buffer = (unsigned char *) malloc(m_width * m_height * (bpp == 32 ? 4 : 3) * sizeof(unsigned char));
        if (!buffer) {
            return NULL;
        }

        switch (bpp) {
        case 8: /* 8bit palletized */
            skip = fill4B(m_width);
            //if 8bit, read pallete first
            fetchPallete(fd, pallete, 256);
            fseek(fd, raster, SEEK_SET);
            //really read image data
            for (i = 0; i < m_height; i++) {
                for (j = 0; j < m_width; j++) {
                    //read(fd, buff, 1);
                    fread (buff, 1, 1, fd);
                    buffer[index++] = pallete[buff[0]].red;
                    buffer[index++] = pallete[buff[0]].green;
                    buffer[index++] = pallete[buff[0]].blue;
                }
                if (skip) {
                    //read(fd, buff, skip);
                    fread (buff, skip, 1, fd);
                }
            }
            break;
        case 24: /* 24bit RGB */
            skip = fill4B(m_width * 3);
            fseek(fd, raster, SEEK_SET);
            for (i = 0; i < m_height; i++) {
                for (j = 0; j < m_width; j++) {
                    //read(fd, buff, 3);
                    fread (buff, 3, 1, fd);
                    buffer[index++] = buff[2];
                    buffer[index++] = buff[1];
                    buffer[index++] = buff[0];
                }
                if (skip) {
                    //read(fd, buff, skip);
                    fread (buff, skip, 1, fd);
                }
            }
            break;
        case 32: /* 32 RGB */
            fseek(fd, raster, SEEK_SET);
            for (i = 0; i < m_height; i++) {
                for (j = 0; j < m_width; j++) {
                    //read(fd, buff, 3);
                    fread (buff, 4, 1, fd);
                    buffer[index++] = buff[2];
                    buffer[index++] = buff[1];
                    buffer[index++] = buff[0];
                    buffer[index++] = buff[3];
                }
            }
            break;
        default:
#ifdef DEBUG
        printf("Unsupport bpp: %d.\n", bpp);
#endif
            return NULL;
        }

        fclose(fd);

        glGenTextures(1, &(texture->textureId));
        glBindTexture(GL_TEXTURE_2D, texture->textureId);

        glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, (bpp == 32 ? GL_RGBA : GL_RGB),
            m_width, m_height, 0, (bpp == 32 ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, buffer);

#ifdef DEBUG
        printf("Load image %s success!\n", filename);
#endif
        FREEANDNULL(buffer);

        return texture;
    }

}
