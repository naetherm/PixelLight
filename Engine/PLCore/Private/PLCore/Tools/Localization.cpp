/*********************************************************\
 *  File: Localization.cpp                               *
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
#include "PLCore/Tools/LocalizationText.h"
#include "PLCore/Tools/LocalizationGroup.h"
#include "PLCore/Tools/Localization.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Template instance                                     ]
//[-------------------------------------------------------]
template class Singleton<Localization>;


//[-------------------------------------------------------]
//[ Public static data                                    ]
//[-------------------------------------------------------]
const String Localization::PixelLight = "PixelLight";


//[-------------------------------------------------------]
//[ Public static PLCore::Singleton functions             ]
//[-------------------------------------------------------]
Localization *Localization::GetInstance()
{
	// The compiler should be able to optimize this extra call, at least inside this project (inlining)
	return Singleton<Localization>::GetInstance();
}

bool Localization::HasInstance()
{
	// The compiler should be able to optimize this extra call, at least inside this project (inlining)
	return Singleton<Localization>::HasInstance();
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the translation of the given text
*/
String Localization::Get(const String &sText, const String &sGroup) const
{
	// Get the group
	const LocalizationGroup *pGroup = m_mapGroups.Get(sGroup);
	if (pGroup) {
		// Get the text
		const LocalizationText *pText = pGroup->GetText(sText);
		if (pText)
			return pText->GetTranslation();
	}

	// Error! Return the given text instead...
	return sText;
}

/**
*  @brief
*    Adds a new group
*/
LocalizationGroup *Localization::AddGroup(const String &sName)
{
	// Is there already a group with the given name?
	LocalizationGroup *pGroup = m_mapGroups.Get(sName);
	if (pGroup)
		return nullptr; // Jap, error!

	// Create the new group
	pGroup = new LocalizationGroup(sName);
	m_lstGroups.Add(pGroup);
	m_mapGroups.Add(sName, pGroup);

	// Return the new group
	return pGroup;
}

/**
*  @brief
*    Removes a group by index
*/
bool Localization::RemoveGroup(uint32 nIndex)
{
	// Get the group
	LocalizationGroup *pGroup = m_lstGroups[nIndex];
	if (!pGroup)
		return false; // Error!

	// Remove the group
	m_mapGroups.Remove(pGroup->GetName());
	m_lstGroups.RemoveAtIndex(nIndex);
	delete pGroup;

	// Done
	return true;
}

/**
*  @brief
*    Removes a group by name
*/
bool Localization::RemoveGroup(const String &sName)
{
	// Get the group
	LocalizationGroup *pGroup = m_mapGroups.Get(sName);
	if (!pGroup)
		return false; // Error!

	// Remove the group
	m_mapGroups.Remove(pGroup->GetName());
	m_lstGroups.Remove(pGroup);
	delete pGroup;

	// Done
	return true;
}

/**
*  @brief
*    Removes all groups
*/
void Localization::RemoveAllGroups()
{
	for (uint32 i=0; i<m_lstGroups.GetNumOfElements(); i++)
		delete m_lstGroups[i];
	m_lstGroups.Clear();
	m_mapGroups.Clear();
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Localization::Localization()
{
}

/**
*  @brief
*    Copy constructor
*/
Localization::Localization(const Localization &cSource)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Destructor
*/
Localization::~Localization()
{
	// Cleanup
	RemoveAllGroups();
}

/**
*  @brief
*    Copy operator
*/
Localization &Localization::operator =(const Localization &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
