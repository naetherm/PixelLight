/*********************************************************\
 *  File: HIDWindows.cpp                                 *
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
#include <PLCore/Log/Log.h>
#include <PLCore/System/DynLib.h>
#include "PLInput/Backend/HID/+Windows/HIDWindows.h"
#include "PLInput/Backend/HID/+Windows/HIDDeviceWindows.h"


//[-------------------------------------------------------]
//[ Global HID function pointers (HIDWindows.cpp)         ]
//[-------------------------------------------------------]
PFNHIDDGETPREPARSEDDATA		HidD_GetPreparsedData	= nullptr;
PFNHIDDFREEPREPARSEDDATA	HidD_FreePreparsedData	= nullptr;
PFNHIDPGETDATA				HidP_GetData			= nullptr;
PFNHIDPSETDATA				HidP_SetData			= nullptr;
PFNHIDPGETHIDGUID			HidD_GetHidGuid			= nullptr;
PFNHIDPGETATTRIBUTES		HidD_GetAttributes		= nullptr;
PFNHIDPGETCAPS				HidP_GetCaps			= nullptr;
PFNHIDPGETBUTTONCAPS		HidP_GetButtonCaps		= nullptr;
PFNHIDPGETVALUECAPS			HidP_GetValueCaps		= nullptr;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLInput {


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
HIDWindows::HIDWindows() :
	m_pDynLibHID(new DynLib())
{
	// Try to load "hid.dll"
	if (m_pDynLibHID->Load("hid.dll")) {
		// Get global HID function pointers
		HidD_GetPreparsedData	= static_cast<PFNHIDDGETPREPARSEDDATA>	(m_pDynLibHID->GetSymbol("HidD_GetPreparsedData"));
		HidD_FreePreparsedData	= static_cast<PFNHIDDFREEPREPARSEDDATA>	(m_pDynLibHID->GetSymbol("HidD_FreePreparsedData"));
		HidP_GetData			= static_cast<PFNHIDPGETDATA>			(m_pDynLibHID->GetSymbol("HidP_GetData"));
		HidP_SetData			= static_cast<PFNHIDPSETDATA>			(m_pDynLibHID->GetSymbol("HidP_SetData"));
		HidD_GetHidGuid			= static_cast<PFNHIDPGETHIDGUID>		(m_pDynLibHID->GetSymbol("HidD_GetHidGuid"));
		HidD_GetAttributes		= static_cast<PFNHIDPGETATTRIBUTES>		(m_pDynLibHID->GetSymbol("HidD_GetAttributes"));
		HidP_GetCaps			= static_cast<PFNHIDPGETCAPS>			(m_pDynLibHID->GetSymbol("HidP_GetCaps"));
		HidP_GetButtonCaps		= static_cast<PFNHIDPGETBUTTONCAPS>		(m_pDynLibHID->GetSymbol("HidP_GetButtonCaps"));
		HidP_GetValueCaps		= static_cast<PFNHIDPGETVALUECAPS>		(m_pDynLibHID->GetSymbol("HidP_GetValueCaps"));

		// Ensure that we have valid function pointers
		if (!HidD_GetPreparsedData || !HidD_FreePreparsedData || !HidP_GetData || !HidP_SetData || !HidD_GetHidGuid ||
			!HidD_GetAttributes || !HidP_GetCaps || !HidP_GetButtonCaps || !HidP_GetValueCaps) {
			// Error!
			PL_LOG(Error, "Not all required symbols within \"hid.dll\" were found, as a result, no HID devices can be used")
		}
	} else {
		// Error!
		PL_LOG(Error, "Failed to load in \"hid.dll\", as a result, no HID devices can be used")
	}

	// Get device interface GUID
	if (HidD_GetHidGuid)
		HidD_GetHidGuid(&m_sGUID);
}

/**
*  @brief
*    Destructor
*/
HIDWindows::~HIDWindows()
{
	// Clear devices
	for (uint32 i=0; i<m_lstDevices.GetNumOfElements(); i++)
		delete m_lstDevices[i];
	m_lstDevices.Clear();

	// Destroy the dynamic HID library object
	delete m_pDynLibHID;
}


//[-------------------------------------------------------]
//[ Private virtual HIDImpl functions                     ]
//[-------------------------------------------------------]
void HIDWindows::EnumerateDevices(List<HIDDevice*> &lstDevices)
{
	// Clear devices
	for (uint32 i=0; i<m_lstDevices.GetNumOfElements(); i++)
		delete m_lstDevices[i];
	m_lstDevices.Clear();

	// We're going to use HID functions, ensure that we have valid function pointers (there's no need for additional tests within HIDDeviceWindows!)
	if (!HidD_GetPreparsedData || !HidD_FreePreparsedData || !HidP_GetData || !HidP_SetData || !HidD_GetHidGuid ||
		!HidD_GetAttributes || !HidP_GetCaps || !HidP_GetButtonCaps || !HidP_GetValueCaps) {
		return;
	}

	// Create handle
	HDEVINFO hDevInfo = SetupDiGetClassDevs(&m_sGUID, nullptr, nullptr, DIGCF_DEVICEINTERFACE);
	if (!hDevInfo)
		return;

	// Get device interface
	SP_DEVICE_INTERFACE_DATA sDevice;
	sDevice.cbSize = sizeof(sDevice);
	uint32 nDevice = 0;
	while (SetupDiEnumDeviceInterfaces(hDevInfo, nullptr, &m_sGUID, nDevice, &sDevice)) {
		// Allocate buffer for device interface details
		DWORD nDetailsSize = 0;
		SetupDiGetDeviceInterfaceDetail(hDevInfo, &sDevice, nullptr, 0, &nDetailsSize, nullptr);
		BYTE *pBuffer = new BYTE[nDetailsSize];
		SP_DEVICE_INTERFACE_DETAIL_DATA *sDeviceDetail = reinterpret_cast<SP_DEVICE_INTERFACE_DETAIL_DATA*>(pBuffer);
		sDeviceDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		// Get device interface details
		if (SetupDiGetDeviceInterfaceDetail(hDevInfo, &sDevice, sDeviceDetail, nDetailsSize, &nDetailsSize, nullptr)) {
			// Create device
			HIDDeviceWindows *pDevice = new HIDDeviceWindows();
			pDevice->m_sName = sDeviceDetail->DevicePath;

			// Open device to get detailed information
			HANDLE hDevice = CreateFile(sDeviceDetail->DevicePath, GENERIC_READ | GENERIC_WRITE,
										FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
			if (hDevice != INVALID_HANDLE_VALUE) {
				// Get device attributes
				HIDD_ATTRIBUTES sAttrib;
				sAttrib.Size = sizeof(sAttrib);
				if (HidD_GetAttributes(hDevice, &sAttrib)) {
					// Set device attributes
					pDevice->m_nVendor	= sAttrib.VendorID;
					pDevice->m_nProduct	= sAttrib.ProductID;

					// Get preparsed data
					if (HidD_GetPreparsedData(hDevice, &pDevice->m_pPreparsedData) == TRUE) {
						// Get device capabilities
						HIDP_CAPS sCaps;
						if (HidP_GetCaps(pDevice->m_pPreparsedData, &sCaps) == HIDP_STATUS_SUCCESS) {
							// Save capabilities
							pDevice->m_nUsage						= sCaps.Usage;
							pDevice->m_nUsagePage					= sCaps.UsagePage;
							pDevice->m_nInputReportSize				= sCaps.InputReportByteLength;
							pDevice->m_nOutputReportSize			= sCaps.OutputReportByteLength;
							pDevice->m_nFeatureReportByteLength		= sCaps.FeatureReportByteLength;
							pDevice->m_nNumberLinkCollectionNodes	= sCaps.NumberLinkCollectionNodes;
							pDevice->m_nNumberInputButtonCaps		= sCaps.NumberInputButtonCaps;
							pDevice->m_nNumberInputValueCaps		= sCaps.NumberInputValueCaps;
							pDevice->m_nNumberInputDataIndices		= sCaps.NumberInputDataIndices;
							pDevice->m_nNumberOutputButtonCaps		= sCaps.NumberOutputButtonCaps;
							pDevice->m_nNumberOutputValueCaps		= sCaps.NumberOutputValueCaps;
							pDevice->m_nNumberOutputDataIndices		= sCaps.NumberOutputDataIndices;
							pDevice->m_nNumberFeatureButtonCaps		= sCaps.NumberFeatureButtonCaps;
							pDevice->m_nNumberFeatureValueCaps		= sCaps.NumberFeatureValueCaps;
							pDevice->m_nNumberFeatureDataIndices	= sCaps.NumberFeatureDataIndices;

							// Enumerate buttons
							if (sCaps.NumberInputButtonCaps > 0) {
								USHORT nSize = sCaps.NumberInputButtonCaps;
								HIDP_BUTTON_CAPS *pButtons = new HIDP_BUTTON_CAPS[nSize];
								if (HidP_GetButtonCaps(HidP_Input, pButtons, &nSize, pDevice->m_pPreparsedData) == HIDP_STATUS_SUCCESS) {
									for (int i=0; i<nSize; i++) {
										// Save button control
										HIDCapability sCapability;
										sCapability.m_nLinkCollection = pButtons[i].LinkCollection;
										sCapability.m_nUsagePage	  = pButtons[i].UsagePage;
										sCapability.m_nUsage		  = pButtons[i].NotRange.Usage;
										sCapability.m_nReportID		  = pButtons[i].ReportID;
										sCapability.m_nDataIndex	  = pButtons[i].NotRange.DataIndex;
										if (pButtons[i].IsRange) {
											sCapability.m_nUsageMin		= pButtons[i].Range.UsageMin;
											sCapability.m_nUsageMax		= pButtons[i].Range.UsageMax;
											sCapability.m_nDataIndexMin = pButtons[i].Range.DataIndexMin;
											sCapability.m_nDataIndexMax = pButtons[i].Range.DataIndexMax;
											sCapability.m_nBitSize	    = 1;
										} else {
											sCapability.m_nUsageMin		= pButtons[i].NotRange.Usage;
											sCapability.m_nUsageMax		= pButtons[i].NotRange.Usage;
											sCapability.m_nDataIndexMin = pButtons[i].NotRange.DataIndex;
											sCapability.m_nDataIndexMax = pButtons[i].NotRange.DataIndex;
											sCapability.m_nBitSize	    = 1;
										}

										// Add button control
										pDevice->m_lstInputButtons.Add(sCapability);
									}
								}
								delete [] pButtons;
							}

							// Enumerate input values
							if (sCaps.NumberInputValueCaps > 0) {
								USHORT nSize = sCaps.NumberInputValueCaps;
								HIDP_VALUE_CAPS *pValues = new HIDP_VALUE_CAPS[nSize];
								if (HidP_GetValueCaps(HidP_Input, pValues, &nSize, pDevice->m_pPreparsedData) == HIDP_STATUS_SUCCESS) {
									for (int i=0; i<nSize; i++) {
										// Save input value control
										HIDCapability sCapability;
										sCapability.m_nLinkCollection = pValues[i].LinkCollection;
										sCapability.m_nUsagePage	  = pValues[i].UsagePage;
										sCapability.m_nUsage		  = pValues[i].NotRange.Usage;
										sCapability.m_nReportID		  = pValues[i].ReportID;
										sCapability.m_nDataIndex	  = pValues[i].NotRange.DataIndex;
										sCapability.m_nBitSize		  = pValues[i].BitSize;
										if (pValues[i].IsRange) {
											sCapability.m_nUsageMin		= pValues[i].Range.UsageMin;
											sCapability.m_nUsageMax		= pValues[i].Range.UsageMax;
											sCapability.m_nDataIndexMin = pValues[i].Range.DataIndexMin;
											sCapability.m_nDataIndexMax = pValues[i].Range.DataIndexMax;
										} else {
											sCapability.m_nUsageMin		= pValues[i].NotRange.Usage;
											sCapability.m_nUsageMax		= pValues[i].NotRange.Usage;
											sCapability.m_nDataIndexMin = pValues[i].NotRange.DataIndex;
											sCapability.m_nDataIndexMax = pValues[i].NotRange.DataIndex;
										}
										sCapability.m_nLogicalMin  = static_cast<uint16>(pValues[i].LogicalMin);
										sCapability.m_nLogicalMax  = static_cast<uint16>(pValues[i].LogicalMax);
										sCapability.m_nPhysicalMin = static_cast<uint16>(pValues[i].PhysicalMin);
										sCapability.m_nPhysicalMax = static_cast<uint16>(pValues[i].PhysicalMax);

										// Add input value control
										pDevice->m_lstInputValues.Add(sCapability);
									}
								}
								delete [] pValues;
							}

							// Enumerate output values
							if (sCaps.NumberOutputValueCaps > 0) {
								USHORT nSize = sCaps.NumberOutputValueCaps;
								HIDP_VALUE_CAPS *pValues = new HIDP_VALUE_CAPS[nSize];
								if (HidP_GetValueCaps(HidP_Output, pValues, &nSize, pDevice->m_pPreparsedData) == HIDP_STATUS_SUCCESS) {
									for (int i=0; i<nSize; i++) {
										// Save output value control
										HIDCapability sCapability;
										sCapability.m_nUsagePage   = pValues[i].UsagePage;
										sCapability.m_nUsage	   = pValues[i].NotRange.Usage;
										sCapability.m_nReportID    = pValues[i].ReportID;
										sCapability.m_nDataIndex   = pValues[i].NotRange.DataIndex;
										sCapability.m_nBitSize	   = pValues[i].BitSize;
										sCapability.m_nLogicalMin  = static_cast<uint16>(pValues[i].LogicalMin);
										sCapability.m_nLogicalMax  = static_cast<uint16>(pValues[i].LogicalMax);
										sCapability.m_nPhysicalMin = static_cast<uint16>(pValues[i].PhysicalMin);
										sCapability.m_nPhysicalMax = static_cast<uint16>(pValues[i].PhysicalMax);

										// Add output value control
										pDevice->m_lstOutputValues.Add(sCapability);
									}
								}
								delete [] pValues;
							}
						}
					}
				}

				// Close device handle
				CloseHandle(hDevice);

				// Device found
				m_lstDevices.Add(pDevice);
				  lstDevices.Add(pDevice);
			} else {
				// [TODO] Write error message into the log?
				// Get error message
				/*
				DWORD nError = GetLastError();
				LPTSTR s;
				::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
								nullptr, nError, 0, static_cast<LPTSTR>(&s), 0, nullptr);
				::LocalFree(s);
				*/

				// Destroy device
				delete pDevice;
			}
		}

		// Delete buffer
		delete [] pBuffer;

		// Next device
		nDevice++;
	}

	// Close info handle
	SetupDiDestroyDeviceInfoList(hDevInfo);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
