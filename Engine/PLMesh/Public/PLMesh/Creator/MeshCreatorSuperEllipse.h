/*********************************************************\
 *  File: MeshCreatorSuperEllipse.h                      *
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


#ifndef __PLMESH_MESHCREATOR_SUPERELLIPSE_H__
#define __PLMESH_MESHCREATOR_SUPERELLIPSE_H__
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
*    Super ellipse mesh creator class
*
*  @remarks
*    The super ellipsoid is a versatile primitive that is controlled by two parameters.
*    As special cases it can represent a sphere, square box, and closed cylindrical.
*    It should be noted that there are some numerical issues with both very small or very large
*    values of these parameters. Typically, for safety, they should be in the range of 0.1 to about 5.\n
*
*    Examples: (Power1/Power2)\n
*    1.0/1.0 = sphere\n
*    0.1/1.0 = rounded cylinder\n
*    0.1/0.1 = rounded box\n
*    1.0/0.1 = similar to a rounded box\n
*/
class MeshCreatorSuperEllipse : public MeshCreator {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLMESH_RTTI_EXPORT, MeshCreatorSuperEllipse, "PLMesh", PLMesh::MeshCreator, "Super ellipse mesh creator class")
		// Attributes
		pl_attribute(Power1,	float,				0.5f,					ReadWrite,	DirectValue,	"Control parameter 1",	"")
		pl_attribute(Power2,	float,				0.1f,					ReadWrite,	DirectValue,	"Control parameter 2",	"")
		pl_attribute(Detail,	PLCore::uint32,		10,						ReadWrite,	DirectValue,	"Detail",				"")
		pl_attribute(Offset,	PLMath::Vector3,	PLMath::Vector3::Zero,	ReadWrite,	DirectValue,	"Offset to center",		"")
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
		PLMESH_API MeshCreatorSuperEllipse();

		/**
		*  @brief
		*    Destructor
		*/
		PLMESH_API virtual ~MeshCreatorSuperEllipse();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Internal helper function
		*/
		void EvalSuperEllipse(float fT1, float fT2, float fP1, float fP2, PLMath::Vector3 *pV) const;


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


#endif // __PLMESH_MESHCREATOR_SUPERELLIPSE_H__
