/*********************************************************\
 *  File: Octree.h                                       *
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


#ifndef __PLMATH_OCTREE_H__
#define __PLMATH_OCTREE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLMath/BoundingBox.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class Bitset;
}
namespace PLMath {
	class PlaneSet;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMath {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Base octree class
*
*  @remarks
*    Visualization of the coordinate system and were the octree
*    sectors are:
*    @code
*                             top _\                  back
*       y               ____________\__________________|
*       |                           |\ 0  \ 1  \       |
*       |                           | \----\----\
*       |                           |  \_3__\_2__\_____|
*       |________ x     ____________|  | 3  | 2  |     |
*       \                   bottom _\  |____|____|    front
*        \                           \ |    |    |
*         \                           \|_7__|_6__|
*         z                           ||         ||
*                                     ||         ||
*                                 left |         | right
*                                      |         |
*
*    Sector position description:
*           x       y         z
*     0.  left     top       back
*     1.  right    top       back
*     2.  right    top       front
*     3.  left     top       front
*     4.  left     bottom    back
*     5.  right    bottom    back
*     6.  right    bottom    front
*     7.  left     bottom    front
*
*    -x = left     +x = right
*    -y = bottom   +y = top
*    -z = back     +z = front
*    @endcode
*/
class Octree {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLMATH_API Octree();

		/**
		*  @brief
		*    Destructor
		*/
		PLMATH_API virtual ~Octree();

		/**
		*  @brief
		*    Returns whether the octree is build or not
		*
		*  @return
		*    'true' if the octree is build, else 'false'
		*/
		PLMATH_API bool IsBuild() const;

		/**
		*  @brief
		*    Initializes the octree
		*
		*  @param[in] pParent
		*    Parent octree, can be a null pointer
		*  @param[in] nSubdivide
		*    Subdivide
		*  @param[in] nMinGeometries
		*    Minimum geometries per octree
		*  @param[in] nIDOffset
		*    ID offset
		*
		*  @note
		*    - You should destroy the old octree before you initialize the new one!
		*/
		PLMATH_API void Init(Octree *pParent = nullptr, int nSubdivide = -1, PLCore::uint32 nMinGeometries = 0, PLCore::uint32 nIDOffset = 0);

		/**
		*  @brief
		*    Destroys the octree
		*/
		PLMATH_API void Destroy();

		/**
		*  @brief
		*    Returns the subdivision level
		*
		*  @return
		*    Subdivision level
		*/
		PLMATH_API PLCore::uint32 GetLevel() const;

		/**
		*  @brief
		*    Returns whether the octree is currently visible or not
		*
		*  @return
		*    'true' if the octree is visible, else 'false'
		*/
		PLMATH_API bool IsVisible() const;

		/**
		*  @brief
		*    Sets whether the octree is currently visible or not
		*
		*  @param[in] bVisible
		*    Should the octree be visible?
		*/
		PLMATH_API void SetVisible(bool bVisible = true);

		/**
		*  @brief
		*    Returns the octrees bounding box
		*
		*  @return
		*    Bounding box
		*/
		PLMATH_API const BoundingBox &GetBoundingBox() const;

		/**
		*  @brief
		*    Returns the subdivide
		*
		*  @return
		*    Subdivide
		*/
		PLMATH_API int GetSubdivide() const;

		/**
		*  @brief
		*    Returns the minimum number of geometries per octree
		*
		*  @return
		*    Minimum number of geometries per octree
		*/
		PLMATH_API PLCore::uint32 GetMinGeometries() const;

		/**
		*  @brief
		*    Returns the current position
		*
		*  @return
		*    Current position
		*/
		PLMATH_API const Vector3 &GetPos() const;

		/**
		*  @brief
		*    Set the current position
		*
		*  @param[in] vPos
		*    New position
		*/
		PLMATH_API void SetPos(const Vector3 &vPos);

		/**
		*  @brief
		*    Returns the current scale
		*
		*  @return
		*    Current scale
		*/
		PLMATH_API const Vector3 &GetScale() const;

		/**
		*  @brief
		*    Set the current scale
		*
		*  @param[in] vScale
		*    New scale
		*/
		PLMATH_API void SetScale(const Vector3 &vScale);

		/**
		*  @brief
		*    Returns the current rotation matrix
		*
		*  @return
		*    Current rotation matrix
		*/
		PLMATH_API const Matrix3x3 &GetRot() const;

		/**
		*  @brief
		*    Set the current rotation matrix
		*
		*  @param[in] mRot
		*    New rotation matrix
		*/
		PLMATH_API void SetRot(const Matrix3x3 &mRot);

		/**
		*  @brief
		*    Updates the visibility information of the octree
		*
		*  @param[in]  cPlaneSet
		*    Plane set to check
		*  @param[out] pBitset
		*    Optional (can be a null pointer) bit set which can for instance be filled in CustomVisible() and
		*    CustomInvisible() with current visibility information
		*
		*  @remarks
		*    The result must be handled in the derived octree class.
		*    The function it will call CustomVisible()/CustomInvisible()
		*    to mark an octree as visible or invisible.
		*/
		PLMATH_API void UpdateVisibility(const PlaneSet &cPlaneSet, PLCore::Bitset *pBitset = nullptr);

		/**
		*  @brief
		*    Check whether a sphere intersects with this octree
		*
		*  @param[in] vPos
		*    Center of the sphere
		*  @param[in] fRadius
		*    Radius of the sphere
		*
		*  @see
		*    - UpdateVisibility()
		*/
		PLMATH_API void CheckSphere(const Vector3 &vPos, float fRadius);

		/**
		*  @brief
		*    Check whether a box intersects with this octree
		*
		*  @param[in] cBox
		*    Box to test
		*
		*  @see
		*    - UpdateVisibility()
		*/
		PLMATH_API void CheckBox(const BoundingBox &cBox);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLCore::uint32	 m_nIDOffset;		/**< ID offset from parent (0-7) */
		PLCore::uint32   m_nID;				/**< Octree ID */
		Octree			*m_pTopmost;		/**< Topmost octree (always valid!) */
		Octree			*m_pParent;			/**< Octree parent, can be a null pointer */
		PLCore::uint32	 m_nLevel;			/**< Node level (0 = topmost) */
		PLCore::uint32   m_nNumOfChildren;	/**< Number of children */
		Octree		   **m_ppChild;			/**< The children, can be a null pointer */
		BoundingBox		 m_cBoundingBox;	/**< Octree bounding box */
		Vector3			 m_vPos;			/**< World position (m_vPos+m_vBBCenter = real world position) */
		Vector3			 m_vBBCenter;		/**< Original bounding box center */
		bool			 m_bVisible;		/**< Is the octree visible? */
		bool			 m_bBuild;			/**< is the octree build? */
		int				 m_nSubdivide;		/**< Subdivide */
		PLCore::uint32   m_nMinGeometries;	/**< Minimum number of geometries per octree */


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		Octree(const Octree &cSource);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		Octree &operator =(const Octree &cSource);


	//[-------------------------------------------------------]
	//[ Private virtual Octree functions                      ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Custom visible function
		*
		*  @param[out] pBitset
		*    Optional bit set, can be a null pointer
		*
		*  @note
		*    - Is called during UpdateVisibility() if the octree is visible
		*/
		PLMATH_API virtual void CustomVisible(PLCore::Bitset *pBitset = nullptr);

		/**
		*  @brief
		*    Destructor
		*
		*  @param[out] pBitset
		*    Optional bit set, can be a null pointer
		*
		*  @note
		*    - Is called during UpdateVisibility() if the octree is invisible
		*/
		PLMATH_API virtual void CustomInvisible(PLCore::Bitset *pBitset = nullptr);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath


#endif // __PLMATH_OCTREE_H__
