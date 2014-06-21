/*********************************************************\
 *  File: MeshCreatorBezierPatch.h                       *
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


#ifndef __PLMESH_MESHCREATOR_BEZIERPATCH_H__
#define __PLMESH_MESHCREATOR_BEZIERPATCH_H__
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
*    Bezier batch mesh creator class
*/
class MeshCreatorBezierPatch : public MeshCreator {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLMESH_RTTI_EXPORT, MeshCreatorBezierPatch, "PLMesh", PLMesh::MeshCreator, "Bezier batch mesh creator class")
		// Attributes
		pl_attribute(Offset,	PLMath::Vector3,	PLMath::Vector3::Zero,	ReadWrite,	DirectValue,	"Offset to center",		"")
		pl_attribute(Divisions,	PLCore::uint32,		10,						ReadWrite,	DirectValue,	"Number of divisions",	"")

		/*
		// [TODO] New RTTI usage
		// Control points row 0
		DEFINE_VAR   (PL_VAR_CLASS,		m_vV[0][0],		"V[0][0]",		"0.0 0.0 0.0",	"Control point 0/0",		"",	"")
		DEFINE_VAR   (PL_VAR_CLASS,		m_vV[1][0],		"V[1][0]",		"0.25 1.0 0.0",	"Control point 1/0",		"",	"")
		DEFINE_VAR   (PL_VAR_CLASS,		m_vV[2][0],		"V[2][0]",		"0.5 0.0 0.0",	"Control point 2/0",		"",	"")
		DEFINE_VAR   (PL_VAR_CLASS,		m_vV[3][0],		"V[3][0]",		"1.0 0.5 0.0",	"Control point 3/0",		"",	"")
		// Control points row 1
		DEFINE_VAR   (PL_VAR_CLASS,		m_vV[0][1],		"V[0][1]",		"0.0 0.3 0.25",	"Control point 0/1",		"",	"")
		DEFINE_VAR   (PL_VAR_CLASS,		m_vV[1][1],		"V[1][1]",		"0.25 0.0 0.25","Control point 1/1",		"",	"")
		DEFINE_VAR   (PL_VAR_CLASS,		m_vV[2][1],		"V[2][1]",		"0.5 0.1 0.25",	"Control point 2/1",		"",	"")
		DEFINE_VAR   (PL_VAR_CLASS,		m_vV[3][1],		"V[3][1]",		"1.0 0.0 0.25",	"Control point 3/1",		"",	"")
		// Control points row 2
		DEFINE_VAR   (PL_VAR_CLASS,		m_vV[0][2],		"V[0][2]",		"0.0 0.1 0.5",	"Control point 0/2",		"",	"")
		DEFINE_VAR   (PL_VAR_CLASS,		m_vV[1][2],		"V[1][2]",		"0.25 0.2 0.5",	"Control point 1/2",		"",	"")
		DEFINE_VAR   (PL_VAR_CLASS,		m_vV[2][2],		"V[2][2]",		"0.5 0.3 0.5",	"Control point 2/2",		"",	"")
		DEFINE_VAR   (PL_VAR_CLASS,		m_vV[3][2],		"V[3][2]",		"1.0 0.4 0.5",	"Control point 3/2",		"",	"")
		// Control points row 3
		DEFINE_VAR   (PL_VAR_CLASS,		m_vV[0][3],		"V[0][3]",		"0.0 0.0 1.0",	"Control point 0/3",		"",	"")
		DEFINE_VAR   (PL_VAR_CLASS,		m_vV[1][3],		"V[1][3]",		"0.25 0.0 1.0",	"Control point 1/3",		"",	"")
		DEFINE_VAR   (PL_VAR_CLASS,		m_vV[2][3],		"V[2][3]",		"0.5 1.0 1.0",	"Control point 2/3",		"",	"")
		DEFINE_VAR   (PL_VAR_CLASS,		m_vV[3][3],		"V[3][3]",		"1.0 0.7 1.0",	"Control point 3/3",		"",	"")
		*/

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
		PLMESH_API MeshCreatorBezierPatch();

		/**
		*  @brief
		*    Destructor
		*/
		PLMESH_API virtual ~MeshCreatorBezierPatch();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Calculates 3rd degree polynomial based on array of 4 points
		*    and a single variable (u) which is generally between 0 and 1
		*
		*  @param fU
		*    Variable
		*  @param pvV
		*    Array of 4 points
		*
		*  @return
		*    Resulting vector
		*/
		PLMath::Vector3 Bernstein(float fU, const PLMath::Vector3 *pvV) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// [TODO] New RTTI usage
		// Exported variables
		PLMath::Vector3   m_vV[4][4];


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


#endif // __PLMESH_MESHCREATOR_BEZIERPATCH_H__
