/*********************************************************\
 *  File: SNCoordinateAxis.h                             *
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


#ifndef __PLSCENE_SCENENODE_COORDINATEAXIS_H__
#define __PLSCENE_SCENENODE_COORDINATEAXIS_H__
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
*    Abstract base scene node for coordinate axis visualization
*/
class SNCoordinateAxis : public SceneNode {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene node flags (SceneNode flags extension)
		*/
		enum EFlags {
			DepthTest = 1<<10,	/**< Perform a depth test */
			NoXText   = 1<<11,	/**< Do NOT draw the x-axis text  */
			NoYText   = 1<<12,	/**< Do NOT draw the y-axis text  */
			NoZText   = 1<<13	/**< Do NOT draw the z-axis text  */
		};
		pl_enum(EFlags)
			pl_enum_base(SceneNode::EFlags)
			pl_enum_value(DepthTest,	"Perform a depth test")
			pl_enum_value(NoXText,		"Do NOT draw the x-axis text")
			pl_enum_value(NoYText,		"Do NOT draw the y-axis text")
			pl_enum_value(NoZText,		"Do NOT draw the z-axis text")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SNCoordinateAxis, "PLScene", PLScene::SceneNode, "Abstract base scene node for coordinate axis visualization")
		// Attributes
		pl_attribute(Width,		float,					1.0f,										ReadWrite,	DirectValue,	"Line width (if supported by the the used renderer API)",	"Min='1.0'")
		pl_attribute(XColor,	PLGraphics::Color4,		PLGraphics::Color4(1.0f, 0.0f, 0.0f, 1.0f),	ReadWrite,	DirectValue,	"Color of the x-axis",										"")
		pl_attribute(YColor,	PLGraphics::Color4,		PLGraphics::Color4(0.0f, 1.0f, 0.0f, 1.0f),	ReadWrite,	DirectValue,	"Color of the y-axis",										"")
		pl_attribute(ZColor,	PLGraphics::Color4,		PLGraphics::Color4(0.0f, 0.0f, 1.0f, 1.0f),	ReadWrite,	DirectValue,	"Color of the z-axis",										"")
			// Overwritten SceneNode attributes
		pl_attribute(Flags,		pl_flag_type(EFlags),	0,											ReadWrite,	GetSet,			"Flags",													"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default Constructor
		*/
		PLS_API SNCoordinateAxis();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SNCoordinateAxis();


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENENODE_COORDINATEAXIS_H__
