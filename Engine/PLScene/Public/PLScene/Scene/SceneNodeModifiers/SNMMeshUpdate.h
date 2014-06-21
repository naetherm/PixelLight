/*********************************************************\
 *  File: SNMMeshUpdate.h                                *
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


#ifndef __PLSCENE_SCENENODEMODIFIER_MESHUPDATE_H__
#define __PLSCENE_SCENENODEMODIFIER_MESHUPDATE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/AABoundingBox.h>
#include "PLScene/Scene/SceneNodeModifiers/SNMMesh.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class VisNode;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Mesh scene node modifier which performs frequent mesh updates
*
*  @note
*    - Usually, there should be only once instance of this scene node modifier per scene node to avoid multiple mesh updates per frame (performance!)
*/
class SNMMeshUpdate : public SNMMesh {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Flags (SceneNodeModifier flags extension)
		*/
		enum EFlags {
			UpdateUnseen = 1<<2	/**< Do also perform an update if the owner scene node is currently not seen */
		};
		pl_enum(EFlags)
			pl_enum_base(SceneNodeModifier::EFlags)
			pl_enum_value(UpdateUnseen, "Do also perform an update if the owner scene node is currently not seen")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SNMMeshUpdate, "PLScene", PLScene::SNMMesh, "Mesh scene node modifier which performs frequent mesh updates")
		// Attributes
			// Overwritten SceneNodeModifier attributes
		pl_attribute(Flags,	pl_flag_type(EFlags),	0,	ReadWrite,	GetSet,	"Flags",	"")
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
		PLS_API SNMMeshUpdate(SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SNMMeshUpdate();


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
		*    Called when the owner scene node was added to a visibility tree
		*
		*  @param[in] cVisNode
		*    Visibility node which is representing the owner scene node within the visibility tree
		*/
		void OnAddedToVisibilityTree(VisNode &cVisNode);

		/**
		*  @brief
		*    Called when the scene node modifier needs to be updated
		*/
		void OnUpdate();


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<VisNode &> EventHandlerAddedToVisibilityTree;
		PLCore::EventHandler<>			EventHandlerUpdate;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool				   m_bUpdate;						/**< Perform an update the next time? */
		bool				   m_bFirstUpdate;					/**< First update performed? */
		PLMath::AABoundingBox  m_cDefaultMeshAABoundingBox;		/**< Default mesh axis align bounding box */
		PLMath::AABoundingBox  m_cDefaultJointAABoundingBox;	/**< Default joint axis align bounding box */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENENODEMODIFIER_MESHUPDATE_H__
