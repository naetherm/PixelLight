/*********************************************************\
 *  File: DatabaseQuery.h                                *
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


#ifndef __PLDATABASE_DATABASEQUERY_H__
#define __PLDATABASE_DATABASEQUERY_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLDatabase/PLDatabase.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class String;
}
namespace PLDatabase {
	class Database;
	class DatabaseQueryResult;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLDatabase {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract database query base class
*/
class DatabaseQuery {


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns whether the given SQL statement is a SELECT
		*
		*  @param[in] sSQL
		*    SQL statement to check
		*
		*  @return
		*    'true' if the given SQL statement is a SELECT one, else 'false'
		*/
		static PLDATABASE_API bool IsSelect(const PLCore::String &sSQL);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLDATABASE_API virtual ~DatabaseQuery();

		/**
		*  @brief
		*    Returns the database this query is in
		*
		*  @return
		*    The database this query is in
		*/
		PLDATABASE_API Database &GetDatabase() const;


	//[-------------------------------------------------------]
	//[ Public virtual DatabaseQuery functions                ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Executes a given SQL statement
		*
		*  @param[in] sSQL
		*    SQL statement to execute
		*
		*  @return
		*    A pointer to a newly created query result you have to delete by own or a null pointer
		*    if the SQL statement is not a select (ie: INSERT, UPDATE, etc.)
		*/
		virtual DatabaseQueryResult *Execute(const PLCore::String &sSQL) = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cParentDd
		*    Database this query is in
		*/
		PLDATABASE_API DatabaseQuery(Database &cParentDd);


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
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Database *m_pParentDb;	/**< Database this query is in (always valid!) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLDatabase


#endif // __PLDATABASE_DATABASEQUERY_H__
