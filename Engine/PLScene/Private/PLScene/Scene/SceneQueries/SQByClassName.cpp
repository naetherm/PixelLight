/*********************************************************\
 *  File: SQByClassName.cpp                              *
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
#include "PLScene/Scene/SceneContainer.h"
#include "PLScene/Scene/SceneQueries/SQByClassName.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SQByClassName)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SQByClassName::SQByClassName()
{
}

/**
*  @brief
*    Destructor
*/
SQByClassName::~SQByClassName()
{
}

/**
*  @brief
*    Returns the used regular expression
*/
const RegEx &SQByClassName::GetRegEx() const
{
	return m_cRegEx;
}

/**
*  @brief
*    Sets the used regular expression
*/
void SQByClassName::SetRegEx(const RegEx &cRegEx)
{
	m_cRegEx = cRegEx;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Recursive part of PerformQuery()
*/
bool SQByClassName::PerformQueryRec(const SceneContainer &cContainer)
{
	// Loop through all scene nodes
	for (uint32 i=0; i<cContainer.GetNumOfElements(); i++) {
		// Get the scene node
		SceneNode *pSceneNode = cContainer.GetByIndex(i);

		// Does the class name of this scene node match the given regular expression?
		if (m_cRegEx.Match(pSceneNode->GetClass()->GetClassName())) {
			// Emit signal
			SignalSceneNode(*this, *pSceneNode);
			if (m_nFlags & StopQuery)
				return false; // Stop the query right now
		} else {
			// Take base classes into account, too?
			if (!(GetFlags() & IgnoreBaseClasses)) {
				// Get the base class
				const Class *pClass = pSceneNode->GetClass()->GetBaseClass();
				while (pClass) {
					// Does the name of this class match the given regular expression?
					if (m_cRegEx.Match(pClass->GetClassName())) {
						// Emit signal
						SignalSceneNode(*this, *pSceneNode);
						if (m_nFlags & StopQuery)
							return false; // Stop the query right now

						// We're done
						pClass = nullptr;
					} else {
						// Next base class, please
						pClass = pClass->GetBaseClass();
					}
				}
			}
		}

		// Is this a container and is recursion allowed?
		if ((m_nFlags & Recursive) && pSceneNode->IsContainer() && !(pSceneNode->GetFlags() & SceneContainer::NoRecursion)) {
			// Container recursion
			if (!PerformQueryRec(static_cast<SceneContainer&>(*pSceneNode)))
				return false; // Stop the query right now
		}
	}

	// Done, continue the query
	return true;
}


//[-------------------------------------------------------]
//[ Public virtual SceneQuery functions                   ]
//[-------------------------------------------------------]
bool SQByClassName::PerformQuery()
{
	// Perform the query recursive...
	m_nFlags &= ~StopQuery;
	return PerformQueryRec(GetSceneContainer());
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
