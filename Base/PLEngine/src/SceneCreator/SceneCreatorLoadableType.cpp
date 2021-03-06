/*********************************************************\
 *  File: SceneCreatorLoadableType.cpp                   *
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
#include <PLCore/Base/Class.h>
#include "PLEngine/SceneCreator/SceneCreatorLoadableType.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLScene;
namespace PLEngine {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SceneCreatorLoadableType, "PLEngine", PLEngine::SceneCreator, "Abstract loadable type scene creator base class")
	// Properties
	pl_properties
		pl_property("Type",	"Unknown")
	pl_properties_end
	// Attributes
	pl_attribute_metadata(Filename,	PLCore::String,	"",	ReadWrite,	"Resource filename",	"")
pl_class_metadata_end(SceneCreatorLoadableType)


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Configures a scene by using a given loadable type in order to determine the scene creator to use
*/
SNCamera *SceneCreatorLoadableType::ConfigureSceneByLoadableType(SceneContainer &cSceneContainer, const String &sType, const String &sFilename, const String &sParameters)
{
	// Get a list of RTTI classes derived from the loadable type scene creator
	List<const Class*> lstClasses;
	ClassManager::GetInstance()->GetClasses(lstClasses, "PLEngine::SceneCreatorLoadableType", Recursive, NoBase, NoAbstract);

	// Iterate over the received class list
	ConstIterator<const Class*> cIterator = lstClasses.GetConstIterator();
	while (cIterator.HasNext()) {
		// Get the current RTTI class
		const Class *pClass = cIterator.Next();

		// Check loadable type
		if (pClass->GetProperties().Get("Type") == sType) {
			// We've found a match - Configure a scene and return the preferred camera scene node
			return ConfigureScene(cSceneContainer, pClass->GetClassName(), "Filename=\"" + sFilename + "\" " + sParameters);
		}
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Returns a list of loadable types were it's possible to use a scene creator in order to configure a scene
*/
void SceneCreatorLoadableType::GetLoadableTypes(Array<String> &lstLoadableTypes)
{
	// Get a list of RTTI classes derived from the loadable type scene creator
	List<const Class*> lstClasses;
	ClassManager::GetInstance()->GetClasses(lstClasses, "PLEngine::SceneCreatorLoadableType", Recursive, NoBase, NoAbstract);

	// Iterate over the received class list
	HashMap<String, bool> mapTypes;
	ConstIterator<const Class*> cIterator = lstClasses.GetConstIterator();
	while (cIterator.HasNext()) {
		// Get the current RTTI class
		const Class *pClass = cIterator.Next();

		// Get loadable type
		const String &sType = pClass->GetProperties().Get("Type");

		// Add the loadable type to the given list... but only if the type string is not empty or was already added
		if (sType.GetLength() && !mapTypes.Get(sType)) {
			lstLoadableTypes.Add(sType);
			mapTypes.Add(sType, true);
		}
	}
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SceneCreatorLoadableType::SceneCreatorLoadableType() :
	Filename(this)
{
}

/**
*  @brief
*    Destructor
*/
SceneCreatorLoadableType::~SceneCreatorLoadableType()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine
