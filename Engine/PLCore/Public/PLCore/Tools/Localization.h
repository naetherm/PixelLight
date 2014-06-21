/*********************************************************\
 *  File: Localization.h                                 *
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


#ifndef __PLCORE_LOCALIZATION_H__
#define __PLCORE_LOCALIZATION_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/String/String.h"
#include "PLCore/Core/Singleton.h"
#include "PLCore/Container/Array.h"
#include "PLCore/Container/HashMap.h"


//[-------------------------------------------------------]
//[ Macro definitions                                     ]
//[-------------------------------------------------------]
#define PLT(Text) PLCore::Localization::GetInstance()->Get(Text)
#define PL_TEXT(Text, Group) PLCore::Localization::GetInstance()->Get(Text, Group)


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class Localization;
	class LocalizationGroup;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Localization manager
*
*  @remarks
*    The localization manager is used to 'translate' a given text into the current active
*    language. This is done by using the given text to translate as 'key' within a certain
*    group and each project can have it's own localization group to avoid conflicts. If the given
*    text to translate was found within the given localization group, the translation result is returned,
*    else the given text is returned instead. As result, within your code you ALWAYS have to use a native
*    language (within PixelLight, this is english) which is used if a text is not translated.
*    The default localization group is called 'PixelLight' and should ONLY be used for engine texts!
*
*  @verbatim
*    Usage example:
*    String sString = PLT("Translate this text");
*    String sMyString = PL_TEXT("Translate this text", "MyProject");
*  @endverbatim
*/
class Localization : public Singleton<Localization> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Singleton<Localization>;


	//[-------------------------------------------------------]
	//[ Public static data                                    ]
	//[-------------------------------------------------------]
	public:
		static PLCORE_API const String PixelLight;	/**< "PixelLight" (default) */


	//[-------------------------------------------------------]
	//[ Public static PLCore::Singleton functions             ]
	//[-------------------------------------------------------]
	// This solution enhances the compatibility with legacy compilers like GCC 4.2.1 used on Mac OS X 10.6
	// -> The C++11 feature "extern template" (C++11, see e.g. http://www2.research.att.com/~bs/C++0xFAQ.html#extern-templates) can only be used on modern compilers like GCC 4.6
	// -> We can't break legacy compiler support, especially when only the singletons are responsible for the break
	// -> See PLCore::Singleton for more details about singletons
	public:
		static PLCORE_API Localization *GetInstance();
		static PLCORE_API bool HasInstance();


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the current language
		*
		*  @return
		*    The current language
		*/
		inline String GetLanguage() const;

		/**
		*  @brief
		*    Sets the current language
		*
		*  @param[in] sLanguage
		*    The new current language
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - You can use the System::GetLocaleLanguage() function to get the current program locale language
		*    - It's recommended to call this function ONLY on startup because it's possible that some
		*      texts can't be updated on runtime if another language is set
		*/
		inline void SetLanguage(const String &sLanguage);

		/**
		*  @brief
		*    Returns the translation of the given text
		*
		*  @param[in] sText
		*    Text to translate
		*  @param[in] sGroup
		*    Group the text to translate is in
		*
		*  @return
		*    The translation of the given text, if no translation is available for this text the
		*    given text is returned instead by default
		*/
		PLCORE_API String Get(const String &sText, const String &sGroup = PixelLight) const;

		/**
		*  @brief
		*    Returns the number of groups
		*
		*  @return
		*    The number of groups
		*/
		inline uint32 GetNumOfGroups() const;

		/**
		*  @brief
		*    Returns a group by index
		*
		*  @param[in] nIndex
		*    Index of the group
		*
		*  @return
		*    The requested group, a null pointer on error
		*/
		inline LocalizationGroup *GetGroup(uint32 nIndex) const;

		/**
		*  @brief
		*    Returns a group by name
		*
		*  @param[in] sName
		*    Name of the group
		*
		*  @return
		*    The requested group, a null pointer on error
		*/
		inline LocalizationGroup *GetGroup(const String &sName) const;

		/**
		*  @brief
		*    Adds a new group
		*
		*  @param[in] sName
		*    Name of the new group
		*
		*  @return
		*    The new group, a null pointer on error (maybe there's already a group with the given name?)
		*/
		PLCORE_API LocalizationGroup *AddGroup(const String &sName);

		/**
		*  @brief
		*    Removes a group by index
		*
		*  @param[in] nIndex
		*    Index of the group
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe there's no group with the given name?)
		*/
		PLCORE_API bool RemoveGroup(uint32 nIndex);

		/**
		*  @brief
		*    Removes a group by name
		*
		*  @param[in] sName
		*    Name of the group
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe there's no group with the given name?)
		*/
		PLCORE_API bool RemoveGroup(const String &sName);

		/**
		*  @brief
		*    Removes all groups
		*/
		PLCORE_API void RemoveAllGroups();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*/
		Localization();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		Localization(const Localization &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Localization();

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
		Localization &operator =(const Localization &cSource);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		String								m_sLanguage;	/**< Current set language */
		Array<LocalizationGroup*>			m_lstGroups;	/**< Localization groups list */
		HashMap<String, LocalizationGroup*>	m_mapGroups;	/**< Localization groups map */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Tools/Localization.inl"


#endif // __PLCORE_LOCALIZATION_H__
