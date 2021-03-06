/*********************************************************\
 *  File: MeshLoaderQuick3D.cpp                          *
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
#include "PLAssimp/AssimpMeshLoader.h"
#include "PLAssimp/MeshLoader/MeshLoaderQuick3D.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMesh;
namespace PLAssimp {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(MeshLoaderQuick3D, "PLAssimp", PLAssimp::MeshLoader, "Mesh loader implementation for q3d/q3s (Quick3D) using Assimp")
	// Properties
	pl_properties
		pl_property("Formats",	"q3d,q3s,q3o,Q3D,Q3S,Q3O")
	pl_properties_end
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
pl_class_metadata_end(MeshLoaderQuick3D)


//[-------------------------------------------------------]
//[ Public virtual MeshLoader functions                   ]
//[-------------------------------------------------------]
bool MeshLoaderQuick3D::LoadParams(Mesh &cMesh, File &cFile, bool bStatic, uint8 nQuality)
{
	// Just let Assimp do all the hard work for us
	return AssimpMeshLoader().Load(cMesh, cFile, bStatic, nQuality, "q3o");
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
MeshLoaderQuick3D::MeshLoaderQuick3D()
{
}

/**
*  @brief
*    Destructor
*/
MeshLoaderQuick3D::~MeshLoaderQuick3D()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLAssimp
