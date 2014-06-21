/*********************************************************\
 *  File: SizeHint.cpp                                   *
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
#include <PLCore/String/RegEx.h>
#include "PLGui/Gui/Data/SizeHint.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLGui {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SizeHint::SizeHint(EPolicy nPolicy, uint32 nSize) :
	m_nPolicy(nPolicy),
	m_fSize(static_cast<float>(nSize))
{
}

/**
*  @brief
*    Copy constructor
*/
SizeHint::SizeHint(const SizeHint &cSizeHint) :
	m_nPolicy(cSizeHint.m_nPolicy),
	m_fSize(cSizeHint.m_fSize)
{
}

/**
*  @brief
*    Constructor
*/
SizeHint::SizeHint(const String &sString)
{
	// The following method sets all components, so we don't need the initializer list in here
	FromString(sString);
}

/**
*  @brief
*    Destructor
*/
SizeHint::~SizeHint()
{
}

/**
*  @brief
*    Copy operator
*/
SizeHint &SizeHint::operator =(const SizeHint &cSizeHint)
{
	// Copy data
	m_nPolicy	= cSizeHint.m_nPolicy;
	m_fSize		= cSizeHint.m_fSize;
	return *this;
}

/**
*  @brief
*    Comparison operator
*/
bool SizeHint::operator ==(const SizeHint &cSizeHint) const
{
	// Compare data
	return (m_nPolicy == cSizeHint.m_nPolicy && m_fSize == cSizeHint.m_fSize);
}

/**
*  @brief
*    Get size policy
*/
SizeHint::EPolicy SizeHint::GetPolicy() const
{
	// Return policy
	return m_nPolicy;
}

/**
*  @brief
*    Set size policy
*/
void SizeHint::SetPolicy(EPolicy nPolicy)
{
	// Set policy
	m_nPolicy = nPolicy;
}

/**
*  @brief
*    Get size
*/
uint32 SizeHint::GetSize() const
{
	// Return size
	return static_cast<uint32>(m_fSize);
}

/**
*  @brief
*    Get size in float
*/
float SizeHint::GetSizeFloat() const
{
	// Return size
	return m_fSize;
}

/**
*  @brief
*    Set size
*/
void SizeHint::SetSize(uint32 nSize)
{
	// Set size
	m_fSize = static_cast<float>(nSize);
}

/**
*  @brief
*    Set size in float
*/
void SizeHint::SetSizeFloat(float fSize)
{
	// Set size
	m_fSize = fSize;
}

/**
*  @brief
*    Set policy and size
*/
void SizeHint::Set(EPolicy nPolicy, uint32 nSize)
{
	// Set policy
	m_nPolicy = nPolicy;

	// Set size
	m_fSize = static_cast<float>(nSize);
}

/**
*  @brief
*    Set policy and size in float
*/
void SizeHint::SetFloat(EPolicy nPolicy, float fSize)
{
	// Set policy
	m_nPolicy = nPolicy;

	// Set size
	m_fSize = fSize;
}

/**
*  @brief
*    Calculate actual size specified by this size hint
*/
int SizeHint::CalculateSize(uint32 nParentSize, int nPreferredSize) const
{
	// Calculate size
		 if (m_nPolicy == Pixel)							return static_cast<int>(m_fSize);
	else if (m_nPolicy == Percent)							return static_cast<int>((m_fSize / 100.0f) * nParentSize);
	else if (m_nPolicy == Preferred && nPreferredSize > -1)	return nPreferredSize;
	else													return -1;
}

/**
*  @brief
*    To string
*/
String SizeHint::ToString() const
{
	// Get policy
	switch (m_nPolicy) {
		// Pixel
		case Pixel:
			return String() + static_cast<int>(m_fSize) + "px";

		// Percent
		case Percent:
			return String() + m_fSize + '%';

		// Preferred
		case Preferred:
			return "preferred";

		// Floating
		case Floating:
		default:
			return "floating";
	}
}

/**
*  @brief
*    From string
*/
bool SizeHint::FromString(const String &sString)
{
	// Get policy
	if (sString == "floating") {
		// Floating
		m_nPolicy = Floating;
		m_fSize	  = 0.0f;
	} else if (sString == "preferred") {
		// Preferred
		m_nPolicy = Preferred;
		m_fSize	  = 0.0f;
	} else {
		// Pixel or Percent?
		static RegEx cRegEx("\\s*([-+]?\\d*\\.?\\d+)\\s*(px|\\%)");
		if (cRegEx.Match(sString)) {
			// Pixel or Percent!
			String sSize   = cRegEx.GetResult(0);
			String sPolicy = cRegEx.GetResult(1);
			m_nPolicy = (sPolicy == "px" ? Pixel : Percent);
			m_fSize   = sSize.GetFloat();
		} else if (sString.GetFloat() > 0.0f) {
			// Just a number
			m_nPolicy = Pixel;
			m_fSize   = sString.GetFloat();
		} else {
			// Invalid
			return false;
		}
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui
