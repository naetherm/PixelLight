/*********************************************************\
 *  File: Module.h                                       *
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


#ifndef __PLCORE_MODULE_H__
#define __PLCORE_MODULE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Class to give each module a unique ID
*
*  @remarks
*    This is implemented as a template to ensure, that the compiler will create a *unique* class
*    in each module (e.g. library or application) that is using that class. So, e.g. calling
*    ModuleID<int> from project A will result in another class being used than calling it
*    from project B, which allows the class to request a unique identifier in each module.
*    The class is accessed using ModuleID<int>.
*/
template <typename T> class ModuleID {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get module ID
		*
		*  @return
		*    Unique module ID
		*/
		static uint32 GetModuleID();

		/**
		*  @brief
		*    Register module
		*
		*  @param[in] sName
		*    Module name
		*  @param[in] sVendor
		*    Vendor name
		*  @param[in] sLicense
		*    Module license
		*  @param[in] sDescription
		*    Module description
		*/
		static void RegisterModule(const String &sName, const String &sVendor, const String &sLicense, const String &sDescription);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		static uint32 m_nModuleID;	/**< Unique module ID */


};

// Get module ID
template <typename T> uint32 ModuleID<T>::GetModuleID()
{
	// Request module ID from ClassManager
//	if (m_nModuleID == 0)
//		m_nModuleID = ClassManager::GetInstance()->GetUniqueModuleID();

	// Return module ID
	return m_nModuleID;
}

template <typename T> void ModuleID<T>::RegisterModule(const String &sName, const String &sVendor, const String &sLicense, const String &sDescription)
{
	// Ensure we have a valid module ID
	GetModuleID();

	// Register module, and provide a pointer to our static module ID variable ("memory anchor")
//	ClassManager::GetInstance()->RegisterModule(&m_nModuleID, sName, sVendor, sLicense, sDescription);
}

// Module ID
template <typename T> uint32	ModuleID<T>::m_nModuleID = 0;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_MODULE_H__
