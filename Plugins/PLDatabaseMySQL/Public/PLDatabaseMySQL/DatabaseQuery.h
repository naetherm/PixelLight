/*********************************************************\
 *  File: DatabaseQuery.h                                *
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


#ifndef __PLDATABASEMYSQL_DATABASEQUERY_H__
#define __PLDATABASEMYSQL_DATABASEQUERY_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLDatabase/DatabaseQuery.h>
#include "PLDatabaseMySQL/PLDatabaseMySQL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLDatabaseMySQL {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Database query MySQL implementation
*/
class DatabaseQuery : public PLDatabase::DatabaseQuery {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Database;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLDATABASEMYSQL_API virtual ~DatabaseQuery();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cParentDd
		*    Database this query is in
		*/
		DatabaseQuery(PLDatabase::Database &cParentDd);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		DatabaseQuery(const DatabaseQuery &cSource);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		DatabaseQuery &operator =(const DatabaseQuery &cSource);


	//[-------------------------------------------------------]
	//[ Public virtual PLDatabase::DatabaseQuery functions    ]
	//[-------------------------------------------------------]
	public:
		PLDATABASEMYSQL_API virtual PLDatabase::DatabaseQueryResult *Execute(const PLCore::String &sSQL) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLDatabaseMySQL


#endif // __PLDATABASEMYSQL_DATABASEQUERY_H__
