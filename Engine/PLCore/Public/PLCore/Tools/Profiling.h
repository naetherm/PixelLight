/*********************************************************\
 *  File: Profiling.h                                    *
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


#ifndef __PLCORE_PROFILING_H__
#define __PLCORE_PROFILING_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Core/Singleton.h"
#include "PLCore/Container/Element.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Profiling group element
*/
class ProfileGroupElement : public Element<ProfileGroupElement> {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sName
		*    Element name to set
		*  @param[in] pManager
		*    Element manager using this element, can be a null pointer
		*/
		PLCORE_API ProfileGroupElement(const String &sName = "", ElementManager<ProfileGroupElement> *pManager = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~ProfileGroupElement();

		/**
		*  @brief
		*    Returns the text of the element
		*
		*  @return
		*    Text of the element
		*/
		inline String GetText() const;

		/**
		*  @brief
		*    Sets the element text
		*
		*  @param[in] sText
		*    Text to be set
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		inline bool SetText(const String &sText);

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
		inline ProfileGroupElement &operator =(const ProfileGroupElement &cSource);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		ProfileGroupElement(const ProfileGroupElement &cSource);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		String m_sText;	/**< Text to be shown */


};

/**
*  @brief
*    Profiling group
*/
class ProfileGroup : public Element<ProfileGroup>, public ElementManager<ProfileGroupElement> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Profiling;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sName
		*    Element name to set
		*  @param[in] pManager
		*    Element manager using this element, can be a null pointer
		*/
		PLCORE_API ProfileGroup(const String &sName = "", ElementManager<ProfileGroup> *pManager = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~ProfileGroup();

		/**
		*  @brief
		*    Returns the text of the element group
		*
		*  @return
		*    Text of the element group
		*/
		inline String GetText() const;

		/**
		*  @brief
		*    Sets the element group text
		*
		*  @param[in] sText
		*    Text to be set
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		inline bool SetText(const String &sText);

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
		inline ProfileGroup &operator =(const ProfileGroup &cSource);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		ProfileGroup(const ProfileGroup &cSource);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		String m_sText;	/**< Text to be shown */


	//[-------------------------------------------------------]
	//[ Private virtual ElementManager functions              ]
	//[-------------------------------------------------------]
	private:
		virtual ProfileGroupElement *CreateElement(const String &sName) override;


};

/**
*  @brief
*    PixelLight profiling system
*
*  @remarks
*    The PL API profiling tool is extreme useful to acquire information about certain code
*    parts and their performance. If the profiling is activated with Profiling::GetInstance()->SetActive()
*    it displays you information like current FPS, triangle count, number of scene nodes, scene
*    update time etc. With this information you are able to find out were your performance is
*    burned. The profiling information are subdivided in groups like general, scene, textures
*    etc. containing elements were you could ’browse’ through using offered functions like
*    Profiling::GetInstance()->SelectNextGroup().
*    By default the PL console offers all required commands to handle the profiling tool in a
*    comfortable way therefore there’s no real need to use the profiling browse functions offered
*    by the profiling interface. :)\n
*
*    Adding own groups and elements:\n
*    The profiling information could be expand with own information you maybe want to add
*    to inspect the runtime behavior of certain codes. In fact that’s much easier as it may
*    sound first... in real it’s only ONE function called Profiling::GetInstance()->Set() you need to
*    use to customize the PL profiling tool!!\n
*    Example:\n
*    // Add profiling information\n
*    Profiling::GetInstance()->Set("MyProfilingGroup", "Name: " + sName);\n
*    Profiling::GetInstance()->Set("MyProfilingGroup", String("Current level: ") + nLevel);\n
*    This will add the defined elements to the own profiling group ’MyProfilingGroup’.
*    If this group doesn’t exist yet it’s created automatically, therefore you done have to be
*    interested in the stuff behind all! If you want to do more that only the described above it’s
*    still possible because you have FULL access to all required to remove groups etc. (because
*    the profiling class is derived from ElementManager)
*
*  @note
*    - You can manipulate each element by hand, but it's recommended
*      to use the helper functions provided by the profiling system
*/
class Profiling : public ElementManager<ProfileGroup>, public Singleton<Profiling> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Singleton<Profiling>;


	//[-------------------------------------------------------]
	//[ Public static PLCore::Singleton functions             ]
	//[-------------------------------------------------------]
	// This solution enhances the compatibility with legacy compilers like GCC 4.2.1 used on Mac OS X 10.6
	// -> The C++11 feature "extern template" (C++11, see e.g. http://www2.research.att.com/~bs/C++0xFAQ.html#extern-templates) can only be used on modern compilers like GCC 4.6
	// -> We can't break legacy compiler support, especially when only the singletons are responsible for the break
	// -> See PLCore::Singleton for more details about singletons
	public:
		static PLCORE_API Profiling *GetInstance();
		static PLCORE_API bool HasInstance();


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns whether the profiling system is active or not
		*
		*  @return
		*    'true' if the profiling system is active, else 'false'
		*
		*  @note
		*    - If the profiling system is active the current profile group
		*      information will be shown automatically
		*/
		inline bool IsActive() const;

		/**
		*  @brief
		*    Sets whether the profiling system is active or not
		*
		*  @param[in] bActive
		*    Should the profiling system be active?
		*/
		inline void SetActive(bool bActive = false);

		/**
		*  @brief
		*    Returns the name of the current selected profile group
		*
		*  @return
		*    Name of the current selected profile group
		*/
		inline String GetSelectedGroup() const;

		/**
		*  @brief
		*    Selects a profile group
		*
		*  @param[in] sName
		*    Name of the profile group which should be the current one
		*    If "" general profile information are shown.
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*    (maybe there's no group with this name??)
		*/
		PLCORE_API bool SelectGroup(const String &sName = "");

		/**
		*  @brief
		*    Selects the next profile group
		*
		*  @return
		*    Name of the current selected profile group
		*/
		PLCORE_API String SelectNextGroup();

		/**
		*  @brief
		*    Selects the previous profile group
		*
		*  @return
		*    Name of the current selected profile group
		*/
		PLCORE_API String SelectPreviousGroup();

		/**
		*  @brief
		*    Sets the element text
		*
		*  @param[in] sGroup
		*    Group name
		*  @param[in] sElement
		*    Element name
		*  @param[in] sText
		*    Text to be set
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*    (maybe the profiling is deactivated??)
		*
		*  @note
		*    - The group/element is created automatically
		*    - If the profiling system is currently deactivated this
		*      function makes nothing
		*/
		PLCORE_API bool Set(const String &sGroup, const String &sElement, const String &sText);

		/**
		*  @brief
		*    Returns the element text
		*
		*  @param[in] sGroup
		*    Group name
		*  @param[in] sElement
		*    Element name
		*
		*  @return
		*    The text of the element, empty if there was an error
		*/
		PLCORE_API String GetText(const String &sGroup, const String &sElement) const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*/
		Profiling();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		Profiling(const Profiling &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Profiling();

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
		Profiling &operator =(const Profiling &cSource);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool   m_bActive;		/**< Is the profiling system active? */
		String m_sGroupName;	/**< Name of the current selected profile group */


	//[-------------------------------------------------------]
	//[ Private virtual ElementManager functions              ]
	//[-------------------------------------------------------]
	private:
		virtual ProfileGroup *CreateElement(const String &sName) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Tools/Profiling.inl"


#endif // __PLCORE_PROFILING_H__
