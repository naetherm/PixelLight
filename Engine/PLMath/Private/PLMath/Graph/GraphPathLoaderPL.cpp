/*********************************************************\
 *  File: GraphPathLoaderPL.cpp                          *
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
#include <PLCore/String/ParseTools.h>
#include "PLMath/Graph/GraphPath.h"
#include "PLMath/Graph/GraphNode.h"
#include "PLMath/Graph/GraphPathLoaderPL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLMath {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(GraphPathLoaderPL)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool GraphPathLoaderPL::Load(GraphPath &cGraphPath, File &cFile)
{
	// Load XML document
	XmlDocument cDocument;
	if (cDocument.Load(cFile)) {
		// Get path element
		const XmlElement *pPathElement = cDocument.GetFirstChildElement("Path");
		if (pPathElement) {
			// Get the format version
			int nVersion = pPathElement->GetAttribute("Version").GetInt();

			// Unknown
			if (nVersion > 1) {
				PL_LOG(Error, cDocument.GetValue() + ": " + UnknownFormatVersion)

			// 1 (current)
			} else if (nVersion == 1) {
				return LoadV1(cGraphPath, *pPathElement);

			// ""/0 (same format as 1)
			} else if (nVersion == 0) {
				// [DEPRECATED]
				PL_LOG(Warning, cDocument.GetValue() + ": " + DeprecatedFormatVersion)
				return LoadV1(cGraphPath, *pPathElement);

			// No longer supported format version
			} else if (nVersion >= 0) {
				PL_LOG(Warning, cDocument.GetValue() + ": " + NoLongerSupportedFormatVersion)

			// Invalid format version (negative!)
			} else {
				PL_LOG(Error, cDocument.GetValue() + ": " + InvalidFormatVersion)
			}
		} else PL_LOG(Error, "Can't find 'Path' element")
	} else PL_LOG(Error, cDocument.GetValue() + ": " + cDocument.GetErrorDesc())

	// Error!
	return false;
}

bool GraphPathLoaderPL::Save(const GraphPath &cGraphPath, File &cFile)
{
	// Create XML document
	XmlDocument cDocument;

	// Add declaration
	XmlDeclaration *pDeclaration = new XmlDeclaration("1.0", "", "");
	cDocument.LinkEndChild(*pDeclaration);

	// Add path
	XmlElement *pPathElement = new XmlElement("Path");
	pPathElement->SetAttribute("Version", "1");
	if (cGraphPath.IsClosed()) pPathElement->SetAttribute("Closed", "1");

	// Loop through all nodes
	for (uint32 nNode=0; nNode<cGraphPath.GetNumOfNodes(); nNode++) {
		// Get the node
		const GraphNode *pNode = cGraphPath.GetNode(nNode);
		if (pNode) {
			// Add node
			XmlElement *pXmlElement = new XmlElement("Node");

			// Set node attributes
			pXmlElement->SetAttribute("Name",     pNode->GetName());
			pXmlElement->SetAttribute("Position", pNode->GetPos().ToString());

			// Link node element
			pPathElement->LinkEndChild(*pXmlElement);
		}
	}

	// Link path element
	cDocument.LinkEndChild(*pPathElement);

	// Save path
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
GraphPathLoaderPL::GraphPathLoaderPL()
{
}

/**
*  @brief
*    Destructor
*/
GraphPathLoaderPL::~GraphPathLoaderPL()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Loader implementation for format version 1
*/
bool GraphPathLoaderPL::LoadV1(GraphPath &cGraphPath, const XmlElement &cGraphPathElement) const
{
	// Is the path closed?
	String sClosed = cGraphPathElement.GetAttribute("Closed");
	cGraphPath.SetClosed(sClosed.GetLength() && sClosed.GetBool());

	// Iterate through all nodes and add them to the graph
	const XmlElement *pXmlElement = cGraphPathElement.GetFirstChildElement("Node");
	while (pXmlElement) {
		// Create the node
		GraphNode *pNode = new GraphNode(pXmlElement->GetAttribute("Name"));
		cGraphPath.AddNode(*pNode);

		// Set the position of the node
		String sPos = pXmlElement->GetAttribute("Position");
		if (sPos.GetLength()) {
			float fPos[3];
			ParseTools::ParseFloatArray(sPos, fPos, 3);
			pNode->SetPos(fPos[Vector3::X], fPos[Vector3::Y], fPos[Vector3::Z]);
		}

		// Next element, please
		pXmlElement = pXmlElement->GetNextSiblingElement("Node");
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLMath
