/*********************************************************\
 *  File: DatabaseQueryResult.h                          *
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


#ifndef __PLDATABASE_DATABASEQUERYRESULT_H__
#define __PLDATABASE_DATABASEQUERYRESULT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/String/String.h>
#include <PLCore/Container/Array.h>
#include <PLCore/Container/HashMap.h>
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
*    Abstract database query result base class
*/
class DatabaseQueryResult {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLDATABASE_API virtual ~DatabaseQueryResult();

		/**
		*  @brief
		*    Returns the database query this data base query result is in
		*
		*  @return
		*    The database query this data base query result is in
		*/
		PLDATABASE_API DatabaseQuery &GetQuery() const;


	//[-------------------------------------------------------]
	//[ Public virtual DatabaseQueryResult functions          ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns whether all query results are processed or not
		*
		*  @return
		*    'true' if all query results are processed, else 'false'
		*/
		virtual bool IsEmpty() const = 0;

		/**
		*  @brief
		*    Resets the query result to the first row
		*/
		virtual void FirstRow() = 0;

		/**
		*  @brief
		*    Fetches the field list
		*
		*  @return
		*    Reference to the current field list
		*/
		virtual const PLCore::Array<PLCore::String> &FetchFieldList() = 0;

		/**
		*  @brief
		*    Fetches current row from cursor and moves forward
		*
		*  @return
		*    Pointer to the current row map, a null pointer on error (do NOT delete this!)
		*/
		virtual const PLCore::HashMap<PLCore::String, PLCore::String> *FetchRow() = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cParentQuery
		*    Database query this database query result is in
		*/
		PLDATABASE_API DatabaseQueryResult(DatabaseQuery &cParentQuery);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLCore::Array<PLCore::String>					m_lstFields;	/**< List of fields */
		PLCore::HashMap<PLCore::String, PLCore::String> m_mapRow;		/**< Row map */


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
		DatabaseQueryResult(const DatabaseQueryResult &cSource);

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
		DatabaseQueryResult &operator =(const DatabaseQueryResult &cSource);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		DatabaseQuery *m_pParentQuery;	/**< Database query this database query result is in (always valid!) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLDatabase


#endif // __PLDATABASE_DATABASEQUERYRESULT_H__
