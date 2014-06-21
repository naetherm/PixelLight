/*********************************************************\
 *  File: SceneRendererLoaderPL.cpp                      *
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
#include <PLCore/Base/Class.h>
#include "PLScene/Compositing/SceneRenderer.h"
#include "PLScene/Compositing/SceneRendererLoaderPL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SceneRendererLoaderPL)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool SceneRendererLoaderPL::Load(SceneRenderer &cSceneRenderer, File &cFile)
{
	// Load XML document
	XmlDocument cDocument;
	if (cDocument.Load(cFile)) {
		// Get scene renderer element
		const XmlElement *pSceneRendererElement = cDocument.GetFirstChildElement("SceneRenderer");
		if (pSceneRendererElement) {
			// Get the format version
			const int nVersion = pSceneRendererElement->GetAttribute("Version").GetInt();

			// Unknown
			if (nVersion > 1) {
				PL_LOG(Error, cDocument.GetValue() + ": " + UnknownFormatVersion)

			// 1 (current)
			} else if (nVersion == 1) {
				return LoadV1(cSceneRenderer, *pSceneRendererElement);

			// ""/0 (same format as 1)
			} else if (nVersion == 0) {
				// [DEPRECATED]
				PL_LOG(Warning, cDocument.GetValue() + ": " + DeprecatedFormatVersion)
				return LoadV1(cSceneRenderer, *pSceneRendererElement);

			// No longer supported format version
			} else if (nVersion >= 0) {
				PL_LOG(Warning, cDocument.GetValue() + ": " + NoLongerSupportedFormatVersion)

			// Invalid format version (negative!)
			} else {
				PL_LOG(Error, cDocument.GetValue() + ": " + InvalidFormatVersion)
			}
		} else {
			PL_LOG(Error, "Can't find 'SceneRenderer' element")
		}
	} else {
		PL_LOG(Error, cDocument.GetValue() + ": " + cDocument.GetErrorDesc())
	}

	// Error!
	return false;
}

bool SceneRendererLoaderPL::Save(SceneRenderer &cSceneRenderer, File &cFile)
{
	return SaveParams(cSceneRenderer, cFile, true);
}

bool SceneRendererLoaderPL::SaveParams(SceneRenderer &cSceneRenderer, File &cFile, bool bNoDefault)
{
	// Create XML document
	XmlDocument cDocument;

	// Add declaration
	XmlDeclaration *pDeclaration = new XmlDeclaration("1.0", "", "");
	cDocument.LinkEndChild(*pDeclaration);

	// Add scene renderer pass
	XmlElement *pSceneRendererElement = new XmlElement("SceneRenderer");
	pSceneRendererElement->SetAttribute("Version", "1");

	// Add all scene renderer passes
	for (uint32 i=0; i<cSceneRenderer.GetNumOfElements(); i++) {
		const SceneRendererPass *pSceneRendererPass = cSceneRenderer.GetByIndex(i);
		if (pSceneRendererPass) {
			XmlElement *pPassElement = new XmlElement("Pass");

			// Add class and parameters
			pPassElement->SetAttribute("Class", pSceneRendererPass->GetClass()->GetClassName());
			pSceneRendererPass->GetValuesXml(*pPassElement, bNoDefault ? NoDefault : WithDefault);

			// Link to pass element
			pSceneRendererElement->LinkEndChild(*pPassElement);
		}
	}

	// Link scene renderer element
	cDocument.LinkEndChild(*pSceneRendererElement);
	
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
SceneRendererLoaderPL::SceneRendererLoaderPL()
{
}

/**
*  @brief
*    Destructor
*/
SceneRendererLoaderPL::~SceneRendererLoaderPL()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Loader implementation for format version 1
*/
bool SceneRendererLoaderPL::LoadV1(SceneRenderer &cSceneRenderer, const XmlElement &cSceneRendererElement) const
{
	// Iterate through all passes
	const XmlElement *pPassElement = cSceneRendererElement.GetFirstChildElement("Pass");
	while (pPassElement) {
		// Get scene renderer pass class name
		const String sClass = pPassElement->GetAttribute("Class");
		if (sClass.GetLength()) {
			// Setup variables
			String sParameters, sName;
			const XmlAttribute *pAttribute = pPassElement->GetFirstAttribute();
			while (pAttribute) {
				// Is this the name?
				if (pAttribute->GetName() == "Name")
					sName = pAttribute->GetValue();

				// Add parameter
				sParameters += pAttribute->GetName();
				sParameters += "=\"";
				sParameters += pAttribute->GetValue();
				sParameters += "\" ";

				// Get the next attribute
				pAttribute = pAttribute->GetNext();
			}

			// Create scene renderer pass
			if (!cSceneRenderer.Create(sClass, sName, sParameters))
				PL_LOG(Error, pPassElement->GetValue() + ": Pass at row " + pPassElement->GetRow() + " column " + pPassElement->GetColumn() + " creation failed!")
		} else {
			PL_LOG(Error, pPassElement->GetValue() + ": Pass at row " + pPassElement->GetRow() + " column " + pPassElement->GetColumn() + " has no class name!")
		}

		// Next pass, please
		pPassElement = pPassElement->GetNextSiblingElement();
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
