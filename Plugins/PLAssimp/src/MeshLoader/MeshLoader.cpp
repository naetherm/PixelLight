/*********************************************************\
 *  File: MeshLoader.cpp                                 *
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
#include "PLAssimp/MeshLoader/MeshLoader.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMesh;
namespace PLAssimp {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(MeshLoader, "PLAssimp", PLMesh::MeshLoader, "Abstract mesh loader implementation using Assimp base class")
	// Properties
	pl_properties
		pl_property("Load",	"1")
		pl_property("Save",	"0")
	pl_properties_end
	// Methods
	pl_method_2_metadata(Load,			pl_ret_type(bool),	PLMesh::Mesh&,	PLCore::File&,							"Load method. The loaded mesh is static and is post processed for maximum quality.",																															"")
	pl_method_4_metadata(LoadParams,	pl_ret_type(bool),	PLMesh::Mesh&,	PLCore::File&,	bool,	PLCore::uint8,	"Load method. Parameters: First boolean parameter determines whether or not the mesh is static, second integer parameter for post processing quality (0=none ... 3=maximum quality but slowest processing).",	"")
	pl_method_2_metadata(Save,			pl_ret_type(bool),	PLMesh::Mesh&,	PLCore::File&,							"Save method",																																																	"")
pl_class_metadata_end(MeshLoader)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool MeshLoader::Load(Mesh &cMesh, File &cFile)
{
	return LoadParams(cMesh, cFile, true, 3);
}

bool MeshLoader::Save(Mesh &cMesh, File &cFile)
{
	// Error - not implemented!
	return false;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
MeshLoader::MeshLoader()
{
}

/**
*  @brief
*    Destructor
*/
MeshLoader::~MeshLoader()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLAssimp
