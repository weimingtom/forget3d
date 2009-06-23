#ifndef __egl_h_
#define __egl_h_

/*
** Copyright 2002-2003 Promoters of the Khronos Group (3Dlabs, ARM Ltd.,
** ATI Technologies, Inc., Discreet, Ericsson Mobile, Imagination
** Technologies Group plc, Motorola, Inc., Nokia, Silicon Graphics, Inc.,
** SK Telecom, and Sun Microsystems).
**
** This document is protected by copyright, and contains information
** proprietary to The Khronos Group. Any copying, adaptation, distribution,
** public performance, or public display of this document without the
** express written consent of the copyright holders is strictly prohibited.
** The receipt or possession of this document does not convey any rights to
** reproduce, disclose, or distribute its contents, or to manufacture, use,
** or sell anything that it may describe, in whole or in part.
*/

/*------------------------------------------------------------------------
 *
 * Hybrid EGL API
 * ---------------
 *
 * (C) 2001-2006 Hybrid Graphics, Ltd.
 * All Rights Reserved.
 *
 * This file consists of source code based on Khronos Group APIs with additional
 * source code and modifications by Hybrid Graphics Ltd. The modified 
 * file is distributed under the terms of the accompanying End User 
 * License Agreement (EULA). By using this file you agree to be bound 
 * by the terms of the EULA 
 *
 * Use and/or disclosure outside these terms may result in 
 * irreparable harm to Hybrid Graphics and legal action against 
 * the party in breach.
 *
 * The added and modified source code is distributed on an "as is" 
 * basis, with all express and implied warranties and conditions 
 * disclaimed, including, without limitation, any implied warranties 
 * and conditions of merchantability, satisfactory quality, fitness 
 * for a particular purpose, and non-infringement.
 *
 * The most up-to-date version of the EGL specification
 * can be found from http://www.khronos.org/opengles/spec.html
 *
 *
 *//**
 * \file
 * \brief   Gerbera EGL interface header.
 *//*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------------
 * Check EGL's version. Must be either 100 (EGL 1.0), 110 (EGL 1.1),
 * or 120 (EGL 1.2).
 *
 * \note DON'T TOUCH UNLESS YOU'RE COMPILING THE SOURCE RELEASE
 * AND YOU KNOW WHAT YOU ARE DOING!!!
 *-----------------------------------------------------------------------*/

/*-------------------------------------------------------------------*//*!
 * \defgroup	api EGL public API
 *
 * \brief		This module describes the EGL functionality exposed
 *				to the end user.
 *				
 *
 *//*-------------------------------------------------------------------*/


#ifdef __cplusplus
extern "C" {
#endif

#if !defined(HYBRID_EGL_VERSION)
#	define HYBRID_EGL_VERSION 120
#elif (HYBRID_EGL_VERSION != 100 && HYBRID_EGL_VERSION != 110 && HYBRID_EGL_VERSION != 120)
#	error Invalid HYBRID_EGL_VERSION! Must be either 100, 110, or 120.
#endif

/*-------------------------------------------------------------------------
 * Definition of EGL_APICALL
 *-----------------------------------------------------------------------*/

#if defined(EGL_STATIC_LIBRARY)
#	define EGL_APICALL
#else
#	if defined(_WIN32) || defined(__VC32__)	|| defined(__MWERKS__) || defined(__CW32__)			/* Win32 */
#		if defined (EGL_DLL_EXPORTS)
#			define EGL_APICALL __declspec(dllexport)
#		else
#			define EGL_APICALL __declspec(dllimport)
#		endif
#	elif defined (__ARMCC_VERSION)						/* ADS or RVCT */
#		if (__ARMCC_VERSION >= 220000)					/* RVCT */
#			if defined (EGL_DLL_EXPORTS)
#				define EGL_APICALL __declspec(dllexport)
#			else
#				define EGL_APICALL __declspec(dllimport)
#			endif
#		else											/* ADS */
#			define EGL_APICALL
#		endif
#	elif defined (__SYMBIAN32__) && defined (__GCC32__)	/* Symbian GCC */
#		define EGL_APICALL __declspec(dllexport)
#	elif defined (__SYMBIAN32__) && defined (__ARMCC_2__)	/* Symbian RVCT MakMake fix */
#		define EGL_APICALL
#	elif defined (__GNUC__)								/* GCC dependencies (kludge) */
#		define EGL_APICALL
#	elif defined(__APPLE__)
#		define EGL_APICALL extern
#	endif /* defined(_WIN32) ||... */
#endif

#ifndef EGL_APICALL
#	error EGL_APICALL must be defined
#endif


/*-------------------------------------------------------------------*//*!
 * \ingroup		api
 * \defgroup	apitypes	EGL API types
 *
 * \brief		Types used in the EGL public API
 *//*-------------------------------------------------------------------*/

/*------------------------------------------------------------------------
 * Platform-independent types.
 *----------------------------------------------------------------------*/

/*! \addtogroup apitypes */
/*@{*/

/*-------------------------------------------------------------------*//*!
 * \brief	Integer type definition
 * \note	This type has to be able to hold a whole native handle pointer
 *//*-------------------------------------------------------------------*/
typedef int             EGLint;			/*!< Integer type definition		*/
#if (HYBRID_EGL_VERSION >= 120)	
typedef int             EGLenum;		/*!< Enumeration type definition	*/
#endif
typedef unsigned int    EGLBoolean;		/*!< Boolean type definition		*/
typedef void*           EGLConfig;		/*!< Config handle type definition	*/
typedef void*           EGLContext;		/*!< Context handle type definition */
typedef void*           EGLDisplay;		/*!< Display handle type definition */
typedef void*           EGLSurface;		/*!< Surface handle type definition */
#if (HYBRID_EGL_VERSION >= 120)
typedef void*           EGLClientBuffer;	/*!< Client API buffer handle type definition */
#endif

typedef EGLint          NativeDisplayType;	/*!< Native display handle type definition					*/
typedef void*           NativeWindowType;   /*!< Native window handle type definition, HWND on win32	*/ 
typedef void*           NativePixmapType;   /*!< Native pixmap handle type definition, HBITMAP on win32 */

/*@}*/


/*-------------------------------------------------------------------*//*!
 * \ingroup		api
 * \defgroup	apihandles	EGL API handle token values
 *
 * \brief		Values for common default tokens
 *//*-------------------------------------------------------------------*/

/*! \addtogroup apihandles */
/*@{*/

#define EGL_DEFAULT_DISPLAY ((NativeDisplayType)0)	/*!< Default native display handle	*/
#define EGL_NO_CONTEXT      ((EGLContext)0)			/*!< Null EGL context handle		*/
#define EGL_NO_DISPLAY      ((EGLDisplay)0)			/*!< Null EGL display handle		*/
#define EGL_NO_SURFACE      ((EGLSurface)0)			/*!< Null EGL surface handle		*/



/*------------------------------------------------------------------------
 * EGL Enumerants.
 *----------------------------------------------------------------------*/

/* Versioning and extensions */
#define EGL_VERSION_1_0                1		/*!< EGL version 1.0 token	*/
#if (HYBRID_EGL_VERSION >= 110)
#	define EGL_VERSION_1_1             1		/*!< EGL version 1.1 token	*/
#endif
#if (HYBRID_EGL_VERSION >= 120)
#	define EGL_VERSION_1_2			   1		/*!< EGL version 1.2 token	*/
#endif

/* Boolean */
#define EGL_FALSE                      0		/*!< EGL boolean variable value FALSE */
#define EGL_TRUE                       1		/*!< EGL boolean variable value TRUE */

/*@}*/

/*-------------------------------------------------------------------*//*!
 * \ingroup		api
 * \defgroup	apierrors	EGL API error values
 *
 * \brief		Error value tokens used in EGL api
 *//*-------------------------------------------------------------------*/
/*! \addtogroup apierrors */
/*@{*/

/* Errors */
#define EGL_SUCCESS                    0x3000	/*!< See EGL specification */
#define EGL_NOT_INITIALIZED            0x3001	/*!< See EGL specification */
#define EGL_BAD_ACCESS                 0x3002	/*!< See EGL specification */
#define EGL_BAD_ALLOC                  0x3003	/*!< See EGL specification */
#define EGL_BAD_ATTRIBUTE              0x3004	/*!< See EGL specification */
#define EGL_BAD_CONFIG                 0x3005	/*!< See EGL specification */
#define EGL_BAD_CONTEXT                0x3006	/*!< See EGL specification */
#define EGL_BAD_CURRENT_SURFACE        0x3007	/*!< See EGL specification */
#define EGL_BAD_DISPLAY                0x3008	/*!< See EGL specification */
#define EGL_BAD_MATCH                  0x3009	/*!< See EGL specification */
#define EGL_BAD_NATIVE_PIXMAP          0x300A	/*!< See EGL specification */
#define EGL_BAD_NATIVE_WINDOW          0x300B	/*!< See EGL specification */
#define EGL_BAD_PARAMETER              0x300C	/*!< See EGL specification */
#define EGL_BAD_SURFACE                0x300D	/*!< See EGL specification */
#define EGL_CONTEXT_LOST               0x300E	/*!< See EGL specification */

/*@}*/

/*-------------------------------------------------------------------*//*!
 * \ingroup		api
 * \defgroup	apiconfigattrs	EGL API config attributes
 *
 * \brief		Configuration attribute tokens used in EGL api
 *//*-------------------------------------------------------------------*/
/*! \addtogroup apiconfigattrs */
/*@{*/

/* Config attributes */
#define EGL_BUFFER_SIZE                0x3020	/*!< See EGL specification */
#define EGL_ALPHA_SIZE                 0x3021	/*!< See EGL specification */
#define EGL_BLUE_SIZE                  0x3022	/*!< See EGL specification */
#define EGL_GREEN_SIZE                 0x3023	/*!< See EGL specification */
#define EGL_RED_SIZE                   0x3024	/*!< See EGL specification */
#define EGL_DEPTH_SIZE                 0x3025	/*!< See EGL specification */
#define EGL_STENCIL_SIZE               0x3026	/*!< See EGL specification */
#define EGL_CONFIG_CAVEAT              0x3027	/*!< See EGL specification */
#define EGL_CONFIG_ID                  0x3028	/*!< See EGL specification */
#define EGL_LEVEL                      0x3029	/*!< See EGL specification */
#define EGL_MAX_PBUFFER_HEIGHT         0x302A	/*!< See EGL specification */
#define EGL_MAX_PBUFFER_PIXELS         0x302B	/*!< See EGL specification */
#define EGL_MAX_PBUFFER_WIDTH          0x302C	/*!< See EGL specification */
#define EGL_NATIVE_RENDERABLE          0x302D	/*!< See EGL specification */
#define EGL_NATIVE_VISUAL_ID           0x302E	/*!< See EGL specification */
#define EGL_NATIVE_VISUAL_TYPE         0x302F	/*!< See EGL specification */
#define EGL_PRESERVED_RESOURCES        0x3030	/*!< See EGL specification */
#define EGL_SAMPLES                    0x3031	/*!< See EGL specification */
#define EGL_SAMPLE_BUFFERS             0x3032	/*!< See EGL specification */
#define EGL_SURFACE_TYPE               0x3033	/*!< See EGL specification */
#define EGL_TRANSPARENT_TYPE           0x3034	/*!< See EGL specification */
#define EGL_TRANSPARENT_BLUE_VALUE     0x3035	/*!< See EGL specification */
#define EGL_TRANSPARENT_GREEN_VALUE    0x3036	/*!< See EGL specification */
#define EGL_TRANSPARENT_RED_VALUE      0x3037	/*!< See EGL specification */
#if (HYBRID_EGL_VERSION >= 110)
#define EGL_BIND_TO_TEXTURE_RGB        0x3039	/*!< See EGL specification */
#define EGL_BIND_TO_TEXTURE_RGBA       0x303A	/*!< See EGL specification */
#define EGL_MIN_SWAP_INTERVAL          0x303B	/*!< See EGL specification */
#define EGL_MAX_SWAP_INTERVAL          0x303C	/*!< See EGL specification */
#endif/* HYBRID_EGL_VERSION >= 110 */
#if (HYBRID_EGL_VERSION >= 120)
#define EGL_LUMINANCE_SIZE				0x303D	/*!< See EGL specification */
#define EGL_ALPHA_MASK_SIZE				0x303E	/*!< See EGL specification */
#define EGL_COLOR_BUFFER_TYPE			0x303F	/*!< See EGL specification */
#define EGL_RENDERABLE_TYPE				0x3040	/*!< See EGL specification */
#define EGL_MATCH_NATIVE_PIXMAP			0x3041	/*!< See EGL specification \todo Right token values */


/*@}*/


/*-------------------------------------------------------------------*//*!
 * \ingroup		api
 * \defgroup	apimisctokens	EGL API miscellaneous tokens
 *
 * \brief		Other tokens and token values used in EGL api
 *//*-------------------------------------------------------------------*/

/*! \addtogroup apimisctokens */
/*@{*/

/*! Unknown display resolution/aspect ratio */
#define EGL_UNKNOWN			((EGLint)-1)

/*@}*/


#define EGL_RENDER_BUFFER				0x3086	/*!< See EGL specification */
#define EGL_COLORSPACE					0x3087	/*!< See EGL specification */
#define EGL_ALPHA_FORMAT				0x3088	/*!< See EGL specification */
#define EGL_COLORSPACE_sRGB				0x3089	/*!< See EGL specification */
#define EGL_COLORSPACE_LINEAR			0x308A	/*!< See EGL specification */
#define EGL_ALPHA_FORMAT_NONPRE			0x308B	/*!< See EGL specification */
#define EGL_ALPHA_FORMAT_PRE			0x308C	/*!< See EGL specification */
#define EGL_CLIENT_APIS					0x308D	/*!< See EGL specification */
#define EGL_RGB_BUFFER					0x308E	/*!< See EGL specification */
#define EGL_LUMINANCE_BUFFER			0x308F	/*!< See EGL specification */
#define EGL_HORIZONTAL_RESOLUTION		0x3090	/*!< See EGL specification */
#define EGL_VERTICAL_RESOLUTION			0x3091	/*!< See EGL specification */
#define EGL_PIXEL_ASPECT_RATIO			0x3092	/*!< See EGL specification */
#define EGL_SWAP_BEHAVIOR				0x3093	/*!< See EGL specification */
#define EGL_BUFFER_PRESERVED			0x3094	/*!< See EGL specification */
#define EGL_BUFFER_DESTROYED			0x3095	/*!< See EGL specification */
/* CreatePbufferFromClientBuffer buffer types */
#define EGL_OPENVG_IMAGE				0x3096	/*!< See EGL specification */
/* QueryContext targets */
#define EGL_CONTEXT_CLIENT_TYPE			0x3097	/*!< See EGL specification */
#define EGL_CONTEXT_CLIENT_VERSION		0x3098	/*!< See EGL specification.\todo Right token values */

#define EGL_OPENGL_ES_API				0x30A0	/*!< See EGL specification */
#define EGL_OPENVG_API					0x30A1	/*!< See EGL specification */

#endif /* HYBRID_EGL_VERSION >= 120 */


/*-------------------------------------------------------------------*//*!
 * \ingroup		api
 * \defgroup	apiconfigvals	EGL API Config attribute values
 *
 * \brief		Configuration attribute values used in EGL api
 *//*-------------------------------------------------------------------*/

/*! \addtogroup apiconfigvals */
/*@{*/


/* Config attribute and value */
#define EGL_NONE                       0x3038			/*!< See EGL specification */
/* 0x303D - 0x304F reserved for additional config attributes. */

/* Config values */
#define EGL_DONT_CARE                  ((EGLint) -1)		/*!< See EGL specification */
#define EGL_PBUFFER_BIT                0x01					/*!< See EGL specification */
#define EGL_PIXMAP_BIT                 0x02					/*!< See EGL specification */
#define EGL_WINDOW_BIT                 0x04					/*!< See EGL specification */
#define EGL_SLOW_CONFIG                0x3050				/*!< See EGL specification */
#define EGL_NON_CONFORMANT_CONFIG      0x3051				/*!< See EGL specification */
#define EGL_TRANSPARENT_RGB            0x3052				/*!< See EGL specification */

#if (HYBRID_EGL_VERSION >= 110)
#define EGL_NO_TEXTURE                 0x305C				/*!< See EGL specification */
#define EGL_TEXTURE_RGB                0x305D				/*!< See EGL specification */
#define EGL_TEXTURE_RGBA               0x305E				/*!< See EGL specification */
#define EGL_TEXTURE_2D                 0x305F				/*!< See EGL specification */
#endif /* HYBRID_EGL_VERSION >= 110 */

#if (HYBRID_EGL_VERSION >= 120)
#define EGL_OPENGL_ES_BIT				0x01				/*!< See EGL specification */
#define EGL_OPENVG_BIT					0x02				/*!< See EGL specification */
#define EGL_OPENGL_ES2_BIT				0x04				/*!< See EGL specification \todo Right token value */
#define EGL_DISPLAY_SCALING				10000				/*!< See EGL specification */
#endif /* HYBRID_EGL_VERSION >= 120 */

/*@}*/

/*! \addtogroup apimisctokens */
/*@{*/

/* String names */
#define EGL_VENDOR                     0x3053			/*!< Vendor string token */
#define EGL_VERSION                    0x3054			/*!< Version string token */
#define EGL_EXTENSIONS                 0x3055			/*!< Extensions string token */

/* Surface attributes */
#define EGL_HEIGHT                     0x3056			/*!< Surface attribute */
#define EGL_WIDTH                      0x3057			/*!< Surface attribute */
#define EGL_LARGEST_PBUFFER            0x3058			/*!< Pixel buffer surface attribute */
#if (HYBRID_EGL_VERSION >= 110)
#define EGL_TEXTURE_FORMAT             0x3080			/*!< Pixel buffer surface attribute */
#define EGL_TEXTURE_TARGET             0x3081			/*!< Pixel buffer surface attribute */
#define EGL_MIPMAP_TEXTURE             0x3082			/*!< Pixel buffer surface attribute */
#define EGL_MIPMAP_LEVEL               0x3083			/*!< Pixel buffer surface attribute */

/* BindTexImage/ReleaseTexImage buffer target */
#define EGL_BACK_BUFFER                0x3084			/*!< Pixel buffer surface attribute */
#define EGL_SINGLE_BUFFER			   0x3085			/*!< Pixel buffer surface attribute */
#endif /* HYBRID_EGL_VERSION >= 110 */

/* Current surfaces */
#define EGL_DRAW                       0x3059			/*!< Surface attribute */
#define EGL_READ                       0x305A			/*!< Surface attribute */

/* Engines */
#define EGL_CORE_NATIVE_ENGINE         0x305B			/*!< See EGL specification */

/*@}*/


/* 0x305C-0x3FFFF reserved for future use */

/*------------------------------------------------------------------------
 * EGL Functions.
 *----------------------------------------------------------------------*/



EGL_APICALL EGLint      eglGetError					(void);

EGL_APICALL EGLDisplay  eglGetDisplay				(NativeDisplayType displayID);
EGL_APICALL EGLBoolean  eglInitialize				(EGLDisplay dpy, EGLint* major, EGLint* minor);
EGL_APICALL EGLBoolean  eglTerminate				(EGLDisplay dpy);

EGL_APICALL const char* eglQueryString				(EGLDisplay dpy, EGLint name);

EGL_APICALL EGLBoolean  eglGetConfigs				(EGLDisplay dpy, EGLConfig* configs, EGLint config_size, EGLint* num_config);
EGL_APICALL EGLBoolean  eglChooseConfig				(EGLDisplay dpy, const EGLint* attrib_list, EGLConfig* configs, EGLint config_size, EGLint* num_config);
EGL_APICALL EGLBoolean  eglGetConfigAttrib			(EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint* value);

EGL_APICALL EGLSurface  eglCreateWindowSurface		(EGLDisplay dpy, EGLConfig config, NativeWindowType win, const EGLint* attrib_list);
EGL_APICALL EGLSurface  eglCreatePbufferSurface		(EGLDisplay dpy, EGLConfig config, const EGLint* attrib_list);
EGL_APICALL EGLSurface  eglCreatePixmapSurface		(EGLDisplay dpy, EGLConfig config, NativePixmapType pixmap, const EGLint* attrib_list);
EGL_APICALL EGLBoolean  eglDestroySurface			(EGLDisplay dpy, EGLSurface surface);
EGL_APICALL EGLBoolean  eglQuerySurface				(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint* value);

#if (HYBRID_EGL_VERSION >= 110)
/* EGL 1.1 render-to-texture APIs */
EGL_APICALL EGLBoolean eglSurfaceAttrib				(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint value);
EGL_APICALL EGLBoolean eglBindTexImage				(EGLDisplay dpy, EGLSurface surface, EGLint buffer);
EGL_APICALL EGLBoolean eglReleaseTexImage			(EGLDisplay dpy, EGLSurface surface, EGLint buffer);

/* EGL 1.1 swap control API */
EGL_APICALL EGLBoolean  eglSwapInterval				(EGLDisplay dpy, EGLint interval);
#endif /* HYBRID_EGL_VERSION >= 110 */


EGL_APICALL EGLContext  eglCreateContext			(EGLDisplay dpy, EGLConfig config, EGLContext share_list, const EGLint* attrib_list);
EGL_APICALL EGLBoolean  eglDestroyContext			(EGLDisplay dpy, EGLContext ctx);
EGL_APICALL EGLBoolean  eglMakeCurrent				(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx);

EGL_APICALL EGLContext  eglGetCurrentContext		(void);
EGL_APICALL EGLSurface  eglGetCurrentSurface		(EGLint readdraw);
EGL_APICALL EGLDisplay  eglGetCurrentDisplay		(void);
EGL_APICALL EGLBoolean  eglQueryContext				(EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint* value);

EGL_APICALL EGLBoolean  eglWaitGL					(void);
EGL_APICALL EGLBoolean  eglWaitNative				(EGLint engine);
EGL_APICALL EGLBoolean  eglSwapBuffers				(EGLDisplay dpy, EGLSurface surface);
EGL_APICALL EGLBoolean  eglCopyBuffers				(EGLDisplay dpy, EGLSurface surface, NativePixmapType target);

EGL_APICALL void (*eglGetProcAddress(const char *procname)) (void);


#if (HYBRID_EGL_VERSION >= 120)
EGL_APICALL EGLSurface	eglCreatePbufferFromClientBuffer (EGLDisplay dpy, EGLenum buftype, EGLClientBuffer buffer, EGLConfig config, const EGLint *attrib_list);
EGL_APICALL EGLBoolean  eglWaitClient				(void);
EGL_APICALL	EGLBoolean	eglBindAPI					(EGLenum api);
EGL_APICALL	EGLenum		eglQueryAPI					(void);
EGL_APICALL EGLBoolean	eglReleaseThread			(void);

#endif /* HYBRID_EGL_VERSION >= 120 */



#ifdef __cplusplus
}
#endif

/*----------------------------------------------------------------------*/
#endif /* __egl_h_ */
