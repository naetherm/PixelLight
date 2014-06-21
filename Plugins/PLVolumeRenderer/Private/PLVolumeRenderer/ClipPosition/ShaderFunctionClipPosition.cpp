/*********************************************************\
 *  File: ShaderFunctionClipPosition.cpp                 *
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
#include "PLVolumeRenderer/ClipPosition/ShaderFunctionClipPosition.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolumeRenderer {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(ShaderFunctionClipPosition)


//[-------------------------------------------------------]
//[ Public virtual ShaderFunctionClipPosition functions   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sets the clip boxes
*/
void ShaderFunctionClipPosition::SetBoxes(PLRenderer::Program &cProgram, const PLScene::VisNode &cVolumeVisNode, const PLCore::Array<const PLScene::VisNode*> &lstClipBoxes)
{
	// No default implementation
}

/**
*  @brief
*    Sets the clip ellipsoids
*/
void ShaderFunctionClipPosition::SetEllipsoids(PLRenderer::Program &cProgram, const PLScene::VisNode &cVolumeVisNode, const PLCore::Array<const PLScene::VisNode*> &lstClipEllipsoids)
{
	// No default implementation
}

/**
*  @brief
*    Sets the clip volume textures
*/
void ShaderFunctionClipPosition::SetVolumeTextures(PLRenderer::Program &cProgram, const PLScene::VisNode &cVolumeVisNode, const PLCore::Array<const PLScene::VisNode*> &lstClipVolumeTextures)
{
	// No default implementation
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
ShaderFunctionClipPosition::ShaderFunctionClipPosition()
{
}

/**
*  @brief
*    Destructor
*/
ShaderFunctionClipPosition::~ShaderFunctionClipPosition()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer
