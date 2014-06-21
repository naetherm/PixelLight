/*********************************************************\
 *  File: GMMSurfacePatch.h                              *
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


#ifndef __PLNATURE_SCENENODE_GMMSURFACEPATCH_H__
#define __PLNATURE_SCENENODE_GMMSURFACEPATCH_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Quadtree.h>
#include "PLNature/PLNature.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLNature {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class GMMSurface;
class GMMSurfacePatch;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    GMM surface patch
*/
class GMMSurfacePatch : public PLMath::QuadtreePatch {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class GMMSurface;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns whether the patch is active or not
		*
		*  @return
		*    'true' if the patch is active, else 'false'
		*/
		PLNATURE_API bool IsActive() const;

		/**
		*  @brief
		*    Sets whether the patch is active or not
		*
		*  @param[in] bActive
		*    Should the patch be active?
		*/
		PLNATURE_API void SetActive(bool bActive = true);

		/**
		*  @brief
		*    Computes the variance of each mip map level
		*
		*  @param[out] nOffset
		*    Will receive the offset
		*/
		PLNATURE_API void ComputeVariance(PLCore::uint32 &nOffset);

		/**
		*  @brief
		*    Updates the patch bounding box
		*/
		PLNATURE_API void UpdateBoundingBox();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cGMMSurface
		*    Owner GMM surface of this patch
		*/
		GMMSurfacePatch(GMMSurface &cGMMSurface);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GMMSurfacePatch();

		/**
		*  @brief
		*    Initializes the GMM surface patch
		*/
		void Init();

		/**
		*  @brief
		*    Creates the GMM surface patch
		*
		*  @param[in] nXOffset
		*    X offset of this patch in the height map
		*  @param[in] nYOffset
		*    Y offset of this patch in the height map
		*  @param[in] nOffset
		*    Current offset (is increased inside the function)
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool Create(PLCore::uint32 nXOffset, PLCore::uint32 nYOffset, PLCore::uint32 &nOffset);

		/**
		*  @brief
		*    Kills the GMM surface patch
		*/
		void Destroy();

		/**
		*  @brief
		*    Updates the GMM surface patch
		*
		*  @note
		*    - Must be done before the GMM surface patch is drawn!!
		*/
		void Update();

	
	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool			 m_bActive;			/**< Is the patch active? */
		PLCore::uint32	 m_nLevel;			/**< Current geo mip map level */
		PLMath::Vector3	 m_vCenter;			/**< Patch center */
		PLCore::uint32	 m_nOffset;			/**< Surface vertex offset */
		PLCore::uint32	 m_nXOffset;		/**< X offset in the height map */
		PLCore::uint32	 m_nYOffset;		/**< Y offset in the height map */
		float			*m_pVariance;		/**< Height variance for each level, can be a null pointer */
		GMMSurface		*m_pGMMSurface;		/**< Owner GMM surface of this patch (always valid!) */
		GMMSurfacePatch	*m_pNeighbour[4];	/**< Neighbor patches, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLNature


#endif // __PLNATURE_SCENENODE_GMMSURFACEPATCH_H__
