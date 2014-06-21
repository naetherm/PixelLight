/*********************************************************\
 *  File: SkyLoaderPL.cpp                                *
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
#include <PLRenderer/Material/Material.h>
#include <PLMesh/MeshHandler.h>
#include "PLNature/Sky/SNSky.h"
#include "PLNature/Sky/SkyLoaderPL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLNature {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SkyLoaderPL)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool SkyLoaderPL::Load(SNSky &cSky, File &cFile)
{
	// Load XML document
	XmlDocument cDocument;
	if (cDocument.Load(cFile)) {
		// Get sky element
		const XmlElement *pSkyElement = cDocument.GetFirstChildElement("Sky");
		if (pSkyElement) {
			// Get the format version
			const int nVersion = pSkyElement->GetAttribute("Version").GetInt();

			// Unknown
			if (nVersion > 1) {
				PL_LOG(Error, cDocument.GetValue() + ": " + UnknownFormatVersion)

			// 1 (current)
			} else if (nVersion == 1) {
				return LoadV1(cSky, *pSkyElement);

			// ""/0 (same format as 1)
			} else if (nVersion == 0) {
				// [DEPRECATED]
				PL_LOG(Warning, cDocument.GetValue() + ": " + DeprecatedFormatVersion)
				return LoadV1(cSky, *pSkyElement);

			// No longer supported format version
			} else if (nVersion >= 0) {
				PL_LOG(Warning, cDocument.GetValue() + ": " + NoLongerSupportedFormatVersion)

			// Invalid format version (negative!)
			} else {
				PL_LOG(Error, cDocument.GetValue() + ": " + InvalidFormatVersion)
			}
		} else {
			PL_LOG(Error, "Can't find 'Sky' element")
		}
	} else {
		PL_LOG(Error, cDocument.GetValue() + ": " + cDocument.GetErrorDesc())
	}

	// Error!
	return false;
}

bool SkyLoaderPL::Save(SNSky &cSky, File &cFile)
{
	// Create XML document
	XmlDocument cDocument;

	// Add declaration
	XmlDeclaration *pDeclaration = new XmlDeclaration("1.0", "", "");
	cDocument.LinkEndChild(*pDeclaration);

	// Add sky
	XmlElement *pSkyElement = new XmlElement("Sky");
	pSkyElement->SetAttribute("Version", "1");

	// Loop through all layers
	for (uint32 nLayer=0; nLayer<cSky.GetNumOfElements(); nLayer++) {
		// Get the layer
		const SkyLayer *pSkyLayer = cSky.GetByIndex(nLayer);
		if (pSkyLayer) {
			// Create the layer element
			XmlElement *pLayerElement = new XmlElement("Layer");

			// Populate the XML parameters element
			pSkyLayer->GetValuesXml(*pLayerElement);

			// Loop through all materials of the layer
			const PLMesh::MeshHandler *pMeshHandler = pSkyLayer->GetMeshHandler();
			if (pMeshHandler) {
				for (uint32 nMaterial=0; nMaterial<pMeshHandler->GetNumOfMaterials(); nMaterial++) {
					// Get the material
					const PLRenderer::Material *pMaterial = pMeshHandler->GetMaterial(nMaterial);
					if (pMaterial) {
						// Create the material element
						XmlElement *pMaterialElement = new XmlElement("Material");

						// Set the attributes
						pMaterialElement->SetAttribute("ID",   String::Format("%d", nMaterial));
						pMaterialElement->SetAttribute("Name", pMaterial->GetName());

						// Link material element
						pLayerElement->LinkEndChild(*pMaterialElement);
					}
				}
			}

			// Link layer element
			pSkyElement->LinkEndChild(*pLayerElement);
		}
	}

	// Link sky element
	cDocument.LinkEndChild(*pSkyElement);

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
SkyLoaderPL::SkyLoaderPL()
{
}

/**
*  @brief
*    Destructor
*/
SkyLoaderPL::~SkyLoaderPL()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Loader implementation for format version 1
*/
bool SkyLoaderPL::LoadV1(SNSky &cSky, const XmlElement &cSkyElement) const
{
	// Iterate through all layers
	const XmlElement *pLayerElement = cSkyElement.GetFirstChildElement("Layer");
	while (pLayerElement) {
		// Get optional sky layer name
		const String sLayerName = pLayerElement->GetAttribute("Name");

		// Create the sky layer
		SkyLayer *pSkyLayer = cSky.Create(sLayerName);
		if (pSkyLayer) {
			// Set variables
			pSkyLayer->SetValuesXml(*pLayerElement);

			// Iterate through all materials
			const XmlElement *pMaterialElement = pLayerElement->GetFirstChildElement("Material");
			while (pMaterialElement) {
				// Get optional material ID
				const int nID = pMaterialElement->GetAttribute("ID").GetInt();

				// Get and load material
				const String sMaterialName = pMaterialElement->GetAttribute("Name");
				if (sMaterialName.GetLength())
					pSkyLayer->LoadMaterial(sMaterialName, nID);

				// Next element, please
				pMaterialElement = pMaterialElement->GetNextSiblingElement("Material");
			}
		}

		// Next element, please
		pLayerElement = pLayerElement->GetNextSiblingElement("Layer");
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLNature
