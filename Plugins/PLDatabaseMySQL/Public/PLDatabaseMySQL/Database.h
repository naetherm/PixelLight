/*********************************************************\
 *  File: Database.h                                     *
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


#ifndef __PLDATABASEMYSQL_DATABASE_H__
#define __PLDATABASEMYSQL_DATABASE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLDatabase/Database.h>
#include "PLDatabaseMySQL/PLDatabaseMySQL.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
typedef struct st_mysql MYSQL;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLDatabaseMySQL {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    MySQL 5.0.24a (http://www.mysql.com/) database implementation
*
*  @note
*    - The following parameters of the Connect()-function are used: sServer, sUserName, sUserPassword, sDatabase
*/
class Database : public PLDatabase::Database {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLDATABASEMYSQL_RTTI_EXPORT, Database, "PLDatabaseMySQL", PLDatabase::Database, "MySQL 5.0.24a (http://www.mysql.com/) database implementation")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLDATABASEMYSQL_API Database();

		/**
		*  @brief
		*    Destructor
		*/
		PLDATABASEMYSQL_API virtual ~Database();

		/**
		*  @brief
		*    Returns the MySQL socket object
		*
		*  @return
		*    The MySQL socket object, can be a null pointer
		*/
		PLDATABASEMYSQL_API MYSQL *GetMySQL() const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		Database(const Database &cSource);

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
		Database &operator =(const Database &cSource);


	//[-------------------------------------------------------]
	//[ Public virtual PLDatabase::Database functions         ]
	//[-------------------------------------------------------]
	public:
		PLDATABASEMYSQL_API virtual PLCore::String GetVersion() const override;
		PLDATABASEMYSQL_API virtual bool Connect(const PLCore::String &sServer, const PLCore::String &sUserName, const PLCore::String &sUserPassword, const PLCore::String &sDatabase) override;
		PLDATABASEMYSQL_API virtual bool IsConnected() const override;
		PLDATABASEMYSQL_API virtual bool IsConnectionActive() const override;
		PLDATABASEMYSQL_API virtual bool Disconnect() override;
		PLDATABASEMYSQL_API virtual PLDatabase::DatabaseQuery *CreateQuery() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		MYSQL *m_pSQL;	/**< MySQL socket object, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLDatabaseMySQL


#endif // __PLDATABASEMYSQL_DATABASE_H__
