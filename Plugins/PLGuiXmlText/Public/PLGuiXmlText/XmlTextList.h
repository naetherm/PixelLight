/*********************************************************\
 *  File: XmlTextList.h                                  *
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


#ifndef __PLGUIXMLTEXT_LIST_H__
#define __PLGUIXMLTEXT_LIST_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLGuiXmlText/PLGuiXmlText.h"
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
*    Element describing a list
*/
class XmlTextList : public XmlTextElement {


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
		PLGUIXMLTEXT_API XmlTextList(XmlTextElement *pParent = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUIXMLTEXT_API virtual ~XmlTextList();


	//[-------------------------------------------------------]
	//[ Protected virtual XmlTextElement functions            ]
	//[-------------------------------------------------------]
	protected:
		PLGUIXMLTEXT_API virtual void OnParse(PLCore::XmlNode &cXmlNode) override;
		PLGUIXMLTEXT_API virtual void OnDraw(XmlTextRenderer &cRenderer) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGuiXmlText


#endif // __PLGUIXMLTEXT_LIST_H__
