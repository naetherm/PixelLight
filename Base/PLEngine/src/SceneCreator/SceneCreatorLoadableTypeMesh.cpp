/*********************************************************\
 *  File: SceneCreatorLoadableTypeMesh.cpp               *
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
#include <PLMesh/MeshHandler.h>
#include <PLScene/Scene/SceneContainer.h>
#include <PLScene/Scene/SceneNodes/SNMesh.h>
#include "PLEngine/SceneCreator/SceneCreatorLoadableTypeMesh.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLScene;
namespace PLEngine {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SceneCreatorLoadableTypeMesh, "PLEngine", PLEngine::SceneCreatorLoadableType, "Mesh loadable type scene creator")
	// Properties
	pl_properties
		pl_property("Type",	"Mesh")
	pl_properties_end
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
pl_class_metadata_end(SceneCreatorLoadableTypeMesh)



//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SceneCreatorLoadableTypeMesh::SceneCreatorLoadableTypeMesh()
{
}

/**
*  @brief
*    Destructor
*/
SceneCreatorLoadableTypeMesh::~SceneCreatorLoadableTypeMesh()
{
}


//[-------------------------------------------------------]
//[ Public virtual SceneCreator functions                 ]
//[-------------------------------------------------------]
SNCamera *SceneCreatorLoadableTypeMesh::Create(SceneContainer &cSceneContainer) const
{
	// Get the mesh filename
	const String sFilename = Filename.Get();

	// Create a mesh scene node
	SceneNode *pPrimarySceneNode = cSceneContainer.Create("PLScene::SNMesh", "Mesh", "Mesh=\"" + sFilename + '\"');

	// Security check for better usability: Has the scene node a mesh with at least one material?
	// -> If not the scene node mesh may not be visible and the user may wonder what's going on, so, in this case enable scene node debug mode
	if (pPrimarySceneNode && pPrimarySceneNode->GetMeshHandler() && !pPrimarySceneNode->GetMeshHandler()->GetNumOfMaterials()) {
		PL_LOG(Warning, "Mesh viewer: The mesh \"" + sFilename + "\" has no materials")

		// Switch the mesh scene node into wireframe debug mode so that the user can see what's going on
		pPrimarySceneNode->SetDebugFlags(SceneNode::DebugEnabled | SceneNode::DebugNoLocalCoordinateAxis | SceneNode::DebugNoName | SceneNode::DebugNoAABBox | SNMesh:: DebugShowWireframe);
	}

	// Configure a generic scene and return the preferred camera scene node
	return SceneCreator::ConfigureGenericScene(cSceneContainer, pPrimarySceneNode);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine
