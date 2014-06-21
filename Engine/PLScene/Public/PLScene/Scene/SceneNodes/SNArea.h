/*********************************************************\
 *  File: SNArea.h                                       *
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


#ifndef __PLSCENE_SCENENODE_AREA_H__
#define __PLSCENE_SCENENODE_AREA_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/PlaneSet.h>
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
*    An area is a set of planes defining a convex area in your scene
*
*  @note
*    - Don't scale an area, this will produce some problems...
*/
class SNArea : public SceneNode, public PLMath::PlaneSet {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SNArea, "PLScene", PLScene::SceneNode, "An area is a set of planes defining a convex area in your scene")
		// Properties
		pl_properties
			pl_property("Icon",	"Data/Textures/IconArea.dds")
		pl_properties_end
		// Attributes
		pl_attribute(Box,		PLCore::String,	"-0.5 -0.5 -0.5 0.5 0.5 0.5",	ReadWrite,	GetSet,	"Box min/max position (creates automatically the required planes)",	"")
		pl_attribute(Planes,	PLCore::String,	"",								ReadWrite,	GetSet,	"Planes defining the (normally convex) area",						"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLS_API PLCore::String GetBox() const;
		PLS_API void SetBox(const PLCore::String &sValue);
		PLS_API PLCore::String GetPlanes() const;
		PLS_API void SetPlanes(const PLCore::String &sValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLS_API SNArea();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SNArea();

		/**
		*  @brief
		*    Draws the area
		*/
		PLS_API void Draw();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Check if an box should be created
		*/
		void CheckBox();

		/**
		*  @brief
		*    Check if planes should be added to the area
		*/
		void CheckPlanes();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::String m_sBox;		/**< Box min/max position (creates automatically the required planes) */
		PLCore::String m_sPlanes;	/**< Planes defining the (normally convex) area */


	//[-------------------------------------------------------]
	//[ Protected virtual SceneNode functions                 ]
	//[-------------------------------------------------------]
	protected:
		PLS_API virtual void InitFunction() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENENODE_AREA_H__
