/*********************************************************\
 *  File: SNClipPrimitivePlane.h                         *
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


#ifndef __PLVOLUME_SNCLIP_PRIMITIVE_PLANE_H__
#define __PLVOLUME_SNCLIP_PRIMITIVE_PLANE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLVolume/Scene/SNClipPrimitive.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolume {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Clip primitive plane scene node (plane in scene node space, xy-plane clipping everything <0 on the z-axis)
*/
class SNClipPrimitivePlane : public SNClipPrimitive {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLVOLUME_RTTI_EXPORT, SNClipPrimitivePlane, "PLVolume", PLVolume::SNClipPrimitive, "Clip primitive plane scene node (plane in scene node space, xy-plane clipping everything <0 on the z-axis)")
		// Attributes
			// Overwritten PLScene::SceneNode attributes
		pl_attribute(AABBMin,		PLMath::Vector3,			PLMath::Vector3(-10.0f, -10.0f, -10.0f),	ReadWrite,	GetSet,	"Minimum position of the 'scene node space' axis aligned bounding box",	"")
		pl_attribute(AABBMax,		PLMath::Vector3,			PLMath::Vector3( 10.0f,  10.0f,  10.0f),	ReadWrite,	GetSet,	"Maximum position of the 'scene node space' axis aligned bounding box",	"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLVOLUME_API SNClipPrimitivePlane();

		/**
		*  @brief
		*    Destructor
		*/
		PLVOLUME_API virtual ~SNClipPrimitivePlane();


	//[-------------------------------------------------------]
	//[ Public virtual PLScene::SceneNode functions           ]
	//[-------------------------------------------------------]
	public:
		PLVOLUME_API virtual void DrawDebug(PLRenderer::Renderer &cRenderer, const PLScene::VisNode *pVisNode = nullptr) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume


#endif // __PLVOLUME_SNCLIP_PRIMITIVE_PLANE_H__
