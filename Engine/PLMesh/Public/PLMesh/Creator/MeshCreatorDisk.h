/*********************************************************\
 *  File: MeshCreatorDisk.h                              *
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


#ifndef __PLMESH_MESHCREATOR_DISK_H__
#define __PLMESH_MESHCREATOR_DISK_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector3.h>
#include "PLMesh/Creator/MeshCreator.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMesh {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Disk mesh creator class
*/
class MeshCreatorDisk : public MeshCreator {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLMESH_RTTI_EXPORT, MeshCreatorDisk, "PLMesh", PLMesh::MeshCreator, "Disk mesh creator class")
		// Attributes
		pl_attribute(InnerRadius,	float,				0.0f,					ReadWrite,	DirectValue,	"Inner radius of the disk (may be zero)",											"")
		pl_attribute(OuterRadius,	float,				0.5f,					ReadWrite,	DirectValue,	"Outer radius of the disk",															"")
		pl_attribute(Slices,		PLCore::uint32,		20,						ReadWrite,	DirectValue,	"Number of subdivisions around the z-axis",											"")
		pl_attribute(Loops,			PLCore::uint32,		1,						ReadWrite,	DirectValue,	"Number of concentric rings about the origin into which the disk is subdivided",	"")
		pl_attribute(Offset,		PLMath::Vector3,	PLMath::Vector3::Zero,	ReadWrite,	DirectValue,	"Offset to center",																	"")
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
		PLMESH_API MeshCreatorDisk();

		/**
		*  @brief
		*    Destructor
		*/
		PLMESH_API virtual ~MeshCreatorDisk();


	//[-------------------------------------------------------]
	//[ Private virtual MeshCreator functions                 ]
	//[-------------------------------------------------------]
	private:
		virtual Mesh *Create(Mesh &cMesh, PLCore::uint32 nLODLevel = 0, bool bStatic = true) const override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh


#endif // __PLMESH_MESHCREATOR_DISK_H__
