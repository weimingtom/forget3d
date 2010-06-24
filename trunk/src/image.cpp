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

//bmp file offset
#define BMP_TORASTER_OFFSET	10
#define BMP_SIZE_OFFSET		18
#define BMP_BPP_OFFSET		28
#define BMP_RLE_OFFSET		30
#define BMP_COLOR_OFFSET	54
//tga file offset
#define TGA_COLORMAP_OFFSET	1
#define TGA_SIZE_OFFSET	    12
#define TGA_DATA_OFFSET	    18

#define fill4B(a)	( ( 4 - ( (a) % 4 ) ) & 0x03)

namespace F3D {
    /**
     * Image class for all games using F3D.
     */

    Image::Image(const char *filename) :
            m_texture(NULL),
            m_color(NULL) {
#ifdef DEBUG
        printf("Image constructor...\n");
#endif
        m_texture = loadTexture(filename);
        m_width = m_texture->width;
        m_height = m_texture->height;

        m_color = (Color4f*) malloc(sizeof(Color4f));
        m_color->red = 1.0f;
        m_color->green = 1.0f;
        m_color->blue = 1.0f;
        m_color->alpha = 1.0f;
    }

    Image::~Image() {
#ifdef DEBUG
        printf("Image destructor...\n");
#endif
        FREEANDNULL(m_color);
        FREEANDNULL(m_texture);
    }

    GLuint Image::getWidth() {
        return m_width;
    }

    GLuint Image::getHeight() {
        return m_height;
    }

    void Image::setImageColor(Color4f* color) {
        m_color->red = color->red;
        m_color->green = color->green;
        m_color->blue = color->blue;
        m_color->alpha = color->alpha;
    }

    Color4f* Image::getImageColor() {
        return m_color;
    }

    void Image::drawImage(int x, int y) {
        drawImage(x, y, 0, 0, m_width, m_height, m_width, m_height);
    }

    void Image::drawImage(int x, int y, int width, int height) {
        drawImage(x, y, 0, 0, m_width, m_height, width, height);
    }

    void Image::drawImage(int x, int y, int crpX, int crpY, int crpWidth, int crpHeight) {
        drawImage(x, y, crpX, crpY, crpWidth, crpHeight, crpWidth, crpHeight);
    }

    void Image::drawImage(int x, int y, int crpX, int crpY, int crpWidth, int crpHeight, int width, int height) {
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

        GLint crop[4] = { crpX, crpY, crpWidth, crpHeight };
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, crop);
        glDrawTexiOES(x, y, 0, width, height);

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);

		glPopMatrix();
#endif // ! GL_OES_draw_texture
    }

    void Image::fetchPallete(FILE *fd, Color pallete[], int count) {
        GLubyte buff[4];
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

    Texture* Image::loadTexture(const char *filename) {
        FILE *fd;
        GLubyte *buffer = NULL;
        Texture *texture = (Texture *) malloc(sizeof(Texture));

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

        // Check for known extensions
        if ( stricmp(strstr(filename, "."), ".bmp") == 0 ) {
            buffer = loadBMP(fd, texture);
        } else if ( stricmp(strstr(filename, "."), ".tga") == 0 ) {
            buffer = loadTGA(fd, texture);
        } else {
            printf("Unsupport image file format!\n");
            return NULL;
        }

        if (buffer == NULL)
            return NULL;

        fclose(fd);

        glGenTextures(1, &(texture->textureId));
        glBindTexture(GL_TEXTURE_2D, texture->textureId);

        glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, texture->type, texture->width, texture->height, 0, texture->type, GL_UNSIGNED_BYTE, buffer);

#ifdef DEBUG
        printf("Load image %s success!\n", filename);
#endif
        FREEANDNULL(buffer);

        return texture;
    }

    GLubyte *Image::loadBMP(FILE *fd, Texture *texture) {
        int  bpp, raster, i, j, skip, compression, width, height, index = 0;
        GLubyte buff[4];
        GLubyte id[2];
        Color pallete[256];

        //check is real bmp file
        fread (id, 2, 1, fd);

        if ( !(id[0]=='B' && id[1]=='M') ) {
            return NULL;
        }

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
        width = buff[0] + (buff[1]<<8) + (buff[2]<<16) + (buff[3]<<24);
        //read height
        fread (buff, 4, 1, fd);
        height = buff[0] + (buff[1]<<8) + (buff[2]<<16) + (buff[3]<<24);
#ifdef DEBUG
        printf("Image width: %d, height: %d.\n", width, height);
#endif
        texture->width = width;
        texture->height = height;

        //read compression
        if (fseek(fd, BMP_RLE_OFFSET, SEEK_SET) == -1) {
            return NULL;
        }
        fread (buff, 4, 1, fd);
        compression = buff[0] + (buff[1]<<8) + (buff[2]<<16) + (buff[3]<<24);

        if (compression != 0) {
#ifdef DEBUG
			printf("Only uncompressed bitmap is supported\n");
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
        GLubyte *buffer = (GLubyte *) malloc(width * height * (bpp == 32 ? 4 : 3) * sizeof(GLubyte));
        if (!buffer) {
            return NULL;
        }

        texture->type = (bpp == 32 ? GL_RGBA : GL_RGB);

        switch (bpp) {
        case 8: /* 8bit palletized */
            skip = fill4B(width);
            //if 8bit, read pallete first
            fetchPallete(fd, pallete, 256);
            fseek(fd, raster, SEEK_SET);
            //really read image data
            for (i = 0; i < height; i++) {
                for (j = 0; j < width; j++) {
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
            skip = fill4B(width * 3);
            fseek(fd, raster, SEEK_SET);
            for (i = 0; i < height; i++) {
                for (j = 0; j < width; j++) {
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
            for (i = 0; i < height; i++) {
                for (j = 0; j < width; j++) {
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

        return buffer;
    }

    GLubyte *Image::loadTGA(FILE *fd, Texture *texture) {
        int  i, j, index = 0;
        GLubyte	color_map, image_type, bpp;
        GLushort width, height;
        GLubyte buff[4];
        GLubyte size[2];

        //read color map & image type
        if (fseek(fd, TGA_COLORMAP_OFFSET, SEEK_SET) == -1) {
            return NULL;
        }
        fread(&color_map, 1, 1, fd);
        //read image type
        fread(&image_type, 1, 1, fd);

        if (image_type != 2) {
#ifdef DEBUG
			printf("Only uncompressed tga is supported!\n");
#endif
			return NULL;
		}
        if (color_map != 0) {
#ifdef DEBUG
			printf("Truecolor image with colormap not supported!\n");
#endif
			return NULL;
		}
        //read size(width, height) & bpp
        if (fseek(fd, TGA_SIZE_OFFSET, SEEK_SET) == -1) {
            return NULL;
        }
        fread(size, 2, 1, fd);
        width = size[0] + (size[1]<<8);
        fread(size, 2, 1, fd);
        height = size[0] + (size[1]<<8);
        //read bpp
        fread(&bpp, 1, 1, fd);
#ifdef DEBUG
        printf("Image[tga] width: %d, height: %d, bpp: %d.\n", width, height, bpp);
#endif
        texture->width = width;
        texture->height = height;
        texture->type = (bpp == 32 ? GL_RGBA : GL_RGB);

        GLubyte *buffer = (GLubyte *) malloc(width * height * (bpp == 32 ? 4 : 3) * sizeof(GLubyte));
        if (!buffer) {
            return NULL;
        }
        //read image buffer
        if (fseek(fd, TGA_DATA_OFFSET, SEEK_SET) == -1) {
            return NULL;
        }

        switch (bpp) {
        case 24: /* 24bit RGB */
            for (i = 0; i < height; i++) {
                for (j = 0; j < width; j++) {
                    //read(fd, buff, 3);
                    fread (buff, 3, 1, fd);
                    buffer[index++] = buff[2];
                    buffer[index++] = buff[1];
                    buffer[index++] = buff[0];
                }
            }
            break;
        case 32: /* 32 RGB */
            for (i = 0; i < height; i++) {
                for (j = 0; j < width; j++) {
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

        return buffer;
    }

}
