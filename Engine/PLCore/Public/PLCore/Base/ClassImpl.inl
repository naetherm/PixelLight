/*********************************************************\
 *  File: ClassImpl.inl                                  *
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
*    Return the pointer to the owner class instance wrapping this class implementation
*/
inline Class *ClassImpl::GetClass() const
{
	return m_pClass;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get full class name (with namespace)
*/
inline String ClassImpl::GetClassName() const
{
	// Return name of class and namespace
	return m_sClassName;
}

/**
*  @brief
*    Get full name of base class (with namespace)
*/
inline String ClassImpl::GetBaseClassName() const
{
	// Return base class
	return m_sBaseClass;
}

/**
*  @brief
*    Get class name (without namespace)
*/
inline String ClassImpl::GetName() const
{
	// Return name of class
	return m_sName;
}

/**
*  @brief
*    Get class description
*/
inline String ClassImpl::GetDescription() const
{
	// Return description
	return m_sDescription;
}

/**
*  @brief
*    Get namespace
*/
inline String ClassImpl::GetNamespace() const
{
	// Return namespace
	return m_sNamespace;
}

/**
*  @brief
*    Get properties
*/
inline const HashMap<String, String> &ClassImpl::GetProperties() const
{
	// Check if class has been initialized
	if (!m_bInitialized)
		InitClass();

	// Return property map
	return m_mapProperties;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
