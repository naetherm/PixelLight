/*********************************************************\
 *  File: SNClip.h                                       *
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


#ifndef __PLVOLUME_SNCLIP_H__
#define __PLVOLUME_SNCLIP_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGraphics/Color/Color4.h>
#include <PLScene/Scene/SceneNode.h>
#include "PLVolume/PLVolume.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolume {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract clip scene node base class
*/
class SNClip : public PLScene::SceneNode {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene node flags (PLScene::SceneNode flags extension)
		*/
		enum EFlags {
			InvertClipping = 1<<10	/**< Invert clipping */
		};
		pl_flag(EFlags)
			pl_enum_base(PLScene::SceneNode::EFlags)
			pl_enum_value(InvertClipping, "Invert clipping")
		pl_enum_end

		/**
		*  @brief
		*    Scene node debug flags (PLScene::SceneNode debug flags extension)
		*/
		enum EDebugFlags {
			DebugNoVisualization = 1<<8	/**< Do not draw the visualization */
		};
		pl_flag(EDebugFlags)
			pl_enum_base(PLScene::SceneNode::EDebugFlags)
			pl_enum_value(DebugNoVisualization, "Do not draw the visualization")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def(PLVOLUME_API)
		// Attributes
		pl_attribute_directvalue(			DebugColorPositive,	PLGraphics::Color4,	PLGraphics::Color4(0.0f, 1.0f, 0.0f, 0.1f),	ReadWrite)
		pl_attribute_directvalue(			DebugColorNegative,	PLGraphics::Color4,	PLGraphics::Color4(1.0f, 0.0f, 0.0f, 0.1f),	ReadWrite)
			// Overwritten PLScene::SceneNode attributes
		pl_attribute_getset		(SNClip,	Flags,				PLCore::uint32,		0,											ReadWrite)
		pl_attribute_getset		(SNClip,	DebugFlags,			PLCore::uint32,		0,											ReadWrite)
	pl_class_def_end


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		PLVOLUME_API SNClip();

		/**
		*  @brief
		*    Destructor
		*/
		PLVOLUME_API virtual ~SNClip();


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume


#endif // __PLVOLUME_SNCLIP_H__
