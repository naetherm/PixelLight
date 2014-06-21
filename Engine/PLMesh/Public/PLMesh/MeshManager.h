/*********************************************************\
 *  File: MeshManager.h                                  *
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


#ifndef __PLMESH_MESHMANAGER_H__
#define __PLMESH_MESHMANAGER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/ResourceManager.h>
#include "PLMesh/MeshHandler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMesh {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    This is a manager for the mesh resource
*
*  @note
*    - Unloads unused resources automatically by default
*/
class MeshManager : public PLCore::ResourceManager<Mesh> {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*/
		PLMESH_API MeshManager(PLRenderer::Renderer &cRenderer);

		/**
		*  @brief
		*    Destructor
		*/
		PLMESH_API virtual ~MeshManager();

		/**
		*  @brief
		*    Returns the used renderer
		*
		*  @return
		*    Pointer to the renderer the meshes should use, can be a null pointer
		*/
		PLMESH_API PLRenderer::Renderer *GetRenderer() const;

		/**
		*  @brief
		*    Creates the mesh and adds the required LODs
		*
		*  @return
		*   The created mesh, a null pointer on error
		*
		*  @note
		*    - If the mesh is created, the 'Material' is added automatically
		*/
		PLMESH_API Mesh *CreateMesh();

		/**
		*  @brief
		*    Creates a new mesh resource
		*
		*  @param[in] sName
		*    Resource name, if empty an unused name is set automatically
		*  @param[in] bStatic
		*    Static mesh? (better performance!)
		*
		*  @return
		*    Pointer to the created resource, a null pointer if there was an error
		*
		*  @note
		*    - If there's already a resource with this name, this resource is returned
		*/
		PLMESH_API Mesh *CreateMesh(const PLCore::String &sName, bool bStatic = true);

		/**
		*  @brief
		*    Creates a mesh resource using a mesh creator
		*
		*  @param[in] sName
		*    Mesh creator class name (for instance "MeshCreatorSphere")
		*  @param[in] bStatic
		*    Static mesh? (better performance!)
		*  @param[in] sParameters
		*    Mesh creator parameters. (for instance "Radius='4.0' Detail='20'")
		*    This parameters depend on the used mesh creator.
		*
		*  @return
		*    Pointer to the created resource, a null pointer if there was an error
		*    (maybe unknown class or the class is not derived from 'MeshCreator')
		*/
		PLMESH_API Mesh *CreateMesh(const PLCore::String &sName, bool bStatic, const PLCore::String &sParameters);

		/**
		*  @brief
		*    Load mesh
		*
		*  @param[in] sFilename
		*    Mesh filename. It's also possible to create meshes dynamically.
		*    "Create MeshCreatorSphere Name=\"Sphere\" Radius=\"4.0\" Detail=\"20.0\""
		*    For instance will use the mesh creator class 'MeshCreatorSphere'
		*    to create a mesh with the name 'Sphere' and some parameters.
		*  @param[in] sParams
		*    Optional load method parameters, can be an empty string
		*  @param[in] sMethod
		*    Optional name of the load method to use, can be an empty string
		*  @param[in] bReloadMesh
		*    Force mesh itself to be reloaded?
		*  @param[in] bStatic
		*    Static mesh? (better performance!)
		*
		*  @return
		*    The loaded mesh, a null pointer on error
		*/
		PLMESH_API Mesh *LoadMesh(const PLCore::String &sFilename, const PLCore::String &sParams = "", const PLCore::String &sMethod = "", bool bReloadMesh = false, bool bStatic = true);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLRenderer::Renderer *m_pRenderer;	/**< Renderer the manager is using (always valid!) */


	//[-------------------------------------------------------]
	//[ Private virtual PLCore::ResourceManager functions     ]
	//[-------------------------------------------------------]
	private:
		virtual Mesh *CreateResource(const PLCore::String &sName) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh


#endif // __PLMESH_MESHMANAGER_H__
