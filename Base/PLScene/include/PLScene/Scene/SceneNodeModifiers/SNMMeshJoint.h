/*********************************************************\
 *  File: SNMMeshJoint.h                                 *
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


#ifndef __PLSCENE_SCENENODEMODIFIER_MESHJOINT_H__
#define __PLSCENE_SCENENODEMODIFIER_MESHJOINT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector3.h>
#include "PLScene/Scene/SceneNodeModifiers/SNMMesh.h"


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
*    Mesh scene node joint modifier class
*
*  @note
*    - Adds automatically a "PLScene::SNMMeshUpdate" scene node modifier (if there's not yet one) in order to frequently update the mesh
*    - If the node has no mesh with a skeleton, this modifier will have no effect
*/
class SNMMeshJoint : public SNMMesh {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Flags (SceneNodeModifier flags extension)
		*/
		enum EFlags {
			UseFallbackRotation = 1<<2	/**< Use fallback rotation if the target is 'out of rotation range' */
		};
		pl_flag(EFlags)
			pl_enum_base(SNMMesh::EFlags)
			pl_enum_value(UseFallbackRotation, "Use fallback rotation if the target is 'out of rotation range'")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def(PLS_API)
		// Attributes
		pl_attribute_getset		(SNMMeshJoint,		Name,				PLCore::String,		"",						ReadWrite)
		pl_attribute_getset		(SNMMeshJoint,		Rotation,			PLMath::Vector3,	PLMath::Vector3::Zero,	ReadWrite)
		pl_attribute_directvalue(					RotationFrom,		PLCore::String,		"",						ReadWrite)
		pl_attribute_getset		(SNMMeshJoint,		Min,				PLMath::Vector3,	PLMath::Vector3::Zero,	ReadWrite)
		pl_attribute_getset		(SNMMeshJoint,		Max,				PLMath::Vector3,	PLMath::Vector3::Zero,	ReadWrite)
		pl_attribute_directvalue(					Speed,				float,				5.0f,					ReadWrite)
		pl_attribute_directvalue(					MaxDifference,		float,				50.0f,					ReadWrite)
		pl_attribute_directvalue(					FallbackRotation,	PLMath::Vector3,	PLMath::Vector3::Zero,	ReadWrite)
			// Overwritten SceneNodeModifier attributes
		pl_attribute_getset		(SNMMeshJoint,		Flags,				PLCore::uint32,		0,						ReadWrite)
		// Slots
		pl_slot_0_def(SNMMeshJoint,	OnUpdate)
		pl_slot_2_def(SNMMeshJoint,	OnDrawDebug,	PLRenderer::Renderer&,	const VisNode*)
	pl_class_def_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLS_API PLCore::String GetName() const;
		PLS_API void SetName(const PLCore::String &sValue);
		PLS_API const PLMath::Vector3 &GetRotation() const;
		PLS_API void SetRotation(const PLMath::Vector3 &vValue);
		PLS_API const PLMath::Vector3 &GetMin() const;
		PLS_API void SetMin(const PLMath::Vector3 &vValue);
		PLS_API const PLMath::Vector3 &GetMax() const;
		PLS_API void SetMax(const PLMath::Vector3 &vValue);


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
		PLS_API SNMMeshJoint(SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SNMMeshJoint();


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
		*    Returns the target rotation within the container space of the owner node
		*
		*  @param[out] qRot
		*    Will receive the rotation of the target scene node
		*
		*  @return
		*    'true' if all went fine, else 'false' (invalid target?)
		*/
		bool GetTargetRotation(PLMath::Quaternion &qRot) const;

		/**
		*  @brief
		*    Updates the joint
		*/
		void UpdateJoint();

		/**
		*  @brief
		*    Called when the scene node modifier needs to be updated
		*/
		void OnUpdate();

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


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::String  m_sName;		/**< Name of the joint to modify */
		PLMath::Vector3 m_vRotation;	/**< Joint rotation in degree (joint space) */
		PLMath::Vector3 m_vMin;			/**< Minimum joint rotation in in degree, if min = max -> no limitation, per component */
		PLMath::Vector3 m_vMax;			/**< Maximum joint rotation in in degree, if min = max -> no limitation, per component */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENENODEMODIFIER_MESHJOINT_H__
