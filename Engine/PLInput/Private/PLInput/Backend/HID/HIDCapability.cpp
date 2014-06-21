/*********************************************************\
 *  File: HIDCapability.cpp                              *
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
#include "PLInput/Backend/HID/HIDCapability.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLInput {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
HIDCapability::HIDCapability() :
	m_nReportID(0),
	m_nLinkCollection(0),
	m_nUsagePage(0),
	m_nUsage(0),
	m_nUsageMin(0),
	m_nUsageMax(0),
	m_nDataIndex(0),
	m_nDataIndexMin(0),
	m_nDataIndexMax(0),
	m_nBitSize(0),
	m_nLogicalMin(0),
	m_nLogicalMax(0),
	m_nPhysicalMin(0),
	m_nPhysicalMax(0),
	m_nValue(0)
{
}

/**
*  @brief
*    Copy constructor
*/
HIDCapability::HIDCapability(const HIDCapability &cSource) :
	m_nReportID(cSource.m_nReportID),
	m_nLinkCollection(cSource.m_nLinkCollection),
	m_nUsagePage(cSource.m_nUsagePage),
	m_nUsage(cSource.m_nUsage),
	m_nUsageMin(cSource.m_nUsageMin),
	m_nUsageMax(cSource.m_nUsageMax),
	m_nDataIndex(cSource.m_nDataIndex),
	m_nDataIndexMin(cSource.m_nDataIndexMin),
	m_nDataIndexMax(cSource.m_nDataIndexMax),
	m_nBitSize(cSource.m_nBitSize),
	m_nLogicalMin(cSource.m_nLogicalMin),
	m_nLogicalMax(cSource.m_nLogicalMax),
	m_nPhysicalMin(cSource.m_nPhysicalMin),
	m_nPhysicalMax(cSource.m_nPhysicalMax),
	m_nValue(cSource.m_nValue)
{
}

/**
*  @brief
*    Destructor
*/
HIDCapability::~HIDCapability()
{
}

/**
*  @brief
*    Assignment operator
*/
HIDCapability &HIDCapability::operator =(const HIDCapability &cSource)
{
	m_nReportID			= cSource.m_nReportID;
	m_nLinkCollection	= cSource.m_nLinkCollection;
	m_nUsagePage		= cSource.m_nUsagePage;
	m_nUsage			= cSource.m_nUsage;
	m_nUsageMin			= cSource.m_nUsageMin;
	m_nUsageMax			= cSource.m_nUsageMax;
	m_nDataIndex		= cSource.m_nDataIndex;
	m_nDataIndexMin		= cSource.m_nDataIndexMin;
	m_nDataIndexMax		= cSource.m_nDataIndexMax;
	m_nBitSize			= cSource.m_nBitSize;
	m_nLogicalMin		= cSource.m_nLogicalMin;
	m_nLogicalMax		= cSource.m_nLogicalMax;
	m_nPhysicalMin		= cSource.m_nPhysicalMin;
	m_nPhysicalMax		= cSource.m_nPhysicalMax;
	m_nValue			= cSource.m_nValue;
	return *this;
}

/**
*  @brief
*    Comparison operator
*/
bool HIDCapability::operator ==(const HIDCapability &cSource) const
{
	return (m_nReportID	== cSource.m_nReportID &&  m_nLinkCollection == cSource.m_nLinkCollection &&
			m_nUsagePage == cSource.m_nUsagePage && m_nUsage == cSource.m_nUsage &&
			m_nUsageMin == cSource.m_nUsageMin && m_nUsageMax == cSource.m_nUsageMax &&
			m_nDataIndex == cSource.m_nDataIndex && m_nDataIndexMin == cSource.m_nDataIndexMin && m_nDataIndexMax == cSource.m_nDataIndexMax &&
			m_nBitSize == cSource.m_nBitSize && m_nLogicalMin == cSource.m_nLogicalMin &&
			m_nLogicalMax == cSource.m_nLogicalMax && m_nPhysicalMin == cSource.m_nPhysicalMin &&
			m_nPhysicalMax == cSource.m_nPhysicalMax && m_nValue == cSource.m_nValue);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
