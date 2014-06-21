/*********************************************************\
 *  File: DynFunc.cpp                                    *
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
#include "PLCore/Base/Func/DynFunc.h"
#include "PLCore/Base/Func/DynParams.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
DynFunc::~DynFunc()
{
}


//[-------------------------------------------------------]
//[ Public virtual DynFunc functions                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get function descriptor
*/
const FuncDesc *DynFunc::GetDesc() const
{
	// No descriptor by default
	return nullptr;
}

/**
*  @brief
*    Call function
*/
void DynFunc::Call(DynParams &cParams)
{
	// No function
}

/**
*  @brief
*    Call function
*/
void DynFunc::Call(const DynParams &cParams)
{
	// No function
}

/**
*  @brief
*    Call function
*/
void DynFunc::Call(const String &sParams)
{
	// No function
}

/**
*  @brief
*    Call function
*/
void DynFunc::Call(const XmlElement &cElement)
{
	// No function
}

/**
*  @brief
*    Call function with return as string
*/
String DynFunc::CallWithReturn(const String &sParams)
{
	// No function
	return "";
}

/**
*  @brief
*    Call function with return as string
*/
String DynFunc::CallWithReturn(const XmlElement &cElement)
{
	// No function
	return "";
}

/**
*  @brief
*    Clone function object
*/
DynFunc *DynFunc::Clone() const
{
	// No function
	return nullptr;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
DynFunc::DynFunc()
{
}

/**
*  @brief
*    Copy constructor
*/
DynFunc::DynFunc(const DynFunc &cDynFunc)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*
*/
DynFunc &DynFunc::operator =(const DynFunc &cDynFunc)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
