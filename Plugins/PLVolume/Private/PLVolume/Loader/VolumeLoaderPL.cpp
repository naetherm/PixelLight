/*********************************************************\
 *  File: VolumeLoaderPL.cpp                             *
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
#include <PLCore/Xml/Xml.h>
#include <PLCore/Log/Log.h>
#include <PLCore/File/File.h>
#include "PLVolume/Volume.h"
#include "PLVolume/TransferFunction.h"
#include "PLVolume/Loader/VolumeLoaderPL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLVolume {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(VolumeLoaderPL)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool VolumeLoaderPL::Load(Volume &cVolume, File &cFile)
{
	// Load XML document
	XmlDocument cDocument;
	if (cDocument.Load(cFile)) {
		// Get volume element
		const XmlElement *pVolumeElement = cDocument.GetFirstChildElement("Volume");
		if (pVolumeElement) {
			// Get the format version
			const int nVersion = pVolumeElement->GetAttribute("Version").GetInt();

			// Unknown
			if (nVersion > 1) {
				PL_LOG(Error, cDocument.GetValue() + ": " + UnknownFormatVersion)

			// 1 (current)
			} else if (nVersion == 1) {
				return LoadV1(cVolume, cFile, *pVolumeElement);

			// ""/0 (same format as 1)
			} else if (nVersion == 0) {
				// [DEPRECATED]
				PL_LOG(Warning, cDocument.GetValue() + ": " + DeprecatedFormatVersion)
				return LoadV1(cVolume, cFile, *pVolumeElement);

			// No longer supported format version
			} else if (nVersion >= 0) {
				PL_LOG(Warning, cDocument.GetValue() + ": " + NoLongerSupportedFormatVersion)

			// Invalid format version (negative!)
			} else {
				PL_LOG(Error, cDocument.GetValue() + ": " + InvalidFormatVersion)
			}
		} else {
			PL_LOG(Error, "Can't find 'Volume' element")
		}
	} else {
		PL_LOG(Error, cDocument.GetValue() + ": " + cDocument.GetErrorDesc())
	}

	// Error!
	return false;
}

bool VolumeLoaderPL::Save(const Volume &cVolume, File &cFile)
{
	// Create XML document
	XmlDocument cDocument;

	// Add declaration
	XmlDeclaration *pDeclaration = new XmlDeclaration("1.0", "", "");
	cDocument.LinkEndChild(*pDeclaration);

	// Add volume
	XmlElement *pVolumeElement = new XmlElement("Volume");
	pVolumeElement->SetAttribute("Version", "1");

	{ // Volume filename
		// Get volume filename
		String sVolumeFilename;
		{
			// Construct filename, use "dds" as default file format because it can be quite effective and blazing fast when it comes to handling compressed data
			// -> See "VolumeLoaderDDS" class documentation for more information
			sVolumeFilename = cFile.GetUrl().GetTitle() + ".dds";

			// Construct absolute filename and save the volume (an endless recursion is impossible in here due to the fixed build in dds extension)
			const String sAbsoluteFilename = GetAbsoluteFilename(cFile, sVolumeFilename);
			if (!const_cast<Volume&>(cVolume).SaveByFilename(sAbsoluteFilename)) {	// Ugly const-cast, but our file format is special/universal so this must do the job
				// Error!
				sVolumeFilename = "";
			}
		}
		if (sVolumeFilename.GetLength()) {
			// Create the volume filename element
			XmlElement *pVolumeFilenameElement = new XmlElement("VolumeFilename");

			// Add value
			XmlText *pValue = new XmlText(sVolumeFilename);
			pVolumeFilenameElement->LinkEndChild(*pValue);

			// Link volume filename element
			pVolumeElement->LinkEndChild(*pVolumeFilenameElement);
		}
	}

	{ // Voxel size
		// Create the voxel size element
		XmlElement *pVoxelSizeElement = new XmlElement("VoxelSize");

		// Add value
		XmlText *pValue = new XmlText(cVolume.GetVoxelSize().ToString());
		pVoxelSizeElement->LinkEndChild(*pValue);

		// Link voxel size element
		pVolumeElement->LinkEndChild(*pVoxelSizeElement);
	}

	{ // Transfer function filename
		// Get transfer function filename
		String sTransferFunctionFilename;
		{
			// Construct filename, use "table" as default file format
			sTransferFunctionFilename = cFile.GetUrl().GetTitle() + ".table";

			// Construct absolute filename and save the transfer function
			const String sAbsoluteFilename = GetAbsoluteFilename(cFile, sTransferFunctionFilename);
			if (!const_cast<Volume&>(cVolume).GetTransferFunction().SaveByFilename(sAbsoluteFilename)) {	// Ugly const-cast, but our file format is special/universal so this must do the job
				// Error!
				sTransferFunctionFilename = "";
			}
		}
		if (sTransferFunctionFilename.GetLength()) {
			// Create the transfer function filename element
			XmlElement *pVolumeFilenameElement = new XmlElement("TransferFunctionFilename");

			// Add value
			XmlText *pValue = new XmlText(sTransferFunctionFilename);
			pVolumeFilenameElement->LinkEndChild(*pValue);

			// Link transfer function filename element
			pVolumeElement->LinkEndChild(*pVolumeFilenameElement);
		}
	}

	// Link volume element
	cDocument.LinkEndChild(*pVolumeElement);

	// Save settings
	cDocument.Save(cFile);

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
VolumeLoaderPL::VolumeLoaderPL()
{
}

/**
*  @brief
*    Destructor
*/
VolumeLoaderPL::~VolumeLoaderPL()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Loader implementation for format version 1
*/
bool VolumeLoaderPL::LoadV1(Volume &cVolume, File &cFile, const XmlElement &cVolumeElement) const
{
	{ // Volume filename
		const XmlElement *pVolumeFilenameElement = cVolumeElement.GetFirstChildElement("VolumeFilename");
		if (pVolumeFilenameElement) {
			// Get value
			const XmlNode *pNode = pVolumeFilenameElement->GetFirstChild();
			if (pNode && pNode->GetType() == XmlNode::Text) {
				const String sValue = pNode->GetValue();
				if (sValue.GetLength()) {
					// Get the absolute data filename
					// -> The given filename can be absolute
					// -> The given filename can be relative to the currently loaded file
					const String sAbsoluteFilename = GetAbsoluteFilename(cFile, sValue);

					// Load the volume, but first check whether or not this would end up in an endless recursion
					if (Url(sAbsoluteFilename).GetUrl() != cFile.GetUrl().GetUrl()) {
						if (!cVolume.LoadByFilename(sAbsoluteFilename))
							return false; // Error!
					} else {
						// Error!
						PL_LOG(Error, "The given volume filename (\"" + sAbsoluteFilename + "\") is identical to the currently loaded volume filename (this would end up in an endless recursion)")
						return false;
					}
				}
			}
		}
	}

	{ // Voxel size
		const XmlElement *pVoxelSizeElement = cVolumeElement.GetFirstChildElement("VoxelSize");
		if (pVoxelSizeElement) {
			// Get value
			const XmlNode *pNode = pVoxelSizeElement->GetFirstChild();
			if (pNode && pNode->GetType() == XmlNode::Text) {
				const String sValue = pNode->GetValue();
				if (sValue.GetLength()) {
					Vector3 vVoxelSize;
					vVoxelSize.FromString(sValue);
					cVolume.SetVoxelSize(vVoxelSize);
				}
			}
		}
	}

	{ // Transfer function filename
		const XmlElement *pTransferFunctionFilenameElement = cVolumeElement.GetFirstChildElement("TransferFunctionFilename");
		if (pTransferFunctionFilenameElement) {
			// Get value
			const XmlNode *pNode = pTransferFunctionFilenameElement->GetFirstChild();
			if (pNode && pNode->GetType() == XmlNode::Text) {
				const String sValue = pNode->GetValue();
				if (sValue.GetLength()) {
					// Get the absolute filename
					// -> The given filename can be absolute
					// -> The given filename can be relative to the currently loaded file
					const String sAbsoluteFilename = GetAbsoluteFilename(cFile, sValue);

					// Load the transfer function
					if (!cVolume.GetTransferFunction().LoadByFilename(sAbsoluteFilename))
						PL_LOG(Warning, "Failed to load in the transfer function \"" + sAbsoluteFilename + '\"')
				}
			}
		}
	}

	// Done
	return true;
}

/**
*  @brief
*    Constructs an absolute filename
*/
String VolumeLoaderPL::GetAbsoluteFilename(const File &cFile, const String &sFilename) const
{
	const Url cUrl = sFilename;
	return cUrl.IsAbsolute() ? cUrl.GetUrl() : (cFile.GetUrl().CutFilename() + cUrl.GetUrl());
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume
