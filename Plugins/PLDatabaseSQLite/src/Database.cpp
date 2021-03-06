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
#include <sqlite3.h>
#include <PLCore/Log/Log.h>
#include "PLDatabaseSQLite/DatabaseQuery.h"
#include "PLDatabaseSQLite/Database.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLDatabaseSQLite {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(Database, "PLDatabaseSQLite", PLDatabase::Database, "SQLite 3.3.7 (http://www.sqlite.org/) database implementation")
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
	m_pSQLite(nullptr)
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
*    Returns the SQLite socket object
*/
sqlite3 *Database::GetSQLite() const
{
	return m_pSQLite;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
Database::Database(const Database &cSource) :
	m_pSQLite(nullptr)
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
	static const String sString = SQLITE_VERSION;
	return sString;
}

bool Database::Connect(const String &sServer, const String &sUserName, const String &sUserPassword, const String &sDatabase)
{
	// Already connected?
	if (!m_pSQLite) {
		// Try to connect to our SQLite server
		if (sqlite3_open(sDatabase, &m_pSQLite) == SQLITE_OK)
			return true; // Done
		else
			PL_LOG(Error, String("SQLite could not connect to specified server! Error: ") + sqlite3_errmsg(m_pSQLite))
	}

	// Error!
	return false;
}

bool Database::IsConnected() const
{
	return (m_pSQLite != nullptr);
}

bool Database::IsConnectionActive() const
{
	// There's no special 'connection active' check within SQLite...
	return (m_pSQLite != nullptr);
}

bool Database::Disconnect()
{
	if (m_pSQLite) {
		sqlite3_close(m_pSQLite);
		m_pSQLite = nullptr;

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
} // PLDatabaseSQLite
