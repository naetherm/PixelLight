/*********************************************************\
 *  File: TerrainLoaderPL.cpp                            *
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
#include <PLCore/Xml/Xml.h>
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Material/MaterialHandler.h>
#include <PLRenderer/Material/MaterialManager.h>
#include <PLScene/Scene/SceneContext.h>
#include "PLNature/Terrain/GMMSurface.h"
#include "PLNature/Terrain/SNTerrain.h"
#include "PLNature/Terrain/TerrainLoaderPL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
namespace PLNature {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(TerrainLoaderPL)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool TerrainLoaderPL::Load(SNTerrain &cSNTerrain, File &cFile)
{
	// Load XML document
	XmlDocument cDocument;
	if (cDocument.Load(cFile)) {
		// Get terrain element
		const XmlElement *pTerrainElement = cDocument.GetFirstChildElement("Terrain");
		if (pTerrainElement) {
			// Get the format version
			int nVersion = pTerrainElement->GetAttribute("Version").GetInt();

			// Unknown
			if (nVersion > 1) {
				PL_LOG(Error, cDocument.GetValue() + ": " + UnknownFormatVersion)

			// 1 (current)
			} else if (nVersion == 1) {
				return LoadV1(cSNTerrain, *pTerrainElement);

			// ""/0 (same format as 1)
			} else if (nVersion == 0) {
				// [DEPRECATED]
				PL_LOG(Warning, cDocument.GetValue() + ": " + DeprecatedFormatVersion)
				return LoadV1(cSNTerrain, *pTerrainElement);

			// No longer supported format version
			} else if (nVersion >= 0) {
				PL_LOG(Warning, cDocument.GetValue() + ": " + NoLongerSupportedFormatVersion)

			// Invalid format version (negative!)
			} else {
				PL_LOG(Error, cDocument.GetValue() + ": " + InvalidFormatVersion)
			}
		} else PL_LOG(Error, "Can't find 'Terrain' element")
	} else PL_LOG(Error, cDocument.GetValue() + ": " + cDocument.GetErrorDesc())

	// Error!
	return false;
}

bool TerrainLoaderPL::Save(SNTerrain &cSNTerrain, File &cFile)
{
	// [TODO] Implement me!
	return false; // Error!
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
TerrainLoaderPL::TerrainLoaderPL()
{
}

/**
*  @brief
*    Destructor
*/
TerrainLoaderPL::~TerrainLoaderPL()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Loader implementation for format version 1
*/
bool TerrainLoaderPL::LoadV1(SNTerrain &cSNTerrain, const XmlElement &cTerrainElement) const
{
	// HeightMap
	String sHeightMap;
	const XmlNode *pNode = cTerrainElement.GetFirstChild("HeightMap");
	if (pNode) {
		pNode = pNode->GetFirstChild();
		if (pNode && pNode->GetType() == XmlNode::Text && pNode->GetValue().GetLength())
			sHeightMap = pNode->GetValue();
	}

	// Material
	String sMaterial;
	pNode = cTerrainElement.GetFirstChild("Material");
	if (pNode) {
		pNode = pNode->GetFirstChild();
		if (pNode && pNode->GetType() == XmlNode::Text && pNode->GetValue().GetLength())
			sMaterial = pNode->GetValue();
	}

	// Scale
	Vector3 vScale(1.0f, 1.0f, 1.0f);
	pNode = cTerrainElement.GetFirstChild("Scale");
	if (pNode) {
		pNode = pNode->GetFirstChild();
		if (pNode && pNode->GetType() == XmlNode::Text && pNode->GetValue().GetLength())
			vScale.FromString(pNode->GetValue());
	}

	// Load the height map and create the terrain
	Image cHeightMap;
	if (cSNTerrain.LoadHeightMap(sHeightMap, cHeightMap)) {
		// Destroy the old height field
		if (cSNTerrain.m_pfHeightMap) {
			delete [] cSNTerrain.m_pfHeightMap;
			cSNTerrain.m_pfHeightMap = nullptr;
		}

		// Get the image buffer
		ImageBuffer *pImageBuffer = cHeightMap.GetBuffer();
		if (pImageBuffer) {
			// Create the height field using the read image
			uint32 nWidth  = pImageBuffer->GetSize().x;
			uint32 nHeight = pImageBuffer->GetSize().y;
			uint32 nSize   = nWidth*nHeight;
			cSNTerrain.m_pfHeightMap = new float[nSize];
			const uint8 *pData = pImageBuffer->GetData();
			float *pHeighField = cSNTerrain.m_pfHeightMap;
			float fYScale      = vScale.y;

			// Initialize min/max
			*pHeighField = (*pData)*fYScale;
			float fYMin = *pHeighField;
			float fYMax = fYMin;
			pData++;
			pHeighField++;

			// Loop through the data
			for (uint32 i=1; i<nSize; i++) {
				// Set height field data
				*pHeighField = static_cast<float>((*pData))*fYScale;

				// Check min/max
				if (fYMin > *pHeighField)
					fYMin = *pHeighField;
				if (fYMax < *pHeighField)
					fYMax = *pHeighField;

				// Next, please
				pData++;
				pHeighField++;
			}

			// Set general standard values
			uint32 nPatchSize = 32;
			uint32 nLevels     = 6;

			// Create the GMM surface
			cSNTerrain.m_pGMMSurface->Create(nWidth, cSNTerrain.m_pfHeightMap, nPatchSize, nLevels);
			cSNTerrain.m_pGMMSurface->GetMaterialHandler().SetResource(cSNTerrain.GetSceneContext()->GetRendererContext().GetMaterialManager().LoadResource(sMaterial));

			// Setup the bounding box
			cSNTerrain.SetAABoundingBox(AABoundingBox(0.0f, fYMin, 0.0f, static_cast<float>(nWidth), fYMax, static_cast<float>(nHeight)));
		}
	} else {
		// Setup the bounding box
		cSNTerrain.SetAABoundingBox(AABoundingBox(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f));
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLNature
