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

#ifndef F3D_H_
#define F3D_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#if (defined(WIN32) || defined(_WIN32_WCE))
#include <windows.h>
#include <tchar.h>
#endif

#include "f3d_gl.h"

#ifndef M_PI
#define M_PI 3.1415926535897932f
#endif

#ifdef _WIN32_WCE
#define sinf(value) sin(value)
#define asinf(value) asin(value)
#define cosf(value) cos(value)
#endif

#define DTOR (M_PI / 180.0f)
#define SQR(x) (x * x)

#ifndef byte
typedef unsigned char byte;
#endif /* byte */

#ifndef uint16
typedef unsigned short  uint16;
#endif /* uint16 */

typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
} Color;

typedef struct {
    GLsizei width;
    GLsizei height;
    GLuint textureId;
} Texture;

typedef struct {
    GLfloat s;
    GLfloat t;
} Vec2f;

typedef struct {
    GLfloat x;
    GLfloat y;
    GLfloat z;
} Vec3f;

#define ONE	(1 << 16)

// Capped conversion from float to fixed.
#define FIXED(value) \
        value < -32768.0f ? -32768 : value > 32767.0f ? 32767 : (int)(value * ONE)

#ifdef ANDROID
//get the ms unit time in Linux & Android
#define CLOCK(v_time) v_time.tv_sec * 1000 + v_time.tv_usec / 1000
#endif

#define FREEANDNULL(pointer) \
    if (pointer != NULL) { \
        free(pointer); \
        pointer = NULL; \
    }

#define DELETEANDNULL(pointer, isArray) \
    if (pointer != NULL) { \
		if (isArray) \
			delete [] pointer; \
		else \
			delete pointer; \
        pointer = NULL; \
    }

#endif // F3D_H_
