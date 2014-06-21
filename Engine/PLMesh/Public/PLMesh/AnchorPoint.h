/*********************************************************\
 *  File: AnchorPoint.h                                  *
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


#ifndef __PLMESH_ANCHORPOINT_H__
#define __PLMESH_ANCHORPOINT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Container/Element.h"
#include "PLCore/Container/ElementManager.h"
#include "PLCore/Container/ElementHandler.h"
#include "PLMesh/PLMesh.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLMesh {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class AnchorPointManager;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    An anchor points holds the index to a vertex or bone
*
*  @remarks
*    SceneNode is the best example for it's usage. You can create anchor points for a mesh in
*    the PixelLight mesh editor and if you want to get it's position in the world you just have
*    to call the function GetAnchorPoint("MyAnchorPoint"). With this anchor points you are able
*    to connect e.g. particle effects etc. to mesh vertices without to much effort.
*    Bone anchor points have the advantage that you will also receive the current bone rotation.
*/
class AnchorPoint : public PLCore::Element<AnchorPoint> {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sName
		*    Name of the anchor point
		*  @param[in] bType
		*    Anchor type (0=vertex  1=bone)
		*  @param[in] nID
		*    ID of the vertex/bone the anchor is attached to
		*  @param[in] pManager
		*    Anchor point manager using this element, can be a null pointer
		*
		*  @note
		*    - If you provide the pointer to the owner manager the anchor point
		*      will check in the anchor point manager name list
		*/
		PLMESH_API AnchorPoint(const PLCore::String &sName = "", bool bType = 0, PLCore::uint32 nID = 0, AnchorPointManager *pManager = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PLMESH_API virtual ~AnchorPoint();

		/**
		*  @brief
		*    Returns the anchor type
		*
		*  @return
		*    Anchor type (0=vertex  1=bone)
		*/
		PLMESH_API bool GetType() const;

		/**
		*  @brief
		*    Sets the anchor type
		*
		*  @param[in] bType
		*    Anchor type (0=vertex  1=bone)
		*/
		PLMESH_API void SetType(bool bType = 0);

		/**
		*  @brief
		*    Returns the anchors vertex/bone ID
		*
		*  @return
		*    ID of the vertex/bone the anchor is attached to
		*/
		PLMESH_API PLCore::uint32 GetID() const;

		/**
		*  @brief
		*    Sets the anchors vertex/bone ID
		*
		*  @param[in] nID
		*    ID of the vertex/bone the anchor is attached to
		*/
		PLMESH_API void SetID(PLCore::uint32 nID = 0);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool		   m_bType;	/**< Anchor point type (0=vertex  1=joint) */
		PLCore::uint32 m_nID;	/**< Vertex/bone ID the anchor is attached to */


	//[-------------------------------------------------------]
	//[ Public virtual PLCore::Element functions              ]
	//[-------------------------------------------------------]
	public:
		PLMESH_API virtual AnchorPoint &operator =(const AnchorPoint &cSource);


};

/**
*  @brief
*    Anchor point manager
*/
class AnchorPointManager : public PLCore::ElementManager<AnchorPoint> {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLMESH_API AnchorPointManager();

		/**
		*  @brief
		*    Destructor
		*/
		PLMESH_API virtual ~AnchorPointManager();


	//[-------------------------------------------------------]
	//[ Private virtual PLCore::ElementManager functions      ]
	//[-------------------------------------------------------]
	private:
		virtual AnchorPoint *CreateElement(const PLCore::String &sName) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMesh


#endif // __PLMESH_ANCHORPOINT_H__
