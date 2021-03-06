/*********************************************************\
 *  File: MeshCreator.cpp                                *
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
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Material/MaterialManager.h>
#include "PLMesh/Mesh.h"
#include "PLMesh/Creator/MeshCreator.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLMesh {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(MeshCreator, "PLMesh", PLCore::Object, "Abstract mesh creator base class")
	// Attributes
	pl_attribute_metadata(DrawStyle,	pl_enum_type_def3(MeshCreator, EDrawStyle),	MeshCreator::Fill,				ReadWrite,	"Draw style",										"")
	pl_attribute_metadata(Order,		bool,										0,								ReadWrite,	"Geometry order (0=clockwise  1=counterclockwise)",	"")
	pl_attribute_metadata(TexCoords,	bool,										true,							ReadWrite,	"Generate texture coordinates?",					"")
	pl_attribute_metadata(Normals,		bool,										true,							ReadWrite,	"Generate normals coordinates?",					"")
	pl_attribute_metadata(Material,		PLCore::String,								"Data/Textures/Default.dds",	ReadWrite,	"Material to use",									"")
pl_class_metadata_end(MeshCreator)


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
MeshCreator::MeshCreator() :
	DrawStyle(this),
	Order(this),
	TexCoords(this),
	Normals(this),
	Material(this)
{
}

/**
*  @brief
*    Destructor
*/
MeshCreator::~MeshCreator()
{
}


//[-------------------------------------------------------]
//[ Protected virtual MeshCreator functions               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Creates a mesh
*/
Mesh *MeshCreator::Create(Mesh &cMesh, uint32 nLODLevel, bool bStatic) const
{
	// Set material
	PLRenderer::Material *pMaterial = cMesh.GetRenderer()->GetRendererContext().GetMaterialManager().LoadResource(Material);
	if (pMaterial)
		cMesh.SetMaterial(0, pMaterial);

	// Return the mesh
	return &cMesh;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh
