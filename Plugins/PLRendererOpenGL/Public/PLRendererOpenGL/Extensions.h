/*********************************************************\
 *  File: Extensions.h                                   *
 *
 *  Copyright (C) 2002-2013 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 *  and associated documentation files (the "Software"), to deal in the Software without
 *  restriction, including without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all copies or
 *  substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 *  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 *  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
\*********************************************************/


#ifndef __PLRENDEREROPENGL_EXTENSIONS_H__
#define __PLRENDEREROPENGL_EXTENSIONS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLRendererOpenGL/PLRendererOpenGL.h"
#if defined(LINUX) && !defined(APPLE)
	// Required for "PFNGLXSWAPINTERVALSGIPROC" ("GLX_SGI_swap_control"-extension)
	#include <X11/Xlib.h>
	#include <X11/extensions/xf86vmode.h>
	#include "../../../../External/Recommended/OpenGL/glx.h"

	// We need to undefine some OS defines to avoid name conflicts... :(
	#undef None
	#undef Always
	#undef Success
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Context;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Supported OpenGL graphic card extensions
*
*  @remarks
*    You must check if the extension is supported by the current hardware before
*    you use it. If the extension isn't available you should offer an alternative
*    technique aka fallback.
*
*  @see
*    - OpenGL extension registry at http://oss.sgi.com/projects/ogl-sample/registry/ for more information about
*      the different extensions
*/
class Extensions {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Renderer;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cContext
		*    Owner context
		*/
		Extensions(Context &cContext);

		/**
		*  @brief
		*    Destructor
		*/
		~Extensions();

		/**
		*  @brief
		*    Returns whether the extensions are initialized or not
		*
		*  @return
		*    'true' if the extension are initialized, else 'false'
		*/
		bool IsInitialized() const;

		///////////////////////////////////////////////////////////
		// Returns whether an extension is supported or not
		///////////////////////////////////////////////////////////
		// WGL (Windows only)
		bool IsWGL_ARB_extensions_string() const;
		bool IsWGL_EXT_swap_control() const;
		bool IsWGL_ARB_pixel_format() const;
		bool IsWGL_ARB_render_texture() const;
		bool IsWGL_ARB_make_current_read() const;
		bool IsWGL_ARB_multisample() const;			// "GLX_ARB_multisample" under Linux
		bool IsWGL_ATI_pixel_format_float() const;	// "GLX_ATI_pixel_format_float" under Linux
		bool IsWGL_NV_float_buffer() const;			// "GLX_NV_float_buffer" under Linux
		// GLX (Linux only)
		bool IsGLX_SGI_swap_control() const;
		// EXT
		bool IsGL_EXT_compiled_vertex_array() const;
		bool IsGL_EXT_draw_range_elements() const;
		bool IsGL_EXT_fog_coord() const;
		bool IsGL_EXT_secondary_color() const;
		bool IsGL_EXT_texture_compression_s3tc() const;
		bool IsGL_EXT_texture_compression_latc() const;
		bool IsGL_EXT_texture_lod_bias() const;
		bool IsGL_EXT_texture_filter_anisotropic() const;
		bool IsGL_EXT_separate_specular_color() const;
		bool IsGL_EXT_texture_edge_clamp() const;
		bool IsGL_EXT_texture_array() const;
		bool IsGL_EXT_texture_rectangle() const;
		bool IsGL_EXT_texture3D() const;
		bool IsGL_EXT_texture_cube_map() const;
		bool IsGL_EXT_stencil_wrap() const;
		bool IsGL_EXT_stencil_two_side() const;
		bool IsGL_EXT_packed_depth_stencil() const;
		bool IsGL_EXT_depth_bounds_test() const;
		bool IsGL_EXT_framebuffer_object() const;
		bool IsGL_EXT_framebuffer_multisample() const;
		bool IsGL_EXT_framebuffer_blit() const;
		bool IsGL_EXT_geometry_shader4() const;
		bool IsGL_EXT_transform_feedback() const;
		// ARB
		bool IsGL_ARB_texture_float() const;
		bool IsGL_ARB_color_buffer_float() const;
		bool IsGL_ARB_multitexture() const;
		bool IsGL_ARB_vertex_buffer_object() const;
		bool IsGL_ARB_texture_border_clamp() const;
		bool IsGL_ARB_texture_mirrored_repeat() const;
		bool IsGL_ARB_texture_cube_map() const;
		bool IsGL_ARB_texture_env_combine() const;
		bool IsGL_ARB_texture_env_dot3() const;
		bool IsGL_ARB_occlusion_query() const;
		bool IsGL_ARB_texture_compression() const;
		bool IsGL_ARB_depth_texture() const;
		bool IsGL_ARB_point_sprite() const;
		bool IsGL_ARB_point_parameters() const;
		bool IsGL_ARB_shading_language_100() const;
		bool IsGL_ARB_vertex_program() const;
		bool IsGL_ARB_fragment_program() const;
		bool IsGL_ARB_draw_buffers() const;
		bool IsGL_ARB_shader_objects() const;
		bool IsGL_ARB_vertex_shader() const;
		bool IsGL_ARB_get_program_binary() const;
		bool IsGL_ARB_texture_non_power_of_two() const;
		bool IsGL_ARB_texture_rectangle() const;
		bool IsGL_ARB_multisample() const;
		bool IsGL_ARB_uniform_buffer_object() const;
		bool IsGL_ARB_half_float_vertex() const;
		bool IsGL_ARB_tessellation_shader() const;
		bool IsGL_ARB_depth_clamp() const;
		bool IsGL_ARB_draw_instanced() const;
		// ATI
		bool IsGL_ATI_meminfo() const;
		bool IsGL_ATI_separate_stencil() const;
		bool IsGL_ATI_draw_buffers() const;
		bool IsGL_ATI_texture_compression_3dc() const;
		// AMD
		bool IsGL_AMD_vertex_shader_tessellator() const;
		// NV
		bool IsGL_NV_texture_rectangle() const;
		bool IsGL_NV_occlusion_query() const;
		bool IsGL_NVX_gpu_memory_info() const;
		bool IsGL_NV_half_float() const;
		bool IsGL_NV_texture_compression_vtc() const;
		// SGIS
		bool IsGL_SGIS_generate_mipmap() const;
		// HP
		bool IsGL_HP_occlusion_test() const;

		///////////////////////////////////////////////////////////
		// Misc
		///////////////////////////////////////////////////////////
		GLint GetGL_MAX_ELEMENTS_VERTICES_EXT() const;
		GLint GetGL_MAX_ELEMENTS_INDICES_EXT() const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Checks whether an extension is supported by the given hardware or not
		*
		*  @param[in] pszExtension
		*    Name of the extension, if a null pointer, nothing happens
		*
		*  @return
		*    'true' if the extensions is supported, else 'false'
		*/
		bool IsSupported(const char *pszExtension) const;

		/**
		*  @brief
		*    Checks whether an extension is supported by the given hardware or not
		*
		*  @param[in] pszExtension
		*    Name of the extension, if a null pointer, nothing happens
		*
		*  @return
		*    'true' if the extensions is supported, else 'false'
		*/
		bool CheckExtension(const char *pszExtension) const;

		/**
		*  @brief
		*    Resets the extensions
		*/
		void ResetExtensions();

		/**
		*  @brief
		*    Initialize the supported extensions
		*
		*  @param[in] bUseExtensions
		*    Use extensions?
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - Platform dependent implementation
		*/
		bool Init(bool bUseExtensions = true);

		/**
		*  @brief
		*    Initialize the supported universal extensions
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - Platform independent implementation
		*/
		bool InitUniversal();

		/**
		*  @brief
		*    Shows general OpenGL information
		*/
		void ShowGeneralOpenGLInformation() const;

		/**
		*  @brief
		*    Shows OpenGL shader program information
		*
		*  @param[in] nTarget
		*    Target (vertex, fragment etc.)
		*/
		void ShowOpenGLShaderProgramInformation(PLCore::uint32 nTarget) const;

		/**
		*  @brief
		*    Writes the extensions within a given extension string into the log
		*
		*  @param[in] pszExtensions
		*    Extension string, if a null pointer, nothing happens
		*/
		void WriteExtensionStringIntoLog(const char *pszExtensions) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Context  *m_pContext;				/**< Owner context, always valid! */
		bool 	  m_bInitialized;			/**< Are the extensions initialized? */
		#ifdef APPLE
			void *m_pOpenGLSharedLibrary;	/**< OpenGL shared library, can be a null pointer */
		#endif

		// Supported extensions
		// WGL (Windows only)
		bool m_bWGL_ARB_extensions_string;
		bool m_bWGL_EXT_swap_control;
		bool m_bWGL_ARB_pixel_format;
		bool m_bWGL_ARB_render_texture;
		bool m_bWGL_ARB_make_current_read;
		bool m_bWGL_ARB_multisample;
		bool m_bWGL_ATI_pixel_format_float;
		bool m_bWGL_NV_float_buffer;
		// GLX (Linux only)
		bool m_bGLX_SGI_swap_control;
		// EXT
		bool m_bGL_EXT_compiled_vertex_array;
		bool m_bGL_EXT_draw_range_elements;
		bool m_bGL_EXT_fog_coord;
		bool m_bGL_EXT_secondary_color;
		bool m_bGL_EXT_texture_compression_s3tc;
		bool m_bGL_EXT_texture_compression_latc;
		bool m_bGL_EXT_texture_lod_bias;
		bool m_bGL_EXT_texture_filter_anisotropic;
		bool m_bGL_EXT_separate_specular_color;
		bool m_bGL_EXT_texture_edge_clamp;
		bool m_bGL_EXT_texture_array;
		bool m_bGL_EXT_texture_rectangle;
		bool m_bGL_EXT_texture3D;
		bool m_bGL_EXT_texture_cube_map;
		bool m_bGL_EXT_stencil_wrap;
		bool m_bGL_EXT_stencil_two_side;
		bool m_bGL_EXT_packed_depth_stencil;
		bool m_bGL_EXT_depth_bounds_test;
		bool m_bGL_EXT_framebuffer_object;
		bool m_bGL_EXT_framebuffer_multisample;
		bool m_bGL_EXT_framebuffer_blit;
		bool m_bGL_EXT_geometry_shader4;
		bool m_bGL_EXT_transform_feedback;
		// ARB
		bool m_bGL_ARB_texture_float;
		bool m_bGL_ARB_color_buffer_float;
		bool m_bGL_ARB_multitexture;
		bool m_bGL_ARB_vertex_buffer_object;
		bool m_bGL_ARB_texture_border_clamp;
		bool m_bGL_ARB_texture_mirrored_repeat;
		bool m_bGL_ARB_texture_cube_map;
		bool m_bGL_ARB_texture_env_combine;
		bool m_bGL_ARB_texture_env_dot3;
		bool m_bGL_ARB_occlusion_query;
		bool m_bGL_ARB_texture_compression;
		bool m_bGL_ARB_depth_texture;
		bool m_bGL_ARB_point_sprite;
		bool m_bGL_ARB_point_parameters;
		bool m_bGL_ARB_shading_language_100;
		bool m_bGL_ARB_vertex_program;
		bool m_bGL_ARB_fragment_program;
		bool m_bGL_ARB_draw_buffers;
		bool m_bGL_ARB_shader_objects;
		bool m_bGL_ARB_vertex_shader;
		bool m_bGL_ARB_get_program_binary;
		bool m_bGL_ARB_texture_non_power_of_two;
		bool m_bGL_ARB_texture_rectangle;
		bool m_bGL_ARB_multisample;
		bool m_bGL_ARB_uniform_buffer_object;
		bool m_bGL_ARB_half_float_vertex;
		bool m_bGL_ARB_tessellation_shader;
		bool m_bGL_ARB_depth_clamp;
		bool m_bGL_ARB_draw_instanced;
		// ATI
		bool m_bGL_ATI_meminfo;
		bool m_bGL_ATI_separate_stencil;
		bool m_bGL_ATI_draw_buffers;
		bool m_bGL_ATI_texture_compression_3dc;
		// AMD
		bool m_bGL_AMD_vertex_shader_tessellator;
		// NV
		bool m_bGL_NV_texture_rectangle;
		bool m_bGL_NV_occlusion_query;
		bool m_bGL_NVX_gpu_memory_info;
		bool m_bGL_NV_half_float;
		bool m_bGL_NV_texture_compression_vtc;
		// SGIS
		bool m_bGL_SGIS_generate_mipmap;
		// HP
		bool m_bGL_HP_occlusion_test;

		// Misc
		GLint m_nGL_MAX_ELEMENTS_VERTICES_EXT;
		GLint m_nGL_MAX_ELEMENTS_INDICES_EXT;


};


//[-------------------------------------------------------]
//[ Define helper macro                                   ]
//[-------------------------------------------------------]
#ifdef EXTENSIONS_DEFINE
	#define FNDEF_EX(funcName, funcTypedef) funcTypedef funcName = nullptr
#else
	#define FNDEF_EX(funcName, funcTypedef) extern funcTypedef funcName
#endif


//[-------------------------------------------------------]
//[ WGL (Windows only) definitions                        ]
//[-------------------------------------------------------]
#ifdef WIN32
	// WGL_ARB_extensions_string
	FNDEF_EX(wglGetExtensionsStringARB,	PFNWGLGETEXTENSIONSSTRINGARBPROC);

	// WGL_EXT_swap_control
	FNDEF_EX(wglSwapIntervalEXT,	PFNWGLSWAPINTERVALEXTPROC);
	FNDEF_EX(wglGetSwapIntervalEXT,	PFNWGLGETSWAPINTERVALEXTPROC);

	// WGL_ARB_pixel_format
	FNDEF_EX(wglGetPixelFormatAttribivARB,	PFNWGLGETPIXELFORMATATTRIBIVARBPROC);
	FNDEF_EX(wglGetPixelFormatAttribfvARB,	PFNWGLGETPIXELFORMATATTRIBFVARBPROC);
	FNDEF_EX(wglChoosePixelFormatARB,		PFNWGLCHOOSEPIXELFORMATARBPROC);

	// WGL_ARB_render_texture
	FNDEF_EX(wglBindTexImageARB,	PFNWGLBINDTEXIMAGEARBPROC);
	FNDEF_EX(wglReleaseTexImageARB,	PFNWGLRELEASETEXIMAGEARBPROC);

	// WGL_ARB_make_current_read
	FNDEF_EX(wglMakeContextCurrentARB,	PFNWGLMAKECONTEXTCURRENTARBPROC);
	FNDEF_EX(wglGetCurrentReadDCARB,	PFNWGLGETCURRENTREADDCARBPROC);
#endif


//[-------------------------------------------------------]
//[ GLX (Linux only)                                      ]
//[-------------------------------------------------------]
#if defined(LINUX) && !defined(APPLE)
	// GLX_SGI_swap_control
	FNDEF_EX(glXSwapIntervalSGI,	PFNGLXSWAPINTERVALSGIPROC);
#endif


//[-------------------------------------------------------]
//[ EXT                                                   ]
//[-------------------------------------------------------]
// GL_EXT_compiled_vertex_array
FNDEF_EX(glLockArraysEXT,	PFNGLLOCKARRAYSEXTPROC);
FNDEF_EX(glUnlockArraysEXT,	PFNGLUNLOCKARRAYSEXTPROC);

// GL_EXT_draw_range_elements
FNDEF_EX(glDrawRangeElementsEXT,	PFNGLDRAWRANGEELEMENTSEXTPROC);

// GL_EXT_fog_coord
FNDEF_EX(glFogCoordfEXT,		PFNGLFOGCOORDFEXTPROC);
FNDEF_EX(glFogCoordfvEXT,		PFNGLFOGCOORDFVEXTPROC);
FNDEF_EX(glFogCoorddEXT,		PFNGLFOGCOORDDEXTPROC);
FNDEF_EX(glFogCoorddvEXT,		PFNGLFOGCOORDDVEXTPROC);
FNDEF_EX(glFogCoordPointerEXT,	PFNGLFOGCOORDPOINTEREXTPROC);

// GL_EXT_secondary_color
FNDEF_EX(glSecondaryColor3bEXT,			PFNGLSECONDARYCOLOR3BEXTPROC);
FNDEF_EX(glSecondaryColor3bvEXT,		PFNGLSECONDARYCOLOR3BVEXTPROC);
FNDEF_EX(glSecondaryColor3dEXT,			PFNGLSECONDARYCOLOR3DEXTPROC);
FNDEF_EX(glSecondaryColor3dvEXT,		PFNGLSECONDARYCOLOR3DVEXTPROC);
FNDEF_EX(glSecondaryColor3fEXT,			PFNGLSECONDARYCOLOR3FEXTPROC);
FNDEF_EX(glSecondaryColor3fvEXT,		PFNGLSECONDARYCOLOR3FVEXTPROC);
FNDEF_EX(glSecondaryColor3iEXT,			PFNGLSECONDARYCOLOR3IEXTPROC);
FNDEF_EX(glSecondaryColor3ivEXT,		PFNGLSECONDARYCOLOR3IVEXTPROC);
FNDEF_EX(glSecondaryColor3sEXT,			PFNGLSECONDARYCOLOR3SEXTPROC);
FNDEF_EX(glSecondaryColor3svEXT,		PFNGLSECONDARYCOLOR3SVEXTPROC);
FNDEF_EX(glSecondaryColor3ubEXT,		PFNGLSECONDARYCOLOR3UBEXTPROC);
FNDEF_EX(glSecondaryColor3ubvEXT,		PFNGLSECONDARYCOLOR3UBVEXTPROC);
FNDEF_EX(glSecondaryColor3uiEXT,		PFNGLSECONDARYCOLOR3UIEXTPROC);
FNDEF_EX(glSecondaryColor3uivEXT,		PFNGLSECONDARYCOLOR3UIVEXTPROC);
FNDEF_EX(glSecondaryColor3usEXT,		PFNGLSECONDARYCOLOR3USEXTPROC);
FNDEF_EX(glSecondaryColor3usvEXT,		PFNGLSECONDARYCOLOR3USVEXTPROC);
FNDEF_EX(glSecondaryColorPointerEXT,	PFNGLSECONDARYCOLORPOINTEREXTPROC);

// GL_EXT_texture3D
FNDEF_EX(glTexImage3DEXT,		PFNGLTEXIMAGE3DEXTPROC);
FNDEF_EX(glTexSubImage3DEXT,	PFNGLTEXSUBIMAGE3DEXTPROC);

// GL_EXT_stencil_two_side
FNDEF_EX(glActiveStencilFaceEXT,	PFNGLACTIVESTENCILFACEEXTPROC);

// GL_EXT_depth_bounds_test
FNDEF_EX(glDepthBoundsEXT,	PFNGLDEPTHBOUNDSEXTPROC);

// GL_EXT_framebuffer_object
FNDEF_EX(glIsRenderbufferEXT,						PFNGLISRENDERBUFFEREXTPROC);
FNDEF_EX(glBindRenderbufferEXT,						PFNGLBINDRENDERBUFFEREXTPROC);
FNDEF_EX(glDeleteRenderbuffersEXT,					PFNGLDELETERENDERBUFFERSEXTPROC);
FNDEF_EX(glGenRenderbuffersEXT,						PFNGLGENRENDERBUFFERSEXTPROC);
FNDEF_EX(glRenderbufferStorageEXT,					PFNGLRENDERBUFFERSTORAGEEXTPROC);
FNDEF_EX(glGetRenderbufferParameterivEXT,			PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC);
FNDEF_EX(glIsFramebufferEXT,						PFNGLISFRAMEBUFFEREXTPROC);
FNDEF_EX(glBindFramebufferEXT,						PFNGLBINDFRAMEBUFFEREXTPROC);
FNDEF_EX(glDeleteFramebuffersEXT,					PFNGLDELETEFRAMEBUFFERSEXTPROC);
FNDEF_EX(glGenFramebuffersEXT,						PFNGLGENFRAMEBUFFERSEXTPROC);
FNDEF_EX(glCheckFramebufferStatusEXT,				PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC);
FNDEF_EX(glFramebufferTexture1DEXT,					PFNGLFRAMEBUFFERTEXTURE1DEXTPROC);
FNDEF_EX(glFramebufferTexture2DEXT,					PFNGLFRAMEBUFFERTEXTURE2DEXTPROC);
FNDEF_EX(glFramebufferTexture3DEXT,					PFNGLFRAMEBUFFERTEXTURE3DEXTPROC);
FNDEF_EX(glFramebufferRenderbufferEXT,				PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC);
FNDEF_EX(glGetFramebufferAttachmentParameterivEXT,	PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC);
FNDEF_EX(glGenerateMipmapEXT,						PFNGLGENERATEMIPMAPEXTPROC);

// GL_EXT_framebuffer_multisample
FNDEF_EX(glRenderbufferStorageMultisampleEXT,	PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC);

// GL_EXT_framebuffer_blit
FNDEF_EX(glBlitFramebufferEXT,	PFNGLBLITFRAMEBUFFEREXTPROC);

// GL_EXT_geometry_shader4
FNDEF_EX(glProgramParameteriEXT,	PFNGLPROGRAMPARAMETERIEXTPROC);

// GL_EXT_transform_feedback
FNDEF_EX(glBeginTransformFeedbackEXT,		PFNGLBEGINTRANSFORMFEEDBACKEXTPROC);
FNDEF_EX(glEndTransformFeedbackEXT,			PFNGLENDTRANSFORMFEEDBACKEXTPROC);
FNDEF_EX(glBindBufferRangeEXT,				PFNGLBINDBUFFERRANGEEXTPROC);
FNDEF_EX(glBindBufferOffsetEXT,				PFNGLBINDBUFFEROFFSETEXTPROC);
FNDEF_EX(glBindBufferBaseEXT,				PFNGLBINDBUFFERBASEEXTPROC);
FNDEF_EX(glTransformFeedbackVaryingsEXT,	PFNGLTRANSFORMFEEDBACKVARYINGSEXTPROC);
FNDEF_EX(glGetTransformFeedbackVaryingEXT,	PFNGLGETTRANSFORMFEEDBACKVARYINGEXTPROC);


//[-------------------------------------------------------]
//[ ARB                                                   ]
//[-------------------------------------------------------]
// GL_ARB_color_buffer_float
FNDEF_EX(glClampColorARB,	PFNGLCLAMPCOLORARBPROC);

// GL_ARB_multitexture
FNDEF_EX(glActiveTextureARB,			PFNGLACTIVETEXTUREARBPROC);
FNDEF_EX(glClientActiveTextureARB,		PFNGLCLIENTACTIVETEXTUREARBPROC);
FNDEF_EX(glMultiTexCoord1dARB,			PFNGLMULTITEXCOORD1DARBPROC);
FNDEF_EX(glMultiTexCoord1dvARB,			PFNGLMULTITEXCOORD1DVARBPROC);
FNDEF_EX(glMultiTexCoord1fARB,			PFNGLMULTITEXCOORD1FARBPROC);
FNDEF_EX(glMultiTexCoord1fvARB,			PFNGLMULTITEXCOORD1FVARBPROC);
FNDEF_EX(glMultiTexCoord1iARB,			PFNGLMULTITEXCOORD1IARBPROC);
FNDEF_EX(glMultiTexCoord1ivARB,			PFNGLMULTITEXCOORD1IVARBPROC);
FNDEF_EX(glMultiTexCoord1sARB,			PFNGLMULTITEXCOORD1SARBPROC);
FNDEF_EX(glMultiTexCoord1svARB,			PFNGLMULTITEXCOORD1SVARBPROC);
FNDEF_EX(glMultiTexCoord2dARB,			PFNGLMULTITEXCOORD2DARBPROC);
FNDEF_EX(glMultiTexCoord2dvARB,			PFNGLMULTITEXCOORD2DVARBPROC);
FNDEF_EX(glMultiTexCoord2fARB,			PFNGLMULTITEXCOORD2FARBPROC);
FNDEF_EX(glMultiTexCoord2fvARB,			PFNGLMULTITEXCOORD2FVARBPROC);
FNDEF_EX(glMultiTexCoord2iARB,			PFNGLMULTITEXCOORD2IARBPROC);
FNDEF_EX(glMultiTexCoord2ivARB,			PFNGLMULTITEXCOORD2IVARBPROC);
FNDEF_EX(glMultiTexCoord2sARB,			PFNGLMULTITEXCOORD2SARBPROC);
FNDEF_EX(glMultiTexCoord2svARB,			PFNGLMULTITEXCOORD2SVARBPROC);
FNDEF_EX(glMultiTexCoord3dARB,			PFNGLMULTITEXCOORD3DARBPROC);
FNDEF_EX(glMultiTexCoord3dvARB,			PFNGLMULTITEXCOORD3DVARBPROC);
FNDEF_EX(glMultiTexCoord3fARB,			PFNGLMULTITEXCOORD3FARBPROC);
FNDEF_EX(glMultiTexCoord3fvARB,			PFNGLMULTITEXCOORD3FVARBPROC);
FNDEF_EX(glMultiTexCoord3iARB,			PFNGLMULTITEXCOORD3IARBPROC);
FNDEF_EX(glMultiTexCoord3ivARB,			PFNGLMULTITEXCOORD3IVARBPROC);
FNDEF_EX(glMultiTexCoord3sARB,			PFNGLMULTITEXCOORD3SARBPROC);
FNDEF_EX(glMultiTexCoord3svARB,			PFNGLMULTITEXCOORD3SVARBPROC);
FNDEF_EX(glMultiTexCoord4dARB,			PFNGLMULTITEXCOORD4DARBPROC);
FNDEF_EX(glMultiTexCoord4dvARB,			PFNGLMULTITEXCOORD4DVARBPROC);
FNDEF_EX(glMultiTexCoord4fARB,			PFNGLMULTITEXCOORD4FARBPROC);
FNDEF_EX(glMultiTexCoord4fvARB,			PFNGLMULTITEXCOORD4FVARBPROC);
FNDEF_EX(glMultiTexCoord4iARB,			PFNGLMULTITEXCOORD4IARBPROC);
FNDEF_EX(glMultiTexCoord4ivARB,			PFNGLMULTITEXCOORD4IVARBPROC);
FNDEF_EX(glMultiTexCoord4sARB,			PFNGLMULTITEXCOORD4SARBPROC);
FNDEF_EX(glMultiTexCoord4svARB,			PFNGLMULTITEXCOORD4SVARBPROC);

// GL_ARB_vertex_buffer_object
FNDEF_EX(glBindBufferARB,			PFNGLBINDBUFFERARBPROC);
FNDEF_EX(glDeleteBuffersARB,		PFNGLDELETEBUFFERSARBPROC);
FNDEF_EX(glGenBuffersARB,			PFNGLGENBUFFERSARBPROC);
FNDEF_EX(glBufferDataARB,			PFNGLBUFFERDATAARBPROC);
FNDEF_EX(glBufferSubDataARB,		PFNGLBUFFERSUBDATAARBPROC);
FNDEF_EX(glGetBufferSubDataARB,		PFNGLGETBUFFERSUBDATAARBPROC);
FNDEF_EX(glMapBufferARB,			PFNGLMAPBUFFERARBPROC);
FNDEF_EX(glUnmapBufferARB,			PFNGLUNMAPBUFFERARBPROC);
FNDEF_EX(glGetBufferParameterivARB,	PFNGLGETBUFFERPARAMETERIVARBPROC);
FNDEF_EX(glGetBufferPointervARB,	PFNGLGETBUFFERPOINTERVARBPROC);

// GL_ARB_occlusion_query
FNDEF_EX(glGenQueriesARB,			PFNGLGENQUERIESARBPROC);
FNDEF_EX(glDeleteQueriesARB,		PFNGLDELETEQUERIESARBPROC);
FNDEF_EX(glIsQueryARB,				PFNGLISQUERYARBPROC);
FNDEF_EX(glBeginQueryARB,			PFNGLBEGINQUERYARBPROC);
FNDEF_EX(glEndQueryARB,				PFNGLENDQUERYARBPROC);
FNDEF_EX(glGetQueryivARB,			PFNGLGETQUERYIVARBPROC);
FNDEF_EX(glGetQueryObjectivARB,		PFNGLGETQUERYOBJECTIVARBPROC);
FNDEF_EX(glGetQueryObjectuivARB,	PFNGLGETQUERYOBJECTUIVARBPROC);

// GL_ARB_texture_compression
FNDEF_EX(glCompressedTexImage3DARB,		PFNGLCOMPRESSEDTEXIMAGE3DARBPROC);
FNDEF_EX(glCompressedTexImage2DARB,		PFNGLCOMPRESSEDTEXIMAGE2DARBPROC);
FNDEF_EX(glCompressedTexImage1DARB,		PFNGLCOMPRESSEDTEXIMAGE1DARBPROC);
FNDEF_EX(glCompressedTexSubImage3DARB,	PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC);
FNDEF_EX(glCompressedTexSubImage2DARB,	PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC);
FNDEF_EX(glCompressedTexSubImage1DARB,	PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC);
FNDEF_EX(glGetCompressedTexImageARB,	PFNGLGETCOMPRESSEDTEXIMAGEARBPROC);

// GL_ARB_point_parameters
FNDEF_EX(glPointParameterfARB,	PFNGLPOINTPARAMETERFARBPROC);
FNDEF_EX(glPointParameterfvARB,	PFNGLPOINTPARAMETERFVARBPROC);

// GL_ARB_vertex_program
FNDEF_EX(glVertexAttribPointerARB,		PFNGLVERTEXATTRIBPOINTERARBPROC);
FNDEF_EX(glEnableVertexAttribArrayARB,	PFNGLENABLEVERTEXATTRIBARRAYARBPROC);
FNDEF_EX(glDisableVertexAttribArrayARB,	PFNGLDISABLEVERTEXATTRIBARRAYARBPROC);
FNDEF_EX(glGetProgramivARB,				PFNGLGETPROGRAMIVARBPROC);

// GL_ARB_draw_buffers
FNDEF_EX(glDrawBuffersARB,	PFNGLDRAWBUFFERSARBPROC);

// GL_ARB_shader_objects
FNDEF_EX(glDeleteObjectARB,			PFNGLDELETEOBJECTARBPROC);
FNDEF_EX(glGetHandleARB,			PFNGLGETHANDLEARBPROC);
FNDEF_EX(glDetachObjectARB,			PFNGLDETACHOBJECTARBPROC);
FNDEF_EX(glCreateShaderObjectARB,	PFNGLCREATESHADEROBJECTARBPROC);
FNDEF_EX(glShaderSourceARB,			PFNGLSHADERSOURCEARBPROC);
FNDEF_EX(glCompileShaderARB,		PFNGLCOMPILESHADERARBPROC);
FNDEF_EX(glCreateProgramObjectARB,	PFNGLCREATEPROGRAMOBJECTARBPROC);
FNDEF_EX(glAttachObjectARB,			PFNGLATTACHOBJECTARBPROC);
FNDEF_EX(glLinkProgramARB,			PFNGLLINKPROGRAMARBPROC);
FNDEF_EX(glUseProgramObjectARB,		PFNGLUSEPROGRAMOBJECTARBPROC);
FNDEF_EX(glValidateProgramARB,		PFNGLVALIDATEPROGRAMARBPROC);
FNDEF_EX(glUniform1fARB,			PFNGLUNIFORM1FARBPROC);
FNDEF_EX(glUniform2fARB,			PFNGLUNIFORM2FARBPROC);
FNDEF_EX(glUniform3fARB,			PFNGLUNIFORM3FARBPROC);
FNDEF_EX(glUniform4fARB,			PFNGLUNIFORM4FARBPROC);
FNDEF_EX(glUniform1iARB,			PFNGLUNIFORM1IARBPROC);
FNDEF_EX(glUniform2iARB,			PFNGLUNIFORM2IARBPROC);
FNDEF_EX(glUniform3iARB,			PFNGLUNIFORM3IARBPROC);
FNDEF_EX(glUniform4iARB,			PFNGLUNIFORM4IARBPROC);
FNDEF_EX(glUniform1fvARB,			PFNGLUNIFORM1FVARBPROC);
FNDEF_EX(glUniform2fvARB,			PFNGLUNIFORM2FVARBPROC);
FNDEF_EX(glUniform3fvARB,			PFNGLUNIFORM3FVARBPROC);
FNDEF_EX(glUniform4fvARB,			PFNGLUNIFORM4FVARBPROC);
FNDEF_EX(glUniform1ivARB,			PFNGLUNIFORM1IVARBPROC);
FNDEF_EX(glUniform2ivARB,			PFNGLUNIFORM2IVARBPROC);
FNDEF_EX(glUniform3ivARB,			PFNGLUNIFORM3IVARBPROC);
FNDEF_EX(glUniform4ivARB,			PFNGLUNIFORM4IVARBPROC);
FNDEF_EX(glUniformMatrix2fvARB,		PFNGLUNIFORMMATRIX2FVARBPROC);
FNDEF_EX(glUniformMatrix3fvARB,		PFNGLUNIFORMMATRIX3FVARBPROC);
FNDEF_EX(glUniformMatrix4fvARB,		PFNGLUNIFORMMATRIX4FVARBPROC);
FNDEF_EX(glGetObjectParameterfvARB,	PFNGLGETOBJECTPARAMETERFVARBPROC);
FNDEF_EX(glGetObjectParameterivARB,	PFNGLGETOBJECTPARAMETERIVARBPROC);
FNDEF_EX(glGetInfoLogARB,			PFNGLGETINFOLOGARBPROC);
FNDEF_EX(glGetAttachedObjectsARB,	PFNGLGETATTACHEDOBJECTSARBPROC);
FNDEF_EX(glGetUniformLocationARB,	PFNGLGETUNIFORMLOCATIONARBPROC);
FNDEF_EX(glGetActiveUniformARB,		PFNGLGETACTIVEUNIFORMARBPROC);
FNDEF_EX(glGetUniformfvARB,			PFNGLGETUNIFORMFVARBPROC);
FNDEF_EX(glGetUniformivARB,			PFNGLGETUNIFORMIVARBPROC);
FNDEF_EX(glGetShaderSourceARB,		PFNGLGETSHADERSOURCEARBPROC);

// GL_ARB_vertex_shader
FNDEF_EX(glBindAttribLocationARB,	PFNGLBINDATTRIBLOCATIONARBPROC);
FNDEF_EX(glGetActiveAttribARB,		PFNGLGETACTIVEATTRIBARBPROC);
FNDEF_EX(glGetAttribLocationARB,	PFNGLGETATTRIBLOCATIONARBPROC);

// GL_ARB_get_program_binary
FNDEF_EX(glGetProgramBinary,	PFNGLGETPROGRAMBINARYPROC);
FNDEF_EX(glProgramBinary,		PFNGLPROGRAMBINARYPROC);
FNDEF_EX(glProgramParameteri,	PFNGLPROGRAMPARAMETERIPROC);

// GL_ARB_uniform_buffer_object
FNDEF_EX(glGetUniformIndices,			PFNGLGETUNIFORMINDICESPROC);
FNDEF_EX(glGetActiveUniformsiv,			PFNGLGETACTIVEUNIFORMSIVPROC);
FNDEF_EX(glGetActiveUniformName,		PFNGLGETACTIVEUNIFORMNAMEPROC);
FNDEF_EX(glGetUniformBlockIndex,		PFNGLGETUNIFORMBLOCKINDEXPROC);
FNDEF_EX(glGetActiveUniformBlockiv,		PFNGLGETACTIVEUNIFORMBLOCKIVPROC);
FNDEF_EX(glGetActiveUniformBlockName,	PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC);
FNDEF_EX(glUniformBlockBinding,			PFNGLUNIFORMBLOCKBINDINGPROC);

// GL_ARB_tessellation_shader
FNDEF_EX(glPatchParameteri,		PFNGLPATCHPARAMETERIPROC);
FNDEF_EX(glPatchParameterfv,	PFNGLPATCHPARAMETERFVPROC);

// GL_ARB_draw_instanced
FNDEF_EX(glDrawArraysInstancedARB,		PFNGLDRAWARRAYSINSTANCEDARBPROC);
FNDEF_EX(glDrawElementsInstancedARB,	PFNGLDRAWELEMENTSINSTANCEDARBPROC);


//[-------------------------------------------------------]
//[ ATI                                                   ]
//[-------------------------------------------------------]
// GL_ATI_separate_stencil
FNDEF_EX(glStencilOpSeparateATI,	PFNGLSTENCILOPSEPARATEATIPROC);
FNDEF_EX(glStencilFuncSeparateATI,	PFNGLSTENCILFUNCSEPARATEATIPROC);

// GL_ATI_draw_buffers
FNDEF_EX(glDrawBuffersATI,	PFNGLDRAWBUFFERSATIPROC);


//[-------------------------------------------------------]
//[ AMD                                                   ]
//[-------------------------------------------------------]
// GL_AMD_vertex_shader_tessellator
// -> The extension is listed as "GL_AMD_vertex_shader_tessellator" (see http://www.opengl.org/registry/specs/AMD/vertex_shader_tessellator.txt)
// -> In "http://www.opengl.org/registry/api/glext.h" it's listed as "GL_AMD_vertex_shader_tesselator" (typo?)
FNDEF_EX(glTessellationFactorAMD,	PFNGLTESSELLATIONFACTORAMDPROC);
FNDEF_EX(glTessellationModeAMD,		PFNGLTESSELLATIONMODEAMDPROC);


//[-------------------------------------------------------]
//[ NV                                                    ]
//[-------------------------------------------------------]
// GL_NV_occlusion_query
FNDEF_EX(glGenOcclusionQueriesNV,		PFNGLGENOCCLUSIONQUERIESNVPROC);
FNDEF_EX(glDeleteOcclusionQueriesNV,	PFNGLDELETEOCCLUSIONQUERIESNVPROC);
FNDEF_EX(glIsOcclusionQueryNV,			PFNGLISOCCLUSIONQUERYNVPROC);
FNDEF_EX(glBeginOcclusionQueryNV,		PFNGLBEGINOCCLUSIONQUERYNVPROC);
FNDEF_EX(glEndOcclusionQueryNV,			PFNGLENDOCCLUSIONQUERYNVPROC);
FNDEF_EX(glGetOcclusionQueryivNV,		PFNGLGETOCCLUSIONQUERYIVNVPROC);
FNDEF_EX(glGetOcclusionQueryuivNV,		PFNGLGETOCCLUSIONQUERYUIVNVPROC);


//[-------------------------------------------------------]
//[ Undefine helper macro                                 ]
//[-------------------------------------------------------]
#undef FNDEF_EX


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL


#endif // __PLRENDEREROPENGL_EXTENSIONS_H__
