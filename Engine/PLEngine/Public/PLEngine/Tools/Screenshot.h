/*********************************************************\
 *  File: Screenshot.h                                   *
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


#ifndef __PLENGINE_TOOLS_SCREENSHOT_H__
#define __PLENGINE_TOOLS_SCREENSHOT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Object.h>
#include "PLEngine/PLEngine.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class RendererContext;
	class SurfacePainter;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLEngine {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Class offering screenshot functionality
*/
class Screenshot : public PLCore::Object {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PL_RTTI_EXPORT, Screenshot, "PLEngine", PLCore::Object, "Class offering screenshot functionality")
		#ifdef PLENGINE_EXPORTS	// The following is only required when compiling PLEngine
			// Methods
			pl_method_0(GetScreenshotDirectory,	pl_ret_type(PLCore::String),							"Get screenshot directory in which the screenshots are saved",																																																															"")
			pl_method_1(SetScreenshotDirectory,	pl_ret_type(void),				const PLCore::String&,	"Set screenshot directory, directory in which the screenshots are saved as first parameter (if set to \"\", the current directory will be used)",																																										"")
			pl_method_1(SaveScreenshot,			pl_ret_type(bool),				const PLCore::String&,	"Save screenshot from current render target, screenshot filename (e.g. \"Screenshot.png\") as first parameter (if string is empty, GetScreenshotFilename() will be used). Returns 'true' if all went fine, else 'false'. Uses the dimension of the current render target.",												"")
			pl_method_1(GetScreenshotFilename,	pl_ret_type(PLCore::String),	const PLCore::String&,	"Get a screenshot filename recommendation, file name extension (for example \"png\") as first parameter. Returns the recommended screenshot filename, empty string on error. The screenshot filename is automatically generated: \"/_Screenshots/Screenshot_0.png\", \"/_Screenshots/Screenshot_1.png\" and so on...",	"")
		#endif
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pRendererContext
		*    Renderer context from which to create screenshots, can be a null pointer
		*  @param[in] pPainter
		*    Surface painter, can be a null pointer (only needed for extended screenshot functionality)
		*/
		PL_API Screenshot(PLRenderer::RendererContext *pRendererContext = nullptr, PLRenderer::SurfacePainter *pPainter = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PL_API virtual ~Screenshot();

		/**
		*  @brief
		*    Get renderer context
		*
		*  @return
		*    Renderer context from which to create screenshots, can be a null pointer
		*/
		PL_API PLRenderer::RendererContext *GetRendererContext() const;

		/**
		*  @brief
		*    Set renderer context
		*
		*  @param[in] pRendererContext
		*    Renderer context from which to create screenshots, can be a null pointer
		*/
		PL_API void SetRendererContext(PLRenderer::RendererContext *pRendererContext);

		/**
		*  @brief
		*    Get surface painter
		*
		*  @return
		*    Surface painter used to create screenshots, can be a null pointer
		*/
		PL_API PLRenderer::SurfacePainter *GetPainter() const;

		/**
		*  @brief
		*    Set surface painter
		*
		*  @param[in] pSurfacePainter
		*    Surface painter used to create screenshots, can be a null pointer
		*/
		PL_API void SetPainter(PLRenderer::SurfacePainter *pSurfacePainter);

		/**
		*  @brief
		*    Get screenshot directory
		*
		*  @return
		*    Directory in which the screenshots are saved
		*/
		PL_API PLCore::String GetScreenshotDirectory() const;

		/**
		*  @brief
		*    Set screenshot directory
		*
		*  @param[in] sPath
		*    Directory in which the screenshots are saved
		*
		*  @note
		*    - If set to "", the current directory will be used
		*/
		PL_API void SetScreenshotDirectory(const PLCore::String &sPath);

		/**
		*  @brief
		*    Save screenshot from current render target
		*
		*  @param[in] sFilename
		*    Screenshot filename (e.g. "Screenshot.png"), if string is empty, GetScreenshotFilename() will be used
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - Uses the dimension of the current render target
		*/
		PL_API bool SaveScreenshot(const PLCore::String &sFilename = "") const;

		/**
		*  @brief
		*    Save screenshot by using the surface painter of the current render target surface
		*
		*  @param[in] sFilename
		*    Screenshot filename (e.g. "Screenshot.png"), if string is empty, GetScreenshotFilename() will be used
		*  @param[in] nWidth
		*    Width of the screenshot (1..<hardware limitation>)
		*  @param[in] nHeight
		*    Height of screenshot (1..<hardware limitation>)
		*  @param[in] nFormat
		*    Texture buffer pixel format for creating the screenshot (example: 'TextureBuffer::R8G8B8')
		*  @param[in] nFlags
		*    Texture buffer surface flags (see PLRenderer::SurfaceTextureBuffer::EFlags) for creating the screenshot
		*    (example: 'SurfaceTextureBuffer::Depth')
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @remarks
		*    Unlike the simple 'SaveScreenshot()'-function from above, this function is rendering again by
		*    using an own render target allowing a more or less 'free choice' of the screenshot dimension.
		*    This dimension is only limited by the 'render to texture' capabilities of the hardware - most
		*    'current' hardware supports dimensions up to 8192x8192. Use the renderer capabilities to figure
		*    out the maximum supported rectangle texture dimension.
		*/
		PL_API bool SaveCustomScreenshot(const PLCore::String &sFilename, PLCore::uint16 nWidth, PLCore::uint16 nHeight,
										 PLRenderer::TextureBuffer::EPixelFormat nFormat, PLCore::uint32 nFlags) const;

		/**
		*  @brief
		*    Get a screenshot filename recommendation
		*
		*  @param[in] sExtension
		*    File name extension (for example "png")
		*
		*  @remarks
		*    The screenshot filename is automatically generated:
		*    "/_Screenshots/Screenshot_0.png", "/_Screenshots/Screenshot_1.png" and so on...
		*
		*  @return
		*    The recommended screenshot filename, empty string on error
		*/
		PL_API PLCore::String GetScreenshotFilename(const PLCore::String &sExtension = "png") const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLRenderer::RendererContext *m_pRendererContext;		/**< Used renderer context, can be a null pointer */
		PLRenderer::SurfacePainter	*m_pSurfacePainter;			/**< Used surface painter, can be a null pointer */
		PLCore::String				 m_sScreenshotDirectory;	/**< Screenshot directory */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine


#endif // __PLENGINE_TOOLS_SCREENSHOT_H__
