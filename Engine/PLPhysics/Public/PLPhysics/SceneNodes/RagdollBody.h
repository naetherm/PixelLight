/*********************************************************\
 *  File: RagdollBody.h                                  *
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


#ifndef __PLPHYSICS_SCENENODES_RAGDOLLBODY_H__
#define __PLPHYSICS_SCENENODES_RAGDOLLBODY_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector3.h>
#include <PLMath/Quaternion.h>
#include "PLPhysics/PLPhysics.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLGraphics {
	class Color4;
}
namespace PLRenderer {
	class Renderer;
}
namespace PLScene {
	class VisNode;
}
namespace PLPhysics {
	class Body;
	class SNRagdoll;
	class ElementHandler;
	class SCPhysicsWorld;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPhysics {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Ragdoll body
*
*  @note
*    - If you changed some of the initial settings you have to recreate
*      the physics object in order to see the changes.
*/
class RagdollBody {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pParent
		*    Parent ragdoll
		*/
		PLPHYSICS_API RagdollBody(SNRagdoll *pParent);

		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICS_API virtual ~RagdollBody();

		/**
		*  @brief
		*    Returns the PL physics body
		*
		*  @return
		*    The PL physics body, a null pointer on error (if that's the case, something went totally wrong :)
		*/
		PLPHYSICS_API Body *GetBody() const;

		/**
		*  @brief
		*    Sets the name of the body
		*
		*  @param[in] sName
		*    New body name
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*    (maybe this name is already used within the ragdoll?)
		*/
		PLPHYSICS_API bool SetName(const PLCore::String &sName);

		/**
		*  @brief
		*    Draws the physics body
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] cColor
		*    Color to use
		*  @param[in] cVisNode
		*    The current visibility node of this scene node
		*/
		PLPHYSICS_API void Draw(PLRenderer::Renderer &cRenderer, const PLGraphics::Color4 &cColor, const PLScene::VisNode &cVisNode) const;

		/**
		*  @brief
		*    Creates the physics body
		*
		*  @note
		*    - After this function the physics body is in it's initial pose
		*/
		PLPHYSICS_API void CreatePhysicsBody();

		/**
		*  @brief
		*    Destroys the physics body
		*/
		PLPHYSICS_API void DestroyPhysicsBody();

		/**
		*  @brief
		*    Returns the current rotation of the physics body
		*
		*  @param[out] qQ
		*    Will receive the current rotation of the physics body
		*/
		PLPHYSICS_API void GetRotation(PLMath::Quaternion &qQ) const;

		/**
		*  @brief
		*    Returns the current transform matrix of the physics body
		*
		*  @param[out] mTrans
		*    Will receive the current transform matrix of the physics body
		*/
		PLPHYSICS_API void GetTransformMatrix(PLMath::Matrix3x4 &mTrans) const;

		/**
		*  @brief
		*    Adds a force to the body
		*
		*  @param[in] vForce
		*    Force to add
		*/
		PLPHYSICS_API void AddForce(const PLMath::Vector3 &vForce);


	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		PLCore::uint32 nIndex;
		PLCore::String sName;

		PLCore::String sJoint;	/**< Joint this body is attached to */

		// Specified whether the body (and any possible joints its connected to) are enabled
		bool bEnabled;

		PLMath::Vector3 vSize;
		PLMath::Vector3 vPos;
		PLMath::Quaternion qRot;
		float fMass;

		SNRagdoll *m_pParentRagdoll;
		ElementHandler *m_pBodyHandler;	/**< Holds the PL physics body */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics


#endif // __PLPHYSICS_SCENENODES_RAGDOLLBODY_H__
