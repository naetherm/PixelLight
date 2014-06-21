/*********************************************************\
 *  File: SkeletonHandler.h                              *
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


#ifndef __PLMESH_SKELETONHANDLER_H__
#define __PLMESH_SKELETONHANDLER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Array.h>
#include <PLCore/Container/ResourceHandler.h>
#include "PLMesh/Joint.h"
#include "PLMesh/Skeleton.h"
#include "PLMesh/JointHandler.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLGraphics {
	class Color4;
}
namespace PLRenderer {
	class Font;
	class Renderer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMesh {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Skeleton handler code
*
*  @remarks
*  @verbatim
*    Skeleton handler animation example:\n
*    ResetJointStates();           // Reset current joint states\n
*    ApplyJointStates(2, 3, 0.5f); // Apply frame 2.5 of the skeleton animation to the current joint states\n
*    ApplyJointStates(cRun);       // Apply the current joint states of the skeleton animation handler\n
*                                  // cRun to the current joint states. Note that we want to add the delta \n
*                                  // values from cRun, so, within this handler the base joint states are NOT added!\n
*    ApplyBaseJointStates();       // Apply base joint states to get the final pose\n
*    CalculateStates();            // Complete the current joint states
*  @endverbatim
*/
class SkeletonHandler : public PLCore::ResourceHandler<Skeleton> {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLMESH_API SkeletonHandler();

		/**
		*  @brief
		*    Destructor
		*/
		PLMESH_API virtual ~SkeletonHandler();

		/**
		*  @brief
		*    Draws the joints
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] cColor
		*    Color to use
		*  @param[in] mWorldViewProjection
		*    World view projection matrix to use
		*  @param[in] fPointSize
		*    Point size
		*/
		PLMESH_API void DrawJoints(PLRenderer::Renderer &cRenderer, const PLGraphics::Color4 &cColor, const PLMath::Matrix4x4 &mWorldViewProjection, float fPointSize = 1.0f) const;

		/**
		*  @brief
		*    Draws the joint names
		*
		*  @param[in] cFont
		*    Font to use
		*  @param[in] cColor
		*    Color to use
		*  @param[in] mWorldViewProjection
		*    World view projection matrix to use
		*/
		PLMESH_API void DrawJointNames(PLRenderer::Font &cFont, const PLGraphics::Color4 &cColor, const PLMath::Matrix4x4 &mWorldViewProjection) const;

		/**
		*  @brief
		*    Draws the skeleton
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] cColor
		*    Color to use
		*  @param[in] mWorldViewProjection
		*    World view projection matrix to use
		*  @param[in] fLineWidth
		*    Line width
		*/
		PLMESH_API void DrawSkeleton(PLRenderer::Renderer &cRenderer, const PLGraphics::Color4 &cColor, const PLMath::Matrix4x4 &mWorldViewProjection, float fLineWidth = 1.0f) const;

		//[-------------------------------------------------------]
		//[ Joint states                                          ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the current joint handlers
		*
		*  @return
		*    Current joint handlers
		*/
		PLMESH_API PLCore::Array<JointHandler> &GetJointHandlers();
		PLMESH_API const PLCore::Array<JointHandler> &GetJointHandlers() const;

		/**
		*  @brief
		*    Returns a joint handler by name
		*
		*  @param[in] sJointName
		*    Name of the joint to return the handler from
		*
		*  @return
		*    Joint handler, a null pointer on error
		*/
		PLMESH_API JointHandler *GetJointHandler(const PLCore::String &sJointName) const;

		/**
		*  @brief
		*    Returns the base joint handlers
		*
		*  @return
		*    Base joint handlers
		*/
		PLMESH_API PLCore::Array<JointHandler> &GetBaseJointHandlers();

		/**
		*  @brief
		*    Returns a base joint handler by name
		*
		*  @param[in] sJointName
		*    Name of the base joint to return the handler from
		*
		*  @return
		*    Base joint handler, a null pointer on error
		*/
		PLMESH_API JointHandler *GetBaseJointHandler(const PLCore::String &sJointName) const;

		/**
		*  @brief
		*    Resets all current joint states
		*
		*  @param[in] bForceAll
		*    If 'true', user controlled joints are reset, too
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe no skeleton set?)
		*/
		PLMESH_API bool ResetJointStates(bool bForceAll = false);

		/**
		*  @brief
		*    Resets all base joint states to the skeleton 'orginal' base joint states
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe no skeleton set?)
		*/
		PLMESH_API bool ResetBaseJointStates();

		/**
		*  @brief
		*    Applies the joint states from the given skeleton animation frame
		*    to the current joint states
		*
		*  @param[in] nFrame
		*    Skeleton animation frame
		*  @param[in] fWeight
		*    Weight (0.0-1.0)
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe no skeleton set?)
		*/
		PLMESH_API bool ApplyJointStates(PLCore::uint32 nFrame, float fWeight = 1.0f);

		/**
		*  @brief
		*    Applies the blended joint states from the given joint animation frames
		*    to the current joint states
		*
		*  @param[in] nFrame1
		*    Skeleton animation frame 1
		*  @param[in] nFrame2
		*    Skeleton animation frame 2
		*  @param[in] fTime
		*    Skeleton animation time (0.0-1.0)
		*  @param[in] fWeight
		*    Weight (0.0-1.0)
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe no skeleton set?)
		*/
		PLMESH_API bool ApplyJointStates(PLCore::uint32 nFrame1, PLCore::uint32 nFrame2, float fTime, float fWeight = 1.0f);

		/**
		*  @brief
		*    Applies the current joint states from the given joint handler
		*    to the current joint states
		*
		*  @param[in] cSkeletonHandler
		*    Skeleton handler to use
		*  @param[in] fWeight
		*    Weight (0.0-1.0)
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe no skeleton set?)
		*
		*  @note
		*    - In order to apply the pSkeletonHandler joint states correctly, it should
		*      only consist of deltas. (ApplyBaseJointStates() not done...)
		*/
		PLMESH_API bool ApplyJointStates(SkeletonHandler &cSkeletonHandler, float fWeight = 1.0f);

		/**
		*  @brief
		*    Applies the base joint states to the current joint states
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe no skeleton set?)
		*/
		PLMESH_API bool ApplyBaseJointStates();

		/**
		*  @brief
		*    Calculates all current absolute joint states
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe no skeleton set?)
		*
		*  @note
		*    - Before applying this skeleton handler on for instance a mesh you have to call
		*      this function to calculate all current absolute joint states!
		*/
		PLMESH_API bool CalculateStates();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Recursive joint state calculations
		*
		*  @param[in] cJointHandler
		*    Current joint handler to process
		*  @param[in] pParentJointHandler
		*    Current parent joint handler to process, can be a null pointer
		*/
		void RecCalculateStates(JointHandler &cJointHandler, JointHandler *pParentJointHandler = nullptr);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::Array<JointHandler> m_lstBaseJointHandlers;	/**< Base joint handlers */
		PLCore::Array<JointHandler> m_lstJointHandlers;		/**< Current joint handlers */


	//[-------------------------------------------------------]
	//[ Public virtual PLCore::ResourceHandler functions      ]
	//[-------------------------------------------------------]
	public:
		PLMESH_API virtual bool SetResource(Skeleton *pResource = nullptr) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh


#endif // __PLMESH_SKELETONHANDLER_H__
