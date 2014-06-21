/*********************************************************\
 *  File: SNMPostProcessDepthOfField.h                   *
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


#ifndef __PLSCENE_SCENENODEMODIFIER_DEPTHOFFIELD_H__
#define __PLSCENE_SCENENODEMODIFIER_DEPTHOFFIELD_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector2.h>
#include "PLCompositing/SceneNodeModifiers/SNMPostProcess.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCompositing {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Depth of field camera scene node post process modifier class
*
*  @remarks
*    The used scene renderer MUST support this effect by writing DOF data into the alpha
*    channel of the frame buffer this effect is using to combine the blurred and original
*    images.
*
*  @note
*    - The implementation is basing on "Advanced Depth of Field": http://ati.amd.com/developer/gdc/Scheuermann_DepthOfField.pdf
*    - Only the first instance of this modifier inside a camera is used
*/
class SNMPostProcessDepthOfField : public SNMPostProcess {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCOM_RTTI_EXPORT, SNMPostProcessDepthOfField, "PLCompositing", PLCompositing::SNMPostProcess, "Depth of field camera scene node post process modifier class")
		// Attributes
		pl_attribute(NearBlurDepth,		float,				0.15f,									ReadWrite,	DirectValue,	"Near blur depth",													"")
		pl_attribute(FocalPlaneDepth,	float,				0.25f,									ReadWrite,	DirectValue,	"Focal plane depth",												"")
		pl_attribute(FarBlurDepth,		float,				25.0f,									ReadWrite,	DirectValue,	"Far blur depth",													"")
		pl_attribute(BlurrinessCutoff,	float,				0.8f,									ReadWrite,	DirectValue,	"Blurriness cutoff constant for objects behind the focal plane",	"")
		pl_attribute(BloomScale,		float,				1.0f,									ReadWrite,	DirectValue,	"Bloom scale",														"")
		pl_attribute(Strength,			PLMath::Vector2,	PLMath::Vector2(4.0f, 4.0f),			ReadWrite,	DirectValue,	"Blur strength",													"")
			// Overloaded SNMPostProcess attributes
		pl_attribute(Filename,			PLCore::String,		"Data/PostProcesses/DepthOfField.pp",	ReadWrite,	ModifyAttr,		"Filename of the post process to use",								"Type='PostProcess'")
		// Constructors
		pl_constructor_1(ParameterConstructor,	PLScene::SceneNode&,	"Parameter constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cSceneNode
		*    Owner scene node
		*/
		PLCOM_API SNMPostProcessDepthOfField(PLScene::SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API virtual ~SNMPostProcessDepthOfField();


	//[-------------------------------------------------------]
	//[ Public virtual SNMPostProcess functions               ]
	//[-------------------------------------------------------]
	public:
		PLCOM_API virtual void SetParameters() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing


#endif // __PLSCENE_SCENENODEMODIFIER_DEPTHOFFIELD_H__
