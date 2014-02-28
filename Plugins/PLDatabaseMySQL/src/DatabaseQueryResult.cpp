/*********************************************************\
 *  File: DatabaseQueryResult.cpp                        *
 *
 *  Copyright (C) 2002-2013 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  PixelLight is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  PixelLight is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with PixelLight. If not, see <http://www.gnu.org/licenses/>.
\*********************************************************/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#ifdef WIN32
	#include <Winsock2.h>
#endif
#include <mysql.h>
#include "PLDatabaseMySQL/DatabaseQueryResult.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLDatabaseMySQL {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
DatabaseQueryResult::~DatabaseQueryResult()
{
	mysql_free_result(m_pMySQLResult);
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
DatabaseQueryResult::DatabaseQueryResult(PLDatabase::DatabaseQuery &cParentQuery, MYSQL_RES &cMySQLResult) :
	PLDatabase::DatabaseQueryResult(cParentQuery),
	m_pMySQLResult(&cMySQLResult)
{
}

/**
*  @brief
*    Copy constructor
*/
DatabaseQueryResult::DatabaseQueryResult(const DatabaseQueryResult &cSource) :
	PLDatabase::DatabaseQueryResult(cSource.GetQuery()),
	m_pMySQLResult(nullptr)
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


//[-------------------------------------------------------]
//[ Public virtual PLDatabase::DatabaseQueryResult functions ]
//[-------------------------------------------------------]
bool DatabaseQueryResult::IsEmpty() const
{
	return (mysql_num_rows(m_pMySQLResult) <= 0);
}

void DatabaseQueryResult::FirstRow()
{
	mysql_data_seek(m_pMySQLResult, 0);
}

const Array<String> &DatabaseQueryResult::FetchFieldList()
{
	// Are there any fields within the current list?
	if (!m_lstFields.GetNumOfElements()) {
		const uint32 nNumOfFields = mysql_num_fields(m_pMySQLResult);
		const MYSQL_FIELD *pFields = mysql_fetch_fields(m_pMySQLResult);

		for (uint32 i=0; i<nNumOfFields; i++)
			m_lstFields.Add(pFields[i].name);
	}

	// Return a reference to the fields list
	return m_lstFields;
}

const HashMap<String, String> *DatabaseQueryResult::FetchRow()
{
	const MYSQL_ROW pRow = mysql_fetch_row(m_pMySQLResult);
	if (!pRow)
		return nullptr; // Error!

	const MYSQL_FIELD *pField = mysql_fetch_fields(m_pMySQLResult);
	const uint32 nNumOfFields = mysql_num_fields(m_pMySQLResult);

	m_mapRow.Clear();
	for (uint32 i=0; i<nNumOfFields; i++)
		m_mapRow.Add(pField[i].name, pRow[i]);

	// Return a pointer to the row map
	return &m_mapRow;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLDatabaseMySQL
