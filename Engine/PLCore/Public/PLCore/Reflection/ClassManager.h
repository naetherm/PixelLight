/*********************************************************\
 *  File: Rtti.h                                         *
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


#ifndef __PLCORE_REFL_CLASSMANAGER_H__
#define __PLCORE_REFL_CLASSMANAGER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Core/Singleton.h>
#include <PLCore/Container/HashMap.h>
#include <PLCore/String/String.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRefl {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Class;

//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    The central reflected class register
*
*  @remarks
*    TODO: describe this in more detail
*/
class ClassManager : public PLCore::Singleton<ClassManager> {

	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Register a new class into the manager
		*
		*  @param[in] sName
		*    Name of the class to register
		*  @param[in] szId
		*    Type Id of the class
		*
		*  @return
		*    New class type instance
		*/
		PLCORE_API PLRefl::Class &RegisterClass(const PLCore::String &sName, const char *szId);


	//[-------------------------------------------------------]
	//[ Private structures                                    ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Class info holder
		*/
		struct ClassInfo
		{
			PLCore::String sName;			/**< Name of the class */
			const char *szId;				/**< Id of the class type */
			PLRefl::Class* pClass;			/**< The actual class instance */

			bool operator==(const ClassInfo &other) const
			{
				return pClass == other.pClass;
			}
		};

	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		typedef PLCore::HashMap<PLCore::String, ClassInfo> ClassNameMap;
		ClassNameMap m_mapClassNames;		/**< All known classes by name */

		typedef PLCore::HashMap<const char*, ClassInfo> ClassIdMap;
		ClassIdMap m_mapClassIds;			/**< All known classes by Id */
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRefl


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "ClassManager.inl"


#endif // __PLCORE_REFL_CLASSMANAGER_H__
