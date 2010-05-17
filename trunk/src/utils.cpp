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

#define IMPORTGL_NO_FNPTR_DEFS
#define IMPORTGL_API
#define IMPORTGL_FNPTRINIT = NULL

#include "utils.h"

#if (defined(WIN32) || defined(__MINGW32__) || defined(_WIN32_WCE))
#define WIN32_LEAN_AND_MEAN
#ifdef USE_WRAPPER_GL
static HMODULE hwdGlesLib = NULL;
#ifdef IS_ALONE_EGL
static HMODULE hwdEglLib = NULL; //system have alone libEGL.dll
#endif // !IS_ALONE_EGL
#endif // !USE_WRAPPER_GL
#endif // WIN32

#if (defined(LINUX) || defined(ANDROID))
#include <dlfcn.h>
#ifdef USE_WRAPPER_GL
static void *hwdGlesLib = NULL;
#ifdef IS_ALONE_EGL
static void *hwdEglLib = NULL; //if Android 2.0, have alone libEGL.dll
#endif // !IS_ALONE_EGL
#endif // !USE_WRAPPER_GL
#endif // LINUX

namespace F3D {

#ifdef _WIN32_WCE
	char* Utils::getFileName(const char *filename) {
		TCHAR wcwd[256];
		static char cwd[256];
		static char tmp[256];
		char *last;

		GetModuleFileName(NULL,wcwd,256);

		_snprintf(cwd,256,"%S",wcwd);

		last=strrchr(cwd,'\\');
		last[0]='\0';

		sprintf(tmp, "%s\\%s", cwd, filename);

		return tmp;
	}
#endif //_WIN32_WCE

#ifdef USE_WRAPPER_GL
	/* San Angeles Observation OpenGL ES version example
	 * Copyright 2004-2005 Jetro Lauha
	 * All rights reserved.
	 * Web: http://iki.fi/jetro/
	 *
	 * This source is free software; you can redistribute it and/or
	 * modify it under the terms of EITHER:
	 *   (1) The GNU Lesser General Public License as published by the Free
	 *       Software Foundation; either version 2.1 of the License, or (at
	 *       your option) any later version. The text of the GNU Lesser
	 *       General Public License is included with this source in the
	 *       file LICENSE-LGPL.txt.
	 *   (2) The BSD-style license that is included with this source in
	 *       the file LICENSE-BSD.txt.
	 *
	 * This source is distributed in the hope that it will be useful,
	 * but WITHOUT ANY WARRANTY; without even the implied warranty of
	 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the files
	 * LICENSE-LGPL.txt and LICENSE-BSD.txt for more details.
	 */

	/* Imports function pointers to selected function calls in OpenGL ES Common
	 * or Common Lite profile DLL or shared object. The function pointers are
	 * stored as global symbols with equivalent function name but prefixed with
	 * "funcPtr_". Standard gl/egl calls are redirected to the function pointers
	 * with preprocessor macros (see importgl.h).
	 */
	int Utils::initGlWrapper() {
		int result = 1;

	#undef IMPORT_FUNC

#if (defined(WIN32) || defined(__MINGW32__) || defined(_WIN32_WCE))
	#ifdef IS_ALONE_EGL
		hwdEglLib = LoadLibrary(TEXT("libEGL.dll")); //if have standar libEGL.dll
		if (hwdEglLib == NULL)
			return 0;   // Cannot find OpenGL ES EGL library.
        else
			MessageBox(0, TEXT("load opengl es library [libEGL.dll] successfully!"), TEXT("EGL Info"), MB_OK);

        #define IMPORT_FUNC_E(funcName) do { \
			void *procAddress = (void *)GetProcAddress(hwdEglLib, TEXT(#funcName)); \
			if (procAddress == NULL) { \
                TCHAR errorStr[512]; \
                wsprintf(errorStr, TEXT("egl function: %s don't exists!"), #funcName); \
                MessageBox(0, errorStr, TEXT("EGL Info"), MB_OK); \
                result = 0; \
            } \
			*((void **)&FNPTR(funcName)) = procAddress; } while (0)
	#endif

		hwdGlesLib = LoadLibrary(TEXT("libGLES_CM_NoE.dll")); //load rasteroid3.1 egl impl

		if (hwdGlesLib == NULL) {
			hwdGlesLib = LoadLibrary(TEXT("libGLESv1_CM.dll")); //load standard libGLESv1_CM.dll

            if (hwdGlesLib == NULL) {
                hwdGlesLib = LoadLibrary(TEXT("libgles_cm.dll"));// standard opengl es dll

                // if no libgles_cm.dll, then load libgles_cl.dll
                if (hwdGlesLib == NULL) {
                    hwdGlesLib = LoadLibrary(TEXT("libgles_cl.dll"));

                    if (hwdGlesLib == NULL) {
                        MessageBox(0, TEXT("Can't load opengl es library!"), TEXT("Error"), MB_OK);

                        return 0;   // Cannot find OpenGL ES Common or Common Lite DLL.
                    } else {
                        MessageBox(0, TEXT("load opengl es library [libgles_cl.dll] successfully!"), TEXT("EGL Info"), MB_OK);
                    }
                } else {
                    MessageBox(0, TEXT("load opengl es library [libgles_cm.dll] successfully!"), TEXT("EGL Info"), MB_OK);
                }
            } else {
                MessageBox(0, TEXT("load opengl es library [libGLESv1_CM.dll] successfully!"), TEXT("EGL Info"), MB_OK);
            }
		} else {
			MessageBox(0, TEXT("load opengl es library [libGLES_CM_NoE.dll] successfully!"), TEXT("EGL Info"), MB_OK);
		}

		/* The following fetches address to each egl & gl function call
		 * and stores it to the related function pointer. Casting through
		 * void * results in warnings with VC warning level 4, which
		 * could be fixed by casting to the true type for each fetch.
		 */
        #define IMPORT_FUNC(funcName) do { \
			void *procAddress = (void *)GetProcAddress(hwdGlesLib, TEXT(#funcName)); \
			if (procAddress == NULL) { \
                TCHAR errorStr[512]; \
                wsprintf(errorStr, TEXT("function: %s don't exists!"), #funcName); \
                MessageBox(0, errorStr, TEXT("EGL Info"), MB_OK); \
                result = 0; \
            } \
			*((void **)&FNPTR(funcName)) = procAddress; } while (0)
	#endif // WIN32

#if (defined(LINUX) || defined(ANDROID))
	#ifdef IS_ALONE_EGL
		hwdEglLib = dlopen("libEGL.so", RTLD_NOW); //if have standar libEGL.so
		if (hwdEglLib == NULL)
			return 0;   // Cannot find OpenGL ES EGL SO.
        else
            printf(("load opengl es egl library [libEGL.so] successfully!\n"));

        #define IMPORT_FUNC_E(funcName) do { \
			void *procAddress = (void *)dlsym(hwdEglLib, #funcName); \
			if (procAddress == NULL) { \
                printf(("function: %s don't exists!\n"), #funcName); \
                result = 0; \
            } \
			*((void **)&FNPTR(funcName)) = procAddress; } while (0)
	#endif // !IS_ALONE_EGL

		hwdGlesLib = dlopen("libGLES_CM.so", RTLD_NOW);

		if (hwdGlesLib == NULL) {
			hwdGlesLib = dlopen("libGLESv1_CM.so", RTLD_NOW);

            if (hwdGlesLib == NULL) {
                hwdGlesLib = dlopen("libGLES_CL.so", RTLD_NOW);

                if (hwdGlesLib == NULL)
                    return 0;   // Cannot find OpenGL ES Common or Common Lite SO.
                else
                    printf(("load opengl es library [libGLES_CL.so] successfully!\n"));
            } else
                printf(("load opengl es library [libGLESv1_CM.so] successfully!\n"));
        } else
            printf(("load opengl es library [libGLES_CM.so] successfully!\n"));

        #define IMPORT_FUNC(funcName) do { \
			void *procAddress = (void *)dlsym(hwdGlesLib, #funcName); \
			if (procAddress == NULL) { \
                printf(("function: %s don't exists!\n"), #funcName); \
                result = 0; \
            } \
			*((void **)&FNPTR(funcName)) = procAddress; } while (0)
#endif // LINUX

#ifdef IS_ALONE_EGL
        // load egl function from libEGL.so
		IMPORT_FUNC_E(eglChooseConfig);
		IMPORT_FUNC_E(eglCreateContext);
		IMPORT_FUNC_E(eglCreateWindowSurface);
		IMPORT_FUNC_E(eglDestroyContext);
		IMPORT_FUNC_E(eglDestroySurface);
		IMPORT_FUNC_E(eglGetConfigAttrib);
		IMPORT_FUNC_E(eglGetConfigs);
		IMPORT_FUNC_E(eglGetDisplay);
		IMPORT_FUNC_E(eglGetError);
		IMPORT_FUNC_E(eglInitialize);
		IMPORT_FUNC_E(eglMakeCurrent);
		IMPORT_FUNC_E(eglSwapBuffers);
		IMPORT_FUNC_E(eglTerminate);
		IMPORT_FUNC_E(eglQueryString);
	#ifdef ANDROID
        IMPORT_FUNC_E(eglQuerySurface);
	#endif
#else
		IMPORT_FUNC(eglChooseConfig);
		IMPORT_FUNC(eglCreateContext);
		IMPORT_FUNC(eglCreateWindowSurface);
		IMPORT_FUNC(eglDestroyContext);
		IMPORT_FUNC(eglDestroySurface);
		IMPORT_FUNC(eglGetConfigAttrib);
		IMPORT_FUNC(eglGetConfigs);
		IMPORT_FUNC(eglGetDisplay);
		IMPORT_FUNC(eglGetError);
		IMPORT_FUNC(eglInitialize);
		IMPORT_FUNC(eglMakeCurrent);
		IMPORT_FUNC(eglSwapBuffers);
		IMPORT_FUNC(eglTerminate);
		IMPORT_FUNC(eglQueryString);
	#ifdef ANDROID
        IMPORT_FUNC(eglQuerySurface);
	#endif
#endif // !IS_ALONE_EGL

		IMPORT_FUNC(glBlendFunc);
		IMPORT_FUNC(glClear);
		IMPORT_FUNC(glClearColorx);
		IMPORT_FUNC(glColor4x);
		IMPORT_FUNC(glColorPointer);
		IMPORT_FUNC(glDisable);
		IMPORT_FUNC(glDisableClientState);
		IMPORT_FUNC(glDrawArrays);
		IMPORT_FUNC(glEnable);
		IMPORT_FUNC(glEnableClientState);
		IMPORT_FUNC(glFrustumx);
		IMPORT_FUNC(glGetError);
		IMPORT_FUNC(glLightxv);
		IMPORT_FUNC(glLoadIdentity);
		IMPORT_FUNC(glMaterialx);
		IMPORT_FUNC(glMaterialxv);
		IMPORT_FUNC(glMatrixMode);
		IMPORT_FUNC(glMultMatrixx);
		IMPORT_FUNC(glNormalPointer);
		IMPORT_FUNC(glPopMatrix);
		IMPORT_FUNC(glPushMatrix);
		IMPORT_FUNC(glRotatex);
		IMPORT_FUNC(glScalex);
		IMPORT_FUNC(glShadeModel);
		IMPORT_FUNC(glTranslatex);
		IMPORT_FUNC(glVertexPointer);
		IMPORT_FUNC(glViewport);
		IMPORT_FUNC(glGetString);
		//add for forget3d
		IMPORT_FUNC(glHint);
		IMPORT_FUNC(glDepthFunc);
		IMPORT_FUNC(glClearColor);
		IMPORT_FUNC(glFrustumf);
		IMPORT_FUNC(glDrawTexiOES);
		IMPORT_FUNC(glTexParameteriv);
		IMPORT_FUNC(glColor4f);
		IMPORT_FUNC(glTexEnvf);
		IMPORT_FUNC(glTexParameterf);
		IMPORT_FUNC(glTexParameterx);
		IMPORT_FUNC(glBindTexture);
		IMPORT_FUNC(glDrawElements);
		IMPORT_FUNC(glTexCoordPointer);
		IMPORT_FUNC(glScalef);
		IMPORT_FUNC(glRotatef);
		IMPORT_FUNC(glTranslatef);
		IMPORT_FUNC(glTexImage2D);
		IMPORT_FUNC(glGenTextures);
		IMPORT_FUNC(glMultMatrixf);
		IMPORT_FUNC(glLightfv);
		IMPORT_FUNC(glFogfv);
		IMPORT_FUNC(glFogf);
		IMPORT_FUNC(glDepthMask);
		IMPORT_FUNC(glClearDepthf);

		return result;
	}

	void Utils::deinitGlWrapper() {
#if (defined(WIN32) || defined(__MINGW32__) || defined(_WIN32_WCE))
		FreeLibrary(hwdGlesLib);
	#ifdef IS_ALONE_EGL
		FreeLibrary(hwdEglLib);
	#endif //!IS_ALONE_EGL
#endif

#ifdef LINUX
		dlclose(hwdGlesLib);
		#ifdef IS_ALONE_EGL
		dlclose(hwdEglLib);
		#endif
#endif // !LINUX
	}
#endif //USE_WRAPPER_GL

}
