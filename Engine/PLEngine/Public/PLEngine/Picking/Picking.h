/*********************************************************\
 *  File: Picking.h                                      *
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


#ifndef __PLENGINE_PICKING_H__
#define __PLENGINE_PICKING_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Renderer/Types.h>
#include "PLEngine/PLEngine.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	template <class ValueType> class Array;
}
namespace PLMath {
	class Vector3;
}
namespace PLScene {
	class SceneNode;
	class SceneQuery;
	class SceneContainer;
}
namespace PLEngine {
	class PickingResult;
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
*    Class offering scene picking (some also call it 'trace line') functionality
*/
class Picking {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PL_API Picking();

		/**
		*  @brief
		*    Destructor
		*/
		PL_API virtual ~Picking();

		/**
		*  @brief
		*    Performs picking by using the given line start and end positions
		*
		*  @param[out] cPickingResult
		*    Receives the picking result if all went fine
		*  @param[in]  cContainer
		*    Scene container we're performing the picking in
		*  @param[in]  vLineStartPos
		*    Picking line start position within the given scene container
		*  @param[in]  vLineEndPos
		*    Picking line end position within the given scene container
		*  @param[in]  nCull
		*    Cull mode (see "PLRenderer::Cull")
		*
		*  @return
		*    'true' if anything has been picked, else 'false'
		*/
		PL_API bool PerformPicking(PickingResult &cPickingResult, PLScene::SceneContainer &cContainer, const PLMath::Vector3 &vLineStartPos, const PLMath::Vector3 &vLineEndPos, PLRenderer::Cull::Enum nCull = PLRenderer::Cull::CCW);

		/**
		*  @brief
		*    Performs picking using the mesh of the given scene node by using the given line start and end positions
		*
		*  @param[out] cPickingResult
		*    Receives the picking result if all went fine
		*  @param[in]  cSceneNode
		*    Scene node we're performing the mesh picking in
		*  @param[in]  vLineStartPos
		*    Picking line start position within the given scene node
		*  @param[in]  vLineEndPos
		*    Picking line end position within the given scene node
		*  @param[in]  plstGeometries
		*    List of mesh geometry indices to use, if a null pointer all mesh geometries are used
		*  @param[in]  nCull
		*    Cull mode (see "PLRenderer::Cull")
		*
		*  @return
		*    'true' if anything has been picked, else 'false'
		*/
		PL_API bool PerformPicking(PickingResult &cPickingResult, PLScene::SceneNode &cSceneNode, const PLMath::Vector3 &vLineStartPos, const PLMath::Vector3 &vLineEndPos,
								   PLCore::Array<PLCore::uint32> *plstGeometries = nullptr, PLRenderer::Cull::Enum nCull = PLRenderer::Cull::CCW);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when a scene node was found
		*
		*  @param[in] cQuery
		*    Query found the scene node
		*  @param[in] cSceneNode
		*    Found scene node
		*/
		void OnSceneNode(PLScene::SceneQuery &cQuery, PLScene::SceneNode &cSceneNode);

		/**
		*  @brief
		*    Perform mesh intersection
		*
		*  @param[in]  cSceneNode
		*    Scene node we're performing the mesh picking in
		*  @param[in]  vLineStartPos
		*    Picking line start position within the given scene node
		*  @param[in]  vLineEndPos
		*    Picking line end position within the given scene node
		*  @param[in]  plstGeometries
		*    List of mesh geometry indices to use, if a null pointer all mesh geometries are used
		*  @param[in]  nCull
		*    Cull mode (see "PLRenderer::Cull")
		*/
		void MeshIntersection(PLScene::SceneNode &cSceneNode, const PLMath::Vector3 &vLineStartPos, const PLMath::Vector3 &vLineEndPos,
							  PLCore::Array<PLCore::uint32> *plstGeometries = nullptr, PLRenderer::Cull::Enum nCull = PLRenderer::Cull::CCW);


	//[-------------------------------------------------------]
	//[ Protected virtual Picking functions                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Function that is called when a picking candidate has been found ("picking filter function")
		*
		*  @param[in] cSceneNode
		*    Scene node candidate
		*
		*  @return
		*    'true' to take this candidate into account, else 'false' to throw it away (no further processing of the scene node)
		*
		*  @note
		*    - The default implementation is empty
		*/
		PL_API virtual bool OnPickingCandidate(PLScene::SceneNode &cSceneNode);


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<PLScene::SceneQuery &, PLScene::SceneNode &> EventHandlerSceneNode;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PickingResult *m_pPickingResult;	/**< Current picking result, always valid! (used inside 'OnSceneNode()') */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine


#endif // __PLENGINE_PICKING_H__
