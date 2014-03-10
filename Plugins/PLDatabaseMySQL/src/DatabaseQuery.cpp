/*********************************************************\
 *  File: DatabaseQuery.cpp                              *
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
	#undef Yield		// We undef this to avoid name conflicts with OS macros, why do they need to use macros?!
	#undef GetClassName // We undef this to avoid name conflicts with OS macros, why do they need to use macros?!
#endif
#include <mysql.h>
#include <PLCore/Log/Log.h>
#include "PLDatabaseMySQL/Database.h"
#include "PLDatabaseMySQL/DatabaseQueryResult.h"
#include "PLDatabaseMySQL/DatabaseQuery.h"


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
DatabaseQuery::~DatabaseQuery()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
DatabaseQuery::DatabaseQuery(PLDatabase::Database &cParentDd) : PLDatabase::DatabaseQuery(cParentDd)
{
}

/**
*  @brief
*    Copy constructor
*/
DatabaseQuery::DatabaseQuery(const DatabaseQuery &cSource) : PLDatabase::DatabaseQuery(cSource.GetDatabase())
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
DatabaseQuery &DatabaseQuery::operator =(const DatabaseQuery &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Public virtual PLDatabase::DatabaseQuery functions    ]
//[-------------------------------------------------------]
PLDatabase::DatabaseQueryResult *DatabaseQuery::Execute(const String &sSQL)
{
	MYSQL *pMySQL = static_cast<Database&>(GetDatabase()).GetMySQL();
	const int nError = mysql_query(pMySQL, sSQL);
	if (!nError) {
		// Get the result
		MYSQL_RES *pResult = mysql_store_result(pMySQL);
		if (pResult)
			return new DatabaseQueryResult(*this, *pResult);
	} else {
		PL_LOG(Error, String("MySQL can't execute the given SQL statement! Error: ") + mysql_error(pMySQL))
	}

	// Error!
	return nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLDatabaseMySQL
