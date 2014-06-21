/*********************************************************\
 *  File: SNMPostProcessCrazyColors.h                    *
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


#ifndef __PLPOSTPROCESSEFFECTS_CRAZYCOLORS_H__
#define __PLPOSTPROCESSEFFECTS_CRAZYCOLORS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
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
*    Crazy colors camera scene node post process modifier class
*
*  @remarks
*    Crazy colors.
*/
class SNMPostProcessCrazyColors : public PLCompositing::SNMPostProcess {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(pl_rtti_export, SNMPostProcessCrazyColors, "PLPostProcessEffects", PLCompositing::SNMPostProcess, "")
		// Attributes
		pl_attribute(ColorScaleX,	PLMath::Vector3,	PLMath::Vector3( 2.0f,  2.0f,  2.0f),	ReadWrite,	DirectValue,	"Color scale x",						"")
		pl_attribute(ColorSpeedX,	PLMath::Vector3,	PLMath::Vector3( 1.0f,  1.0f,  1.0f),	ReadWrite,	DirectValue,	"Color speed x",						"")
		pl_attribute(ColorOffsetX,	PLMath::Vector3,	PLMath::Vector3(10.0f, 20.0f, 30.0f),	ReadWrite,	DirectValue,	"Color offset x",						"")
		pl_attribute(ColorScaleY,	PLMath::Vector3,	PLMath::Vector3( 2.0f,  2.0f,  2.0f),	ReadWrite,	DirectValue,	"Color scale y",						"")
		pl_attribute(ColorSpeedY,	PLMath::Vector3,	PLMath::Vector3( 2.0f,  2.5f,  3.0f),	ReadWrite,	DirectValue,	"Color speed y",						"")
		pl_attribute(ColorOffsetY,	PLMath::Vector3,	PLMath::Vector3(10.0f, 20.0f, 30.0f),	ReadWrite,	DirectValue,	"Color offset y",						"")
			// Overloaded SNMPostProcess attributes
		pl_attribute(Filename,		PLCore::String,		"Data/PostProcesses/CrazyColors.pp",	ReadWrite,	ModifyAttr,		"Filename of the post process to use",	"Type='PostProcess'")
		// Constructors
		pl_constructor_1(ParameterConstructor,	PLScene::SceneNode&,	"Parameter constructor",	"")
		// Slots
		pl_slot_0(OnSceneNodeUpdate,	"Slot for SceneNode::EventUpdate",	"")
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
		SNMPostProcessCrazyColors(PLScene::SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SNMPostProcessCrazyColors();


	//[-------------------------------------------------------]
	//[ Private virtual PLScene::SceneNodeModifier functions  ]
	//[-------------------------------------------------------]
	private:
		virtual void OnActivate(bool bActivate) override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when the scene node needs to be updated
		*/
		void OnSceneNodeUpdate();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Private data
		float m_fTimer;


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


#endif // __PLPOSTPROCESSEFFECTS_CRAZYCOLORS_H__
