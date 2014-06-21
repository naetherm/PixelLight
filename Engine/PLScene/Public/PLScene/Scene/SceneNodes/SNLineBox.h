/*********************************************************\
 *  File: SNLineBox.h                                    *
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


#ifndef __PLSCENE_SCENENODE_LINEBOX_H__
#define __PLSCENE_SCENENODE_LINEBOX_H__
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
*    Line box scene node
*/
class SNLineBox : public SceneNode {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene node flags (SceneNode flags extension)
		*/
		enum EFlags {
			No3DPosition = 1<<10,	/**< The line box is not placed within 3D. If this flag is set, the node position is in 'screen space' between 0.0 and 1.0. */
			NoDepthTest  = 1<<11	/**< Do not perform a depth test */
		};
		pl_enum(EFlags)
			pl_enum_base(SceneNode::EFlags)
			pl_enum_value(No3DPosition,	"The line box is not placed within 3D. If this flag is set, the node position is in 'screen space' between 0.0 and 1.0.")
			pl_enum_value(NoDepthTest,	"Do not perform a depth test")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SNLineBox, "PLScene", PLScene::SceneNode, "Line box scene node")
		// Attributes
		pl_attribute(StartPosition,	PLMath::Vector3,		PLMath::Vector3::Zero,		ReadWrite,	GetSet,			"Line box start position (relative to this node)",				"")
		pl_attribute(EndPosition,	PLMath::Vector3,		PLMath::Vector3::UnitZ,		ReadWrite,	GetSet,			"Line box end position (relative to this node)",				"")
		pl_attribute(Width,			float,					1.0f,						ReadWrite,	DirectValue,	"Line box width (if supported by the the used renderer API)",	"Min='1.0'")
		pl_attribute(Color,			PLGraphics::Color4,		PLGraphics::Color4::White,	ReadWrite,	DirectValue,	"Line box color (r/g/b/a)",										"")
			// Overwritten SceneNode attributes
		pl_attribute(Flags,			pl_flag_type(EFlags),	0,							ReadWrite,	GetSet,			"Flags",														"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLS_API const PLMath::Vector3 &GetStartPosition() const;
		PLS_API void SetStartPosition(const PLMath::Vector3 &vValue);
		PLS_API const PLMath::Vector3 &GetEndPosition() const;
		PLS_API void SetEndPosition(const PLMath::Vector3 &vValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLS_API SNLineBox();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SNLineBox();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLMath::Vector3 m_vStartPosition;	/**< Line box start position (relative to this node) */
		PLMath::Vector3	m_vEndPosition;		/**< Line box end position (relative to this node) */


	//[-------------------------------------------------------]
	//[ Public virtual SceneNode functions                    ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual void DrawSolid(PLRenderer::Renderer &cRenderer, const VisNode *pVisNode = nullptr) override;
		PLS_API virtual void DrawTransparent(PLRenderer::Renderer &cRenderer, const VisNode *pVisNode = nullptr) override;


	//[-------------------------------------------------------]
	//[ Protected virtual SceneNode functions                 ]
	//[-------------------------------------------------------]
	protected:
		PLS_API virtual void UpdateAABoundingBox() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENENODE_LINEBOX_H__
