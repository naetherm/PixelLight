/*********************************************************\
 *  File: Profiling.cpp                                  *
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


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Tools/Profiling.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Template instance                                     ]
//[-------------------------------------------------------]
template class Singleton<Profiling>;


//[-------------------------------------------------------]
//[ Public static PLCore::Singleton functions             ]
//[-------------------------------------------------------]
Profiling *Profiling::GetInstance()
{
	// The compiler should be able to optimize this extra call, at least inside this project (inlining)
	return Singleton<Profiling>::GetInstance();
}

bool Profiling::HasInstance()
{
	// The compiler should be able to optimize this extra call, at least inside this project (inlining)
	return Singleton<Profiling>::HasInstance();
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ProfileGroupElement::ProfileGroupElement(const String &sName, ElementManager<ProfileGroupElement> *pManager) :
	Element<ProfileGroupElement>(sName, pManager)
{
}

/**
*  @brief
*    Destructor
*/
ProfileGroupElement::~ProfileGroupElement()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
ProfileGroupElement::ProfileGroupElement(const ProfileGroupElement &cSource)
{
	// No implementation because the copy constructor is never used
}




//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ProfileGroup::ProfileGroup(const String &sName, ElementManager<ProfileGroup> *pManager) :
	Element<ProfileGroup>(sName, pManager)
{
}

/**
*  @brief
*    Destructor
*/
ProfileGroup::~ProfileGroup()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
ProfileGroup::ProfileGroup(const ProfileGroup &cSource)
{
	// No implementation because the copy constructor is never used
}


//[-------------------------------------------------------]
//[ Private virtual ElementManager functions              ]
//[-------------------------------------------------------]
ProfileGroupElement *ProfileGroup::CreateElement(const String &sName)
{
	return new ProfileGroupElement(sName, this);
}




//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Selects a profile group
*/
bool Profiling::SelectGroup(const String &sName)
{
	// Check given group name and check if there's a group with this name
	if (sName.GetLength() && GetByName(sName)) {
		// Select group
		m_sGroupName = sName;

		// Done
		return true;
	} else {
		// Error!
		return false;
	}
}

/**
*  @brief
*    Selects the next profile group
*/
String Profiling::SelectNextGroup()
{
	// Get current group
	const ProfileGroup *pGroup = GetByName(m_sGroupName);
	if (pGroup) {
		// Select the next group
		const uint32 i = pGroup->GetID()+1;
		m_sGroupName = i < GetNumOfElements() ? GetByIndex(i)->GetName() : "";
	} else {
		// Select first group
		pGroup = GetByIndex(0);
		if (pGroup)
			m_sGroupName = pGroup->GetName();
	}

	// Return the group name
	return m_sGroupName;
}

/**
*  @brief
*    Selects the previous profile group
*/
String Profiling::SelectPreviousGroup()
{
	// Get current group
	const ProfileGroup *pGroup = GetByName(m_sGroupName);
	if (pGroup) {
		// Select the previous group
		const uint32 i = pGroup->GetID();
		m_sGroupName = i ? GetByIndex(i-1)->GetName() : "";
	} else {
		// Select first group
		pGroup = GetByIndex(GetNumOfElements()-1);
		if (pGroup)
			m_sGroupName = pGroup->GetName();
	}

	// Return the group name
	return m_sGroupName;
}

/**
*  @brief
*    Sets the element text
*/
bool Profiling::Set(const String &sGroup, const String &sElement, const String &sText)
{
	// Is the profiling system active and are the parameters ok?
	if (m_bActive && sGroup.GetLength() && sElement.GetLength()) {
		// Check if there's a group with this name
		ProfileGroup *pGroup = GetByName(sGroup);
		if (!pGroup)
			pGroup = Create(sGroup);
		if (pGroup) {
			// Check if there's an element with this name
			ProfileGroupElement *pGroupElement = pGroup->GetByName(sElement);
			if (!pGroupElement)
				pGroupElement = pGroup->Create(sElement);
			if (pGroupElement) {
				// Set element text
				pGroupElement->SetText(sText);

				// Done
				return true;
			}
		}
	}

	// Error!
	return false;
}

/**
*  @brief
*    Returns the element text
*/
String Profiling::GetText(const String &sGroup, const String &sElement) const
{
	// Get group
	const ProfileGroup *pGroup = GetByName(sGroup);
	if (pGroup) {
		// Get element
		const ProfileGroupElement *pGroupElement = pGroup->GetByName(sElement);
		if (pGroupElement)
			return pGroupElement->GetText(); // Return text
	}

	// Error!
	return "";
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Profiling::Profiling() :
	m_bActive(false)
{
}

/**
*  @brief
*    Copy constructor
*/
Profiling::Profiling(const Profiling &cSource) :
	m_bActive(false)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Destructor
*/
Profiling::~Profiling()
{
}

/**
*  @brief
*    Copy operator
*/
Profiling &Profiling::operator =(const Profiling &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Private virtual ElementManager functions              ]
//[-------------------------------------------------------]
ProfileGroup *Profiling::CreateElement(const String &sName)
{
	return new ProfileGroup(sName, this);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
