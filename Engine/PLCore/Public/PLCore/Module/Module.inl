/*********************************************************\
 *  File: Module.inl                                     *
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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get module ID
*/
inline uint32 Module::GetModuleID() const
{
	// Return ID
	return m_nModuleID;
}

/**
*  @brief
*    Check if module is loaded as a plugin
*/
inline bool Module::IsPlugin() const
{
	// Return if module is plugin
	return m_bPlugin;
}

/**
*  @brief
*    Get dynamic library that contains the plugin
*/
inline DynLib *Module::GetDynLib() const
{
	// Return library
	return m_pDynLib;
}

/**
*  @brief
*    Get absolute filename of dynamic library or executable that contains the plugin
*/
inline String Module::GetFilename() const
{
	// Return filename
	return m_sFilename;
}

/**
*  @brief
*    Get module name
*/
inline String Module::GetName() const
{
	// Return name
	return m_sName;
}

/**
*  @brief
*    Get module vendor
*/
inline String Module::GetVendor() const
{
	// Return vendor
	return m_sVendor;
}

/**
*  @brief
*    Get module license
*/
inline String Module::GetLicense() const
{
	// Return license
	return m_sLicense;
}

/**
*  @brief
*    Get module description
*/
inline String Module::GetDescription() const
{
	// Return description
	return m_sDescription;
}

/**
*  @brief
*    Get classes of module
*/
inline const List<const Class*> &Module::GetClasses() const
{
	// Return class list
	return m_lstClasses;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Add class
*/
inline void Module::AddClass(const Class *pClass)
{
	// Add class to list
	m_lstClasses.Add(pClass);
}

/**
*  @brief
*    Remove class
*/
inline void Module::RemoveClass(const Class *pClass)
{
	// Remove class from list
	m_lstClasses.Remove(pClass);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
