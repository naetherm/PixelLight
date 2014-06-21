/*********************************************************\
 *  File: ChunkLoaderPLXml.cpp                           *
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
#include "PLCore/Xml/Xml.h"
#include "PLCore/Log/Log.h"
#include "PLCore/File/File.h"
#include "PLCore/Tools/Chunk.h"
#include "PLCore/Tools/ChunkLoaderPLXml.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(ChunkLoaderPLXml)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool ChunkLoaderPLXml::Load(Chunk &cChunk, File &cFile)
{
	// Load XML document
	XmlDocument cDocument;
	if (cDocument.Load(cFile)) {
		// Get chunk element
		const XmlElement *pChunkElement = cDocument.GetFirstChildElement("Chunk");
		if (pChunkElement) {
			// Get the format version
			int nVersion = pChunkElement->GetAttribute("Version").GetInt();

			// Unknown
			if (nVersion > 1) {
				PL_LOG(Error, cDocument.GetValue() + ": " + UnknownFormatVersion)

			// 1 (current)
			} else if (nVersion == 1) {
				return LoadV1(cChunk, *pChunkElement);

			// No longer supported format version
			} else if (nVersion >= 0) {
				PL_LOG(Warning, cDocument.GetValue() + ": " + NoLongerSupportedFormatVersion)

			// Invalid format version (negative!)
			} else {
				PL_LOG(Error, cDocument.GetValue() + ": " + InvalidFormatVersion)
			}
		} else {
			PL_LOG(Error, "Can't find 'Chunk' element")
		}
	} else {
		PL_LOG(Error, cDocument.GetValue() + ": " + cDocument.GetErrorDesc())
	}

	// Error!
	return false;
}

bool ChunkLoaderPLXml::Save(const Chunk &cChunk, File &cFile)
{
	// Create XML document
	XmlDocument cDocument;

	// Add declaration
	XmlDeclaration *pDeclaration = new XmlDeclaration("1.0", "", "");
	cDocument.LinkEndChild(*pDeclaration);

	// Add chunk
	XmlElement *pChunkElement = new XmlElement("Chunk");
	pChunkElement->SetAttribute("Version", "1");

	{ // Write semantic attribute
		String sSemantic;
		switch (cChunk.GetSemantic()) {
			case Chunk::Position:	sSemantic = "Position";	break;
			case Chunk::Rotation:	sSemantic = "Rotation";	break;
			case Chunk::Scale:		sSemantic = "Scale";	break;
			case Chunk::Unknown:	sSemantic = "Unknown";	break;
			default:
				sSemantic = String::Format("%d", cChunk.GetSemantic());
				break;
		}
		pChunkElement->SetAttribute("Semantic", sSemantic);
	}

	{ // Write element type attribute
		String sElementType;
		switch (cChunk.GetElementType()) {
			case Chunk::Int8:	sElementType = "Int8";		break;
			case Chunk::Int16:	sElementType = "Int16";		break;
			case Chunk::Int32:	sElementType = "Int32";		break;
			case Chunk::UInt8:	sElementType = "UInt8";		break;
			case Chunk::UInt16:	sElementType = "UInt16";	break;
			case Chunk::UInt32:	sElementType = "UInt32";	break;
			case Chunk::Float:	sElementType = "Float";		break;
			case Chunk::Double:	sElementType = "Double";	break;
			default:
				sElementType = String::Format("%d", cChunk.GetElementType());
				break;
		}
		pChunkElement->SetAttribute("ElementType", sElementType);
	}

	// Write number of components per element attribute
	pChunkElement->SetAttribute("NumOfComponentsPerElement", String::Format("%d", cChunk.GetNumOfComponentsPerElement()));

	// Write number of elements attribute
	pChunkElement->SetAttribute("NumOfElements", String::Format("%d", cChunk.GetNumOfElements()));

	// Loop through all elements and save them
	for (uint32 nElement=0; nElement<cChunk.GetNumOfElements(); nElement++) {
		// Create the element element
		XmlElement *pElement = new XmlElement("Element");

		// Loop through all components and save them
		for (uint32 nComponent=0; nComponent<cChunk.GetNumOfComponentsPerElement(); nComponent++) {
			// Get the chunk element component data
			const uint8 *pElementComponentData = cChunk.GetElementComponentData(nElement, nComponent);

			// Create the component element
			XmlElement *pComponent = new XmlElement("Component");

			// Add value
			XmlText *pValue = new XmlText("");
			switch (cChunk.GetElementType()) {
				case Chunk::Int8:	pValue->SetValue(String::Format("%d", *(reinterpret_cast<const int8*>(pElementComponentData))));	break;
				case Chunk::Int16:	pValue->SetValue(String::Format("%d", *(reinterpret_cast<const int16*>(pElementComponentData))));	break;
				case Chunk::Int32:	pValue->SetValue(String::Format("%d", *(reinterpret_cast<const int32*>(pElementComponentData))));	break;
				case Chunk::UInt8:	pValue->SetValue(String::Format("%u", *(reinterpret_cast<const uint8*>(pElementComponentData))));	break;
				case Chunk::UInt16:	pValue->SetValue(String::Format("%u", *(reinterpret_cast<const uint16*>(pElementComponentData))));	break;
				case Chunk::UInt32:	pValue->SetValue(String::Format("%u", *(reinterpret_cast<const uint32*>(pElementComponentData))));	break;
				case Chunk::Float:	pValue->SetValue(String::Format("%f", *(reinterpret_cast<const float*>(pElementComponentData))));	break;
				case Chunk::Double:	pValue->SetValue(String::Format("%f", *(reinterpret_cast<const double*>(pElementComponentData))));	break;
			}
			pComponent->LinkEndChild(*pValue);

			// Link text element to parent
			pElement->LinkEndChild(*pComponent);
		}

		// Link text element to parent
		pChunkElement->LinkEndChild(*pElement);
	}

	// Link chunk element to parent
	cDocument.LinkEndChild(*pChunkElement);

	// Save config
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
ChunkLoaderPLXml::ChunkLoaderPLXml()
{
}

/**
*  @brief
*    Destructor
*/
ChunkLoaderPLXml::~ChunkLoaderPLXml()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Loader implementation for format version 1
*/
bool ChunkLoaderPLXml::LoadV1(Chunk &cChunk, const XmlElement &cChunkElement) const
{
	{ // Read semantic attribute
		String sSemantic = cChunkElement.GetAttribute("Semantic");
		if (sSemantic == "Position")
			cChunk.SetSemantic(Chunk::Position);
		else if (sSemantic == "Rotation")
			cChunk.SetSemantic(Chunk::Rotation);
		else if (sSemantic == "Scale")
			cChunk.SetSemantic(Chunk::Scale);
		else if (sSemantic == "Unknown")
			cChunk.SetSemantic(Chunk::Unknown);
		else
			cChunk.SetSemantic(static_cast<Chunk::ESemantic>(sSemantic.GetInt()));
	}

	// Read element type attribute
	Chunk::EElementType nElementType = Chunk::Int8;
	{
		String sElementType = cChunkElement.GetAttribute("ElementType");
		if (sElementType == "Int8")
			nElementType = Chunk::Int8;
		else if (sElementType == "Int16")
			nElementType = Chunk::Int16;
		else if (sElementType == "Int32")
			nElementType = Chunk::Int32;
		else if (sElementType == "UInt8")
			nElementType = Chunk::UInt8;
		else if (sElementType == "UInt16")
			nElementType = Chunk::UInt16;
		else if (sElementType == "UInt32")
			nElementType = Chunk::UInt32;
		else if (sElementType == "Float")
			nElementType = Chunk::Float;
		else if (sElementType == "Double")
			nElementType = Chunk::Double;
		else
			nElementType = static_cast<Chunk::EElementType>(sElementType.GetInt());
	}

	// Read number of components per element
	const uint32 nNumOfComponentsPerElement = cChunkElement.GetAttribute("NumOfComponentsPerElement").GetUInt32();

	// Read number of elements
	const uint32 nNumOfElements = cChunkElement.GetAttribute("NumOfElements").GetUInt32();

	// Allocate the chunk data
	if (cChunk.Allocate(nElementType, nNumOfComponentsPerElement, nNumOfElements)) {
		// Iterate through all elements
		uint32 nNumOfReadElements = 0;
		const XmlElement *pElement = cChunkElement.GetFirstChildElement("Element");
		while (pElement) {
			// Iterate through all components
			uint32 nNumOfReadComponents = 0;
			const XmlElement *pComponent = pElement->GetFirstChildElement("Component");
			while (pComponent) {
				// Get value
				const XmlNode *pNode = pComponent->GetFirstChild();
				if (pNode && pNode->GetType() == XmlNode::Text) {
					const String sValue = pNode->GetValue();
					if (sValue.GetLength()) {
						// Get the chunk element component data
						uint8 *pElementComponentData = cChunk.GetElementComponentData(nNumOfReadElements, nNumOfReadComponents);

						// Get value
						switch (cChunk.GetElementType()) {
							case Chunk::Int8:	*(reinterpret_cast<int8*>  (pElementComponentData)) = static_cast<int8>(sValue.GetInt());	break;
							case Chunk::Int16:	*(reinterpret_cast<int16*> (pElementComponentData)) = static_cast<int16>(sValue.GetInt());	break;
							case Chunk::Int32:	*(reinterpret_cast<int32*> (pElementComponentData)) = sValue.GetInt();						break;
							case Chunk::UInt8:	*(reinterpret_cast<uint8*> (pElementComponentData)) = static_cast<uint8>(sValue.GetInt());	break;
							case Chunk::UInt16:	*(reinterpret_cast<uint16*>(pElementComponentData)) = static_cast<uint16>(sValue.GetInt());	break;
							case Chunk::UInt32:	*(reinterpret_cast<uint32*>(pElementComponentData)) = sValue.GetUInt32();					break;
							case Chunk::Float:	*(reinterpret_cast<float*> (pElementComponentData)) = sValue.GetFloat();					break;
							case Chunk::Double:	*(reinterpret_cast<double*>(pElementComponentData)) = sValue.GetDouble();					break;
						}
					}
				}

				// Next component, please
				pComponent = pComponent->GetNextSiblingElement("Component");
				nNumOfReadComponents++;
			}

			// Check whether or not the number of read components is correct
			if (nNumOfReadComponents != nNumOfComponentsPerElement)
				PL_LOG(Error, String::Format("Element %d: Number of components is %d, but should be %d", nNumOfReadElements, nNumOfReadComponents, nNumOfComponentsPerElement))

			// Next element, please
			pElement = pElement->GetNextSiblingElement("Element");
			nNumOfReadElements++;
		}

		// Check whether or not the number of read elements is correct
		if (nNumOfReadElements != nNumOfElements)
			PL_LOG(Error, String::Format("Number of elements is %d, but should be %d", nNumOfReadElements, nNumOfElements))
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
