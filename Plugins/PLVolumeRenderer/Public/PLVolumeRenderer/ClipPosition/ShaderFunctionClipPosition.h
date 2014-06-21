/*********************************************************\
 *  File: ShaderFunctionClipPosition.h                   *
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


#ifndef __PLVOLUMERENDERER_SHADERFUNCTION_CLIPPOSITION_H__
#define __PLVOLUMERENDERER_SHADERFUNCTION_CLIPPOSITION_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLVolumeRenderer/ShaderFunction.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolumeRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract shader function base class for clip position
*
*  @remarks
*    Shader functions derived from this class will clip individual positions inside
*    the volume. There's no guarantee that due to clip operations the ray is only
*    clipped by modifying the ray origin or the maximum travel length. The ray will
*    potentially be segmented. Thus, while performing clip operations, the performance
*    may be reduced due to additional calculations performed while traveling through the
*    volume.
*/
class ShaderFunctionClipPosition : public ShaderFunction {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLVOLUMERENDERER_RTTI_EXPORT, ShaderFunctionClipPosition, "PLVolumeRenderer", PLVolumeRenderer::ShaderFunction, "Abstract shader function base class for clip position")
		// Properties
		pl_properties
			pl_property("FunctionTitle",			"2.1 - Clip Position")
			pl_property("FunctionName",				"ClipPosition")
			pl_property("FunctionSignature_GLSL",	"bool ClipPosition(vec3 Position)")
			pl_property("FunctionSignature_Cg",		"bool ClipPosition(float3 Position)")
		pl_properties_end
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public virtual ShaderFunctionClipPosition functions   ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Sets the clip boxes
		*
		*  @param[in] cProgram
		*    Used GPU program
		*  @param[in] cVolumeVisNode
		*    Volume visibility node
		*  @param[in] lstClipBoxes
		*    List of found clip boxes, the visibility scene nodes must be clip boxes, there's no internal additional security check
		*
		*  @note
		*    - The default implementation does nothing
		*/
		PLVOLUMERENDERER_API virtual void SetBoxes(PLRenderer::Program &cProgram, const PLScene::VisNode &cVolumeVisNode, const PLCore::Array<const PLScene::VisNode*> &lstClipBoxes);

		/**
		*  @brief
		*    Sets the clip ellipsoids
		*
		*  @param[in] cProgram
		*    Used GPU program
		*  @param[in] cVolumeVisNode
		*    Volume visibility node
		*  @param[in] lstClipEllipsoids
		*    List of found clip ellipsoids, the visibility scene nodes must be clip ellipsoids, there's no internal additional security check
		*
		*  @note
		*    - The default implementation does nothing
		*/
		PLVOLUMERENDERER_API virtual void SetEllipsoids(PLRenderer::Program &cProgram, const PLScene::VisNode &cVolumeVisNode, const PLCore::Array<const PLScene::VisNode*> &lstClipEllipsoids);

		/**
		*  @brief
		*    Sets the clip volume textures
		*
		*  @param[in] cProgram
		*    Used GPU program
		*  @param[in] cVolumeVisNode
		*    Volume visibility node
		*  @param[in] lstClipVolumeTextures
		*    List of found clip volume textures, the visibility scene nodes must be clip volume textures, there's no internal additional security check
		*
		*  @note
		*    - The default implementation does nothing
		*/
		PLVOLUMERENDERER_API virtual void SetVolumeTextures(PLRenderer::Program &cProgram, const PLScene::VisNode &cVolumeVisNode, const PLCore::Array<const PLScene::VisNode*> &lstClipVolumeTextures);


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		PLVOLUMERENDERER_API ShaderFunctionClipPosition();

		/**
		*  @brief
		*    Destructor
		*/
		PLVOLUMERENDERER_API virtual ~ShaderFunctionClipPosition();


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer


#endif // __PLVOLUMERENDERER_SHADERFUNCTION_CLIPPOSITION_H__
