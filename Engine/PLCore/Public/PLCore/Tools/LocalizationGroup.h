/*********************************************************\
 *  File: LocalizationGroup.h                            *
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


#ifndef __PLCORE_LOCALIZATIONGROUP_H__
#define __PLCORE_LOCALIZATIONGROUP_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Container/Array.h"
#include "PLCore/Container/HashMap.h"
#include "PLCore/Tools/Loadable.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class LocalizationText;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Localization group
*/
class LocalizationGroup : public Loadable {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Localization;


	//[-------------------------------------------------------]
	//[ Public static data                                    ]
	//[-------------------------------------------------------]
	public:
		static PLCORE_API const String English;	/**< "English" (default) */


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the name of the group
		*
		*  @return
		*    The name of the group
		*/
		inline String GetName() const;

		/**
		*  @brief
		*    Returns the language used as 'from-key'
		*
		*  @return
		*    The language used as 'from-key'
		*/
		inline String GetFromLanguage() const;

		/**
		*  @brief
		*    Sets the language used as 'from-key'
		*
		*  @param[in] sFrom
		*    The language used as 'from-key'
		*/
		inline void SetFromLanguage(const String &sFrom = English);

		/**
		*  @brief
		*    Returns the language used as 'to-key'
		*
		*  @return
		*    The language used as 'to-key'
		*/
		inline String GetToLanguage() const;

		/**
		*  @brief
		*    Sets the language used as 'to-key'
		*
		*  @param[in] sTo
		*    The language used as 'to-key'
		*/
		inline void SetToLanguage(const String &sTo = English);

		/**
		*  @brief
		*    Returns the number of texts
		*
		*  @return
		*    The number of texts
		*/
		inline uint32 GetNumOfTexts() const;

		/**
		*  @brief
		*    Returns a text by index
		*
		*  @param[in] nIndex
		*    Index of the text
		*
		*  @return
		*    The requested text, a null pointer on error
		*/
		inline LocalizationText *GetText(uint32 nIndex) const;

		/**
		*  @brief
		*    Returns a text by name
		*
		*  @param[in] sName
		*    Name of the text
		*
		*  @return
		*    The requested text, a null pointer on error
		*/
		inline LocalizationText *GetText(const String &sName) const;

		/**
		*  @brief
		*    Adds a new text
		*
		*  @param[in] sName
		*    Name of the new text
		*  @param[in] sTranslation
		*    The translation of the text
		*
		*  @return
		*    The new text, a null pointer on error (maybe there's already a text with the given name?)
		*/
		PLCORE_API LocalizationText *AddText(const String &sName, const String &sTranslation);

		/**
		*  @brief
		*    Removes a text by index
		*
		*  @param[in] nIndex
		*    Index of the text
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe there's no text with the given name?)
		*/
		PLCORE_API bool RemoveText(uint32 nIndex);

		/**
		*  @brief
		*    Removes a text by name
		*
		*  @param[in] sName
		*    Name of the text
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe there's no text with the given name?)
		*/
		PLCORE_API bool RemoveText(const String &sName);

		/**
		*  @brief
		*    Removes all texts
		*/
		PLCORE_API void RemoveAllTexts();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sName
		*    Name of the group
		*/
		LocalizationGroup(const String &sName);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		LocalizationGroup(const LocalizationGroup &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~LocalizationGroup();

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
		LocalizationGroup &operator =(const LocalizationGroup &cSource);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		String							   m_sName;			/**< Name of the group */
		String							   m_sFromLanguage;	/**< The language used as 'from-key' */
		String							   m_sToLanguage;	/**< The language used as 'to-key' */
		Array<LocalizationText*>		   m_lstTexts;		/**< Localization texts list */
		HashMap<String, LocalizationText*> m_mapTexts;		/**< Localization texts map */


	//[-------------------------------------------------------]
	//[ Public virtual Loadable functions                     ]
	//[-------------------------------------------------------]
	public:
		PLCORE_API virtual bool Unload() override;
		PLCORE_API virtual String GetLoadableTypeName() const override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Tools/LocalizationGroup.inl"


#endif // __PLCORE_LOCALIZATIONGROUP_H__
