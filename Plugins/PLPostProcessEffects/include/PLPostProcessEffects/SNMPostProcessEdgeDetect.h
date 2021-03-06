/*********************************************************\
 *  File: SNMPostProcessEdgeDetect.h                     *
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


#ifndef __PLPOSTPROCESSEFFECTS_EDGEDETECT_H__
#define __PLPOSTPROCESSEFFECTS_EDGEDETECT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector2.h>
#include <PLMath/Vector3.h>
#include <PLCompositing/SceneNodeModifiers/SNMPostProcess.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPostProcessEffects {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Edge detect camera scene node post process modifier class
*
*  @remarks
*    Edge detection.
*
*  @note
*    - For 'glowing' edges use the post process 'Data/PostProcesses/EdgeGlow.pp' together with this modifier
*/
class SNMPostProcessEdgeDetect : public PLCompositing::SNMPostProcess {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def()
		// Attributes
		pl_attribute_directvalue(								LuminanceConvert,	PLMath::Vector3,	PLMath::Vector3(0.2125f, 0.7154f, 0.0721f),	ReadWrite)
		pl_attribute_directvalue(								PixelKernel0,		PLMath::Vector2,	PLMath::Vector2( 0.0f,  1.0f),				ReadWrite)
		pl_attribute_directvalue(								PixelKernel1,		PLMath::Vector2,	PLMath::Vector2( 1.0f,  0.0f),				ReadWrite)
		pl_attribute_directvalue(								PixelKernel2,		PLMath::Vector2,	PLMath::Vector2( 0.0f, -1.0f),				ReadWrite)
		pl_attribute_directvalue(								PixelKernel3,		PLMath::Vector2,	PLMath::Vector2(-1.0f,  0.0f),				ReadWrite)
			// Overloaded SNMPostProcess attributes
		pl_attribute_modifyattr	(PLCompositing::SNMPostProcess,	Filename,			PLCore::String,		"Data/PostProcesses/EdgeDetect.pp",			ReadWrite)
	pl_class_def_end


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
		SNMPostProcessEdgeDetect(PLScene::SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SNMPostProcessEdgeDetect();


	//[-------------------------------------------------------]
	//[ Public virtual PLCompositing::SNMPostProcess functions ]
	//[-------------------------------------------------------]
	public:
		virtual void SetParameters() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPostProcessEffects


#endif // __PLPOSTPROCESSEFFECTS_EDGEDETECT_H__
