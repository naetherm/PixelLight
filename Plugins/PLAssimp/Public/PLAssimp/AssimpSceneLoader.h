/*********************************************************\
 *  File: AssimpSceneLoader.h                            *
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


#ifndef __PLASSIMP_ASSIMPSCENELOADER_H__
#define __PLASSIMP_ASSIMPSCENELOADER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMesh/Geometry.h>
#include <PLScene/Scene/SceneLoader/SceneLoader.h>
#include "PLAssimp/AssimpLoader.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
struct aiNode;
struct aiMesh;
struct aiScene;
namespace PLCore {
	class File;
}
namespace PLRenderer {
	class IndexBuffer;
	class VertexBuffer;
}
namespace PLMesh {
	class Mesh;
}
namespace PLScene {
	class SceneNode;
	class SceneContainer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLAssimp {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Scene loader using Assimp
*/
class AssimpSceneLoader : public AssimpLoader {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		AssimpSceneLoader();

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sDefaultTextureFileExtension
		*    Default texture file extension (completely in lower case is highly recommended)
		*
		*  @see
		*   - "AssimpLoader::GetDefaultTextureFileExtension()"
		*/
		AssimpSceneLoader(const PLCore::String &sDefaultTextureFileExtension);

		/**
		*  @brief
		*    Destructor
		*/
		~AssimpSceneLoader();

		/**
		*  @brief
		*    Loads the scene
		*
		*  @param[in,out] cContainer
		*    Scene container to fill
		*  @param[in]     cFile
		*    File to load from, must be opened and readable
		*  @param[in]     nQuality
		*    Integer parameter for post processing quality (0=none ... 3=maximum quality but slowest processing)
		*  @param[in]     sHint
		*    Filename extension hint for Assimp, if empty string (e.g. load from memory), the filename
		*    extension (if there's one) of the used file will be given to Assimp
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool Load(PLScene::SceneContainer &cContainer, PLCore::File &cFile, PLCore::uint8 nQuality, const PLCore::String &sHint = "");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		static const PLCore::uint32 MaxNumOfTextureCoords = 0x4;	/**< Supported number of texture coord sets (UV(W) channels) - same as AI_MAX_NUMBER_OF_TEXTURECOORDS */


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Loads the scene lights
		*
		*  @param[in] cParentContainer
		*    PixelLight parent scene container
		*/
		void LoadLights(PLScene::SceneContainer &cParentContainer);

		/**
		*  @brief
		*    Loads the scene cameras
		*
		*  @param[in] cParentContainer
		*    PixelLight parent scene container
		*/
		void LoadCameras(PLScene::SceneContainer &cParentContainer);

		/**
		*  @brief
		*    Loads the scene recursively
		*
		*  @param[in] cParentContainer
		*    Current PixelLight parent scene container
		*  @param[in] cAssimpNode
		*    Current Assimp node
		*/
		void LoadRec(PLScene::SceneContainer &cParentContainer, const aiNode &cAssimpNode);

		/**
		*  @brief
		*    Loads the given Assimp node (not recursively)
		*
		*  @param[in] cParentContainer
		*    Current PixelLight parent scene container
		*  @param[in] cAssimpNode
		*    Assimp node to load
		*  @param[in] bSetTransformation
		*    Set PixelLight scene node transformation?
		*/
		void LoadNode(PLScene::SceneContainer &cParentContainer, const aiNode &cAssimpNode, bool bSetTransformation);

		/**
		*  @brief
		*    Set's the transformation (relative to the parent) of a PixelLight scene node by using a given Assimp node
		*
		*  @param[in] cSceneNode
		*    PixelLight scene node
		*  @param[in] cAssimpNode
		*    Assimp node
		*/
		void SetNodeTransformation(PLScene::SceneNode &cSceneNode, const aiNode &cAssimpNode) const;

		/**
		*  @brief
		*    Loads the PixelLight mesh scene node of the given Assimp mesh
		*
		*  @param[in] cParentContainer
		*    Current PixelLight parent scene container
		*  @param[in] cAssimpMesh
		*    Assimp mesh to load the PixelLight mesh scene node from
		*  @param[in] sSceneNodeName
		*    Name of the PixelLight mesh scene node to create
		*
		*  @return
		*    The created PixelLight mesh scene node, null pointer on error
		*/
		PLScene::SceneNode *LoadMeshNode(PLScene::SceneContainer &cParentContainer, const aiMesh &cAssimpMesh, const PLCore::String &sSceneNodeName);

		/**
		*  @brief
		*    Loads the PixelLight mesh of the given Assimp mesh
		*
		*  @param[in]  cAssimpMesh
		*    Assimp mesh to load the PixelLight mesh from
		*  @param[out] cPLMesh
		*    PixelLight mesh to fill (this mesh is not cleared before it's filled)
		*/
		void LoadMesh(const aiMesh &cAssimpMesh, PLMesh::Mesh &cPLMesh);

		/**
		*  @brief
		*    Adds the mesh material (there's only one in here)
		*
		*  @param[in]  cAssimpMesh
		*    Assimp mesh to load the PixelLight mesh from
		*  @param[out] cPLMesh
		*    PixelLight mesh to fill (this mesh is not cleared before it's filled)
		*/
		void AddMaterial(const aiMesh &cAssimpMesh, PLMesh::Mesh &cPLMesh);

		/**
		*  @brief
		*    Fills the mesh data
		*
		*  @param[in]  cAssimpMesh
		*    Assimp mesh to gather the data from
		*  @param[in]  cVertexBuffer
		*    Vertex buffer to fill
		*  @param[in]  cIndexBuffer
		*    Index buffer to fill
		*  @param[in]  lstGeometries
		*    Geometries to fill
		*/
		void FillMesh(const aiMesh &cAssimpMesh, PLRenderer::VertexBuffer &cVertexBuffer, PLRenderer::IndexBuffer &cIndexBuffer, PLCore::Array<PLMesh::Geometry> &lstGeometries);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLScene::SceneContainer								   *m_pContainer;				/**< Scene container to fill, can be a null pointer */
		const aiScene										   *m_pAssimpScene;				/**< Used Assimp scene, can be a null pointer */
		PLCore::HashMap<PLCore::String, PLMesh::Mesh*>			m_mapAssimpMeshToPL;		/**< Maps a Assimp mesh onto a PixelLight mesh */
		PLCore::HashMap<PLCore::String, PLRenderer::Material*>  m_mapAssimpMaterialToPL;	/**< Maps a Assimp material onto a PixelLight material */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLAssimp


#endif // __PLASSIMP_ASSIMPSCENELOADER_H__
