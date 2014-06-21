/*********************************************************\
 *  File: PostProcessLoaderPL.cpp                        *
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
#include <PLCore/Xml/Xml.h>
#include <PLCore/Log/Log.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Material/MaterialManager.h>
#include "PLCompositing/Shaders/PostProcessing/PostProcessManager.h"
#include "PLCompositing/Shaders/PostProcessing/PostProcessLoaderPL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
namespace PLCompositing {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(PostProcessLoaderPL)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool PostProcessLoaderPL::Load(PostProcessManager &cPostProcessManager, File &cFile)
{
	// Load XML document
	XmlDocument cDocument;
	if (cDocument.Load(cFile)) {
		// Get post process element
		const XmlElement *pPostProcessElement = cDocument.GetFirstChildElement("PostProcess");
		if (pPostProcessElement) {
			// Get the format version
			const int nVersion = pPostProcessElement->GetAttribute("Version").GetInt();

			// Unknown
			if (nVersion > 1) {
				PL_LOG(Error, cDocument.GetValue() + ": " + UnknownFormatVersion)

			// 1 (current)
			} else if (nVersion == 1) {
				return LoadV1(cPostProcessManager, *pPostProcessElement);

			// ""/0 (same format as 1)
			} else if (nVersion == 0) {
				// [DEPRECATED]
				PL_LOG(Warning, cDocument.GetValue() + ": " + DeprecatedFormatVersion)
				return LoadV1(cPostProcessManager, *pPostProcessElement);

			// No longer supported format version
			} else if (nVersion >= 0) {
				PL_LOG(Warning, cDocument.GetValue() + ": " + NoLongerSupportedFormatVersion)

			// Invalid format version (negative!)
			} else {
				PL_LOG(Error, cDocument.GetValue() + ": " + InvalidFormatVersion)
			}
		} else {
			PL_LOG(Error, "Can't find 'PostProcess' element")
		}
	} else {
		PL_LOG(Error, cDocument.GetValue() + ": " + cDocument.GetErrorDesc())
	}

	// Error!
	return false;
}

bool PostProcessLoaderPL::Save(PostProcessManager &cPostProcessManager, File &cFile)
{
	// Create XML document
	XmlDocument cDocument;

	// Add declaration
	XmlDeclaration *pDeclaration = new XmlDeclaration("1.0", "", "");
	cDocument.LinkEndChild(*pDeclaration);

	// Add post process
	XmlElement *pPostProcessElement = new XmlElement("PostProcess");
	pPostProcessElement->SetAttribute("Version", "1");

	// Add general information if required
	if (cPostProcessManager.GetTextureFormat() != TextureBuffer::R8G8B8A8) {
		XmlElement *pGeneralElement = new XmlElement("General");
		switch (cPostProcessManager.GetTextureFormat()) {
			case TextureBuffer::R8G8B8:
				pGeneralElement->SetAttribute("TextureFormat", "R8G8B8");
				break;

			case TextureBuffer::R16G16B16A16F:
				pGeneralElement->SetAttribute("TextureFormat", "R16G16B16A16F");
				break;

			case TextureBuffer::R32G32B32A32F:
				pGeneralElement->SetAttribute("TextureFormat", "R32G32B32A32F");
				break;

			case TextureBuffer::L8:
			case TextureBuffer::L16:
			case TextureBuffer::A8:
			case TextureBuffer::L4A4:
			case TextureBuffer::L8A8:
			case TextureBuffer::D16:
			case TextureBuffer::D24:
			case TextureBuffer::D32:
			case TextureBuffer::R3G3B2:
			case TextureBuffer::R5G6B5:
			case TextureBuffer::R5G5B5A1:
			case TextureBuffer::R4G4B4A4:
			case TextureBuffer::R8G8B8A8:
			case TextureBuffer::R10G10B10A2:
			case TextureBuffer::R16G16B16A16:
			case TextureBuffer::DXT1:
			case TextureBuffer::DXT3:
			case TextureBuffer::DXT5:
			case TextureBuffer::LATC1:
			case TextureBuffer::LATC2:
			case TextureBuffer::L16F:
			case TextureBuffer::L32F:
			case TextureBuffer::Unknown:
			default:
				// Error, not supported!
				break;
		}
		pPostProcessElement->LinkEndChild(*pGeneralElement);
	}

	// Add all post processes
	for (uint32 i=0; i<cPostProcessManager.GetNumOfElements(); i++) {
		const PostProcess *pPP = cPostProcessManager.GetByIndex(i);
		if (pPP) {
			XmlElement *pPassElement = new XmlElement("Pass");

			// Add material
			const Material *pMaterial = pPP->GetMaterial();
			if (pMaterial)
				pPassElement->SetAttribute("Material", pMaterial->GetName());

			// Link to pass element
			pPostProcessElement->LinkEndChild(*pPassElement);
		}
	}

	// Link post process element
	cDocument.LinkEndChild(*pPostProcessElement);
	
	// Save settings
	cDocument.Save(cFile);

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
PostProcessLoaderPL::PostProcessLoaderPL()
{
}

/**
*  @brief
*    Destructor
*/
PostProcessLoaderPL::~PostProcessLoaderPL()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Loader implementation for format version 1
*/
bool PostProcessLoaderPL::LoadV1(PostProcessManager &cPostProcessManager, const XmlElement &cPostProcessElement) const
{
	// Are there general settings?
	const XmlElement *pGeneralElement = cPostProcessElement.GetFirstChildElement("General");
	if (pGeneralElement) {
		// Texture buffer format
		TextureBuffer::EPixelFormat nTextureFormat = TextureBuffer::R8G8B8A8;
		String sTextureFormat = pGeneralElement->GetAttribute("TextureFormat");
		if (sTextureFormat.GetLength()) {
			if (sTextureFormat == "R8G8B8")
				nTextureFormat = TextureBuffer::R8G8B8;
			else if (sTextureFormat == "R8G8B8A8")
				nTextureFormat = TextureBuffer::R8G8B8A8;
			else if (sTextureFormat == "R16G16B16A16F")
				nTextureFormat = TextureBuffer::R16G16B16A16F;
			else if (sTextureFormat == "R32G32B32A32F")
				nTextureFormat = TextureBuffer::R32G32B32A32F;
		}
		cPostProcessManager.SetTextureFormat(nTextureFormat);
	}

	// Iterate through all passes
	const XmlElement *pPassElement = cPostProcessElement.GetFirstChildElement("Pass");
	while (pPassElement) {
		// Get optional material
		String sMaterial = pPassElement->GetAttribute("Material");

		// Create the post process
		PostProcess *pPP = new PostProcess("", cPostProcessManager);
		if (pPP) {
			pPP->SetMaterial(cPostProcessManager.GetRendererContext().GetMaterialManager().LoadResource(sMaterial));
			cPostProcessManager.Add(*pPP);
		}

		// Next element, please
		pPassElement = pPassElement->GetNextSiblingElement("Pass");
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
