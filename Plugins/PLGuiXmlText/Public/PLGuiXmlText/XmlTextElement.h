/*********************************************************\
 *  File: XmlTextElement.h                               *
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


#ifndef __PLGUIXMLTEXT_ELEMENT_H__
#define __PLGUIXMLTEXT_ELEMENT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/List.h>
#include <PLGui/Gui/Resources/Image.h>
#include "PLGuiXmlText/PLGuiXmlText.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class XmlNode;
}
namespace PLGraphics {
	class Color4;
}
namespace PLGui {
	class Gui;
}
namespace PLGuiXmlText {
	class XmlTextView;
	class XmlTextRenderer;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGuiXmlText {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Base class for XmlText elements
*/
class XmlTextElement {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class XmlTextView;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pParent
		*    Parent object, can be a null pointer
		*/
		PLGUIXMLTEXT_API XmlTextElement(XmlTextElement *pParent = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUIXMLTEXT_API virtual ~XmlTextElement();

		/**
		*  @brief
		*    Load XML file
		*
		*  @param[in] sFilename
		*    Filename of XML file
		*/
		PLGUIXMLTEXT_API void Load(const PLCore::String &sFilename);

		/**
		*  @brief
		*    Draw element and all children to screen
		*
		*  @param[in] cRenderer
		*    XmlText renderer
		*/
		PLGUIXMLTEXT_API void Draw(XmlTextRenderer &cRenderer);

		/**
		*  @brief
		*    Get size of XMLText content (looks for the first section)
		*/
		PLGUIXMLTEXT_API PLMath::Vector2i GetSize() const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Clear element and all children
		*/
		PLGUIXMLTEXT_API void Clear();

		/**
		*  @brief
		*    Parse XML node
		*
		*  @param[in] cXmlNode
		*    XML node to be parsed
		*/
		PLGUIXMLTEXT_API void Parse(PLCore::XmlNode &cXmlNode);

		/**
		*  @brief
		*    Draw all children
		*
		*  @param[in] cRenderer
		*    XmlText renderer
		*/
		PLGUIXMLTEXT_API void DrawChildren(XmlTextRenderer &cRenderer);

		/**
		*  @brief
		*    Parse HTML compatible color value
		*
		*  @param[in] sColor
		*    Color value or name
		*
		*  @return
		*    Color
		*/
		PLGUIXMLTEXT_API PLGraphics::Color4 GetColor(const PLCore::String &sColor);

		/**
		*  @brief
		*    Load image file
		*
		*  @param[in] cGui
		*    Gui to which the image belongs
		*  @param[in] sFilename
		*    Image file name
		*
		*  @return
		*    Image instance
		*/
		PLGUIXMLTEXT_API PLGui::Image LoadImage(PLGui::Gui &cGui, const PLCore::String &sFilename);


	//[-------------------------------------------------------]
	//[ Protected virtual XmlTextElement functions            ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Parse XML node
		*
		*  @param[in] cXmlNode
		*    XML node to be parsed
		*/
		PLGUIXMLTEXT_API virtual void OnParse(PLCore::XmlNode &cXmlNode);

		/**
		*  @brief
		*    Draw element
		*
		*  @param[in] cRenderer
		*    XmlText renderer
		*/
		PLGUIXMLTEXT_API virtual void OnDraw(XmlTextRenderer &cRenderer);

		/**
		*  @brief
		*    Get element size
		*
		*  @return
		*    Size of element
		*/
		PLGUIXMLTEXT_API virtual PLMath::Vector2i OnGetSize() const;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Element tree
		XmlTextView						*m_pXmlText;		/**< XmlView control */
		XmlTextElement					*m_pParent;			/**< Parent element */
		PLCore::List<XmlTextElement*>	 m_lstChildren;		/**< Child elements */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGuiXmlText


#endif // __PLGUIXMLTEXT_ELEMENT_H__
