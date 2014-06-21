/*********************************************************\
 *  File: SNMMeshMorphCameraNear.h                       *
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


#ifndef __PLSCENE_SCENENODEMODIFIER_MESHMORPHCAMERANEAR_H__
#define __PLSCENE_SCENENODEMODIFIER_MESHMORPHCAMERANEAR_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLScene/Scene/SceneNodeModifiers/SNMMeshMorph.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Animated scene node mesh morph modifier class
*
*  @remarks
*    This modifier can be used for a mesh morph target like 'smile' if the camera is near to the scene node.
*/
class SNMMeshMorphCameraNear : public SNMMeshMorph {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SNMMeshMorphCameraNear, "PLScene", PLScene::SNMMeshMorph, "Animated scene node mesh morph modifier class")
		// Attributes
		pl_attribute(Joint,		PLCore::String,	"",		ReadWrite,	DirectValue,	"Name of the skeleton joint to use as reference point, if not given, the scene node position is used instead",	"")
		pl_attribute(Distance,	float,			1.0f,	ReadWrite,	DirectValue,	"Camera near distance",																							"")
		pl_attribute(InSpeed,	float,			4.0f,	ReadWrite,	DirectValue,	"Blend in speed",																								"")
		pl_attribute(OutSpeed,	float,			2.0f,	ReadWrite,	DirectValue,	"Blend out speed",																								"")
		// Constructors
		pl_constructor_1(ParameterConstructor,	SceneNode&,	"Parameter constructor",	"")
		// Slots
		pl_slot_0(OnUpdate,	"Called when the scene node modifier needs to be updated",	"")
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
		PLS_API SNMMeshMorphCameraNear(SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SNMMeshMorphCameraNear();


	//[-------------------------------------------------------]
	//[ Protected virtual SceneNodeModifier functions         ]
	//[-------------------------------------------------------]
	protected:
		PLS_API virtual void OnActivate(bool bActivate) override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when the scene node modifier needs to be updated
		*/
		void OnUpdate();


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENENODEMODIFIER_MESHMORPHCAMERANEAR_H__
