/*********************************************************\
 *  File: HIDDeviceWindows.cpp                           *
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
#include "PLInput/Backend/HID/+Windows/HIDDeviceWindows.h"


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
HIDDeviceWindows::HIDDeviceWindows() :
	m_hDevice(nullptr),
	m_pPreparsedData(nullptr)
{
	// Do not destroy device implementation automatically, because this is managed by HIDWindows
	m_bDelete = false;
}

/**
*  @brief
*    Destructor
*/
HIDDeviceWindows::~HIDDeviceWindows()
{
	// Close a possibly open connection
	Close();

	// Release preparsed data
	if (m_pPreparsedData)
		HidD_FreePreparsedData(m_pPreparsedData);
}


//[-------------------------------------------------------]
//[ Protected virtual HIDDevice functions                 ]
//[-------------------------------------------------------]
void HIDDeviceWindows::ParseInputReportData(const uint8 *pInputReport, uint32 nSize)
{
	// Reset buttons to 0 (not pressed)
	for (uint32 i=0; i<m_lstInputButtons.GetNumOfElements(); i++)
		m_lstInputButtons[i].m_nValue = 0;

	// Create buffer
	unsigned long nItems = m_nNumberInputDataIndices;
	HIDP_DATA *pData = new HIDP_DATA[nItems];

	// Get data
	HidP_GetData(HidP_Input, pData, &nItems, m_pPreparsedData, reinterpret_cast<char*>(const_cast<uint8*>(pInputReport)), nSize);
	for (uint32 i=0; i<nItems; i++) {
		// Find button with the given data index
		for (uint32 j=0; j<m_lstInputButtons.GetNumOfElements(); j++) {
			// Get button
			HIDCapability *pCapability = &m_lstInputButtons[j];

			// Go through all buttons (usages)
			if (pData[i].DataIndex >= pCapability->m_nDataIndexMin && pData[i].DataIndex <= pCapability->m_nDataIndexMax) {
				// Is button set?
				if (pData[i].On) {
					// Set button state to 'pressed'
					uint32 nValue = static_cast<uint32>(1) << (pData[i].DataIndex - pCapability->m_nDataIndexMin);
					pCapability->m_nValue |= nValue;
				}
			}
		}

		// Find input value with the given data index
		for (uint32 j=0; j<m_lstInputValues.GetNumOfElements(); j++) {
			// Get input value
			HIDCapability *pCapability = &m_lstInputValues[j];
			if (pCapability->m_nDataIndex == pData[i].DataIndex) {
				// Set value
				pCapability->m_nValue = pData[i].RawValue;
			}
		}
	}

	// Delete data
	delete[] pData;
}

void HIDDeviceWindows::SendOutputReportData()
{
	// Check output report
	if (m_pOutputBuffer && m_nOutputReportSize > 0) {
		// Get number of distinct report IDs
		List<uint8> lstReportIDs;
		for (uint32 i=0; i<m_lstOutputValues.GetNumOfElements(); i++) {
			// Add report ID to list
			uint8 nReportID = m_lstOutputValues[i].m_nReportID;
			if (!lstReportIDs.IsElement(nReportID))
				lstReportIDs.Add(nReportID);
		}

		// Send one output report for every report ID
		for (uint32 i=0; i<lstReportIDs.GetNumOfElements(); i++) {
			// Clear output report
			MemoryManager::Set(m_pOutputBuffer, 0, m_nOutputReportSize);

			// Set report ID
			uint8 nReportID = lstReportIDs[i];
			m_pOutputBuffer[0] = nReportID;

			// Set output values
			uint32 nValues = 0;
			HIDP_DATA sData[32];
			for (uint32 j=0; j<m_lstOutputValues.GetNumOfElements(); j++) {
				// Get output value and check, if it belongs to the right report (check report ID)
				HIDCapability *pCapability = &m_lstOutputValues[j];
				if (pCapability->m_nReportID == nReportID) {
					// Set value data
					sData[nValues].DataIndex = pCapability->m_nDataIndex;
					sData[nValues].RawValue  = pCapability->m_nValue;
					nValues++;
				}
			}

			// Fill output report
			if (HidP_SetData(HidP_Output, sData, reinterpret_cast<PULONG>(&nValues), m_pPreparsedData, reinterpret_cast<char*>(m_pOutputBuffer), m_nOutputReportSize) == HIDP_STATUS_SUCCESS) {
				// Send report
				Write(m_pOutputBuffer, m_nOutputReportSize);
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Public virtual ConnectionDevice functions             ]
//[-------------------------------------------------------]
bool HIDDeviceWindows::Open(uint16 nOutputPort, uint16 nInputPort)
{
	// Close first
	if (IsOpen())
		Close();

	// Connect to device
	m_hDevice = CreateFile(GetName().GetUnicode(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, nullptr);
	if (m_hDevice != INVALID_HANDLE_VALUE) {
		// Start read thread
		InitThread();

		// Call event
		EventOnConnect();

		// Done
		return true;
	}

	// Error
	return false;
}

bool HIDDeviceWindows::Close()
{
	// Check device handle
	if (m_hDevice && m_hDevice != INVALID_HANDLE_VALUE) {
		// Stop read thread
		StopThread();

		// Call event
		EventOnDisconnect();

		// Close handle
		bool bResult = (CloseHandle(m_hDevice) != 0);
		m_hDevice = INVALID_HANDLE_VALUE;
		return bResult;
	}

	// Error
	return false;
}

bool HIDDeviceWindows::IsOpen() const
{
	// Check device handle
	return (m_hDevice != INVALID_HANDLE_VALUE);
}

bool HIDDeviceWindows::Read(uint8 *pBuffer, uint32 nSize)
{
	// Check handle
	if (m_hDevice && m_hDevice != INVALID_HANDLE_VALUE) {
		// Setup asynchronous IO
		m_sOverlapped.sOverlapped.Offset	 = 0;
		m_sOverlapped.sOverlapped.OffsetHigh = 0;
		m_sOverlapped.sOverlapped.hEvent	 = nullptr;
		m_sOverlapped.pDevice				 = this;

		// Read from device and give back immediately. EventOnRead will be emitted, when data has been read
		if (ReadFileEx(m_hDevice, pBuffer, nSize, reinterpret_cast<OVERLAPPED*>(&m_sOverlapped), &HIDDeviceWindows::OnReadComplete)) {
			// Wait for read operation to complete
			SleepEx(1000, TRUE);

			// Get error condition
			return (GetLastError() == ERROR_SUCCESS);
		}
	}

	// Error!
	return false;
}

bool HIDDeviceWindows::Write(const uint8 *pBuffer, uint32 nSize)
{
	// Check handle
	if (m_hDevice && m_hDevice != INVALID_HANDLE_VALUE) {
		// Setup asynchronous IO
		m_sOverlapped.sOverlapped.Offset	 = 0;
		m_sOverlapped.sOverlapped.OffsetHigh = 0;
		m_sOverlapped.sOverlapped.hEvent	 = nullptr;
		m_sOverlapped.pDevice				 = this;

		// Write to device
		if (WriteFileEx(m_hDevice, pBuffer, nSize, reinterpret_cast<OVERLAPPED*>(&m_sOverlapped), &HIDDeviceWindows::OnWriteComplete)) {
			// Get error condition
			return (GetLastError() == ERROR_SUCCESS);
		}
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Private static functions                              ]
//[-------------------------------------------------------]
void CALLBACK HIDDeviceWindows::OnReadComplete(DWORD nErrorCode, DWORD nNumberOfBytesTransfered, LPOVERLAPPED pOverlapped)
{
	// Get object
	HIDDeviceWindows *pThis = reinterpret_cast<ExtendedOverlapped*>(pOverlapped)->pDevice;
	if (pThis) {
		// Data has been read
		pThis->EventOnRead();
	}
}

void CALLBACK HIDDeviceWindows::OnWriteComplete(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLInput
