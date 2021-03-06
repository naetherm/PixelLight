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
	pl_class_def()
		// Attributes
		pl_attribute_directvalue(								ColorScaleX,	PLMath::Vector3,	PLMath::Vector3( 2.0f,  2.0f,  2.0f),	ReadWrite)
		pl_attribute_directvalue(								ColorSpeedX,	PLMath::Vector3,	PLMath::Vector3( 1.0f,  1.0f,  1.0f),	ReadWrite)
		pl_attribute_directvalue(								ColorOffsetX,	PLMath::Vector3,	PLMath::Vector3(10.0f, 20.0f, 30.0f),	ReadWrite)
		pl_attribute_directvalue(								ColorScaleY,	PLMath::Vector3,	PLMath::Vector3( 2.0f,  2.0f,  2.0f),	ReadWrite)
		pl_attribute_directvalue(								ColorSpeedY,	PLMath::Vector3,	PLMath::Vector3( 2.0f,  2.5f,  3.0f),	ReadWrite)
		pl_attribute_directvalue(								ColorOffsetY,	PLMath::Vector3,	PLMath::Vector3(10.0f, 20.0f, 30.0f),	ReadWrite)
			// Overloaded SNMPostProcess attributes
		pl_attribute_modifyattr	(PLCompositing::SNMPostProcess,	Filename,		PLCore::String,		"Data/PostProcesses/CrazyColors.pp",	ReadWrite)
		// Slots
		pl_slot_0_def(SNMPostProcessCrazyColors,	OnSceneNodeUpdate)
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
