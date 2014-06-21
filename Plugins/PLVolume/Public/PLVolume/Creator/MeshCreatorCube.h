/*********************************************************\
 *  File: MeshCreatorCube.h                              *
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


#ifndef __PLVOLUME_MESHCREATOR_CUBE_H__
#define __PLVOLUME_MESHCREATOR_CUBE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector3.h>
#include <PLMesh/Creator/MeshCreator.h>
#include "PLVolume/PLVolume.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class VertexBuffer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolume {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Cube mesh creator class
*/
class MeshCreatorCube : public PLMesh::MeshCreator {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLVOLUME_RTTI_EXPORT, MeshCreatorCube, "PLVolume", PLMesh::MeshCreator, "Cube mesh creator class")
		// Attributes
		pl_attribute(Dimension,	PLMath::Vector3,	PLMath::Vector3(1.0f, 1.0f, 1.0f),	ReadWrite,	DirectValue,	"Cube dimension",	"")
		pl_attribute(Offset,	PLMath::Vector3,	PLMath::Vector3(0.0f, 0.0f, 0.0f),	ReadWrite,	DirectValue,	"Offset to center",	"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MeshCreatorCube();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MeshCreatorCube();


	//[-------------------------------------------------------]
	//[ Private virtual PLMesh::MeshCreator functions         ]
	//[-------------------------------------------------------]
	private:
		virtual PLMesh::Mesh *Create(PLMesh::Mesh &cMesh, PLCore::uint32 nLODLevel = 0, bool bStatic = true) const override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume


#endif // __PLVOLUME_MESHCREATOR_CUBE_H__
