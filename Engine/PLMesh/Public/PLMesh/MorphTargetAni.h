/*********************************************************\
 *  File: MorphTargetAni.h                               *
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


#ifndef __PLMESH_MORPHTARGETANI_H__
#define __PLMESH_MORPHTARGETANI_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Element.h>
#include <PLCore/Container/ElementManager.h>
#include <PLCore/Container/ElementHandler.h>
#include <PLRenderer/Animation/AnimationBase.h>
#include "PLMesh/PLMesh.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMesh {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Mesh;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Morph target animation class
*/
class MorphTargetAni : public PLRenderer::AnimationBase, public PLCore::Element<MorphTargetAni> {


	//[-------------------------------------------------------]
	//[ Public structures                                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Animation morph target
		*/
		class AniMorphTarget {


			//[-------------------------------------------------------]
			//[ Public data                                           ]
			//[-------------------------------------------------------]
			public:
				PLCore::String sName; /**< Morph target name */


			//[-------------------------------------------------------]
			//[ Public functions                                      ]
			//[-------------------------------------------------------]
			public:
				PLMESH_API AniMorphTarget()
				{
				}

				PLMESH_API ~AniMorphTarget()
				{
				}

				PLMESH_API bool operator ==(const AniMorphTarget &cAniMorphTarget) const
				{
					return sName == cAniMorphTarget.sName;
				}


		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sName
		*    Name
		*  @param[in] pManager
		*    Element manager using this element, can be a null pointer
		*
		*  @note
		*    - If you provide the pointer to the owner manager the morph target animation
		*      will check in the owner morph target animation manager name list
		*    - If the desired name is already in use inside the owner manager an alternative
		*      name is set
		*/
		PLMESH_API MorphTargetAni(const PLCore::String &sName, PLCore::ElementManager<MorphTargetAni> *pManager = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PLMESH_API virtual ~MorphTargetAni();

		/**
		*  @brief
		*    Returns the morph target animation morph targets
		*
		*  @return
		*    Morph target animation morph targets
		*/
		PLMESH_API PLCore::Array<AniMorphTarget> &GetMorphTargets();

		/**
		*  @brief
		*    Applies the morph target weight from the given morph target frame
		*    to the given morph target weights
		*
		*  @param[out] lstMorphTargetWeights
		*    Morph target weights to manipulate
		*  @param[in]  nFrame
		*    Morph target animation frame
		*  @param[in]  fWeight
		*    Weight (0.0-1.0)
		*  @param[in]  pMesh
		*    Mesh of the given morph target weights array, can be a null pointer
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - If pMesh is not a null pointer lstMorphTargetWeights is a list of ALL morph target influences
		*      of the given mesh, else lstMorphTargetWeights is only a list of the morph target
		*      weights of THIS animation!
		*/
		PLMESH_API bool ApplyMorphTargetWeight(PLCore::Array<float> &lstMorphTargetWeights,
											   PLCore::uint32 nFrame, float fWeight = 1.0f, const Mesh *pMesh = nullptr) const;

		/**
		*  @brief
		*    Applies the blended morph target weight from the given morph target frames
		*    to the given morph target weights
		*
		*  @param[out] lstMorphTargetWeights
		*    Morph target weights to manipulate
		*  @param[in]  nFrame1
		*    Morph target animation frame 1
		*  @param[in]  nFrame2
		*    Morph target animation frame 2
		*  @param[in]  fTime
		*    Morph target animation time (0.0-1.0)
		*  @param[in]  fWeight
		*    Weight (0.0-1.0)
		*  @param[in]  pMesh
		*    Mesh of the given morph target weights array, can be a null pointer
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @see
		*    - ApplyMorphTargetWeight() above
		*/
		PLMESH_API bool ApplyMorphTargetWeight(PLCore::Array<float> &lstMorphTargetWeights,
											   PLCore::uint32 nFrame1, PLCore::uint32 nFrame2, float fTime,
											   float fWeight = 1.0f, const Mesh *pMesh = nullptr) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::Array<AniMorphTarget> m_lstAniMorphTargets;


	//[-------------------------------------------------------]
	//[ Public virtual AnimationInfo functions                ]
	//[-------------------------------------------------------]
	public:
		PLMESH_API virtual MorphTargetAni &operator =(const MorphTargetAni &cSource);
		PLMESH_API virtual PLCore::String GetSourceName() const override;


};


/**
*  @brief
*    Morph target animation manager
*/
class MorphTargetAniManager : public PLCore::ElementManager<MorphTargetAni> {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLMESH_API MorphTargetAniManager();

		/**
		*  @brief
		*    Destructor
		*/
		PLMESH_API virtual ~MorphTargetAniManager();


	//[-------------------------------------------------------]
	//[ Private virtual PLCore::ElementManager functions      ]
	//[-------------------------------------------------------]
	private:
		virtual MorphTargetAni *CreateElement(const PLCore::String &sName) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh


#endif // __PLMESH_MORPHTARGETANI_H__
