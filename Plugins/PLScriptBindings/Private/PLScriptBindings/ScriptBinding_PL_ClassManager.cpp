/*********************************************************\
 *  File: ScriptBinding_PL_ClassManager.cpp              *
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
#include <PLCore/Base/ClassManager.h>
#include "PLScriptBindings/ScriptBinding_PL_ClassManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLScriptBindings {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(ScriptBinding_PL_ClassManager)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool ScriptBinding_PL_ClassManager::ScanPlugins(String sPath, bool bRecursive, bool bDelayedPluginLoading)
{
	return m_pClassManager->ScanPlugins(sPath, bRecursive ? Recursive : NonRecursive, bDelayedPluginLoading);
}

Object *ScriptBinding_PL_ClassManager::Create(String sClass, String sParameters)
{
	// Get the requested RTTI class
	const Class *pClass =  m_pClassManager->GetClass(sClass);
	if (pClass) {
		// Create an instance of the requested RTTI class
		Object *pObject = pClass->Create();
		if (pObject) {
			// By default, a created RTTI class instance has an initial reference count of 1
			// -> This RTTI class instance was possibly created by a script, so, set an initial reference
			//    count of 0 so that the instance is destroyed automatically as soon as it's no longer referenced
			pObject->SoftRelease();

			// Set optional parameters
			if (pObject && sParameters.GetLength())
				pObject->SetValues(sParameters);

			// Return the created RTTI class instance
			return pObject;
		}
	}

	// Error!
	return nullptr;
}

Object *ScriptBinding_PL_ClassManager::CreateByConstructor(String sClass, String sConstructor, String sConstructorParameters, String sParameters)
{
	// Get the requested RTTI class
	const Class *pClass =  m_pClassManager->GetClass(sClass);
	if (pClass) {
		// Create an instance of the requested RTTI class
		Object *pObject = pClass->Create(sConstructor, sConstructorParameters);
		if (pObject) {
			// By default, a created RTTI class instance has an initial reference count of 1
			// -> This RTTI class instance was possibly created by a script, so, set an initial reference
			//    count of 0 so that the instance is destroyed automatically as soon as it's no longer referenced
			pObject->SoftRelease();

			// Set optional parameters
			if (pObject && sParameters.GetLength())
				pObject->SetValues(sParameters);

			// Return the created RTTI class instance
			return pObject;
		}
	}

	// Error!
	return nullptr;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ScriptBinding_PL_ClassManager::ScriptBinding_PL_ClassManager() :
	m_pClassManager(ClassManager::GetInstance())
{
}

/**
*  @brief
*    Destructor
*/
ScriptBinding_PL_ClassManager::~ScriptBinding_PL_ClassManager()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScriptBindings
