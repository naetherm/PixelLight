/*********************************************************\
 *  File: Extensions.cpp                                 *
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Log/Log.h>
#define EXTENSIONS_DEFINE
#include "PLRendererOpenGL/Extensions.h"
#ifdef APPLE
	#include <dlfcn.h>	// For dlopen, dlclose, dlsym
	#include <string.h>	// For strstr
#elif LINUX
	#include <string.h>	// For strstr
	#include "PLRendererOpenGL/Linux/ContextLinux.h"
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Extensions::Extensions(Context &cContext) :
	m_pContext(&cContext),
	m_bInitialized(false)
{
	#ifdef APPLE
		// Open the OpenGL shared library
		m_pOpenGLSharedLibrary = dlopen("/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL", RTLD_LAZY);
		if (!m_pOpenGLSharedLibrary)
			PL_LOG(Error, "Failed to open the OpenGL shared library \"/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL\" in order to load in the extensions entry points")
	#endif

	// Reset extensions
	ResetExtensions();
}

/**
*  @brief
*    Destructor
*/
Extensions::~Extensions()
{
	#ifdef APPLE
		// Close the OpenGL shared library
		if (m_pOpenGLSharedLibrary)
			dlclose(m_pOpenGLSharedLibrary);
	#endif
}

/**
*  @brief
*    Returns whether the extensions are initialized or not
*/
bool Extensions::IsInitialized() const
{
	return m_bInitialized;
}

///////////////////////////////////////////////////////////
// Returns whether an extension is supported or not
///////////////////////////////////////////////////////////
// WGL (Windows only)
bool Extensions::IsWGL_ARB_extensions_string()			const { return m_bWGL_ARB_extensions_string;		 }
bool Extensions::IsWGL_EXT_swap_control()				const { return m_bWGL_EXT_swap_control;				 }
bool Extensions::IsWGL_ARB_pixel_format()				const { return m_bWGL_ARB_pixel_format;				 }
bool Extensions::IsWGL_ARB_render_texture()				const { return m_bWGL_ARB_render_texture;			 }
bool Extensions::IsWGL_ARB_make_current_read()			const { return m_bWGL_ARB_make_current_read;		 }
bool Extensions::IsWGL_ARB_multisample()				const { return m_bWGL_ARB_multisample;				 }
bool Extensions::IsWGL_ATI_pixel_format_float()			const { return m_bWGL_ATI_pixel_format_float;		 }
bool Extensions::IsWGL_NV_float_buffer()				const { return m_bWGL_NV_float_buffer;				 }
// GLX (Linux only)
bool Extensions::IsGLX_SGI_swap_control()				const { return m_bGLX_SGI_swap_control;				 }
// EXT
bool Extensions::IsGL_EXT_compiled_vertex_array()		const { return m_bGL_EXT_compiled_vertex_array;		 }
bool Extensions::IsGL_EXT_draw_range_elements()			const { return m_bGL_EXT_draw_range_elements;		 }
bool Extensions::IsGL_EXT_fog_coord()					const { return m_bGL_EXT_fog_coord;					 }
bool Extensions::IsGL_EXT_secondary_color()				const { return m_bGL_EXT_secondary_color;			 }
bool Extensions::IsGL_EXT_texture_compression_s3tc()	const { return m_bGL_EXT_texture_compression_s3tc;	 }
bool Extensions::IsGL_EXT_texture_compression_latc()	const { return m_bGL_EXT_texture_compression_latc;	 }
bool Extensions::IsGL_EXT_texture_lod_bias()			const { return m_bGL_EXT_texture_lod_bias;			 }
bool Extensions::IsGL_EXT_texture_filter_anisotropic()	const { return m_bGL_EXT_texture_filter_anisotropic; }
bool Extensions::IsGL_EXT_separate_specular_color()		const { return m_bGL_EXT_separate_specular_color;	 }
bool Extensions::IsGL_EXT_texture_edge_clamp()			const { return m_bGL_EXT_texture_edge_clamp;		 }
bool Extensions::IsGL_EXT_texture_array()				const { return m_bGL_EXT_texture_array;				 }
bool Extensions::IsGL_EXT_texture_rectangle()			const { return m_bGL_EXT_texture_rectangle;			 }
bool Extensions::IsGL_EXT_texture3D()					const { return m_bGL_EXT_texture3D;					 }
bool Extensions::IsGL_EXT_texture_cube_map()			const { return m_bGL_EXT_texture_cube_map;			 }
bool Extensions::IsGL_EXT_stencil_wrap()				const { return m_bGL_EXT_stencil_wrap;				 }
bool Extensions::IsGL_EXT_stencil_two_side()			const { return m_bGL_EXT_stencil_two_side;			 }
bool Extensions::IsGL_EXT_packed_depth_stencil()		const { return m_bGL_EXT_packed_depth_stencil;		 }
bool Extensions::IsGL_EXT_depth_bounds_test()			const { return m_bGL_EXT_depth_bounds_test;			 }
bool Extensions::IsGL_EXT_framebuffer_object()			const { return m_bGL_EXT_framebuffer_object;		 }
bool Extensions::IsGL_EXT_framebuffer_multisample()		const { return m_bGL_EXT_framebuffer_multisample;	 }
bool Extensions::IsGL_EXT_framebuffer_blit()			const { return m_bGL_EXT_framebuffer_blit;			 }
bool Extensions::IsGL_EXT_geometry_shader4()			const { return m_bGL_EXT_geometry_shader4;			 }
bool Extensions::IsGL_EXT_transform_feedback()			const { return m_bGL_EXT_transform_feedback;		 }
// ARB
bool Extensions::IsGL_ARB_texture_float()				const { return m_bGL_ARB_texture_float;				 }
bool Extensions::IsGL_ARB_color_buffer_float()			const { return m_bGL_ARB_color_buffer_float;		 }
bool Extensions::IsGL_ARB_multitexture()				const { return m_bGL_ARB_multitexture;				 }
bool Extensions::IsGL_ARB_vertex_buffer_object()		const { return m_bGL_ARB_vertex_buffer_object;		 }
bool Extensions::IsGL_ARB_texture_border_clamp()		const { return m_bGL_ARB_texture_border_clamp;		 }
bool Extensions::IsGL_ARB_texture_mirrored_repeat()		const { return m_bGL_ARB_texture_mirrored_repeat;	 }
bool Extensions::IsGL_ARB_texture_cube_map()			const { return m_bGL_ARB_texture_cube_map;			 }
bool Extensions::IsGL_ARB_texture_env_combine()			const { return m_bGL_ARB_texture_env_combine;		 }
bool Extensions::IsGL_ARB_texture_env_dot3()			const { return m_bGL_ARB_texture_env_dot3;			 }
bool Extensions::IsGL_ARB_occlusion_query()				const { return m_bGL_ARB_occlusion_query;			 }
bool Extensions::IsGL_ARB_texture_compression()			const { return m_bGL_ARB_texture_compression;		 }
bool Extensions::IsGL_ARB_depth_texture()				const { return m_bGL_ARB_depth_texture;				 }
bool Extensions::IsGL_ARB_point_sprite()				const { return m_bGL_ARB_point_sprite;				 }
bool Extensions::IsGL_ARB_point_parameters()			const { return m_bGL_ARB_point_parameters;			 }
bool Extensions::IsGL_ARB_shading_language_100()		const { return m_bGL_ARB_shading_language_100;		 }
bool Extensions::IsGL_ARB_vertex_program()				const { return m_bGL_ARB_vertex_program;			 }
bool Extensions::IsGL_ARB_fragment_program()			const { return m_bGL_ARB_fragment_program;			 }
bool Extensions::IsGL_ARB_draw_buffers()				const { return m_bGL_ARB_draw_buffers;				 }
bool Extensions::IsGL_ARB_shader_objects()				const { return m_bGL_ARB_shader_objects;			 }
bool Extensions::IsGL_ARB_vertex_shader()				const { return m_bGL_ARB_vertex_shader;				 }
bool Extensions::IsGL_ARB_get_program_binary()			const { return m_bGL_ARB_get_program_binary;		 }
bool Extensions::IsGL_ARB_texture_non_power_of_two()	const { return m_bGL_ARB_texture_non_power_of_two;	 }
bool Extensions::IsGL_ARB_texture_rectangle()			const { return m_bGL_ARB_texture_rectangle;			 }
bool Extensions::IsGL_ARB_multisample()					const { return m_bGL_ARB_multisample;				 }
bool Extensions::IsGL_ARB_uniform_buffer_object()		const { return m_bGL_ARB_uniform_buffer_object;		 }
bool Extensions::IsGL_ARB_half_float_vertex()			const { return m_bGL_ARB_half_float_vertex;			 }
bool Extensions::IsGL_ARB_tessellation_shader()			const { return m_bGL_ARB_tessellation_shader;		 }
bool Extensions::IsGL_ARB_depth_clamp()					const { return m_bGL_ARB_depth_clamp;				 }
bool Extensions::IsGL_ARB_draw_instanced()				const { return m_bGL_ARB_draw_instanced;			 }
// ATI
bool Extensions::IsGL_ATI_meminfo()						const { return m_bGL_ATI_meminfo;					 }
bool Extensions::IsGL_ATI_separate_stencil()			const { return m_bGL_ATI_separate_stencil;			 }
bool Extensions::IsGL_ATI_draw_buffers()				const { return m_bGL_ATI_draw_buffers;				 }
bool Extensions::IsGL_ATI_texture_compression_3dc()		const { return m_bGL_ATI_texture_compression_3dc;	 }
// AMD
bool Extensions::IsGL_AMD_vertex_shader_tessellator()	const { return m_bGL_AMD_vertex_shader_tessellator;	 }
// NV
bool Extensions::IsGL_NV_texture_rectangle()			const { return m_bGL_NV_texture_rectangle;			 }
bool Extensions::IsGL_NV_occlusion_query()				const { return m_bGL_NV_occlusion_query;			 }
bool Extensions::IsGL_NVX_gpu_memory_info()				const { return m_bGL_NVX_gpu_memory_info;			 }
bool Extensions::IsGL_NV_half_float()					const { return m_bGL_NV_half_float;					 }
bool Extensions::IsGL_NV_texture_compression_vtc()		const { return m_bGL_NV_texture_compression_vtc;	 }
// SGIS
bool Extensions::IsGL_SGIS_generate_mipmap()			const { return m_bGL_SGIS_generate_mipmap;			 }
// HP
bool Extensions::IsGL_HP_occlusion_test()				const { return m_bGL_HP_occlusion_test;				 }


///////////////////////////////////////////////////////////
// Misc
///////////////////////////////////////////////////////////
GLint Extensions::GetGL_MAX_ELEMENTS_VERTICES_EXT() const	{ return m_nGL_MAX_ELEMENTS_VERTICES_EXT; }
GLint Extensions::GetGL_MAX_ELEMENTS_INDICES_EXT() const	{ return m_nGL_MAX_ELEMENTS_INDICES_EXT; }


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Checks whether an extension is supported by the given hardware or not
*/
bool Extensions::IsSupported(const char *pszExtension) const
{
	// Check whether or not the given extension string pointer is valid
	if (pszExtension) {
		// Is the extension supported by the hardware?
		if (CheckExtension(pszExtension)) {
			// Extension is supported!
			PL_LOG(Info, String("Use extension: ") + pszExtension)
			return true;
		} else {
			PL_LOG(Info, String("Extension '") + pszExtension + "' not found (nothing critical)")
		}
	}

	// Extension isn't supported!
	return false;
}

/**
*  @brief
*    Checks whether an extension is supported by the given hardware or not
*/
bool Extensions::CheckExtension(const char *pszExtension) const
{
	// Check whether or not the given extension string pointer is valid
	if (pszExtension) {
		// Under Windows all available extensions can be received via one additional function
		// but under Linux there are two additional functions for this
		#ifdef WIN32
			// "glGetString()" & "wglGetExtensionsStringARB()"
			const int nLoopMax = 2;
		#elif APPLE
			// On Mac OS X, only "glGetString(GL_EXTENSIONS)" is required
			const int nLoopMax = 1;
		#elif LINUX
			// "glGetString()" & "glXQueryExtensionsString()" & "glXGetClientString()"
			const int nLoopMax = 3;
		#endif
		const char *pszExtensions = nullptr;
		for (int i=0; i<nLoopMax; i++) {
			// Extension names should not have spaces
			const char *pszWhere = strchr(pszExtension, ' ');
			if (pszWhere || *pszExtension == '\0')
				return false; // Extension not found

			// Advanced extensions
			if (i) {
				#ifdef WIN32
					// WGL extensions
					if (!m_bWGL_ARB_extensions_string)
						return false; // Extension not found
					pszExtensions = static_cast<const char*>(wglGetExtensionsStringARB(wglGetCurrentDC()));
				#elif APPLE
					// On Mac OS X, only "glGetString(GL_EXTENSIONS)" is required
				#elif LINUX
					// Get the X server display connection
					Display *pDisplay = static_cast<ContextLinux&>(*m_pContext).GetDisplay();
					if (pDisplay) {
						if (i == 2)
							pszExtensions = static_cast<const char*>(glXQueryExtensionsString(pDisplay, XDefaultScreen(pDisplay)));
						else
							pszExtensions = static_cast<const char*>(glXGetClientString(pDisplay, GLX_EXTENSIONS));
					}
				#endif

			// Normal extensions
			} else {
				pszExtensions = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
			}
			if (!pszExtensions)
			  return false; // Extension not found

			// It takes a bit of care to be fool-proof about parsing the
			// OpenGL extensions string. Don't be fooled by substrings,
			// etc:
			const char *pszStart = pszExtensions;
			pszWhere = strstr(pszStart, pszExtension);
			while (pszWhere)  {
				const char *pszTerminator = pszWhere + strlen(pszExtension);
				if (pszWhere == pszStart || *(pszWhere - 1) == ' ' || *pszTerminator == ' ' || *pszTerminator == '\0')
					return true; // Extension found
				pszStart = pszTerminator;
				pszWhere = strstr(pszStart, pszExtension);
			}
		}
	}

	// Extension not found
	return false;
}

/**
*  @brief
*    Resets the extensions
*/
void Extensions::ResetExtensions()
{
	m_bInitialized = false;

	// Extensions
	// WGL (Windows only)
	m_bWGL_ARB_extensions_string			= false;
	m_bWGL_EXT_swap_control					= false;
	m_bWGL_ARB_pixel_format					= false;
	m_bWGL_ARB_render_texture				= false;
	m_bWGL_ARB_make_current_read			= false;
	m_bWGL_ARB_multisample					= false;
	m_bWGL_ATI_pixel_format_float			= false;
	m_bWGL_NV_float_buffer					= false;
	// GLX (Linux only)
	m_bGLX_SGI_swap_control					= false;
	// EXT
	m_bGL_EXT_compiled_vertex_array			= false;
	m_bGL_EXT_draw_range_elements			= false;
	m_bGL_EXT_fog_coord						= false;
	m_bGL_EXT_secondary_color				= false;
	m_bGL_EXT_texture_compression_s3tc		= false;
	m_bGL_EXT_texture_compression_latc		= false;
	m_bGL_EXT_texture_lod_bias				= false;
	m_bGL_EXT_texture_filter_anisotropic	= false;
	m_bGL_EXT_separate_specular_color		= false;
	m_bGL_EXT_texture_edge_clamp			= false;
	m_bGL_EXT_texture_array					= false;
	m_bGL_EXT_texture_rectangle				= false;
	m_bGL_EXT_texture3D						= false;
	m_bGL_EXT_texture_cube_map				= false;
	m_bGL_EXT_stencil_wrap					= false;
	m_bGL_EXT_stencil_two_side				= false;
	m_bGL_EXT_packed_depth_stencil			= false;
	m_bGL_EXT_depth_bounds_test				= false;
	m_bGL_EXT_framebuffer_object			= false;
	m_bGL_EXT_framebuffer_multisample		= false;
	m_bGL_EXT_framebuffer_blit				= false;
	m_bGL_EXT_geometry_shader4				= false;
	m_bGL_EXT_transform_feedback			= false;
	// ARB
	m_bGL_ARB_texture_float					= false;
	m_bGL_ARB_color_buffer_float			= false;
	m_bGL_ARB_multitexture					= false;
	m_bGL_ARB_vertex_buffer_object			= false;
	m_bGL_ARB_texture_border_clamp			= false;
	m_bGL_ARB_texture_mirrored_repeat		= false;
	m_bGL_ARB_texture_cube_map				= false;
	m_bGL_ARB_texture_env_combine			= false;
	m_bGL_ARB_texture_env_dot3				= false;
	m_bGL_ARB_occlusion_query				= false;
	m_bGL_ARB_texture_compression			= false;
	m_bGL_ARB_depth_texture					= false;
	m_bGL_ARB_point_sprite					= false;
	m_bGL_ARB_point_parameters				= false;
	m_bGL_ARB_shading_language_100			= false;
	m_bGL_ARB_vertex_program				= false;
	m_bGL_ARB_fragment_program				= false;
	m_bGL_ARB_draw_buffers					= false;
	m_bGL_ARB_shader_objects				= false;
	m_bGL_ARB_vertex_shader					= false;
	m_bGL_ARB_get_program_binary			= false;
	m_bGL_ARB_texture_non_power_of_two		= false;
	m_bGL_ARB_texture_rectangle				= false;
	m_bGL_ARB_multisample					= false;
	m_bGL_ARB_uniform_buffer_object			= false;
	m_bGL_ARB_half_float_vertex				= false;
	m_bGL_ARB_tessellation_shader			= false;
	m_bGL_ARB_depth_clamp					= false;
	m_bGL_ARB_draw_instanced				= false;
	// ATI
	m_bGL_ATI_meminfo						= false;
	m_bGL_ATI_separate_stencil				= false;
	m_bGL_ATI_draw_buffers					= false;
	m_bGL_ATI_texture_compression_3dc		= false;
	// AMD
	m_bGL_AMD_vertex_shader_tessellator		= false;
	// NV
	m_bGL_NV_texture_rectangle				= false;
	m_bGL_NV_occlusion_query				= false;
	m_bGL_NVX_gpu_memory_info				= false;
	m_bGL_NV_half_float						= false;
	m_bGL_NV_texture_compression_vtc		= false;
	// SGIS
	m_bGL_SGIS_generate_mipmap				= false;
	// HP
	m_bGL_HP_occlusion_test					= false;
	// Misc
	m_nGL_MAX_ELEMENTS_VERTICES_EXT = m_nGL_MAX_ELEMENTS_INDICES_EXT = 0;
}

/**
*  @brief
*    Initialize the supported universal extensions
*/
bool Extensions::InitUniversal()
{
	// Define a platform dependent helper macro
	#ifdef WIN32
		#define IMPORT_FUNC(funcName)																								\
			if (bResult) {																											\
				void *pSymbol = wglGetProcAddress(#funcName);																		\
				if (pSymbol) {																										\
					*(reinterpret_cast<void**>(&(funcName))) = pSymbol;																\
				} else {																											\
					PL_LOG(Error, String("Failed to find the entry point \"") + #funcName + "\" within the OpenGL dynamic library")	\
					bResult = false;																								\
				}																													\
			}
	#elif APPLE
		// For OpenGL extension handling, Apple provides several documents like
		// - "Technical Note TN2080 Understanding and Detecting OpenGL Functionality" (http://developer.apple.com/library/mac/#technotes/tn2080/_index.html)
		// - "Cross-Development Programming Guide" (http://www.filibeto.org/unix/macos/lib/dev/documentation/DeveloperTools/Conceptual/cross_development/cross_development.pdf)
		// -> All referencing to "QA1188: GetProcAdress and OpenGL Entry Points" (http://developer.apple.com/qa/qa2001/qa1188.html).
		//    Sadly, it appears that this site no longer exists.
		// -> It appears that for Mac OS X v10.6 >, the "dlopen"-way is recommended.
		#define IMPORT_FUNC(funcName)																								\
			if (bResult) {																											\
				void *pSymbol = m_pOpenGLSharedLibrary ? dlsym(m_pOpenGLSharedLibrary, #funcName) : nullptr;						\
				if (pSymbol) {																										\
					*(reinterpret_cast<void**>(&(funcName))) = pSymbol;																\
				} else {																											\
					PL_LOG(Error, String("Failed to find the entry point \"") + #funcName + "\" within the OpenGL dynamic library")	\
					bResult = false;																								\
				}																													\
			}
	#elif LINUX
		typedef void (*GLfunction)();
		#define IMPORT_FUNC(funcName)																								\
			if (bResult) {																											\
				GLfunction pSymbol = glXGetProcAddressARB(reinterpret_cast<const GLubyte*>(#funcName));								\
				if (pSymbol) {																										\
					*(reinterpret_cast<GLfunction*>(&(funcName))) = pSymbol;														\
				} else {																											\
					PL_LOG(Error, String("Failed to find the entry point \"") + #funcName + "\" within the OpenGL dynamic library")	\
					bResult = false;																								\
				}																													\
			}
	#endif


	//[-------------------------------------------------------]
	//[ EXT                                                   ]
	//[-------------------------------------------------------]
	// GL_EXT_compiled_vertex_array
	m_bGL_EXT_compiled_vertex_array = IsSupported("GL_EXT_compiled_vertex_array");
	if (m_bGL_EXT_compiled_vertex_array) {
		// Load the entry points
		bool bResult = true;	// Success by default
		IMPORT_FUNC(glLockArraysEXT)
		IMPORT_FUNC(glUnlockArraysEXT)
		m_bGL_EXT_compiled_vertex_array = bResult;
	}

	// GL_EXT_draw_range_elements
	m_bGL_EXT_draw_range_elements = IsSupported("GL_EXT_draw_range_elements");
	if (m_bGL_EXT_draw_range_elements) {
		// Load the entry points
		bool bResult = true;	// Success by default
		IMPORT_FUNC(glDrawRangeElementsEXT)
		m_bGL_EXT_draw_range_elements = bResult;
	}
	if (m_bGL_EXT_draw_range_elements) {	
		glGetIntegerv(GL_MAX_ELEMENTS_VERTICES_EXT, &m_nGL_MAX_ELEMENTS_VERTICES_EXT);
		glGetIntegerv(GL_MAX_ELEMENTS_INDICES_EXT,	&m_nGL_MAX_ELEMENTS_INDICES_EXT);
	} else {
		m_nGL_MAX_ELEMENTS_VERTICES_EXT = m_nGL_MAX_ELEMENTS_INDICES_EXT = 0;
	}

	// GL_EXT_fog_coord
	m_bGL_EXT_fog_coord = IsSupported("GL_EXT_fog_coord");
	if (m_bGL_EXT_fog_coord) {
		// Load the entry points
		bool bResult = true;	// Success by default
		IMPORT_FUNC(glFogCoordfEXT)
		IMPORT_FUNC(glFogCoordfvEXT)
		IMPORT_FUNC(glFogCoorddEXT)
		IMPORT_FUNC(glFogCoorddvEXT)
		IMPORT_FUNC(glFogCoordPointerEXT)
		m_bGL_EXT_fog_coord = bResult;
	}

	// GL_EXT_secondary_color
	m_bGL_EXT_secondary_color = IsSupported("GL_EXT_secondary_color");
	if (m_bGL_EXT_secondary_color) {
		// Load the entry points
		bool bResult = true;	// Success by default
		IMPORT_FUNC(glSecondaryColor3bEXT)
		IMPORT_FUNC(glSecondaryColor3bvEXT)
		IMPORT_FUNC(glSecondaryColor3dEXT)
		IMPORT_FUNC(glSecondaryColor3dvEXT)
		IMPORT_FUNC(glSecondaryColor3fEXT)
		IMPORT_FUNC(glSecondaryColor3fvEXT)
		IMPORT_FUNC(glSecondaryColor3iEXT)
		IMPORT_FUNC(glSecondaryColor3ivEXT)
		IMPORT_FUNC(glSecondaryColor3sEXT)
		IMPORT_FUNC(glSecondaryColor3svEXT)
		IMPORT_FUNC(glSecondaryColor3ubEXT)
		IMPORT_FUNC(glSecondaryColor3ubvEXT)
		IMPORT_FUNC(glSecondaryColor3uiEXT)
		IMPORT_FUNC(glSecondaryColor3uivEXT)
		IMPORT_FUNC(glSecondaryColor3usEXT)
		IMPORT_FUNC(glSecondaryColor3usvEXT)
		IMPORT_FUNC(glSecondaryColorPointerEXT)
		m_bGL_EXT_secondary_color = bResult;
	}

	// GL_EXT_texture_compression_s3tc
	m_bGL_EXT_texture_compression_s3tc = IsSupported("GL_EXT_texture_compression_s3tc");

	// GL_EXT_texture_compression_latc
	if (IsSupported("GL_EXT_texture_compression_latc"))
		m_bGL_EXT_texture_compression_latc = true;
	else
		m_bGL_EXT_texture_compression_latc = false;

	// GL_EXT_texture_lod_bias
	m_bGL_EXT_texture_lod_bias = IsSupported("GL_EXT_texture_lod_bias");

	// GL_EXT_texture_filter_anisotropic
	m_bGL_EXT_texture_filter_anisotropic = IsSupported("GL_EXT_texture_filter_anisotropic");

	// GL_EXT_separate_specular_color
	m_bGL_EXT_separate_specular_color = IsSupported("GL_EXT_separate_specular_color");

	// GL_EXT_texture_edge_clamp
	m_bGL_EXT_texture_edge_clamp = IsSupported("GL_EXT_texture_edge_clamp");

	// GL_EXT_texture_array
	m_bGL_EXT_texture_array = IsSupported("GL_EXT_texture_array");

	// GL_EXT_texture_rectangle
	m_bGL_EXT_texture_rectangle = IsSupported("GL_EXT_texture_rectangle");

	// GL_EXT_texture3D
	m_bGL_EXT_texture3D = IsSupported("GL_EXT_texture3D");
	if (m_bGL_EXT_texture3D) {
		// Load the entry points
		bool bResult = true;	// Success by default
		IMPORT_FUNC(glTexImage3DEXT)
		IMPORT_FUNC(glTexSubImage3DEXT)
		m_bGL_EXT_texture3D = bResult;
	}

	// GL_EXT_texture_cube_map
	m_bGL_EXT_texture_cube_map = IsSupported("GL_EXT_texture_cube_map");

	// GL_EXT_stencil_wrap
	m_bGL_EXT_stencil_wrap = IsSupported("GL_EXT_stencil_wrap");

	// GL_EXT_stencil_two_side
	m_bGL_EXT_stencil_two_side = IsSupported("GL_EXT_stencil_two_side");
	if (m_bGL_EXT_stencil_two_side) {
		// Load the entry points
		bool bResult = true;	// Success by default
		IMPORT_FUNC(glActiveStencilFaceEXT)
		m_bGL_EXT_stencil_two_side = bResult;
	}

	// GL_EXT_packed_depth_stencil
	m_bGL_EXT_packed_depth_stencil = IsSupported("GL_EXT_packed_depth_stencil");

	// GL_EXT_depth_bounds_test
	m_bGL_EXT_depth_bounds_test = IsSupported("GL_EXT_depth_bounds_test");
	if (m_bGL_EXT_depth_bounds_test) {
		// Load the entry points
		bool bResult = true;	// Success by default
		IMPORT_FUNC(glDepthBoundsEXT)
		m_bGL_EXT_depth_bounds_test = bResult;
	}

	// GL_EXT_framebuffer_object
	m_bGL_EXT_framebuffer_object = IsSupported("GL_EXT_framebuffer_object");
	if (m_bGL_EXT_framebuffer_object) {
		// Load the entry points
		bool bResult = true;	// Success by default
		IMPORT_FUNC(glIsRenderbufferEXT)
		IMPORT_FUNC(glBindRenderbufferEXT)
		IMPORT_FUNC(glDeleteRenderbuffersEXT)
		IMPORT_FUNC(glGenRenderbuffersEXT)
		IMPORT_FUNC(glRenderbufferStorageEXT)
		IMPORT_FUNC(glGetRenderbufferParameterivEXT)
		IMPORT_FUNC(glIsFramebufferEXT)
		IMPORT_FUNC(glBindFramebufferEXT)
		IMPORT_FUNC(glDeleteFramebuffersEXT)
		IMPORT_FUNC(glGenFramebuffersEXT)
		IMPORT_FUNC(glCheckFramebufferStatusEXT)
		IMPORT_FUNC(glFramebufferTexture1DEXT)
		IMPORT_FUNC(glFramebufferTexture2DEXT)
		IMPORT_FUNC(glFramebufferTexture3DEXT)
		IMPORT_FUNC(glFramebufferRenderbufferEXT)
		IMPORT_FUNC(glGetFramebufferAttachmentParameterivEXT)
		IMPORT_FUNC(glGenerateMipmapEXT)
		m_bGL_EXT_framebuffer_object = bResult;
	}

	// GL_EXT_framebuffer_multisample
	m_bGL_EXT_framebuffer_multisample = IsSupported("GL_EXT_framebuffer_multisample");
	if (m_bGL_EXT_framebuffer_multisample) {
		// Load the entry points
		bool bResult = true;	// Success by default
		IMPORT_FUNC(glRenderbufferStorageMultisampleEXT)
		m_bGL_EXT_framebuffer_multisample = bResult;
	}

	// GL_EXT_framebuffer_blit
	m_bGL_EXT_framebuffer_blit = IsSupported("GL_EXT_framebuffer_blit");
	if (m_bGL_EXT_framebuffer_blit) {
		// Load the entry points
		bool bResult = true;	// Success by default
		IMPORT_FUNC(glBlitFramebufferEXT)
		m_bGL_EXT_framebuffer_blit = bResult;
	}

	// GL_EXT_geometry_shader4
	m_bGL_EXT_geometry_shader4 = IsSupported("GL_EXT_geometry_shader4");
	if (m_bGL_EXT_geometry_shader4) {
		// Load the entry points
		bool bResult = true;	// Success by default
		IMPORT_FUNC(glProgramParameteriEXT)
		m_bGL_EXT_geometry_shader4 = bResult;
	}

	// GL_EXT_transform_feedback
	m_bGL_EXT_transform_feedback = IsSupported("GL_EXT_transform_feedback");
	if (m_bGL_EXT_transform_feedback) {
		// Load the entry points
		bool bResult = true;	// Success by default
		IMPORT_FUNC(glBeginTransformFeedbackEXT)
		IMPORT_FUNC(glEndTransformFeedbackEXT)
		IMPORT_FUNC(glBindBufferRangeEXT)
		IMPORT_FUNC(glBindBufferOffsetEXT)
		IMPORT_FUNC(glBindBufferBaseEXT)
		IMPORT_FUNC(glTransformFeedbackVaryingsEXT)
		IMPORT_FUNC(glGetTransformFeedbackVaryingEXT)
		m_bGL_EXT_transform_feedback = bResult;
	}


	//[-------------------------------------------------------]
	//[ ARB                                                   ]
	//[-------------------------------------------------------]
	// GL_ARB_texture_float
	m_bGL_ARB_texture_float = IsSupported("GL_ARB_texture_float");

	// GL_ARB_color_buffer_float
	m_bGL_ARB_color_buffer_float = IsSupported("GL_ARB_color_buffer_float");
	if (m_bGL_ARB_color_buffer_float) {
		// Load the entry points
		bool bResult = true;	// Success by default
		IMPORT_FUNC(glClampColorARB)
		m_bGL_ARB_color_buffer_float = bResult;
	}

	// GL_ARB_multitexture
	m_bGL_ARB_multitexture = IsSupported("GL_ARB_multitexture");
	if (m_bGL_ARB_multitexture) {
		// Load the entry points
		bool bResult = true;	// Success by default
		IMPORT_FUNC(glActiveTextureARB)
		IMPORT_FUNC(glClientActiveTextureARB)
		IMPORT_FUNC(glMultiTexCoord1dARB)
		IMPORT_FUNC(glMultiTexCoord1dvARB)
		IMPORT_FUNC(glMultiTexCoord1fARB)
		IMPORT_FUNC(glMultiTexCoord1fvARB)
		IMPORT_FUNC(glMultiTexCoord1iARB)
		IMPORT_FUNC(glMultiTexCoord1ivARB)
		IMPORT_FUNC(glMultiTexCoord1sARB)
		IMPORT_FUNC(glMultiTexCoord1svARB)
		IMPORT_FUNC(glMultiTexCoord2dARB)
		IMPORT_FUNC(glMultiTexCoord2dvARB)
		IMPORT_FUNC(glMultiTexCoord2fARB)
		IMPORT_FUNC(glMultiTexCoord2fvARB)
		IMPORT_FUNC(glMultiTexCoord2iARB)
		IMPORT_FUNC(glMultiTexCoord2ivARB)
		IMPORT_FUNC(glMultiTexCoord2sARB)
		IMPORT_FUNC(glMultiTexCoord2svARB)
		IMPORT_FUNC(glMultiTexCoord3dARB)
		IMPORT_FUNC(glMultiTexCoord3dvARB)
		IMPORT_FUNC(glMultiTexCoord3fARB)
		IMPORT_FUNC(glMultiTexCoord3fvARB)
		IMPORT_FUNC(glMultiTexCoord3iARB)
		IMPORT_FUNC(glMultiTexCoord3ivARB)
		IMPORT_FUNC(glMultiTexCoord3sARB)
		IMPORT_FUNC(glMultiTexCoord3svARB)
		IMPORT_FUNC(glMultiTexCoord4dARB)
		IMPORT_FUNC(glMultiTexCoord4dvARB)
		IMPORT_FUNC(glMultiTexCoord4fARB)
		IMPORT_FUNC(glMultiTexCoord4fvARB)
		IMPORT_FUNC(glMultiTexCoord4iARB)
		IMPORT_FUNC(glMultiTexCoord4ivARB)
		IMPORT_FUNC(glMultiTexCoord4sARB)
		IMPORT_FUNC(glMultiTexCoord4svARB)
		m_bGL_ARB_multitexture = bResult;
	}

	// GL_ARB_vertex_buffer_object
	m_bGL_ARB_vertex_buffer_object = IsSupported("GL_ARB_vertex_buffer_object");
	if (m_bGL_ARB_vertex_buffer_object) {
		// Load the entry points
		bool bResult = true;	// Success by default
		IMPORT_FUNC(glBindBufferARB)
		IMPORT_FUNC(glDeleteBuffersARB)
		IMPORT_FUNC(glGenBuffersARB)
		IMPORT_FUNC(glBufferDataARB)
		IMPORT_FUNC(glBufferSubDataARB)
		IMPORT_FUNC(glGetBufferSubDataARB)
		IMPORT_FUNC(glMapBufferARB)
		IMPORT_FUNC(glUnmapBufferARB)
		IMPORT_FUNC(glGetBufferParameterivARB)
		IMPORT_FUNC(glGetBufferPointervARB)
		m_bGL_ARB_vertex_buffer_object = bResult;
	}

	// GL_ARB_texture_border_clamp
	m_bGL_ARB_texture_border_clamp = IsSupported("GL_ARB_texture_border_clamp");

	// GL_ARB_texture_mirrored_repeat 
	m_bGL_ARB_texture_mirrored_repeat = IsSupported("GL_ARB_texture_mirrored_repeat");

	// GL_ARB_texture_cube_map
	m_bGL_ARB_texture_cube_map = IsSupported("GL_ARB_texture_cube_map");

	// GL_ARB_texture_env_combine
	m_bGL_ARB_texture_env_combine = IsSupported("GL_ARB_texture_env_combine");

	// GL_ARB_texture_env_dot3
	m_bGL_ARB_texture_env_dot3 = IsSupported("GL_ARB_texture_env_dot3");

	// GL_ARB_occlusion_query
	m_bGL_ARB_occlusion_query = IsSupported("GL_ARB_occlusion_query");
	if (m_bGL_ARB_occlusion_query) {
		// Load the entry points
		bool bResult = true;	// Success by default
		IMPORT_FUNC(glGenQueriesARB)
		IMPORT_FUNC(glDeleteQueriesARB)
		IMPORT_FUNC(glIsQueryARB)
		IMPORT_FUNC(glBeginQueryARB)
		IMPORT_FUNC(glEndQueryARB)
		IMPORT_FUNC(glGetQueryivARB)
		IMPORT_FUNC(glGetQueryObjectivARB)
		IMPORT_FUNC(glGetQueryObjectuivARB)
		m_bGL_ARB_occlusion_query = bResult;
	}

	// GL_ARB_texture_compression
	m_bGL_ARB_texture_compression = IsSupported("GL_ARB_texture_compression");
	if (m_bGL_ARB_texture_compression) {
		// Load the entry points
		bool bResult = true;	// Success by default
		IMPORT_FUNC(glCompressedTexImage3DARB)
		IMPORT_FUNC(glCompressedTexImage2DARB)
		IMPORT_FUNC(glCompressedTexImage1DARB)
		IMPORT_FUNC(glCompressedTexSubImage3DARB)
		IMPORT_FUNC(glCompressedTexSubImage2DARB)
		IMPORT_FUNC(glCompressedTexSubImage1DARB)
		IMPORT_FUNC(glGetCompressedTexImageARB)
		m_bGL_ARB_texture_compression = bResult;
	}

	// GL_ARB_depth_texture
	m_bGL_ARB_depth_texture = IsSupported("GL_ARB_depth_texture");

	// GL_ARB_point_sprite
	m_bGL_ARB_point_sprite = IsSupported("GL_ARB_point_sprite");

	// GL_ARB_point_parameters
	m_bGL_ARB_point_parameters = IsSupported("GL_ARB_point_parameters");
	if (m_bGL_ARB_point_parameters) {
		// Load the entry points
		bool bResult = true;	// Success by default
		IMPORT_FUNC(glPointParameterfARB)
		IMPORT_FUNC(glPointParameterfvARB)
		m_bGL_ARB_point_parameters = bResult;
	}

	// GL_ARB_shading_language_100
	m_bGL_ARB_shading_language_100 = IsSupported("GL_ARB_shading_language_100");

	// GL_ARB_vertex_program
	m_bGL_ARB_vertex_program = IsSupported("GL_ARB_vertex_program");
	if (m_bGL_ARB_vertex_program) {
		// Load the entry points
		bool bResult = true;	// Success by default
		IMPORT_FUNC(glVertexAttribPointerARB)
		IMPORT_FUNC(glEnableVertexAttribArrayARB)
		IMPORT_FUNC(glDisableVertexAttribArrayARB)
		IMPORT_FUNC(glGetProgramivARB)
		m_bGL_ARB_vertex_program = bResult;
	}

	// GL_ARB_fragment_program (we do not need any of the functions this extension provides)
	m_bGL_ARB_fragment_program = IsSupported("GL_ARB_fragment_program");

	// GL_ARB_draw_buffers
	m_bGL_ARB_draw_buffers = IsSupported("GL_ARB_draw_buffers");
	if (m_bGL_ARB_draw_buffers) {
		// Load the entry points
		bool bResult = true;	// Success by default
		IMPORT_FUNC(glDrawBuffersARB)
		m_bGL_ARB_draw_buffers = bResult;
	}

	// GL_ARB_shader_objects
	m_bGL_ARB_shader_objects = IsSupported("GL_ARB_shader_objects");
	if (m_bGL_ARB_shader_objects) {
		// Load the entry points
		bool bResult = true;	// Success by default
		IMPORT_FUNC(glDeleteObjectARB)
		IMPORT_FUNC(glGetHandleARB)
		IMPORT_FUNC(glDetachObjectARB)
		IMPORT_FUNC(glCreateShaderObjectARB)
		IMPORT_FUNC(glShaderSourceARB)
		IMPORT_FUNC(glCompileShaderARB)
		IMPORT_FUNC(glCreateProgramObjectARB)
		IMPORT_FUNC(glAttachObjectARB)
		IMPORT_FUNC(glLinkProgramARB)
		IMPORT_FUNC(glUseProgramObjectARB)
		IMPORT_FUNC(glValidateProgramARB)
		IMPORT_FUNC(glUniform1fARB)
		IMPORT_FUNC(glUniform2fARB)
		IMPORT_FUNC(glUniform3fARB)
		IMPORT_FUNC(glUniform4fARB)
		IMPORT_FUNC(glUniform1iARB)
		IMPORT_FUNC(glUniform2iARB)
		IMPORT_FUNC(glUniform3iARB)
		IMPORT_FUNC(glUniform4iARB)
		IMPORT_FUNC(glUniform1fvARB)
		IMPORT_FUNC(glUniform2fvARB)
		IMPORT_FUNC(glUniform3fvARB)
		IMPORT_FUNC(glUniform4fvARB)
		IMPORT_FUNC(glUniform1ivARB)
		IMPORT_FUNC(glUniform2ivARB)
		IMPORT_FUNC(glUniform3ivARB)
		IMPORT_FUNC(glUniform4ivARB)
		IMPORT_FUNC(glUniformMatrix2fvARB)
		IMPORT_FUNC(glUniformMatrix3fvARB)
		IMPORT_FUNC(glUniformMatrix4fvARB)
		IMPORT_FUNC(glGetObjectParameterfvARB)
		IMPORT_FUNC(glGetObjectParameterivARB)
		IMPORT_FUNC(glGetInfoLogARB)
		IMPORT_FUNC(glGetAttachedObjectsARB)
		IMPORT_FUNC(glGetUniformLocationARB)
		IMPORT_FUNC(glGetActiveUniformARB)
		IMPORT_FUNC(glGetUniformfvARB)
		IMPORT_FUNC(glGetUniformivARB)
		IMPORT_FUNC(glGetShaderSourceARB)
		m_bGL_ARB_shader_objects = bResult;
	}

	// GL_ARB_vertex_shader
	m_bGL_ARB_vertex_shader = IsSupported("GL_ARB_vertex_shader");
	if (m_bGL_ARB_vertex_shader) {
		// Load the entry points
		bool bResult = true;	// Success by default
		IMPORT_FUNC(glBindAttribLocationARB)
		IMPORT_FUNC(glGetActiveAttribARB)
		IMPORT_FUNC(glGetAttribLocationARB)
		m_bGL_ARB_vertex_shader = bResult;
	}

	// GL_ARB_get_program_binary
	m_bGL_ARB_get_program_binary = IsSupported("GL_ARB_get_program_binary");
	if (m_bGL_ARB_get_program_binary) {
		// Load the entry points
		bool bResult = true;	// Success by default
		IMPORT_FUNC(glGetProgramBinary)
		IMPORT_FUNC(glProgramBinary)
		IMPORT_FUNC(glProgramParameteri)
		m_bGL_ARB_get_program_binary = bResult;
	}

	// GL_ARB_texture_non_power_of_two
	m_bGL_ARB_texture_non_power_of_two = IsSupported("GL_ARB_texture_non_power_of_two");

	// GL_ARB_texture_rectangle
	m_bGL_ARB_texture_rectangle = IsSupported("GL_ARB_texture_rectangle");

	// GL_ARB_multisample
	m_bGL_ARB_multisample = IsSupported("GL_ARB_multisample");

	// GL_ARB_uniform_buffer_object
	m_bGL_ARB_uniform_buffer_object = IsSupported("GL_ARB_uniform_buffer_object");
	if (m_bGL_ARB_uniform_buffer_object) {
		// Load the entry points
		bool bResult = true;	// Success by default
		IMPORT_FUNC(glGetUniformIndices)
		IMPORT_FUNC(glGetActiveUniformsiv)
		IMPORT_FUNC(glGetActiveUniformName)
		IMPORT_FUNC(glGetUniformBlockIndex)
		IMPORT_FUNC(glGetActiveUniformBlockiv)
		IMPORT_FUNC(glGetActiveUniformBlockName)
		IMPORT_FUNC(glUniformBlockBinding)
		m_bGL_ARB_uniform_buffer_object = bResult;
	}

	// GL_ARB_half_float_vertex
	m_bGL_ARB_half_float_vertex = IsSupported("GL_ARB_half_float_vertex");

	// GL_ARB_tessellation_shader
	m_bGL_ARB_tessellation_shader = IsSupported("GL_ARB_tessellation_shader");
	if (m_bGL_ARB_tessellation_shader) {
		// Load the entry points
		bool bResult = true;	// Success by default
		IMPORT_FUNC(glPatchParameteri)
		IMPORT_FUNC(glPatchParameterfv)
		m_bGL_ARB_tessellation_shader = bResult;
	}

	// GL_ARB_depth_clamp
	m_bGL_ARB_depth_clamp = IsSupported("GL_ARB_depth_clamp");

	// GL_ARB_draw_instanced
	m_bGL_ARB_draw_instanced = IsSupported("GL_ARB_draw_instanced");
	if (m_bGL_ARB_draw_instanced) {
		// Load the entry points
		bool bResult = true;	// Success by default
		IMPORT_FUNC(glDrawArraysInstancedARB)
		IMPORT_FUNC(glDrawElementsInstancedARB)
		m_bGL_ARB_draw_instanced = bResult;
	}


	//[-------------------------------------------------------]
	//[ ATI                                                   ]
	//[-------------------------------------------------------]
	// GL_ATI_meminfo
	m_bGL_ATI_meminfo = IsSupported("GL_ATI_meminfo");

	// GL_ATI_separate_stencil
	m_bGL_ATI_separate_stencil = IsSupported("GL_ATI_separate_stencil");
	if (m_bGL_ATI_separate_stencil) {
		// Load the entry points
		bool bResult = true;	// Success by default
		IMPORT_FUNC(glStencilOpSeparateATI)
		IMPORT_FUNC(glStencilFuncSeparateATI)
		m_bGL_ATI_separate_stencil = bResult;
	}

	// GL_ATI_draw_buffers
	m_bGL_ATI_draw_buffers = IsSupported("GL_ATI_draw_buffers");
	if (m_bGL_ATI_draw_buffers) {
		// Load the entry points
		bool bResult = true;	// Success by default
		IMPORT_FUNC(glDrawBuffersATI)
		m_bGL_ATI_draw_buffers = bResult;
	}

	// GL_ATI_texture_compression_3dc
	m_bGL_ATI_texture_compression_3dc = IsSupported("GL_ATI_texture_compression_3dc");


	//[-------------------------------------------------------]
	//[ AMD                                                   ]
	//[-------------------------------------------------------]
	// GL_AMD_vertex_shader_tessellator
	// -> The extension is listed as "GL_AMD_vertex_shader_tessellator" (see http://www.opengl.org/registry/specs/AMD/vertex_shader_tessellator.txt)
	// -> In "http://www.opengl.org/registry/api/glext.h" it's listed as "GL_AMD_vertex_shader_tesselator" (typo?)
	m_bGL_AMD_vertex_shader_tessellator = IsSupported("GL_AMD_vertex_shader_tessellator");
	if (m_bGL_AMD_vertex_shader_tessellator) {
		// Load the entry points
		bool bResult = true;	// Success by default
		IMPORT_FUNC(glTessellationFactorAMD)
		IMPORT_FUNC(glTessellationModeAMD)
		m_bGL_AMD_vertex_shader_tessellator = bResult;
	}


	//[-------------------------------------------------------]
	//[ NV                                                    ]
	//[-------------------------------------------------------]
	// GL_NV_texture_rectangle
	m_bGL_NV_texture_rectangle = IsSupported("GL_NV_texture_rectangle");

	// GL_NV_occlusion_query
	m_bGL_NV_occlusion_query = IsSupported("GL_NV_occlusion_query");
	if (m_bGL_NV_occlusion_query) {
		// Load the entry points
		bool bResult = true;	// Success by default
		IMPORT_FUNC(glGenOcclusionQueriesNV)
		IMPORT_FUNC(glDeleteOcclusionQueriesNV)
		IMPORT_FUNC(glIsOcclusionQueryNV)
		IMPORT_FUNC(glBeginOcclusionQueryNV)
		IMPORT_FUNC(glEndOcclusionQueryNV)
		IMPORT_FUNC(glGetOcclusionQueryivNV)
		IMPORT_FUNC(glGetOcclusionQueryuivNV)
		m_bGL_NV_occlusion_query = bResult;
	}

	// GL_NVX_gpu_memory_info
	m_bGL_NVX_gpu_memory_info = IsSupported("GL_NVX_gpu_memory_info");

	// GL_NV_half_float
	m_bGL_NV_half_float = IsSupported("GL_NV_half_float");

	// GL_NV_texture_compression_vtc
	m_bGL_NV_texture_compression_vtc = IsSupported("GL_NV_texture_compression_vtc");


	//[-------------------------------------------------------]
	//[ SGIS                                                  ]
	//[-------------------------------------------------------]
	// GL_SGIS_generate_mipmap
	m_bGL_SGIS_generate_mipmap = IsSupported("GL_SGIS_generate_mipmap");


	//[-------------------------------------------------------]
	//[ HP                                                    ]
	//[-------------------------------------------------------]
	// GL_HP_occlusion_test
	m_bGL_HP_occlusion_test = IsSupported("GL_HP_occlusion_test");


	// Undefine the helper macro
	#undef IMPORT_FUNC

	// Done
	return true;
}

/**
*  @brief
*    Shows general OpenGL information
*/
void Extensions::ShowGeneralOpenGLInformation() const
{
	GLint nTemp;

	PL_LOG(Info, "General OpenGL information:")

	if (m_bGL_ARB_shading_language_100)
		PL_LOG(Info, String::Format("OpenGL shading language version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION_ARB) ? reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION_ARB)) : "-"))

	if (m_bGL_ARB_multitexture) {
		glGetIntegerv(GL_MAX_TEXTURE_COORDS_ARB, &nTemp);
		PL_LOG(Info, String::Format("  GL_MAX_TEXTURE_COORDS_ARB: %d", nTemp))

		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &nTemp);
		PL_LOG(Info, String::Format("  GL_MAX_TEXTURE_IMAGE_UNITS_ARB: %d", nTemp))
	}

	if (glDrawRangeElementsEXT) {
		PL_LOG(Info, "glDrawRangeElementsEXT information:")
		glGetIntegerv(GL_MAX_ELEMENTS_VERTICES_EXT, &nTemp);
		PL_LOG(Info, String::Format("  Max elements vertices = %d", nTemp))
		glGetIntegerv(GL_MAX_ELEMENTS_INDICES_EXT, &nTemp);
		PL_LOG(Info, String::Format("  Max elements indices = %d", nTemp))
	}

	if (m_bGL_ARB_vertex_program || m_bGL_ARB_fragment_program) {
		PL_LOG(Info, "OpenGL vertex & fragment shader program information:")

		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS_ARB, &nTemp);
		PL_LOG(Info, String::Format("  GL_MAX_VERTEX_ATTRIBS_ARB: %d", nTemp))

		glGetIntegerv(GL_MAX_PROGRAM_MATRICES_ARB, &nTemp);
		PL_LOG(Info, String::Format("  GL_MAX_PROGRAM_MATRICES_ARB: %d", nTemp))

		glGetIntegerv(GL_MAX_PROGRAM_MATRIX_STACK_DEPTH_ARB, &nTemp);
		PL_LOG(Info, String::Format("  GL_MAX_PROGRAM_MATRIX_STACK_DEPTH_ARB: %d", nTemp))
	}

	if (m_bGL_ARB_vertex_program) {
		PL_LOG(Info, "OpenGL vertex shader program information:")
		ShowOpenGLShaderProgramInformation(GL_VERTEX_PROGRAM_ARB);
	}

	if (m_bGL_ARB_fragment_program) {
		PL_LOG(Info, "OpenGL fragment shader program information:")

		glGetIntegerv(GL_MAX_TEXTURE_COORDS_ARB, &nTemp);
		PL_LOG(Info, String::Format("  GL_MAX_TEXTURE_COORDS_ARB: %d", nTemp))

		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &nTemp);
		PL_LOG(Info, String::Format("  GL_MAX_TEXTURE_IMAGE_UNITS_ARB: %d", nTemp))

		ShowOpenGLShaderProgramInformation(GL_FRAGMENT_PROGRAM_ARB);
	}

	if (m_bGL_ARB_draw_buffers) {
		PL_LOG(Info, "GL_ARB_draw_buffers information:")
		glGetIntegerv(GL_MAX_DRAW_BUFFERS_ARB, &nTemp);
		PL_LOG(Info, String::Format("  Max draw buffers = %d", nTemp))
		glGetIntegerv(GL_AUX_BUFFERS, &nTemp);
		PL_LOG(Info, String::Format("  Aux buffers = %d", nTemp))
	}

	if (m_bGL_ATI_draw_buffers) {
		PL_LOG(Info, "GL_ATI_draw_buffers information:")
		glGetIntegerv(GL_MAX_DRAW_BUFFERS_ATI, &nTemp);
		PL_LOG(Info, String::Format("  Max draw buffers = %d", nTemp))
		glGetIntegerv(GL_AUX_BUFFERS, &nTemp);
		PL_LOG(Info, String::Format("  Aux buffers = %d", nTemp))
	}

	if (m_bGL_EXT_framebuffer_object) {
		PL_LOG(Info, "GL_EXT_framebuffer_object information:")
		glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT, &nTemp);
		PL_LOG(Info, String::Format("  Max color attachments = %d", nTemp))
		glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE_EXT, &nTemp);
		PL_LOG(Info, String::Format("  Max renderbuffer size = %d", nTemp))
	}

	if (m_bGL_EXT_framebuffer_multisample) {
		PL_LOG(Info, "GL_EXT_framebuffer_multisample information:")
		glGetIntegerv(GL_MAX_SAMPLES_EXT, &nTemp);
		PL_LOG(Info, String::Format("  Max framebuffer samples = %d", nTemp))
	}

	if (m_bGL_ARB_texture_compression) {
		// Get the number of compressed formats
		GLint nNumOfCompressedFormats;
		glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB, &nNumOfCompressedFormats);
		if (nNumOfCompressedFormats) {
			// Get the compressed formats
			GLint *pCompressedFormat = new GLint[nNumOfCompressedFormats];
			glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS_ARB, pCompressedFormat);

			// Log output
			PL_LOG(Info, String("GL_ARB_texture_compression: Number of compressed formats: ") + nNumOfCompressedFormats)
			for (GLint i=0; i<nNumOfCompressedFormats; i++) {
				switch (pCompressedFormat[i]) {
					case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:  PL_LOG(Info, "  GL_COMPRESSED_RGB_S3TC_DXT1_EXT")					 break;
					case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT: PL_LOG(Info, "  GL_COMPRESSED_RGBA_S3TC_DXT1_EXT")					 break;
					case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT: PL_LOG(Info, "  GL_COMPRESSED_RGBA_S3TC_DXT3_EXT")					 break;
					case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT: PL_LOG(Info, "  GL_COMPRESSED_RGBA_S3TC_DXT5_EXT")					 break;
					default:							   PL_LOG(Info, String("  ") + pCompressedFormat[i] + " (unknown name)") break;
				}
			}

			// Cleanup
			delete [] pCompressedFormat;
		}
	}

	if (m_bGL_ARB_uniform_buffer_object) {
		PL_LOG(Info, "GL_ARB_uniform_buffer_object information:")
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &nTemp);
		PL_LOG(Info, String::Format("  Maximum number of vertex uniform buffers per program = %d", nTemp))
		glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &nTemp);
		PL_LOG(Info, String::Format("  Maximum number of fragment uniform buffers per program = %d", nTemp))
		glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_BLOCKS, &nTemp);
		PL_LOG(Info, String::Format("  Maximum number of geometry uniform buffers per program = %d", nTemp))
		glGetIntegerv(GL_MAX_COMBINED_UNIFORM_BLOCKS, &nTemp);
		PL_LOG(Info, String::Format("  Maximum number of uniform buffers per program = %d", nTemp))
		glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &nTemp);
		PL_LOG(Info, String::Format("  Maximum number of uniform buffer binding points on the context = %d", nTemp))
		glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &nTemp);
		PL_LOG(Info, String::Format("  Max size in basic machine units of a uniform block = %d", nTemp))
		glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &nTemp);
		PL_LOG(Info, String::Format("  Number of words for vertex shader uniform variables in default uniform block = %d", nTemp))
		glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &nTemp);
		PL_LOG(Info, String::Format("  Number of words for fragment shader uniform variables in default uniform block = %d", nTemp))
		glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS, &nTemp);
		PL_LOG(Info, String::Format("  Number of words for geometry shader uniform variables in default uniform block = %d", nTemp))
		glGetIntegerv(GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS, &nTemp);
		PL_LOG(Info, String::Format("  Number of words for vertex shader uniform variables in all uniform blocks (including default) = %d", nTemp))
		glGetIntegerv(GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS, &nTemp);
		PL_LOG(Info, String::Format("  Number of words for fragment shader uniform variables in all uniform blocks (including default) = %d", nTemp))
		glGetIntegerv(GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS, &nTemp);
		PL_LOG(Info, String::Format("  Number of words for geometry shader uniform variables in all uniform blocks (including default) = %d", nTemp))
		glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &nTemp);
		PL_LOG(Info, String::Format("  Minimum required alignment for uniform buffer sizes and offsets = %d", nTemp))
	}
}

/**
*  @brief
*    Shows OpenGL shader program information
*/
void Extensions::ShowOpenGLShaderProgramInformation(uint32 nTarget) const
{
	GLint i;

// Vertex and fragment
	glGetProgramivARB(nTarget, GL_MAX_PROGRAM_INSTRUCTIONS_ARB, &i);
	PL_LOG(Info, String::Format("  GL_MAX_PROGRAM_INSTRUCTIONS_ARB: %d", i))

	glGetProgramivARB(nTarget, GL_MAX_PROGRAM_NATIVE_INSTRUCTIONS_ARB, &i);
	PL_LOG(Info, String::Format("  GL_MAX_PROGRAM_NATIVE_INSTRUCTIONS_ARB: %d", i))

	glGetProgramivARB(nTarget, GL_MAX_PROGRAM_TEMPORARIES_ARB, &i);
	PL_LOG(Info, String::Format("  GL_MAX_PROGRAM_TEMPORARIES_ARB: %d", i))

	glGetProgramivARB(nTarget, GL_MAX_PROGRAM_NATIVE_TEMPORARIES_ARB, &i);
	PL_LOG(Info, String::Format("  GL_MAX_PROGRAM_NATIVE_TEMPORARIES_ARB: %d", i))

	glGetProgramivARB(nTarget, GL_MAX_PROGRAM_PARAMETERS_ARB, &i);
	PL_LOG(Info, String::Format("  GL_MAX_PROGRAM_PARAMETERS_ARB: %d", i))

	glGetProgramivARB(nTarget, GL_MAX_PROGRAM_NATIVE_PARAMETERS_ARB, &i);
	PL_LOG(Info, String::Format("  GL_MAX_PROGRAM_NATIVE_PARAMETERS_ARB: %d", i))

	glGetProgramivARB(nTarget, GL_MAX_PROGRAM_ATTRIBS_ARB, &i);
	PL_LOG(Info, String::Format("  GL_MAX_PROGRAM_ATTRIBS_ARB: %d", i))

	glGetProgramivARB(nTarget, GL_MAX_PROGRAM_NATIVE_ATTRIBS_ARB, &i);
	PL_LOG(Info, String::Format("  GL_MAX_PROGRAM_NATIVE_ATTRIBS_ARB: %d", i))

	glGetProgramivARB(nTarget, GL_MAX_PROGRAM_ADDRESS_REGISTERS_ARB, &i);
	PL_LOG(Info, String::Format("  GL_MAX_PROGRAM_ADDRESS_REGISTERS_ARB: %d", i))

	glGetProgramivARB(nTarget, GL_MAX_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB, &i);
	PL_LOG(Info, String::Format("  GL_MAX_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB: %d", i))

	glGetProgramivARB(nTarget, GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB, &i);
	PL_LOG(Info, String::Format("  GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB: %d", i))

	glGetProgramivARB(nTarget, GL_MAX_PROGRAM_ENV_PARAMETERS_ARB, &i);
	PL_LOG(Info, String::Format("  GL_MAX_PROGRAM_ENV_PARAMETERS_ARB: %d", i))

// Fragment only
	if (nTarget == GL_FRAGMENT_PROGRAM_ARB) {
		glGetProgramivARB(nTarget, GL_MAX_PROGRAM_ALU_INSTRUCTIONS_ARB, &i);
		PL_LOG(Info, String::Format("  GL_MAX_PROGRAM_ALU_INSTRUCTIONS_ARB: %d", i))

		glGetProgramivARB(nTarget, GL_MAX_PROGRAM_TEX_INSTRUCTIONS_ARB, &i);
		PL_LOG(Info, String::Format("  GL_MAX_PROGRAM_TEX_INSTRUCTIONS_ARB: %d", i))

		glGetProgramivARB(nTarget, GL_MAX_PROGRAM_TEX_INDIRECTIONS_ARB, &i);
		PL_LOG(Info, String::Format("  GL_MAX_PROGRAM_TEX_INDIRECTIONS_ARB: %d", i))

		glGetProgramivARB(nTarget, GL_MAX_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB, &i);
		PL_LOG(Info, String::Format("  GL_MAX_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB: %d", i))

		glGetProgramivARB(nTarget, GL_MAX_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB, &i);
		PL_LOG(Info, String::Format("  GL_MAX_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB: %d", i))

		glGetProgramivARB(nTarget, GL_MAX_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB, &i);
		PL_LOG(Info, String::Format("  GL_MAX_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB: %d", i))
	}
}

/**
*  @brief
*    Writes the extensions within a given extension string into the log
*/
void Extensions::WriteExtensionStringIntoLog(const char *pszExtensions) const
{
	// Check whether or not the given extensions string pointer is valid
	if (pszExtensions) {
		// Write the extensions into the log
		const char *pszSpace = pszExtensions;
		while (*pszSpace != '\0') {
			while (*pszSpace != ' ' && *pszSpace != '\0')
				pszSpace++;
			if (pszSpace-pszExtensions) {
				String sExtension;
				sExtension.Copy(pszExtensions, static_cast<uint32>(pszSpace-pszExtensions));
				PL_LOG(Info, sExtension)
			}
			if (*pszSpace != '\0') {
				pszSpace++; // Skip the space
				pszExtensions = pszSpace;
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL
