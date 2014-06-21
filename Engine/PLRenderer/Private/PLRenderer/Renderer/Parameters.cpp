/*********************************************************\
 *  File: Parameters.cpp                                 *
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
#include "PLRenderer/Renderer/Parameters.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Global variables                                      ]
//[-------------------------------------------------------]
// For fast Parameters::DataTypeToString()
const String g_sDataTypeToString[] = {
	"String",
	"Integer",
	"Integer2",
	"Integer3",
	"Integer4",
	"Float",
	"Float2",
	"Float3",
	"Float4",
	"Double",
	"Double2",
	"Double3",
	"Double4",
	"Float3x3",
	"Float3x4",
	"Float4x4",
	"Double4x4",
	"Texture", // [TODO] Currently not 'TextureBuffer'
	"Unknown"
};
// For fast Parameters::DataTypeFromString()
HashMap<String, Parameters::EDataType> g_mapDataTypeFromString;


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns a data type as string
*/
String Parameters::DataTypeToString(EDataType nDataType)
{
	return (nDataType <= UnknownDataType) ? g_sDataTypeToString[nDataType] : "";
}

/**
*  @brief
*    Returns a data type as number
*/
Parameters::EDataType Parameters::DataTypeFromString(const PLCore::String &sDataType)
{
	// Check parameter
	if (sDataType.GetLength()) {
		// Initialize the hash map?
		if (!g_mapDataTypeFromString.GetNumOfElements()) {
			g_mapDataTypeFromString.Add("String",	 String);
			g_mapDataTypeFromString.Add("Integer",	 Integer);
			g_mapDataTypeFromString.Add("Integer2",	 Integer2);
			g_mapDataTypeFromString.Add("Integer3",	 Integer3);
			g_mapDataTypeFromString.Add("Integer4",	 Integer4);
			g_mapDataTypeFromString.Add("Float",	 Float);
			g_mapDataTypeFromString.Add("Float2",	 Float2);
			g_mapDataTypeFromString.Add("Float3",	 Float3);
			g_mapDataTypeFromString.Add("Float4",	 Float4);
			g_mapDataTypeFromString.Add("Double",	 Double);
			g_mapDataTypeFromString.Add("Double2",	 Double2);
			g_mapDataTypeFromString.Add("Double3",	 Double3);
			g_mapDataTypeFromString.Add("Double4",	 Double4);
			g_mapDataTypeFromString.Add("Float3x3",	 Float3x4);
			g_mapDataTypeFromString.Add("Float3x4",	 Float3x4);
			g_mapDataTypeFromString.Add("Float4x4",	 Float4x4);
			g_mapDataTypeFromString.Add("Double4x4", Double4x4);
			g_mapDataTypeFromString.Add("Texture",   TextureBuffer); // [TODO] Currently not 'TextureBuffer'
			g_mapDataTypeFromString.Add("Unknown",	 UnknownDataType);
		}

		// Get the data type as number
		const EDataType &nDataType = g_mapDataTypeFromString.Get(sDataType);
		if (&nDataType != &HashMap<PLCore::String, EDataType>::Null)
			return nDataType;
	}

	// Error!
	return UnknownDataType;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Parameters::Parameters()
{
}

/**
*  @brief
*    Destructor
*/
Parameters::~Parameters()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
Parameters::Parameters(const Parameters &cSource)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
Parameters &Parameters::operator =(const Parameters &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
