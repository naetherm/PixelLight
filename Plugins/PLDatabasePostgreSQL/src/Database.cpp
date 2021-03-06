/*********************************************************\
 *  File: Database.cpp                                   *
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
#include <PostgreSQL/libpq-fe.h>
#include <PostgreSQL/pg_config.h>
#include <PLCore/Log/Log.h>
#include "PLDatabasePostgreSQL/DatabaseQuery.h"
#include "PLDatabasePostgreSQL/Database.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLDatabasePostgreSQL {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(Database, "PLDatabasePostgreSQL", PLDatabase::Database, "PostgreSQL 8.1.4 (http://www.postgresql.org/) database implementation")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
pl_class_metadata_end(Database)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
Database::Database() :
	m_pPostgreSQL(nullptr)
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
*    Returns the PostgreSQL socket object
*/
PGconn *Database::GetPostgreSQL() const
{
	return m_pPostgreSQL;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
Database::Database(const Database &cSource) :
	m_pPostgreSQL(nullptr)
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
	static const String sString = PACKAGE_STRING;
	return sString;
}

bool Database::Connect(const String &sServer, const String &sUserName, const String &sUserPassword, const String &sDatabase)
{
	const String sFinal = "host=" + sServer + " user=" + sUserName + " password=" + sUserPassword + " dbname=" + sDatabase;
	m_pPostgreSQL = PQconnectdb(sFinal);
	if (m_pPostgreSQL)
		return true; // Done
	else {
		PL_LOG(Error, String("PostgreSQL could not connect to specified server! Error: ") + PQerrorMessage(m_pPostgreSQL))

		// Error!
		return false;
	}
}

bool Database::IsConnected() const
{
	return (m_pPostgreSQL != nullptr);
}

bool Database::IsConnectionActive() const
{
	return (PQstatus(m_pPostgreSQL) == CONNECTION_OK);
}

bool Database::Disconnect()
{
	if (m_pPostgreSQL) {
		PQfinish(m_pPostgreSQL);
		m_pPostgreSQL = nullptr;

		// Done
		return true;
	} else {
		// Error!
		return false;
	}
}

PLDatabase::DatabaseQuery *Database::CreateQuery()
{
	return new DatabaseQuery(*this);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLDatabasePostgreSQL
