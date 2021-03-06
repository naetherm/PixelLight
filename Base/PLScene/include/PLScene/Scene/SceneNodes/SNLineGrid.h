/*********************************************************\
 *  File: SNLineGrid.h                                   *
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


#ifndef __PLSCENE_SCENENODE_LINEGRID_H__
#define __PLSCENE_SCENENODE_LINEGRID_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGraphics/Color/Color4.h>
#include "PLScene/Scene/SceneNode.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScene {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Line grid scene node on the nodes local space x/y-plane
*/
class SNLineGrid : public SceneNode {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene node flags (SceneNode flags extension)
		*/
		enum EFlags {
			NoDepthTest  = 1<<10,	/**< Do not perform a depth test */
			NoMainLines  = 1<<11,	/**< Do not draw the main lines */
			NoMinorLines = 1<<12	/**< Do not draw the minor lines */
		};
		pl_flag(EFlags)
			pl_enum_base(SceneNode::EFlags)
			pl_enum_value(NoDepthTest,	"Do not perform a depth test")
			pl_enum_value(NoMainLines,	"Do not draw the main lines")
			pl_enum_value(NoMinorLines,	"Do not draw the minor lines")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def(PLS_API)
		// Attributes
		pl_attribute_directvalue(				NumOfXLines,	PLCore::uint32,		5,												ReadWrite)
		pl_attribute_directvalue(				NumOfYLines,	PLCore::uint32,		5,												ReadWrite)
		pl_attribute_directvalue(				MainWidth,		float,				1.0f,											ReadWrite)
		pl_attribute_directvalue(				MainColor,		PLGraphics::Color4,	PLGraphics::Color4(0.1f, 0.1f, 0.1f, 1.0f),		ReadWrite)
		pl_attribute_directvalue(				MinorWidth,		float,				1.0f,											ReadWrite)
		pl_attribute_directvalue(				MinorColor,		PLGraphics::Color4,	PLGraphics::Color4(0.55f, 0.55f, 0.55f, 1.0f),	ReadWrite)
			// Overwritten SceneNode attributes
		pl_attribute_getset		(SNLineGrid,	Flags,			PLCore::uint32,		0,												ReadWrite)
		pl_attribute_getset		(SNLineGrid,	AABBMin,		PLMath::Vector3,	PLMath::Vector3(-0.5f, -0.5f, -0.5f),			ReadWrite)
		pl_attribute_getset		(SNLineGrid,	AABBMax,		PLMath::Vector3,	PLMath::Vector3( 0.5f,  0.5f,  0.5f),			ReadWrite)
	pl_class_def_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLS_API SNLineGrid();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SNLineGrid();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Draws the grid
		*
		*  @param[in] cRenderer
		*    Renderer to use
		*  @param[in] cVisNode
		*    The current visibility node of this scene node
		*/
		void DrawGrid(PLRenderer::Renderer &cRenderer, const VisNode &cVisNode);


	//[-------------------------------------------------------]
	//[ Public virtual SceneNode functions                    ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual void DrawSolid(PLRenderer::Renderer &cRenderer, const VisNode *pVisNode = nullptr) override;
		PLS_API virtual void DrawTransparent(PLRenderer::Renderer &cRenderer, const VisNode *pVisNode = nullptr) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENENODE_LINEGRID_H__
