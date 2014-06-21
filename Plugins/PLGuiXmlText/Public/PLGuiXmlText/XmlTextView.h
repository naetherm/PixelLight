/*********************************************************\
 *  File: XmlTextView.h                                  *
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


#ifndef __PLGUIXMLTEXT_VIEW_H__
#define __PLGUIXMLTEXT_VIEW_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGui/Widgets/Widget.h>
#include "PLGuiXmlText/PLGuiXmlText.h"
#include "PLGuiXmlText/XmlTextRenderer.h"
#include "PLGuiXmlText/XmlTextElement.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGuiXmlText {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Control that display's a text using a XML based markup language
*/
class XmlTextView : public PLGui::Widget {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLGUIXMLTEXT_RTTI_EXPORT, XmlTextView, "PLGuiXmlText", PLGui::Widget, "Control that display's a text using a XML based markup language")
		// Attributes
		pl_attribute(Filename,	PLCore::String,	"",	ReadWrite,	GetSet,	"Filename of XmlText file",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLGUIXMLTEXT_API PLCore::String GetFilename() const;
		PLGUIXMLTEXT_API void SetFilename(const PLCore::String &sValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pParent
		*    Parent widget, can be a null pointer
		*/
		PLGUIXMLTEXT_API XmlTextView(Widget *pParent = nullptr);

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
		*    Get document root
		*
		*  @return
		*    Path to document root (directory containing XmlText document)
		*/
		PLGUIXMLTEXT_API PLCore::String GetDocumentRoot() const;

		/**
		*  @brief
		*    Get size of content
		*
		*  @return
		*    Content size
		*/
		PLGUIXMLTEXT_API PLMath::Vector2i GetContentSize() const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Destructor
		*/
		PLGUIXMLTEXT_API virtual ~XmlTextView();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Exported variables
		PLCore::String		m_sFilename;		/**< File name */
		PLCore::String		m_sDocumentRoot;	/**< Path containing the file */

		// XML text
		XmlTextElement		m_cXmlText;			/**< XmlText element tree */
		XmlTextRenderer		m_cRenderer;		/**< XmlText renderer */


	//[-------------------------------------------------------]
	//[ Public virtual Window functions                       ]
	//[-------------------------------------------------------]
	public:
		PLGUIXMLTEXT_API virtual void OnDraw(PLGui::Graphics &cGraphics) override;
		PLGUIXMLTEXT_API virtual void OnSize(const PLMath::Vector2i &vSize) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGuiXmlText


#endif // __PLGUIXMLTEXT_VIEW_H__
