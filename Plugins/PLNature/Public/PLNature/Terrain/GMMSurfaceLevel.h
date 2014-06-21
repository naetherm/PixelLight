/*********************************************************\
 *  File: GMMSurfaceLevel.h                              *
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


#ifndef __PLNATURE_SCENENODE_GMMSURFACELEVEL_H__
#define __PLNATURE_SCENENODE_GMMSURFACELEVEL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLNature/PLNature.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class Renderer;
	class IndexBuffer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLNature {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    GMM surface geometrically mip mapping level
*/
class GMMSurfaceLevel {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class GMMSurface;


	//[-------------------------------------------------------]
	//[ Public structures                                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    GMM surface level body piece
		**/
		struct BodyPiece {
			PLCore::uint32			*pnIndexBuffer;			/**< Can be a null pointer */
			PLCore::uint32			 nIndexBufferSize;
			PLRenderer::IndexBuffer *pIndexBuffer;			/**< Can be a null pointer */

			PLCore::uint32			*pnCornerIndexBuffer;	/**< Can be a null pointer */
			PLCore::uint32			 nCornerIndexSize;
			PLRenderer::IndexBuffer *pCornerIndexBuffer;	/**< Can be a null pointer */
		};

		/**
		*  @brief
		*    GMM surface level linking piece
		**/
		struct LinkingPiece {
			PLCore::uint32			*pnIndexBuffer[4];		/**< Can be a null pointer */
			PLCore::uint32			 nIndexBufferSize[4];
			PLRenderer::IndexBuffer *pIndexBuffer[4];		/**< Can be a null pointer */
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*/
		PLNATURE_API GMMSurfaceLevel(PLRenderer::Renderer &cRenderer);

		/**
		*  @brief
		*    Destructor
		*/
		PLNATURE_API ~GMMSurfaceLevel();

		/**
		*  @brief
		*    Builds the bodies
		*
		*  @param[in] nLevel
		*    Number of geo mip map levels
		*  @param[in] nPatchSize
		*    Patch size
		*/
		PLNATURE_API void BuildBodies(PLCore::uint32 nLevel, PLCore::uint32 nPatchSize);

		/**
		*  @brief
		*    Builds the linkers
		*
		*  @param[in] nLevel
		*    Number of geo mip map levels
		*  @param[in] nPatchSize
		*    Patch size
		*/
		PLNATURE_API void BuildLinkers(PLCore::uint32 nLevel, PLCore::uint32 nPatchSize);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Side
		*/
		enum ESide {
			Right,	/**< Right side */
			Top,	/**< Top side */
			Left,	/**< Left side */
			Bottom	/**< Bottom side */
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLRenderer::Renderer *m_pRenderer;	/**< Used renderer (always valid!) */
		PLCore::uint32		  m_nLevels;	/**< Number of levels */
		BodyPiece			 *m_pBodies;	/**< Bodies, can be a null pointer */
		LinkingPiece		 *m_pLinkers;	/**< Linkers, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLNature


#endif // __PLNATURE_SCENENODE_GMMSURFACELEVEL_H__
