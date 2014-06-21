/*********************************************************\
 *  File: LocalizationText.h                             *
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


#ifndef __PLCORE_LOCALIZATIONTEXT_H__
#define __PLCORE_LOCALIZATIONTEXT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/String/String.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class LocalizationGroup;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Localization text
*/
class LocalizationText {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class LocalizationGroup;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the group the text is in
		*
		*  @return
		*    The group the text is in
		*/
		inline LocalizationGroup &GetGroup() const;

		/**
		*  @brief
		*    Returns the name of the text
		*
		*  @return
		*    The name of the text
		*/
		inline String GetName() const;

		/**
		*  @brief
		*    Returns the translation of the text
		*
		*  @return
		*    The translation of the text
		*/
		inline String GetTranslation() const;

		/**
		*  @brief
		*    Sets the translation of the text
		*
		*  @param[in] sTranslation
		*    The translation of the text
		*/
		inline void SetTranslation(const String &sTranslation);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cGroup
		*    The group the text is in
		*  @param[in] sName
		*    Name of the text
		*  @param[in] sTranslation
		*    The translation of the text
		*
		*  @note
		*    - By default, the translation if set to the text name
		*/
		LocalizationText(LocalizationGroup &cGroup, const String &sName, const String &sTranslation);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		LocalizationText(const LocalizationText &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		~LocalizationText();

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		LocalizationText &operator =(const LocalizationText &cSource);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		LocalizationGroup *m_pGroup;		/**< The group the text is in (always valid!) */
		String			   m_sName;			/**< Name of the group */
		String			   m_sTranslation;	/**< The translation of the text */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Tools/LocalizationText.inl"


#endif // __PLCORE_LOCALIZATIONTEXT_H__
