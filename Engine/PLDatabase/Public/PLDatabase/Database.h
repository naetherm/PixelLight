/*********************************************************\
 *  File: Database.h                                     *
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


#ifndef __PLDATABASE_DATABASE_H__
#define __PLDATABASE_DATABASE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Object.h>
#include "PLDatabase/PLDatabase.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLDatabase {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class DatabaseQuery;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract SQL (Structured Query Language) database base class
*/
class Database : public PLCore::Object {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLDATABASE_RTTI_EXPORT, Database, "PLDatabase", PLCore::Object, "Abstract SQL (Structured Query Language) database base class")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Creates a database instance
		*
		*  @param[in] sClass
		*    Class name of the database implementation
		*
		*  @return
		*    The database instance, a null pointer on error
		*/
		static PLDATABASE_API Database *Create(const PLCore::String &sClass);


	//[-------------------------------------------------------]
	//[ Public virtual Database functions                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLDATABASE_API virtual ~Database();

		/**
		*  @brief
		*    Returns the version of the database implementation
		*
		*  @return
		*    The version of the database implementation
		*/
		virtual PLCore::String GetVersion() const = 0;

		/**
		*  @brief
		*    Connect to a database
		*
		*  @param[in] sServer
		*    Server name, can be a hostname (for instance 'localhost') or a IP-address
		*  @param[in] sUserName
		*    User name
		*  @param[in] sUserPassword
		*    User password
		*  @param[in] sDatabase
		*    Database
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		virtual bool Connect(const PLCore::String &sServer, const PLCore::String &sUserName, const PLCore::String &sUserPassword, const PLCore::String &sDatabase) = 0;

		/**
		*  @brief
		*    Returns whether there's an active database connection or not
		*
		*  @return
		*    'true' if there's an active database connection, else 'false'
		*/
		virtual bool IsConnected() const = 0;

		/**
		*  @brief
		*    Returns whether the connection to the database is still active or not
		*
		*  @return
		*    'true' the connection to the database is still active, else 'false'
		*
		*  @remarks
		*    Checks whether the server connection is still active. If the connection has broken, normally
		*    an automatic reconnection is attempted. If you as client idle for a long tíme, you can use this
		*    function to check whether the server has closed the connection and reconnect if necessary.
		*/
		virtual bool IsConnectionActive() const = 0;

		/**
		*  @brief
		*    Disconnect from the database
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe there's no connection to disconnect?)
		*/
		virtual bool Disconnect() = 0;

		/**
		*  @brief
		*    Creates a database query
		*
		*  @return
		*    The database query, a null pointer on error
		*/
		virtual DatabaseQuery *CreateQuery() = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		PLDATABASE_API Database();


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


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLDatabase


#endif // __PLDATABASE_DATABASE_H__
