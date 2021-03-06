/*********************************************************\
 *  File: SNMPhysicsBodyMesh.cpp                         *
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
#include <PLScene/Scene/SceneContext.h>
#include "PLPhysics/Body.h"
#include "PLPhysics/World.h"
#include "PLPhysics/SceneNodes/SCPhysicsWorld.h"
#include "PLPhysics/SceneNodeModifiers/SNMPhysicsBodyMesh.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLScene;
namespace PLPhysics {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SNMPhysicsBodyMesh, "PLPhysics", PLPhysics::SNMPhysicsBody, "Physics mesh body scene node modifier")
	// Constructors
	pl_constructor_1_metadata(ParameterConstructor,	PLScene::SceneNode&,	"Parameter constructor",	"")
	// Attributes
	pl_attribute_metadata(Mesh,		PLCore::String,	"",		ReadWrite,	"Collision mesh, if empty, the variable 'Mesh' (if available) of the scene node is used instead",								"Type='Mesh'")
	pl_attribute_metadata(Optimize,	bool,			false,	ReadWrite,	"Flag that indicates whether it should optimize this mesh. Set to 1 to optimize (may take some time!) the mesh, otherwise 0.",	"")
pl_class_metadata_end(SNMPhysicsBodyMesh)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
String SNMPhysicsBodyMesh::GetMesh() const
{
	return m_sMesh;
}

void SNMPhysicsBodyMesh::SetMesh(const String &sValue)
{
	if (m_sMesh != sValue) {
		m_sMesh = sValue;
		RecreatePhysicsBody();
	}
}

bool SNMPhysicsBodyMesh::GetOptimize() const
{
	return m_bOptimize;
}

void SNMPhysicsBodyMesh::SetOptimize(bool bValue)
{
	if (m_bOptimize != bValue) {
		m_bOptimize = bValue;
		RecreatePhysicsBody();
	}
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SNMPhysicsBodyMesh::SNMPhysicsBodyMesh(SceneNode &cSceneNode) : SNMPhysicsBody(cSceneNode),
	Mesh(this),
	Optimize(this),
	m_bOptimize(false)
{
}

/**
*  @brief
*    Destructor
*/
SNMPhysicsBodyMesh::~SNMPhysicsBodyMesh()
{
}


//[-------------------------------------------------------]
//[ Private virtual SNMPhysicsBody functions              ]
//[-------------------------------------------------------]
void SNMPhysicsBodyMesh::CreatePhysicsBody()
{
	if (m_pWorldContainer && m_pWorldContainer->GetWorld()) {
		// Create the PL physics body
		String sMesh = m_sMesh;
		if (!m_sMesh.GetLength()) {
			DynVarPtr pDynVar = GetSceneNode().GetAttribute("Mesh");
			if (pDynVar)
				sMesh = pDynVar->GetString();
		}
		if (sMesh.GetLength())
			m_pBodyHandler->SetElement(m_pWorldContainer->GetWorld()->CreateBodyMesh(GetSceneNode().GetSceneContext()->GetMeshManager(), sMesh, GetSceneNode().GetTransform().GetScale(), m_bOptimize));

		// Call base implementation
		SNMPhysicsBody::CreatePhysicsBody();
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics
