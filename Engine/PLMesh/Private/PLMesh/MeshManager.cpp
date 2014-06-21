/*********************************************************\
 *  File: MeshManager.cpp                                *
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
#include <PLCore/File/Url.h>
#include <PLCore/Base/Class.h>
#include <PLCore/String/Tokenizer.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Material/MaterialManager.h>
#include "PLMesh/Creator/MeshCreator.h"
#include "PLMesh/MeshLODLevel.h"
#include "PLMesh/MeshManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
namespace PLMesh {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
MeshManager::MeshManager(Renderer &cRenderer) :
	m_pRenderer(&cRenderer)
{
	SetManagerName("Mesh manager");

	// Unload unused resources, please
	SetUnloadUnused(true);

	// Create the standard mesh
	Mesh *pMesh = CreateMesh("PLMesh::MeshCreatorCube", true, "Dimension=\"1.0 1.0 1.0\"");
	if (pMesh) {
		pMesh->SetName("Default");
		pMesh->SetProtected(true);
		SetStandard(pMesh);

		// It's a good idea to give the default mesh tangent space vectors (tangent & binormal) so it can also be used for normal mapping
		pMesh->CalculateTangentSpaceVectors();
	}
}

/**
*  @brief
*    Destructor
*/
MeshManager::~MeshManager()
{
}

/**
*  @brief
*    Returns the used renderer
*/
Renderer *MeshManager::GetRenderer() const
{
	return m_pRenderer;
}

/**
*  @brief
*    Creates the mesh if a null pointer is given and adds the required LODs
*/
Mesh *MeshManager::CreateMesh()
{
	Mesh *pMesh = Create();
	pMesh->AddMorphTarget();

	// Set standard material
	pMesh->AddMaterial(GetRenderer()->GetRendererContext().GetMaterialManager().GetStandard());
	while (pMesh->GetNumOfLODLevels() <= 0) {
		MeshLODLevel *pLODLevel = pMesh->AddLODLevel();
		pLODLevel->CreateGeometries();
		pLODLevel->CreateIndexBuffer();
	}

	// Done
	return pMesh;
}

/**
*  @brief
*    Creates a new mesh resource
*/
Mesh *MeshManager::CreateMesh(const String &sName, bool bStatic)
{
	// Check whether this resource is already within the manager
	Mesh *pResource = GetByName(sName);
	if (pResource)
		return pResource;

	// Get name
	String sNameT;
	if (sName.GetLength())
		sNameT = sName;
	else {
		// Find an unused resource name
		sNameT = "0";
		for (int i=1; GetByName(sNameT); i++)
			sNameT = i;
	}

	// Create and return the new resource
	return new Mesh(m_pRenderer, *this, sNameT, bStatic);
}

/**
*  @brief
*    Creates a mesh resource using a mesh creator
*/
Mesh *MeshManager::CreateMesh(const String &sName, bool bStatic, const String &sParameters)
{
	// To keep things as fast as possible we store a pointer to the base class
	// (no hash map operation + quite efficient internal string comparison)
	static const PLCore::Class *pBaseClass = PLCore::ClassManager::GetInstance()->GetClass("PLMesh::MeshCreator");

	// Create the mesh creator
	if (pBaseClass) {
		const PLCore::Class *pClass = PLCore::ClassManager::GetInstance()->GetClass(sName);
		if (pClass && pClass->IsDerivedFrom(*pBaseClass)) {
			MeshCreator *pMC = static_cast<MeshCreator*>(pClass->Create());
			if (pMC) {
				// Set parameters
				if (sParameters.GetLength())
					pMC->SetValues(sParameters);

				// Create the mesh
				Mesh *pMesh = CreateMesh();
				if (pMesh)
					pMC->Create(*pMesh, 0, bStatic);

				// Destroy the mesh creator
				delete pMC;

				// Return the created mesh
				return pMesh;
			}
		}
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Load mesh
*/
Mesh *MeshManager::LoadMesh(const String &sFilename, const String &sParams, const String &sMethod, bool bReloadMesh, bool bStatic)
{
	// Is the mesh already loaded?
	Mesh *pMesh = GetByName(sFilename);
	if (pMesh) {
		// Reload this mesh?
		if (bReloadMesh)
			pMesh->Reload();
	} else {
		// Create the mesh by using a mesh creator?
		if (sFilename.Compare("Create ", 0, 7)) {
			// Get mesh creator class to use
			Tokenizer cTokenizer;
			cTokenizer.Start(sFilename);
			cTokenizer.GetNextToken(); // Skip 'Create '
			String sClass = cTokenizer.GetNextToken();
			if (sClass.GetLength()) {
				// Get mesh name
				bool bUseName = false;
				String sName;
				cTokenizer.PushState();
				if (cTokenizer.FindToken("Name")) {
					// Skip '='
					cTokenizer.GetNextToken();

					// Get name
					if (cTokenizer.GetNextToken().GetLength()) {
						if (cTokenizer.GetNextToken().GetLength() == 1 && cTokenizer.GetQuotes().IsSubstring(cTokenizer.GetToken())) {
							sName = cTokenizer.GetNextToken();
							// Read "
							if (!cTokenizer.GetNextToken().GetLength() == 1 && cTokenizer.GetQuotes().IsSubstring(cTokenizer.GetToken()))
								PL_LOG(Error, "Name var closing \" is missed! (" + sFilename + ')')
						} else {
							sName = cTokenizer.GetToken();
						}
						bUseName = true;
					}
				} else {
					// There's no name! :(
					cTokenizer.PopState();
				}

				// Check whether the mesh is already within the mesh manager
				pMesh = bUseName ? GetByName(sName) : nullptr;
				if (!pMesh) {
					// Get the parameters
					String sParameters = sFilename.GetSubstring(cTokenizer.GetPosition());

					// Try to create the mesh
					pMesh = CreateMesh(sClass, bStatic, sParameters);
					if (bUseName && pMesh)
						pMesh->SetName(sName);
				}
			}
			cTokenizer.Stop();

		// Try to get/load the mesh
		// -> There are file formats without an extension, so no extension must also be valid
		} else {
			const String sExtension = Url(sFilename).GetExtension();
			pMesh = Create(sFilename);
			if (pMesh) {
				if (bStatic) {
					// We do NOT need to give parameters because 'Static' is 'true' by default!
					if (!pMesh->LoadByFilename(sFilename, sParams, sMethod)) {
						// Can't load mesh...
						delete pMesh;
						pMesh = nullptr;
					}
				} else {
					if (!pMesh->LoadByFilename(sFilename, String("Static=") + bStatic + '\"')) {
						// Can't load mesh...
						delete pMesh;
						pMesh = nullptr;
					}
				}
			}
		}
	}

	// Return the mesh
	return pMesh;
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::ResourceManager functions     ]
//[-------------------------------------------------------]
Mesh *MeshManager::CreateResource(const String &sName)
{
	return new Mesh(m_pRenderer, *this, sName);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh
