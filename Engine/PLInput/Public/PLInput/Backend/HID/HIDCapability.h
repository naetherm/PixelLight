/*********************************************************\
 *  File: HIDCapability.h                                *
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


#ifndef __PLINPUT_HIDCAPABILITY_H__
#define __PLINPUT_HIDCAPABILITY_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLInput/PLInput.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLInput {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Capability of a HID device
*/
class HIDCapability {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLINPUT_API HIDCapability();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Capability to copy from
		*/
		PLINPUT_API HIDCapability(const HIDCapability &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		PLINPUT_API ~HIDCapability();

		/**
		*  @brief
		*    Assignment operator
		*
		*  @param[in] cSource
		*    Capability to copy from
		*
		*  @return
		*    Reference to this HIDCapability
		*/
		PLINPUT_API HIDCapability &operator =(const HIDCapability &cSource);

		/**
		*  @brief
		*    Comparison operator
		*
		*  @param[in] cSource
		*    Capability to compare with
		*
		*  @return
		*    'true' if both are equal, else 'false'
		*/
		PLINPUT_API bool operator ==(const HIDCapability &cSource) const;


	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		// Capability info
		PLCore::uint8  m_nReportID;			/**< Report ID */
		PLCore::uint16 m_nLinkCollection;	/**< Link collection */
		PLCore::uint16 m_nUsagePage;		/**< Usage page ID */
		PLCore::uint16 m_nUsage;			/**< Usage ID */
		PLCore::uint16 m_nUsageMin;			/**< Usage ID minimum */
		PLCore::uint16 m_nUsageMax;			/**< Usage ID maximum */
		PLCore::uint16 m_nDataIndex;		/**< Data index (index in input report - NOT byte index!) */
		PLCore::uint16 m_nDataIndexMin;		/**< Data index minimum */
		PLCore::uint16 m_nDataIndexMax;		/**< Data index maximum */
		PLCore::uint16 m_nBitSize;			/**< Size of data in bits */
		PLCore::uint16 m_nLogicalMin;		/**< Logical minimum value (e.g. for axes) */
		PLCore::uint16 m_nLogicalMax;		/**< Logical maximum value (e.g. for axes) */
		PLCore::uint16 m_nPhysicalMin;		/**< Physical minimum value (e.g. for axes) */
		PLCore::uint16 m_nPhysicalMax;		/**< Physical maximum value (e.g. for axes) */

		// Current data value
		PLCore::uint32 m_nValue;			/**< Current value of input data */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput


#endif // __PLINPUT_HIDCAPABILITY_H__
