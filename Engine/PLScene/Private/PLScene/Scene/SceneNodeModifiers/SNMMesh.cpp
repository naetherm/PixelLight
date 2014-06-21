/*********************************************************\
 *  File: SNMMesh.cpp                                    *
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
#include "PLScene/Scene/SceneNodeModifiers/SNMMesh.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLScene {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNMMesh)


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMMesh::SNMMesh(SceneNode &cSceneNode) : SceneNodeModifier(cSceneNode)
{
}

/**
*  @brief
*    Destructor
*/
SNMMesh::~SNMMesh()
{
}

/**
*  @brief
*    Returns a "PLScene::SNMMeshUpdate" instance from the owner scene node
*/
SNMMeshUpdate *SNMMesh::GetSNMMeshUpdate() const
{
	// Is there already an instance of the "PLScene::SNMMeshUpdate" scene node modifier?
	static const String sSNMMeshUpdate = "PLScene::SNMMeshUpdate";
	SNMMeshUpdate *pSNMMeshUpdate = reinterpret_cast<SNMMeshUpdate*>(GetSceneNode().GetModifier(sSNMMeshUpdate));
	if (!pSNMMeshUpdate)
		pSNMMeshUpdate = reinterpret_cast<SNMMeshUpdate*>(GetSceneNode().AddModifier(sSNMMeshUpdate, "Flags=\"Automatic\""));

	// Return the SNMMeshUpdate instance
	return pSNMMeshUpdate;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene
