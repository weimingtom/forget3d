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

#ifndef F3D_GL_H_
#define F3D_GL_H_

#ifdef __cplusplus
extern "C" {
#endif

//opengl es header
#include <GLES/gl.h>
#include <GLES/egl.h>

//if Android platform, add glext.h
#ifdef ANDROID
#include <GLES/glext.h>
#endif

/* Use USE_WRAPPER_GL if you want to link the OpenGL ES at
 * compile/link time and not import it dynamically runtime.
 */
#ifdef USE_WRAPPER_GL

#ifndef IMPORTGL_API
#define IMPORTGL_API extern
#endif
#ifndef IMPORTGL_FNPTRINIT
#define IMPORTGL_FNPTRINIT
#endif

#define FNDEF(retType, funcName, args) IMPORTGL_API retType (*funcPtr_##funcName) args IMPORTGL_FNPTRINIT

FNDEF(EGLBoolean, eglChooseConfig, (EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config));
FNDEF(EGLContext, eglCreateContext, (EGLDisplay dpy, EGLConfig config, EGLContext share_list, const EGLint *attrib_list));
FNDEF(EGLSurface, eglCreateWindowSurface, (EGLDisplay dpy, EGLConfig config, NativeWindowType window, const EGLint *attrib_list));
FNDEF(EGLBoolean, eglDestroyContext, (EGLDisplay dpy, EGLContext ctx));
FNDEF(EGLBoolean, eglDestroySurface, (EGLDisplay dpy, EGLSurface surface));
FNDEF(EGLBoolean, eglGetConfigAttrib, (EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value));
FNDEF(EGLBoolean, eglGetConfigs, (EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config));
FNDEF(EGLDisplay, eglGetDisplay, (NativeDisplayType display));
FNDEF(EGLint, eglGetError, (void));
FNDEF(EGLBoolean, eglInitialize, (EGLDisplay dpy, EGLint *major, EGLint *minor));
FNDEF(EGLBoolean, eglMakeCurrent, (EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx));
FNDEF(EGLBoolean, eglSwapBuffers, (EGLDisplay dpy, EGLSurface draw));
FNDEF(EGLBoolean, eglTerminate, (EGLDisplay dpy));
//add by martin
FNDEF(const char*, eglQueryString, (EGLDisplay dpy, EGLint name));
#ifdef ANDROID
FNDEF(EGLBoolean, eglQuerySurface, (EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint* value));
#endif

FNDEF(void, glBlendFunc, (GLenum sfactor, GLenum dfactor));
FNDEF(void, glClear, (GLbitfield mask));
FNDEF(void, glClearColorx, (GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha));
FNDEF(void, glColor4x, (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha));
FNDEF(void, glColorPointer, (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer));
FNDEF(void, glDisable, (GLenum cap));
FNDEF(void, glDisableClientState, (GLenum array));
FNDEF(void, glDrawArrays, (GLenum mode, GLint first, GLsizei count));
FNDEF(void, glEnable, (GLenum cap));
FNDEF(void, glEnableClientState, (GLenum array));
FNDEF(void, glFrustumx, (GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar));
FNDEF(GLenum, glGetError, (void));
FNDEF(void, glLightxv, (GLenum light, GLenum pname, const GLfixed *params));
FNDEF(void, glLoadIdentity, (void));
FNDEF(void, glMaterialx, (GLenum face, GLenum pname, GLfixed param));
FNDEF(void, glMaterialxv, (GLenum face, GLenum pname, const GLfixed *params));
FNDEF(void, glMatrixMode, (GLenum mode));
FNDEF(void, glMultMatrixx, (const GLfixed *m));
FNDEF(void, glNormalPointer, (GLenum type, GLsizei stride, const GLvoid *pointer));
FNDEF(void, glPopMatrix, (void));
FNDEF(void, glPushMatrix, (void));
FNDEF(void, glRotatex, (GLfixed angle, GLfixed x, GLfixed y, GLfixed z));
FNDEF(void, glScalex, (GLfixed x, GLfixed y, GLfixed z));
FNDEF(void, glShadeModel, (GLenum mode));
FNDEF(void, glTranslatex, (GLfixed x, GLfixed y, GLfixed z));
FNDEF(void, glVertexPointer, (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer));
FNDEF(void, glViewport, (GLint x, GLint y, GLsizei width, GLsizei height));
//add by martin
FNDEF(const GLubyte*, glGetString, (EGLint name));
FNDEF(void, glHint, (GLenum target, GLenum mode));
FNDEF(void, glDepthFunc, (GLenum func));
FNDEF(void, glClearColor, (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha));
FNDEF(void, glFrustumf, (GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near_val, GLfloat far_val));
FNDEF(void, glDrawTexiOES, (GLint sx, GLint sy, GLint sz, GLint sw, GLint sh));
FNDEF(void, glTexParameteriv, (GLenum target, GLenum pname, const GLint* param));
FNDEF(void, glColor4f, (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha));
FNDEF(void, glTexEnvf, (GLenum target, GLenum pname, GLfloat param));
FNDEF(void, glTexParameterf, (GLenum target, GLenum pname, GLfloat param));
FNDEF(void, glTexParameterx, (GLenum target, GLenum pname, GLfixed param));
FNDEF(void, glBindTexture, (GLenum target, GLuint texture));
FNDEF(void, glDrawElements, (GLenum mode, GLsizei count, GLenum type, const GLvoid* indices));
FNDEF(void, glTexCoordPointer, (GLint size, GLenum type, GLsizei stride, const GLvoid* ptr));
FNDEF(void, glScalef, (GLfloat x, GLfloat y, GLfloat z));
FNDEF(void, glRotatef, (GLfloat angle, GLfloat x, GLfloat y, GLfloat z));
FNDEF(void, glTranslatef, (GLfloat x, GLfloat y, GLfloat z));
FNDEF(void, glTexImage2D, (GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels));
FNDEF(void, glGenTextures, (GLsizei n, GLuint* textures));
FNDEF(void, glMultMatrixf, (const GLfloat* m));
FNDEF(void, glLightfv, (GLenum light, GLenum pname, const GLfloat* params));
FNDEF(void, glFogfv, (GLenum pname, const GLfloat* params));
FNDEF(void, glFogf, (GLenum pname, GLfloat param));
FNDEF(void, glDepthMask, (GLboolean flag));
FNDEF(void, glClearDepthf, (GLclampf depth));

#undef FN
#define FNPTR(name) funcPtr_##name

#ifndef IMPORTGL_NO_FNPTR_DEFS
// Redirect egl* and gl* function calls to funcPtr_egl* and funcPtr_gl*.

#define eglChooseConfig         FNPTR(eglChooseConfig)
#define eglCreateContext        FNPTR(eglCreateContext)
#define eglCreateWindowSurface  FNPTR(eglCreateWindowSurface)
#define eglDestroyContext       FNPTR(eglDestroyContext)
#define eglDestroySurface       FNPTR(eglDestroySurface)
#define eglGetConfigAttrib      FNPTR(eglGetConfigAttrib)
#define eglGetConfigs           FNPTR(eglGetConfigs)
#define eglGetDisplay           FNPTR(eglGetDisplay)
#define eglGetError             FNPTR(eglGetError)
#define eglInitialize           FNPTR(eglInitialize)
#define eglMakeCurrent          FNPTR(eglMakeCurrent)
#define eglSwapBuffers          FNPTR(eglSwapBuffers)
#define eglTerminate            FNPTR(eglTerminate)
//add by martin
#define eglQueryString          FNPTR(eglQueryString)
#ifdef ANDROID
#define eglQuerySurface          FNPTR(eglQuerySurface)
#endif

#define glBlendFunc             FNPTR(glBlendFunc)
#define glClear                 FNPTR(glClear)
#define glClearColorx           FNPTR(glClearColorx)
#define glColor4x               FNPTR(glColor4x)
#define glColorPointer          FNPTR(glColorPointer)
#define glDisable               FNPTR(glDisable)
#define glDisableClientState    FNPTR(glDisableClientState)
#define glDrawArrays            FNPTR(glDrawArrays)
#define glEnable                FNPTR(glEnable)
#define glEnableClientState     FNPTR(glEnableClientState)
#define glFrustumx              FNPTR(glFrustumx)
#define glGetError              FNPTR(glGetError)
#define glLightxv               FNPTR(glLightxv)
#define glLoadIdentity          FNPTR(glLoadIdentity)
#define glMaterialx             FNPTR(glMaterialx)
#define glMaterialxv            FNPTR(glMaterialxv)
#define glMatrixMode            FNPTR(glMatrixMode)
#define glMultMatrixx           FNPTR(glMultMatrixx)
#define glNormalPointer         FNPTR(glNormalPointer)
#define glPopMatrix             FNPTR(glPopMatrix)
#define glPushMatrix            FNPTR(glPushMatrix)
#define glRotatex               FNPTR(glRotatex)
#define glScalex                FNPTR(glScalex)
#define glShadeModel            FNPTR(glShadeModel)
#define glTranslatex            FNPTR(glTranslatex)
#define glVertexPointer         FNPTR(glVertexPointer)
#define glViewport              FNPTR(glViewport)
//add by martin
#define glGetString				FNPTR(glGetString)
#define glHint					FNPTR(glHint)
#define glDepthFunc				FNPTR(glDepthFunc)
#define glClearColor			FNPTR(glClearColor)
#define glFrustumf				FNPTR(glFrustumf)
#define glDrawTexiOES			FNPTR(glDrawTexiOES)
#define glTexParameteriv		FNPTR(glTexParameteriv)
#define glColor4f				FNPTR(glColor4f)
#define glTexEnvf				FNPTR(glTexEnvf)
#define glTexParameterf			FNPTR(glTexParameterf)
#define glTexParameterx			FNPTR(glTexParameterx)
#define glBindTexture			FNPTR(glBindTexture)
#define glDrawElements			FNPTR(glDrawElements)
#define glTexCoordPointer		FNPTR(glTexCoordPointer)
#define glScalef				FNPTR(glScalef)
#define glRotatef				FNPTR(glRotatef)
#define glTranslatef			FNPTR(glTranslatef)
#define glTexImage2D			FNPTR(glTexImage2D)
#define glGenTextures			FNPTR(glGenTextures)
#define glMultMatrixf			FNPTR(glMultMatrixf)
#define glLightfv				FNPTR(glLightfv)
#define glFogfv					FNPTR(glFogfv)
#define glFogf					FNPTR(glFogf)
#define glDepthMask				FNPTR(glDepthMask)
#define glClearDepthf			FNPTR(glClearDepthf)

#endif // !IMPORTGL_NO_FNPTR_DEFS

//#else


#endif // !USE_WRAPPER_GL

#ifdef __cplusplus
}
#endif

#endif // !F3D_GL_H_
