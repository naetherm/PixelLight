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


#ifndef __PLDATABASENULL_DATABASE_H__
#define __PLDATABASENULL_DATABASE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLDatabase/Database.h>
#include "PLDatabaseNull/PLDatabaseNull.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLDatabaseNull {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Null database implementation
*/
class Database : public PLDatabase::Database {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class_def(PLDATABASENULL_API)
	pl_class_def_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLDATABASENULL_API Database();

		/**
		*  @brief
		*    Destructor
		*/
		PLDATABASENULL_API virtual ~Database();


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
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool m_bConnected;	/**< Are we currently connected to the database? */


	//[-------------------------------------------------------]
	//[ Public virtual PLDatabase::Database functions         ]
	//[-------------------------------------------------------]
	public:
		PLDATABASENULL_API virtual PLCore::String GetVersion() const override;
		PLDATABASENULL_API virtual bool Connect(const PLCore::String &sServer, const PLCore::String &sUserName, const PLCore::String &sUserPassword, const PLCore::String &sDatabase) override;
		PLDATABASENULL_API virtual bool IsConnected() const override;
		PLDATABASENULL_API virtual bool IsConnectionActive() const override;
		PLDATABASENULL_API virtual bool Disconnect() override;
		PLDATABASENULL_API virtual PLDatabase::DatabaseQuery *CreateQuery() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLDatabaseNull


#endif // __PLDATABASENULL_DATABASE_H__
