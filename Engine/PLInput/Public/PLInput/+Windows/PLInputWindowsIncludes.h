/*********************************************************\
 *  File: PLInputWindowsIncludes.h                       *
 *      Windows includes for PLInput
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


#ifndef __PLINPUT_WINDOWSINCLUDES_H__
#define __PLINPUT_WINDOWSINCLUDES_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
// Set windows version to Windows XP
#define WINVER			0x0501
#define _WIN32_WINNT	0x0501

// Exclude some stuff from windows.h to speed up compilation a bit
#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
#define NOMENUS
#define NOICONS
#define NOKEYSTATES
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define OEMRESOURCE
#define NOATOM
#define NOMEMMGR
#define NOMETAFILE
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX

// Include windows header
__pragma(warning(push))
	__pragma(warning(disable: 4201)) // "nonstandard extension used : nameless struct/union"
	#include <windows.h>
	#include <mmsystem.h>
	extern "C" {
		#include <setupapi.h> 
	}
__pragma(warning(pop))

// We undef these to avoid name conflicts
#undef DrawText
#undef LoadImage
#undef MessageBox
#undef GetClassName
#undef CreateDirectory
#undef SetCurrentDirectory
#undef GetCurrentDirectory
#undef GetEnvironmentVariable
#undef SetEnvironmentVariable
#undef GetComputerName
#undef GetUserName
#undef Yield


//[-------------------------------------------------------]
//[ Definitions                                           ]
//[-------------------------------------------------------]
// Used HID structures
typedef struct _HIDP_PREPARSED_DATA *PHIDP_PREPARSED_DATA;
typedef enum _HIDP_REPORT_TYPE {
	HidP_Input,
	HidP_Output,
	HidP_Feature
} HIDP_REPORT_TYPE;
typedef struct _HIDP_DATA {
	USHORT DataIndex;
	USHORT Reserved;
	union {
		ULONG   RawValue;
		BOOLEAN On;
	};
} HIDP_DATA;
typedef struct _HIDD_ATTRIBUTES {
	ULONG  Size;
	USHORT VendorID;
	USHORT ProductID;
	USHORT VersionNumber;
} HIDD_ATTRIBUTES;
typedef struct _HIDP_CAPS {
	USHORT Usage;
	USHORT UsagePage;
	USHORT InputReportByteLength;
	USHORT OutputReportByteLength;
	USHORT FeatureReportByteLength;
	USHORT Reserved[17];
	USHORT NumberLinkCollectionNodes;
	USHORT NumberInputButtonCaps;
	USHORT NumberInputValueCaps;
	USHORT NumberInputDataIndices;
	USHORT NumberOutputButtonCaps;
	USHORT NumberOutputValueCaps;
	USHORT NumberOutputDataIndices;
	USHORT NumberFeatureButtonCaps;
	USHORT NumberFeatureValueCaps;
	USHORT NumberFeatureDataIndices;
} HIDP_CAPS;
typedef struct _HIDP_BUTTON_CAPS {
	USHORT  UsagePage;
	UCHAR   ReportID;
	BOOLEAN IsAlias;
	USHORT  BitField;
	USHORT  LinkCollection;
	USHORT  LinkUsage;
	USHORT  LinkUsagePage;
	BOOLEAN IsRange;
	BOOLEAN IsStringRange;
	BOOLEAN IsDesignatorRange;
	BOOLEAN IsAbsolute;
	ULONG   Reserved[10];
	union {
		struct {
			USHORT UsageMin,        UsageMax;
			USHORT StringMin,       StringMax;
			USHORT DesignatorMin,   DesignatorMax;
			USHORT DataIndexMin,    DataIndexMax;
		} Range;
		struct  {
			USHORT Usage,           Reserved1;
			USHORT StringIndex,     Reserved2;
			USHORT DesignatorIndex, Reserved3;
			USHORT DataIndex,       Reserved4;
		} NotRange;
	};
} HIDP_BUTTON_CAPS;
typedef struct _HIDP_VALUE_CAPS {
	USHORT  UsagePage;
	UCHAR   ReportID;
	BOOLEAN IsAlias;
	USHORT  BitField;
	USHORT  LinkCollection;
	USHORT  LinkUsage;
	USHORT  LinkUsagePage;
	BOOLEAN IsRange;
	BOOLEAN IsStringRange;
	BOOLEAN IsDesignatorRange;
	BOOLEAN IsAbsolute;
	BOOLEAN HasNull;
	UCHAR   Reserved;
	USHORT  BitSize;
	USHORT  ReportCount;
	USHORT  Reserved2[5];
	ULONG   UnitsExp;
	ULONG   Units;
	LONG    LogicalMin,  LogicalMax;
	LONG    PhysicalMin, PhysicalMax;
	union {
		struct {
			USHORT UsageMin,        UsageMax;
			USHORT StringMin,       StringMax;
			USHORT DesignatorMin,   DesignatorMax;
			USHORT DataIndexMin,    DataIndexMax;
		} Range;
		struct {
			USHORT Usage,           Reserved1;
			USHORT StringIndex,     Reserved2;
			USHORT DesignatorIndex, Reserved3;
			USHORT DataIndex,       Reserved4;
		} NotRange;
	};
} HIDP_VALUE_CAPS;
#define HIDP_STATUS_SUCCESS (((LONG) (((0x0) << 28) | (0x11 << 16) | (0))))

// Raw Input definitions
#ifndef HID_USAGE_PAGE_GENERIC
	#define HID_USAGE_PAGE_GENERIC		((USHORT) 0x01)
	#define HID_USAGE_GENERIC_MOUSE		((USHORT) 0x02)
	#define HID_USAGE_GENERIC_JOYSTICK	((USHORT) 0x04)
	#define HID_USAGE_GENERIC_GAMEPAD	((USHORT) 0x05)
	#define HID_USAGE_GENERIC_KEYBOARD	((USHORT) 0x06)
#endif


//[-------------------------------------------------------]
//[ Global HID function pointers (HIDWindows.cpp)         ]
//[-------------------------------------------------------]
// HidD_GetPreparsedData
typedef BOOLEAN (__stdcall *PFNHIDDGETPREPARSEDDATA) (IN HANDLE HidDeviceObject, OUT PHIDP_PREPARSED_DATA *PreparsedData);
extern PFNHIDDGETPREPARSEDDATA HidD_GetPreparsedData;
// HidD_FreePreparsedData
typedef BOOLEAN (__stdcall *PFNHIDDFREEPREPARSEDDATA) (IN PHIDP_PREPARSED_DATA PreparsedData);
extern PFNHIDDFREEPREPARSEDDATA HidD_FreePreparsedData;
// HidP_GetData
typedef LONG (__stdcall *PFNHIDPGETDATA) (IN HIDP_REPORT_TYPE ReportType, OUT HIDP_DATA *DataList, IN OUT PULONG DataLength, IN PHIDP_PREPARSED_DATA PreparsedData, IN PCHAR Report, IN ULONG ReportLength);
extern PFNHIDPGETDATA HidP_GetData;
// HidP_SetData
typedef LONG (__stdcall *PFNHIDPSETDATA) (IN HIDP_REPORT_TYPE ReportType, IN HIDP_DATA *DataList, IN OUT PULONG DataLength, IN PHIDP_PREPARSED_DATA PreparsedData, IN OUT PCHAR Report, IN ULONG ReportLength);
extern PFNHIDPSETDATA HidP_SetData;
// HidD_GetHidGuid
typedef void (__stdcall *PFNHIDPGETHIDGUID) (OUT LPGUID HidGuid);
extern PFNHIDPGETHIDGUID HidD_GetHidGuid;
// HidD_GetAttributes
typedef BOOLEAN (__stdcall *PFNHIDPGETATTRIBUTES) (IN HANDLE HidDeviceObject, OUT HIDD_ATTRIBUTES *Attributes);
extern PFNHIDPGETATTRIBUTES HidD_GetAttributes;
// HidP_GetCaps
typedef LONG (__stdcall *PFNHIDPGETCAPS) (IN PHIDP_PREPARSED_DATA PreparsedData, OUT HIDP_CAPS *Capabilities);
extern PFNHIDPGETCAPS HidP_GetCaps;
// HidP_GetButtonCaps
typedef LONG (__stdcall *PFNHIDPGETBUTTONCAPS) (IN HIDP_REPORT_TYPE ReportType, OUT HIDP_BUTTON_CAPS *ButtonCaps, IN OUT PUSHORT ButtonCapsLength, IN PHIDP_PREPARSED_DATA PreparsedData);
extern PFNHIDPGETBUTTONCAPS HidP_GetButtonCaps;
// HidP_GetValueCaps
typedef LONG (__stdcall *PFNHIDPGETVALUECAPS) (IN HIDP_REPORT_TYPE ReportType, OUT HIDP_VALUE_CAPS *ValueCaps, IN OUT PUSHORT ValueCapsLength, IN PHIDP_PREPARSED_DATA PreparsedData);
extern PFNHIDPGETVALUECAPS HidP_GetValueCaps;


#endif // __PLINPUT_WINDOWSINCLUDES_H__
