/*********************************************************\
 *  File: HIDDevice.cpp                                  *
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
#include "PLInput/Backend/HID/HIDDevice.h"


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
HIDDevice::HIDDevice() :
	// Device data
	m_nVendor(0),
	m_nProduct(0),
	// Device capabilities
	m_nUsagePage(0),
	m_nUsage(0),
	m_nFeatureReportByteLength(0),
	m_nNumberLinkCollectionNodes(0),
	m_nNumberInputButtonCaps(0),
	m_nNumberInputValueCaps(0),
	m_nNumberInputDataIndices(0),
	m_nNumberOutputButtonCaps(0),
	m_nNumberOutputValueCaps(0),
	m_nNumberOutputDataIndices(0),
	m_nNumberFeatureButtonCaps(0),
	m_nNumberFeatureValueCaps(0),
	m_nNumberFeatureDataIndices(0)
{
	// Set backend type
	m_nBackendType = BackendHID;

	// Set device type
	m_nDeviceType = ConnectionDevice::DeviceTypeHID;
}

/**
*  @brief
*    Copy constructor
*/
HIDDevice::HIDDevice(const HIDDevice &cSource) :
	// Device data
	m_sName(cSource.m_sName),
	m_nVendor(cSource.m_nVendor),
	m_nProduct(cSource.m_nProduct),
	// Device capabilities
	m_nUsagePage(cSource.m_nUsagePage),
	m_nUsage(cSource.m_nUsage),
	m_nFeatureReportByteLength(cSource.m_nFeatureReportByteLength),
	m_nNumberLinkCollectionNodes(cSource.m_nNumberLinkCollectionNodes),
	m_nNumberInputButtonCaps(cSource.m_nNumberInputButtonCaps),
	m_nNumberInputValueCaps(cSource.m_nNumberInputValueCaps),
	m_nNumberInputDataIndices(cSource.m_nNumberInputDataIndices),
	m_nNumberOutputButtonCaps(cSource.m_nNumberOutputButtonCaps),
	m_nNumberOutputValueCaps(cSource.m_nNumberOutputValueCaps),
	m_nNumberOutputDataIndices(cSource.m_nNumberOutputDataIndices),
	m_nNumberFeatureButtonCaps(cSource.m_nNumberFeatureButtonCaps),
	m_nNumberFeatureValueCaps(cSource.m_nNumberFeatureValueCaps),
	m_nNumberFeatureDataIndices(cSource.m_nNumberFeatureDataIndices),
	// Controls
	m_lstInputButtons(cSource.m_lstInputButtons),
	m_lstInputValues(cSource.m_lstInputValues),
	m_lstOutputValues(cSource.m_lstOutputValues)
{
	// Set device type
	m_nDeviceType = ConnectionDevice::DeviceTypeHID;

	// Set input and output report sizes
	m_nInputReportSize  = cSource.m_nInputReportSize;
	m_nOutputReportSize = cSource.m_nOutputReportSize;
}

/**
*  @brief
*    Destructor
*/
HIDDevice::~HIDDevice()
{
}

/**
*  @brief
*    Assignment operator
*/
HIDDevice &HIDDevice::operator =(const HIDDevice &cSource)
{
	// Device data
	m_sName							= cSource.m_sName;
	m_nVendor						= cSource.m_nVendor;
	m_nProduct						= cSource.m_nProduct;

	// Device capabilities
	m_nUsagePage					= cSource.m_nUsagePage;
	m_nUsage						= cSource.m_nUsage;
	m_nInputReportSize				= cSource.m_nInputReportSize;
	m_nOutputReportSize				= cSource.m_nOutputReportSize;
	m_nFeatureReportByteLength		= cSource.m_nFeatureReportByteLength;
	m_nNumberLinkCollectionNodes	= cSource.m_nNumberLinkCollectionNodes;
	m_nNumberInputButtonCaps		= cSource.m_nNumberInputButtonCaps;
	m_nNumberInputValueCaps			= cSource.m_nNumberInputValueCaps;
	m_nNumberInputDataIndices		= cSource.m_nNumberInputDataIndices;
	m_nNumberOutputButtonCaps		= cSource.m_nNumberOutputButtonCaps;
	m_nNumberOutputValueCaps		= cSource.m_nNumberOutputValueCaps;
	m_nNumberOutputDataIndices		= cSource.m_nNumberOutputDataIndices;
	m_nNumberFeatureButtonCaps		= cSource.m_nNumberFeatureButtonCaps;
	m_nNumberFeatureValueCaps		= cSource.m_nNumberFeatureValueCaps;
	m_nNumberFeatureDataIndices		= cSource.m_nNumberFeatureDataIndices;

	// Controls
	m_lstInputButtons				= cSource.m_lstInputButtons;
	m_lstInputValues				= cSource.m_lstInputValues;
	m_lstOutputValues				= cSource.m_lstOutputValues;

	// Done
	return *this;
}

/**
*  @brief
*    Comparison operator
*/
bool HIDDevice::operator ==(const HIDDevice &cSource)
{
			// Device data
	return (m_nVendor						== cSource.m_nVendor &&
			m_nProduct						== cSource.m_nProduct &&
			// Device capabilities
			m_nUsagePage					== cSource.m_nUsagePage &&
			m_nUsage						== cSource.m_nUsage &&
			m_nInputReportSize				== cSource.m_nInputReportSize &&
			m_nOutputReportSize				== cSource.m_nOutputReportSize &&
			m_nFeatureReportByteLength		== cSource.m_nFeatureReportByteLength &&
			m_nNumberLinkCollectionNodes	== cSource.m_nNumberLinkCollectionNodes &&
			m_nNumberInputButtonCaps		== cSource.m_nNumberInputButtonCaps &&
			m_nNumberInputValueCaps			== cSource.m_nNumberInputValueCaps &&
			m_nNumberInputDataIndices		== cSource.m_nNumberInputDataIndices &&
			m_nNumberOutputButtonCaps		== cSource.m_nNumberOutputButtonCaps &&
			m_nNumberOutputValueCaps		== cSource.m_nNumberOutputValueCaps &&
			m_nNumberOutputDataIndices		== cSource.m_nNumberOutputDataIndices &&
			m_nNumberFeatureButtonCaps		== cSource.m_nNumberFeatureButtonCaps &&
			m_nNumberFeatureValueCaps		== cSource.m_nNumberFeatureValueCaps &&
			m_nNumberFeatureDataIndices		== cSource.m_nNumberFeatureDataIndices &&
			// Controls
			m_lstInputButtons				== cSource.m_lstInputButtons &&
			m_lstInputValues				== cSource.m_lstInputValues &&
			m_lstOutputValues				== cSource.m_lstOutputValues);
}

/**
*  @brief
*    Get device name
*/
String HIDDevice::GetName() const
{
	return m_sName;
}

/**
*  @brief
*    Get vendor
*/
uint32 HIDDevice::GetVendor() const
{
	return m_nVendor;
}

/**
*  @brief
*    Get product
*/
uint32 HIDDevice::GetProduct() const
{
	return m_nProduct;
}

/**
*  @brief
*    Get usage
*/
uint16 HIDDevice::GetUsage() const
{
	return m_nUsage;
}

/**
*  @brief
*    Get usage page
*/
uint16 HIDDevice::GetUsagePage() const
{
	return m_nUsagePage;
}

/**
*  @brief
*    Get input button controls
*/
const List<HIDCapability> &HIDDevice::GetInputButtons() const
{
	return m_lstInputButtons;
}

/**
*  @brief
*    Get input value controls
*/
const List<HIDCapability> &HIDDevice::GetInputValues() const
{
	return m_lstInputValues;
}

/**
*  @brief
*    Get output value controls
*/
const List<HIDCapability> &HIDDevice::GetOutputValues() const
{
	return m_lstOutputValues;
}

/**
*  @brief
*    Parse input report
*/
void HIDDevice::ParseInputReport(const uint8 *pInputReport, uint32 nSize)
{
	// Call system specific backend function to parse input report data
	ParseInputReportData(pInputReport, nSize);

	// [TODO]
	// This function should contain an own implementation to parse the input report rather than
	// relying on system-specific APIs. However, my attempt to implement one failed because the data indices
	// seem to be awkward and I can't see how to parse the data items correctly. So, let's stick with this
	// for a while, until someone with time and knowledge takes the challenge :-)
}


//[-------------------------------------------------------]
//[ Public virtual HIDDevice functions                    ]
//[-------------------------------------------------------]
/**
*  @brief
*    Send HID output report
*/
void HIDDevice::SendOutputReport()
{
	// Call system specific backend function
	SendOutputReportData();

	// [TODO]
	// This function should contain an own implementation rather than relying on system-specific APIs
}


//[-------------------------------------------------------]
//[ Protected virtual HIDDevice functions                 ]
//[-------------------------------------------------------]
/**
*  @brief
*    Parse HID input report
*/
void HIDDevice::ParseInputReportData(const uint8 *pInputReport, uint32 nSize)
{
	// To be implemented in derived classes
}

/**
*  @brief
*    Send HID output report
*/
void HIDDevice::SendOutputReportData()
{
	// To be implemented in derived classes
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
