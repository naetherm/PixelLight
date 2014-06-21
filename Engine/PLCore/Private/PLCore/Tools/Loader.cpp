/*********************************************************\
 *  File: Loader.cpp                                     *
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
#include "PLCore/Base/Class.h"
#include "PLCore/String/Tokenizer.h"
#include "PLCore/Tools/LoaderImpl.h"
#include "PLCore/Tools/Loader.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the loader implementation
*/
LoaderImpl *Loader::GetImpl()
{
	// Create the loader implementation class instance if required right now
	if (!m_pLoaderImpl)
		m_pLoaderImpl = reinterpret_cast<LoaderImpl*>(m_pClass->Create());

	// Return the loader implementation class instance
	return m_pLoaderImpl;
}

/**
*  @brief
*    Returns the name of the loadable type
*/
String Loader::GetTypeName() const
{
	const Class *pClass = m_pClass;
	return pClass ? pClass->GetProperties().Get("Type") : "";
}

/**
*  @brief
*    Checks if a format is supported in a list of extensions
*/
bool Loader::IsFormatSupported(const String &sExtension)
{
	// Check all supported formats
	InitFormatsList();
	for (uint32 i=0; i<m_lstFormats.GetNumOfElements(); i++) {
		if (m_lstFormats[i] == sExtension)
			return true; // Format found
	}

	// The format is not supported
	return false;
}

/**
*  @brief
*    Returns the formats string
*/
String Loader::GetFormats() const
{
	const Class *pClass = m_pClass;
	return pClass ? pClass->GetProperties().Get("Formats") : "";
}

/**
*  @brief
*    Returns the description string
*/
String Loader::GetDescription() const
{
	const Class *pClass = m_pClass;
	return pClass ? pClass->GetDescription() : "";
}

/**
*  @brief
*    Returns the whether loading is supported
*/
bool Loader::CanLoad() const
{
	const Class *pClass = m_pClass;
	return pClass ? pClass->GetProperties().Get("Load").GetBool() : false;
}

/**
*  @brief
*    Returns the whether saving is supported
*/
bool Loader::CanSave() const
{
	const Class *pClass = m_pClass;
	return pClass ? pClass->GetProperties().Get("Save").GetBool() : false;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
Loader::Loader() :
	m_pClass(nullptr),
	m_pLoaderImpl(nullptr),
	m_pLoadableType(nullptr)
{
	// No implementation because the copy operator is never used
}

/**
*  @brief
*    Constructor
*/
Loader::Loader(const Class &cClass) :
	m_pClass(&cClass),
	m_pLoaderImpl(nullptr),
	m_pLoadableType(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
Loader::~Loader()
{
	// Destroy the loader implementation class instance
	if (m_pLoaderImpl)
		delete m_pLoaderImpl;
}

/**
*  @brief
*    Copy operator
*/
Loader &Loader::operator =(const Loader &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}

/**
*  @brief
*    Initializes the formats list
*/
void Loader::InitFormatsList()
{
	// Is the list already initialized?
	if (!m_lstFormats.GetNumOfElements()) {
		// Parse formats all supported formats
		const String sFormats = GetFormats();
		if (sFormats.GetLength()) {
			// Setup the tokenizer
			Tokenizer cTokenizer;
			cTokenizer.Start(sFormats);
			cTokenizer.SetDelimiters(" ,\t\r\n");
			cTokenizer.SetSingleChars("");

			// Special case: There are file formats which don't have any filename extension or
			// even expect that it's data is within a provided directory (example: DICOM)
			// -> This is no funny thing, but this system must be able to deal with such formats as well
			if (sFormats[static_cast<uint32>(0)] == ',') {
				// No filename extension
				m_lstFormats.Add("");
			}

			// Register the filename extensions of this loader
			String sToken = cTokenizer.GetNextToken();
			while (sToken.GetLength()) {
				m_lstFormats.Add(sToken);
				sToken = cTokenizer.GetNextToken();
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
