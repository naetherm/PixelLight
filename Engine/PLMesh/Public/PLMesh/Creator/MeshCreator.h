/*********************************************************\
 *  File: MeshCreator.h                                  *
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


#ifndef __PLMESH_MESHCREATOR_H__
#define __PLMESH_MESHCREATOR_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Object.h>
#include "PLMesh/PLMesh.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMesh {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Mesh;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract mesh creator base class
*/
class MeshCreator : public PLCore::Object {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MeshManager;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Draw style
		*/
		enum EDrawStyle {
			Fill       = 0,	/**< Fill */
			Line       = 1,	/**< Line */
			Silhouette = 2,	/**< Silhouette */
			Point      = 3	/**< Point */
		};
		pl_enum(EDrawStyle)
			pl_enum_value(Fill,			"Fill")
			pl_enum_value(Line,			"Line")
			pl_enum_value(Silhouette,	"Silhouette")
			pl_enum_value(Point,		"Point")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLMESH_RTTI_EXPORT, MeshCreator, "PLMesh", PLCore::Object, "Abstract mesh creator base class")
		// Attributes
		pl_attribute(DrawStyle,	pl_enum_type(EDrawStyle),	Fill,							ReadWrite,	DirectValue,	"Draw style",										"")
		pl_attribute(Order,		bool,						0,								ReadWrite,	DirectValue,	"Geometry order (0=clockwise  1=counterclockwise)",	"")
		pl_attribute(TexCoords,	bool,						true,							ReadWrite,	DirectValue,	"Generate texture coordinates?",					"")
		pl_attribute(Normals,	bool,						true,							ReadWrite,	DirectValue,	"Generate normals coordinates?",					"")
		pl_attribute(Material,	PLCore::String,				"Data/Textures/Default.dds",	ReadWrite,	DirectValue,	"Material to use",									"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		PLMESH_API MeshCreator();

		/**
		*  @brief
		*    Destructor
		*/
		PLMESH_API virtual ~MeshCreator();


	//[-------------------------------------------------------]
	//[ Protected virtual MeshCreator functions               ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Creates a mesh
		*
		*  @param[in] cMesh
		*    Mesh to manipulate
		*  @param[in] nLODLevel
		*    Mesh LOD level to manipulate
		*  @param[in] bStatic
		*    Static mesh?
		*
		*  @return
		*    The created/manipulated mesh, a null pointer on error
		*
		*  @note
		*    - The default implementation sets the first material
		*/
		PLMESH_API virtual Mesh *Create(Mesh &cMesh, PLCore::uint32 nLODLevel = 0, bool bStatic = true) const;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh


#endif // __PLMESH_MESHCREATOR_H__
