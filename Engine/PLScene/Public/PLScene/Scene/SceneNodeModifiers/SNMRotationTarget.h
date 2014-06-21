/*********************************************************\
 *  File: SNMRotationTarget.h                            *
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


#ifndef __PLSCENE_SCENENODEMODIFIER_ROTATIONTARGET_H__
#define __PLSCENE_SCENENODEMODIFIER_ROTATIONTARGET_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Event/EventHandler.h>
#include <PLMath/Vector3.h>
#include "PLScene/Scene/SceneNodeModifiers/SNMTransform.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class Renderer;
}
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
*    Scene node modifier class rotating a scene node towards a target scene node
*
*  @remarks
*    Rotates the scene node so it's always 'looking at' a given target scene node. This modifier doesn't
*    offer 'target offsets' because they can be easy and universal constructed by using for instance a 'SNMAnchor'
*    scene node modifier in combination with a 'SNHelper' scene node.
*/
class SNMRotationTarget : public SNMTransform {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SNMRotationTarget, "PLScene", PLScene::SNMTransform, "Scene node modifier class rotating a scene node towards a target scene node")
		// Attributes
		pl_attribute(Target,	PLCore::String,		"",						ReadWrite,	DirectValue,	"Target scene node to 'look at', if empty nothing happens",	"")
		pl_attribute(Offset,	PLMath::Vector3,	PLMath::Vector3::Zero,	ReadWrite,	DirectValue,	"Rotation offset to add in degree, [0, 360]",				"")
		pl_attribute(UpVector,	PLMath::Vector3,	PLMath::Vector3::UnitY,	ReadWrite,	DirectValue,	"Up vector",												"")
		// Constructors
		pl_constructor_1(ParameterConstructor,	SceneNode&,	"Parameter constructor",	"")
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
		PLS_API SNMRotationTarget(SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SNMRotationTarget();


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
		*    Returns the target position within the container space of the owner node
		*
		*  @param[out] vPos
		*    Will receive the position of the target scene node, not manipulated on error
		*
		*  @return
		*    'true' if all went fine, else 'false' (invalid target?)
		*/
		bool GetTargetPosition(PLMath::Vector3 &vPos) const;

		/**
		*  @brief
		*    Called on scene node debug draw
		*
		*  @param[in] cRenderer
		*    The used renderer
		*  @param[in] pVisNode
		*    The current visibility node of this scene node, can be a null pointer
		*/
		void OnDrawDebug(PLRenderer::Renderer &cRenderer, const VisNode *pVisNode);

		/**
		*  @brief
		*    Called on scene node position change or update request
		*/
		void OnPositionUpdate();


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<>										  EventHandlerPositionUpdate;
		PLCore::EventHandler<PLRenderer::Renderer &, const VisNode *> EventHandlerDrawDebug;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENENODEMODIFIER_ROTATIONTARGET_H__
