/*********************************************************\
 *  File: Database.cpp                                   *
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
#include "PLDatabaseMySQL/DatabaseQuery.h"
#include "PLDatabaseMySQL/Database.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLDatabaseMySQL {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(Database)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
Database::Database() :
	m_pSQL(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
Database::~Database()
{
	Disconnect();
}

/**
*  @brief
*    Returns the MySQL socket object
*/
MYSQL *Database::GetMySQL() const
{
	return m_pSQL;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
Database::Database(const Database &cSource) :
	m_pSQL(nullptr)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
Database &Database::operator =(const Database &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Public virtual PLDatabase::Database functions         ]
//[-------------------------------------------------------]
String Database::GetVersion() const
{
	static const String sString = MYSQL_SERVER_VERSION;
	return sString;
}

bool Database::Connect(const String &sServer, const String &sUserName, const String &sUserPassword, const String &sDatabase)
{
	// Already connected?
	if (!m_pSQL) {
		// Allocate/initialize the connection object
		m_pSQL = mysql_init(nullptr);
		if (m_pSQL) {
			// Try to connect to our MySQL server
			if (mysql_real_connect(m_pSQL, sServer, sUserName, sUserPassword, sDatabase, MYSQL_PORT, nullptr, 0))
				return true; // Done
			PL_LOG(Error, String("MySQL could not connect to specified server! Error: ") + mysql_error(m_pSQL))
		}
	}

	// Error!
	return false;
}

bool Database::IsConnected() const
{
	return (m_pSQL != nullptr);
}

bool Database::IsConnectionActive() const
{
	return !mysql_ping(m_pSQL);
}

bool Database::Disconnect()
{
	if (m_pSQL) {
		mysql_close(m_pSQL);
		m_pSQL = nullptr;

		// Done
		return true;
	} else {
		return false; // Error!
	}
}

PLDatabase::DatabaseQuery *Database::CreateQuery()
{
	return new DatabaseQuery(*this);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLDatabaseMySQL
