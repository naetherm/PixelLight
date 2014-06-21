/*********************************************************\
 *  File: Screenshot.cpp                                 *
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
#include <PLCore/File/File.h>
#include <PLCore/File/Directory.h>
#include <PLCore/System/System.h>
#include <PLGraphics/Image/Image.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/SurfacePainter.h>
#include "PLEngine/Tools/Screenshot.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
namespace PLEngine {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(Screenshot)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Screenshot::Screenshot(RendererContext *pRendererContext, SurfacePainter *pSurfacePainter) :
	m_pRendererContext(pRendererContext),
	m_pSurfacePainter(pSurfacePainter)
{
}

/**
*  @brief
*    Destructor
*/
Screenshot::~Screenshot()
{
}

/**
*  @brief
*    Get renderer context
*/
RendererContext *Screenshot::GetRendererContext() const
{
	// Return renderer context
	return m_pRendererContext;
}

/**
*  @brief
*    Set renderer context
*/
void Screenshot::SetRendererContext(RendererContext *pRendererContext)
{
	// Set renderer context
	m_pRendererContext = pRendererContext;
}

/**
*  @brief
*    Get surface painter
*/
SurfacePainter *Screenshot::GetPainter() const
{
	// Return surface painter
	return m_pSurfacePainter;
}

/**
*  @brief
*    Set surface painter
*/
void Screenshot::SetPainter(SurfacePainter *pSurfacePainter)
{
	// Set surface painter
	m_pSurfacePainter = pSurfacePainter;
}

/**
*  @brief
*    Get screenshot directory
*/
String Screenshot::GetScreenshotDirectory() const
{
	// Return directory
	return m_sScreenshotDirectory;
}

/**
*  @brief
*    Set screenshot directory
*/
void Screenshot::SetScreenshotDirectory(const String &sPath)
{
	// Set directory
	m_sScreenshotDirectory = sPath;
}

/**
*  @brief
*    Save screenshot from current render target
*/
bool Screenshot::SaveScreenshot(const String &sFilename) const
{
	// Is there a renderer context?
	if (GetRendererContext()) {
		// Get screenshot
		Image cImage;
		GetRendererContext()->GetRenderer().MakeScreenshot(cImage);

		// Save screenshot
		return cImage.SaveByFilename((sFilename.GetLength() > 0 ? sFilename : GetScreenshotFilename()));
	} else {
		// Error!
		return false;
	}
}

/**
*  @brief
*    Save screenshot by using the surface painter of the current render target surface
*/
bool Screenshot::SaveCustomScreenshot(const String &sFilename, uint16 nWidth, uint16 nHeight, TextureBuffer::EPixelFormat nFormat, uint32 nFlags) const
{
	bool bResult = false; // Error by default

	// Check if width and height are valid and if there is a renderer context
	if (nWidth > 0 && nHeight > 0 && GetRendererContext()) {
		// Get renderer
		Renderer &cRenderer = GetRendererContext()->GetRenderer();

		// Backup current render target
		Surface *pRenderTargetBackup = cRenderer.GetRenderTarget();

		// Get painter of current render target
		if (m_pSurfacePainter) {
			// Create render target
			SurfaceTextureBuffer *pRenderTarget = cRenderer.CreateSurfaceTextureBufferRectangle(Vector2i(nWidth, nHeight), nFormat, nFlags);
			if (pRenderTarget) {
				// Set surface painter of our new render target
				pRenderTarget->SetPainter(m_pSurfacePainter);

				// Draw using the set surface painter
				pRenderTarget->Draw();

				// Get screenshot
				Image cImage;
				cRenderer.MakeScreenshot(cImage);

				// Save screenshot
				bResult = cImage.SaveByFilename((sFilename.GetLength() > 0 ? sFilename : GetScreenshotFilename()));

				// Restore previous render target
				cRenderer.SetRenderTarget(pRenderTargetBackup);

				// Unset surface painter of our new render target - else the shared surface painter is destroyed
				pRenderTarget->SetPainter(nullptr, false);

				// Destroy the render target
				delete pRenderTarget;
			}
		}
	}

	// Done
	return bResult;
}

/**
*  @brief
*    Get a screenshot filename recommendation
*/
String Screenshot::GetScreenshotFilename(const String &sExtension) const
{
	// Open/create screenshots directory
	String sDirectory = m_sScreenshotDirectory;
	if (!sDirectory.GetLength())
		sDirectory = System::GetInstance()->GetCurrentDir();
	Directory cDirectory(sDirectory + "/_Screenshots");
	if (!cDirectory.Exists())
		cDirectory.Create();

	// Is this REALLY a directory?
	if (cDirectory.Exists() && cDirectory.IsDirectory()) {
		// Save screenshot
		uint32 nIndex = 0;
		do {
			// Get a screenshot name
			const String sFilename = String("Screenshot_") + nIndex + '.' + sExtension;

			// Is there already a file with this name?
			File cFile(sDirectory + "/_Screenshots/" + sFilename);
			if (cFile.Exists() && cFile.IsFile()) {
				// Jep, we have to find another filename :(
				nIndex++;

			// Save screenshot
			} else {
				// Save
				return sDirectory + "/_Screenshots/" + sFilename;
			}
		} while (nIndex);
	}

	// Error!
	return "";
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine
