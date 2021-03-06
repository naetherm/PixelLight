/*********************************************************\
 *  File: SceneLoader3DGameStudioTerrain.cpp             *
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
#include "PLAssimp/AssimpSceneLoader.h"
#include "PLAssimp/SceneLoader/SceneLoader3DGameStudioTerrain.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLScene;
namespace PLAssimp {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SceneLoader3DGameStudioTerrain, "PLAssimp", PLAssimp::SceneLoader, "Scene loader implementation for hmp (3D GameStudio Terrain) using Assimp")
	// Properties
	pl_properties
		pl_property("Formats",	"hmp,HMP")
	pl_properties_end
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
pl_class_metadata_end(SceneLoader3DGameStudioTerrain)


//[-------------------------------------------------------]
//[ Public virtual SceneLoader functions                  ]
//[-------------------------------------------------------]
bool SceneLoader3DGameStudioTerrain::LoadParams(SceneContainer &cContainer, File &cFile, uint8 nQuality)
{
	// Just let Assimp do all the hard work for us
	return AssimpSceneLoader().Load(cContainer, cFile, nQuality, "hmp");
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SceneLoader3DGameStudioTerrain::SceneLoader3DGameStudioTerrain()
{
}

/**
*  @brief
*    Destructor
*/
SceneLoader3DGameStudioTerrain::~SceneLoader3DGameStudioTerrain()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLAssimp
