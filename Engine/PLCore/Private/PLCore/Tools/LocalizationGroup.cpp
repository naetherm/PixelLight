/*********************************************************\
 *  File: LocalizationGroup.cpp                          *
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
#include "PLCore/Tools/Loader.h"
#include "PLCore/Tools/LocalizationText.h"
#include "PLCore/Tools/LocalizationGroup.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public static data                                    ]
//[-------------------------------------------------------]
const String LocalizationGroup::English = "English";


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Adds a new text
*/
LocalizationText *LocalizationGroup::AddText(const String &sName, const String &sTranslation)
{
	// Is there already a text with the given name?
	LocalizationText *pText = m_mapTexts.Get(sName);
	if (pText)
		return nullptr; // Jap, error!

	// Create the new text
	pText = new LocalizationText(*this, sName, sTranslation);
	m_lstTexts.Add(pText);
	m_mapTexts.Add(sName, pText);

	// Return the new text
	return pText;
}

/**
*  @brief
*    Removes a text by index
*/
bool LocalizationGroup::RemoveText(uint32 nIndex)
{
	// Get the text
	LocalizationText *pText = m_lstTexts[nIndex];
	if (!pText)
		return false; // Error!

	// Remove the text
	m_mapTexts.Remove(pText->GetName());
	m_lstTexts.RemoveAtIndex(nIndex);
	delete pText;

	// Done
	return true;
}

/**
*  @brief
*    Removes a text by name
*/
bool LocalizationGroup::RemoveText(const String &sName)
{
	// Get the text
	LocalizationText *pText = m_mapTexts.Get(sName);
	if (!pText)
		return false; // Error!

	// Remove the text
	m_mapTexts.Remove(pText->GetName());
	m_lstTexts.Remove(pText);
	delete pText;

	// Done
	return true;
}

/**
*  @brief
*    Removes all texts
*/
void LocalizationGroup::RemoveAllTexts()
{
	for (uint32 i=0; i<m_lstTexts.GetNumOfElements(); i++)
		delete m_lstTexts[i];
	m_lstTexts.Clear();
	m_mapTexts.Clear();
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
LocalizationGroup::LocalizationGroup(const String &sName) :
	m_sName(sName),
	m_sFromLanguage(English),
	m_sToLanguage(English)
{
}

/**
*  @brief
*    Copy constructor
*/
LocalizationGroup::LocalizationGroup(const LocalizationGroup &cSource)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Destructor
*/
LocalizationGroup::~LocalizationGroup()
{
}

/**
*  @brief
*    Copy operator
*/
LocalizationGroup &LocalizationGroup::operator =(const LocalizationGroup &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Public virtual Loadable functions                     ]
//[-------------------------------------------------------]
bool LocalizationGroup::Unload()
{
	// Cleanup
	RemoveAllTexts();
	m_sFromLanguage = English;
	m_sToLanguage   = English;

	// Call base implementation
	return Loadable::Unload();
}

String LocalizationGroup::GetLoadableTypeName() const
{
	return "Localization";
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
