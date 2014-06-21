/*********************************************************\
 *  File: AssimpMeshLoader.h                             *
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


#ifndef __PLASSIMP_ASSIMPMESHLOADER_H__
#define __PLASSIMP_ASSIMPMESHLOADER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMesh/Geometry.h>
#include "PLAssimp/AssimpLoader.h"

#include <assimp/vector3.h>
#include <assimp/matrix4x4.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
struct aiNode;
struct aiScene;
//struct aiMatrix4x4;
namespace PLCore {
	class File;
}
namespace PLMesh {
	class Mesh;
}
namespace PLRenderer {
	class IndexBuffer;
	class VertexBuffer;
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
*    Mesh loader using Assimp
*/
class AssimpMeshLoader : public AssimpLoader {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		AssimpMeshLoader();

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
		AssimpMeshLoader(const PLCore::String &sDefaultTextureFileExtension);

		/**
		*  @brief
		*    Destructor
		*/
		~AssimpMeshLoader();

		/**
		*  @brief
		*    Loads the mesh
		*
		*  @param[in,out] cMesh
		*    Mesh to fill
		*  @param[in]     cFile
		*    File to load from, must be opened and readable
		*  @param[in]     bStatic
		*    'true' if the mesh is static, else 'false'
		*  @param[in]     nQuality
		*    Integer parameter for post processing quality (0=none ... 3=maximum quality but slowest processing)
		*  @param[in]     sHint
		*    Filename extension hint for Assimp, if empty string (e.g. load from memory), the filename
		*    extension (if there's one) of the used file will be given to Assimp
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool Load(PLMesh::Mesh &cMesh, PLCore::File &cFile, bool bStatic, PLCore::uint8 nQuality, const PLCore::String &sHint = "");


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
		*    Adds the materials
		*/
		void AddMaterials();

		/**
		*  @brief
		*    Gets the total number of vertices and indices required for everything as one PixelLight mesh recursively
		*
		*  @param[in]  cAssimpNode
		*    Assimp node to gather the data from
		*  @param[out] nNumOfVertices
		*    Receives the number of vertices
		*  @param[out] nNumOfIndices
		*    Receives the number of indices
		*/
		void GetNumOfVerticesAndIndicesRec(const aiNode &cAssimpNode, PLCore::uint32 &nNumOfVertices, PLCore::uint32 &nNumOfIndices);

		/**
		*  @brief
		*    Fills the mesh data recursively
		*
		*  @param[in]  cAssimpNode
		*    Assimp node to gather the data from
		*  @param[in]  cVertexBuffer
		*    Vertex buffer to fill
		*  @param[in]  cIndexBuffer
		*    Index buffer to fill
		*  @param[in]  lstGeometries
		*    Geometries to fill
		*  @param[in]  mAssimpTransformation
		*    Current absolute Assimp transformation matrix (local to global space)
		*  @param[out] nNumOfVertices
		*    Receives the number of processed vertices
		*  @param[out] nNumOfIndices
		*    Receives the number of processed indices
		*/
		void FillMeshRec(const aiNode &cAssimpNode, PLRenderer::VertexBuffer &cVertexBuffer, PLRenderer::IndexBuffer &cIndexBuffer, PLCore::Array<PLMesh::Geometry> &lstGeometries,
						 const aiMatrix4x4 &mAssimpTransformation, PLCore::uint32 &nNumOfVertices, PLCore::uint32 &nNumOfIndices);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLMesh::Mesh  *m_pMesh;										/**< Mesh to fill, can be a null pointer */
		const aiScene *m_pAssimpScene;								/**< Used Assimp scene, can be a null pointer */
		bool		   m_bHasNormals;								/**< Are there normals? */
		bool		   m_bHasTangentsAndBitangents;					/**< Are there tangents and bitangents? */
		bool		   m_bHasTexCoords;								/**< Are there texture coordinates? */
		unsigned int   m_nNumUVComponents[MaxNumOfTextureCoords];	/**< Specifies the number of components for a given UV channel (see aiMesh for details) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLAssimp


#endif // __PLASSIMP_ASSIMPMESHLOADER_H__
