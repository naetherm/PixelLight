/*********************************************************\
 *  File: SNText.h                                       *
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


#ifndef __PLSCENE_SCENENODE_TEXT_H__
#define __PLSCENE_SCENENODE_TEXT_H__
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
*    Abstract text scene node
*/
class SNText : public SceneNode {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Scene node flags (SceneNode flags extension)
		*/
		enum EFlags {
			NoCenter = 1<<10	/**< Do not center the text */
		};
		pl_enum(EFlags)
			pl_enum_base(SceneNode::EFlags)
			pl_enum_value(NoCenter,	"Do not center the text")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLS_RTTI_EXPORT, SNText, "PLScene", PLScene::SceneNode, "Abstract text scene node")
		// Attributes
		pl_attribute(Text,		PLCore::String,			"",							ReadWrite,	GetSet,			"Text do draw",																						"")
		pl_attribute(Color,		PLGraphics::Color4,		PLGraphics::Color4::White,	ReadWrite,	DirectValue,	"Text color",																						"")
		pl_attribute(Font,		PLCore::String,			"",							ReadWrite,	GetSet,			"The used font, if empty the default font is used",													"")
		pl_attribute(FontSize,	PLCore::uint32,			0,							ReadWrite,	GetSet,			"The font size, if null the default font size is used - use scene node scale for dynamic scale",	"Max='100'")
			// Overwritten SceneNode attributes
		pl_attribute(Flags,		pl_flag_type(EFlags),	0,							ReadWrite,	GetSet,			"Flags",																							"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLS_API virtual PLCore::String GetText() const;
		PLS_API virtual void SetText(const PLCore::String &sValue);
		PLS_API virtual PLCore::String GetFont() const;
		PLS_API virtual void SetFont(const PLCore::String &sValue);
		PLS_API virtual PLCore::uint32 GetFontSize() const;
		PLS_API virtual void SetFontSize(PLCore::uint32 nValue);


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		PLS_API SNText();

		/**
		*  @brief
		*    Destructor
		*/
		PLS_API virtual ~SNText();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::String m_sText;		/**< Text do draw */
		PLCore::String m_sFont;		/**< The used font, if empty the default font is used */
		PLCore::uint32 m_nFontSize;	/**< The font size, if null the default font size is used - use scene node scale for dynamic scale */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScene


#endif // __PLSCENE_SCENENODE_TEXT_H__
