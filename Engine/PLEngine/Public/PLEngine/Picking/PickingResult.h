/*********************************************************\
 *  File: PickingResult.h                                *
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


#ifndef __PLENGINE_PICKINGRESULT_H__
#define __PLENGINE_PICKINGRESULT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector3.h>
#include <PLCore/Base/Event/EventHandler.h>
#include "PLEngine/PLEngine.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLScene {
	class SceneNode;
	class SceneContainer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLEngine {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Class receiving the scene picking result and offering further functionality
*/
class PickingResult {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Picking;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PL_API PickingResult();

		/**
		*  @brief
		*    Destructor
		*/
		PL_API ~PickingResult();

		//[-------------------------------------------------------]
		//[ Picking result access                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the picked scene node
		*
		*  @return
		*    The picked scene node, a null pointer on error
		*/
		PL_API PLScene::SceneNode *GetSceneNode() const;

		/**
		*  @brief
		*    Returns the picked geometry
		*
		*  @return
		*    The picked geometry (if there's one)
		*/
		PL_API PLCore::uint32 GetGeometry() const;

		/**
		*  @brief
		*    Returns the picked triangle
		*
		*  @return
		*    The picked triangle (if there's one)
		*/
		PL_API PLCore::uint32 GetTriangle() const;

		/**
		*  @brief
		*    Returns the picked point in node space
		*
		*  @return
		*    The picked point in node space (if there's one)
		*/
		PL_API const PLMath::Vector3 &GetPoint() const;

		/**
		*  @brief
		*    Returns the scene container the picking line and distance is in
		*
		*  @return
		*    The scene container the picking line and distance is in, a null pointer on error (in this case result is scene node space)
		*/
		PL_API PLScene::SceneContainer *GetSceneContainer() const;

		/**
		*  @brief
		*    Returns the picking line start position
		*
		*  @return
		*    The picking line start position
		*
		*  @see
		*    - GetSceneContainer()
		*/
		PL_API const PLMath::Vector3 &GetLineStartPos() const;

		/**
		*  @brief
		*    Returns the nearest squared distance to the line start position
		*
		*  @return
		*    The nearest squared distance to the line start position
		*
		*  @see
		*    - GetSceneContainer()
		*
		*  @note
		*    - Use for example 'Math::Sqrt(cMyPicking.GetNearestSquaredDistance())' to
		*      get the actual nearest distance if you really need it
		*/
		PL_API float GetNearestSquaredDistance() const;

		//[-------------------------------------------------------]
		//[ Further functionality                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns the currently picked texture coordinate (can also be outside the 0..1 interval)
		*
		*  @param[out] vTexCoord
		*    Receives the picked texture coordinate if all went fine
		*  @param[in]  nTexCoordChannel
		*    Texture coordinate channel to use
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PL_API bool GetTextureCoordinate(PLMath::Vector2 &vTexCoord, PLCore::uint32 nTexCoordChannel = 0) const;

		/**
		*  @brief
		*    Returns the currently picked wrapped texture coordinate (always between 0..1)
		*
		*  @param[out] vTexCoord
		*    Receives the picked wrapped texture coordinate if all went fine
		*  @param[in]  nTexCoordChannel
		*    Texture coordinate channel to use
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PL_API bool GetWrappedTextureCoordinate(PLMath::Vector2 &vTexCoord, PLCore::uint32 nTexCoordChannel = 0) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLScene::SceneNode		*m_pSceneNode;				/**< Current picked scene node, can be a null pointer */
		PLCore::uint32			 m_nGeometry;				/**< Picked geometry */
		PLCore::uint32			 m_nTriangle;				/**< Picked triangle */
		PLMath::Vector3			 m_vPoint;					/**< Picked point in node space */
		PLScene::SceneContainer	*m_pSceneContainer;			/**< Scene container our line start position and squared distance are in, can be a null pointer */
		PLMath::Vector3			 m_vLineStartPos;			/**< Line start position, see 'm_pSceneContainer' */
		float					 m_fNearestSquaredDistance;	/**< Nearest squared distance to the line start position, less 0 means 'not initialized yet', see 'm_pSceneContainer' */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine


#endif // __PLENGINE_PICKING_H__
