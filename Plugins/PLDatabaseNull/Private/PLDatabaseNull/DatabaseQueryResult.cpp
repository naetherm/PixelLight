/*********************************************************\
 *  File: DatabaseQueryResult.cpp                        *
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
#include "PLDatabaseNull/DatabaseQueryResult.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLDatabaseNull {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
DatabaseQueryResult::~DatabaseQueryResult()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
DatabaseQueryResult::DatabaseQueryResult(PLDatabase::DatabaseQuery &cParentQuery) : PLDatabase::DatabaseQueryResult(cParentQuery)
{
}

/**
*  @brief
*    Copy constructor
*/
DatabaseQueryResult::DatabaseQueryResult(const DatabaseQueryResult &cSource) : PLDatabase::DatabaseQueryResult(cSource.GetQuery())
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
DatabaseQueryResult &DatabaseQueryResult::operator =(const DatabaseQueryResult &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}

void DatabaseQueryResult::Reset()
{
}

bool DatabaseQueryResult::IncRow()
{
	return true;
}

bool DatabaseQueryResult::EndOfRow() const
{
	return true;
}

const char *DatabaseQueryResult::GetCurrentValue(uint32 nColumn) const
{
	return nullptr;
}

const char *DatabaseQueryResult::GetFieldName(uint32 nColumn) const
{
	return nullptr;
}


//[-------------------------------------------------------]
//[ Public virtual PLDatabase::DatabaseQueryResult functions ]
//[-------------------------------------------------------]
bool DatabaseQueryResult::IsEmpty() const
{
	return true;
}

void DatabaseQueryResult::FirstRow()
{
	Reset();
}

const Array<String> &DatabaseQueryResult::FetchFieldList()
{
	// Return a reference to the fields list
	return m_lstFields;
}

const HashMap<String, String> *DatabaseQueryResult::FetchRow()
{
	// Error!
	return nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLDatabaseNull
