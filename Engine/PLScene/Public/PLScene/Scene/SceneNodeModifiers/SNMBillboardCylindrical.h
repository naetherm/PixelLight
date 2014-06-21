/*********************************************************\
 *  File: SNMBillboardCylindrical.h                      *
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


#ifndef __PLSCENE_SCENENODEMODIFIER_BILLBOARDCYLINDRICAL_H__
#define __PLSCENE_SCENENODEMODIFIER_BILLBOARDCYLINDRICAL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector3.h>
#include "PLScene/Scene/SceneNodeModifiers/SNMBillboard.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLScene {
	class VisNode;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Cylindrical (axis aligned) billboard scene node modifier class
*
*  @note
*    - The scale of the owner scene node is taken into account
*/
class SNMBillboardCylindrical : public SNMBillboard {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SNMBillboardCylindrical, "PLScene", PLScene::SNMBillboard, "Cylindrical (axis aligned) billboard scene node modifier class")
		// Attributes
		pl_attribute(UpVector,	PLMath::Vector3,	PLMath::Vector3::UnitY,	ReadWrite,	DirectValue,	"Up vector",	"")
		// Constructors
		pl_constructor_1(ParameterConstructor,	SceneNode&,	"Parameter constructor",	"")
		// Slots
		pl_slot_1(OnAddedToVisibilityTree,	VisNode&,	"Called when the owner scene node was added to a visibility tree, visibility node which is representing the owner scene node within the visibility tree as first parameter",	"")
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
		PLS_API SNMBillboardCylindrical(SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SNMBillboardCylindrical();


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
		*    Builds a transform matrix
		*
		*  @param[in]  mView
		*    View matrix
		*  @param[in]  mWorld
		*    World matrix
		*  @param[out] mTransform
		*    Receives the new world transform matrix
		*/
		void BuildTransformMatrix(const PLMath::Matrix4x4 &mView, const PLMath::Matrix4x4 &mWorld, PLMath::Matrix3x4 &mTransform);

		/**
		*  @brief
		*    Called when the owner scene node was added to a visibility tree
		*
		*  @param[in] cVisNode
		*    Visibility node which is representing the owner scene node within the visibility tree
		*/
		void OnAddedToVisibilityTree(VisNode &cVisNode);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENENODEMODIFIER_BILLBOARDCYLINDRICAL_H__
