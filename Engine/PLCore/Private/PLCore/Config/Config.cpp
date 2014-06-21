/*********************************************************\
 *  File: Config.cpp                                     *
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
#include "PLCore/Base/Class.h"
#include "PLCore/String/ParseTools.h"
#include "PLCore/Config/ConfigLoader.h"
#include "PLCore/Config/Config.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(ConfigGroup)


//[-------------------------------------------------------]
//[ ConfigGroup functions                                 ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
ConfigGroup::ConfigGroup()
{
}

/**
*  @brief
*    Destructor
*/
ConfigGroup::~ConfigGroup()
{
}


//[-------------------------------------------------------]
//[ Config functions                                      ]
//[-------------------------------------------------------]
//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Config::Config()
{
}

/**
*  @brief
*    Destructor
*/
Config::~Config()
{
	// Destroy configuration plugin instances
	for (uint32 i=0; i<m_lstConfig.GetNumOfElements(); i++)
		delete m_lstConfig[i];
}

/**
*  @brief
*    Returns the value of a configuration class variable
*/
String Config::GetVar(const String &sClass, const String &sVariable)
{
	// Get configuration class
	ConfigGroup *pClass = GetClass(sClass);
	if (pClass) {
		// Get attribute
		const DynVar *pDynVar = pClass->GetAttribute(sVariable);
		if (pDynVar)
			return pDynVar->GetString();
	}

	// Error!
	return "";
}

/**
*  @brief
*    Returns the value of a configuration class variable
*/
int Config::GetVarInt(const String &sClass, const String &sVariable)
{
	// Get configuration class
	ConfigGroup *pClass = GetClass(sClass);
	if (pClass) {
		// Get attribute
		const DynVar *pDynVar = pClass->GetAttribute(sVariable);
		if (pDynVar)
			return pDynVar->GetInt();
	}

	// Error!
	return 0;
}

/**
*  @brief
*    Set the value of a configuration class variable
*/
bool Config::SetVar(const String &sClass, const String &sVariable, const String &sValue)
{
	// Get configuration class
	ConfigGroup *pClass = GetClass(sClass);
	if (pClass) {
		// Get attribute
		DynVar *pDynVar = pClass->GetAttribute(sVariable);
		if (pDynVar) {
			pDynVar->SetString(sValue);

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

/**
*  @brief
*    Set default configuration settings
*/
bool Config::SetDefault(const String &sClass, const String &sVariable)
{
	// Set all classes to default settings?
	if (sClass.GetLength())
		return SetClassDefault(sClass, sVariable);
	else {
		// Set a class to default settings
		bool bResult = true; // No error by default
		Iterator<ConfigGroup*> cIterator = m_lstConfig.GetIterator();
		while (cIterator.HasNext()) {
			ConfigGroup *pClass = cIterator.Next();
			if (!SetClassDefault(pClass->GetClass()->GetClassName(), sVariable))
				bResult = false; // Something went wrong
		}

		// Done
		return bResult;
	}
}

/**
*  @brief
*    Returns a configuration class instance
*/
ConfigGroup *Config::GetClass(const String &sName)
{
	// Is the configuration class already created?
	ConfigGroup *pConfig = m_mapConfig.Get(sName);
	if (pConfig)
		return pConfig;

	// No, check if it is a valid class
	const Class *pClass = ClassManager::GetInstance()->GetClass(sName);
	if (pClass && pClass->IsDerivedFrom("PLCore::ConfigGroup")) {
		// Create configuration object
		pConfig = static_cast<ConfigGroup*>(pClass->Create());
		if (pConfig) {
			// Add configuration object
			m_lstConfig.Add(pConfig);
			m_mapConfig.Add(pClass->GetClassName(), pConfig);
			return pConfig;
		}
	}

	// Class not found
	return nullptr;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
Config::Config(const Config &cSource)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
Config &Config::operator =(const Config &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}

/**
*  @brief
*    Set a class to default configuration settings
*/
bool Config::SetClassDefault(const String &sClass, const String &sVariable)
{
	// Get class
	ConfigGroup *pClass = GetClass(sClass);
	if (!pClass || !pClass->GetClass())
		return false; // Error!

	// Set all variables to default?
	if (sVariable.GetLength()) {
		// Get attribute
		DynVar *pDynVar = pClass->GetAttribute(sVariable);
		if (pDynVar)
			pDynVar->SetDefault();
	} else {
		pClass->SetDefaultValues();
	}

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Public virtual Loadable functions                     ]
//[-------------------------------------------------------]
String Config::GetLoadableTypeName() const
{
	static const String sString = "Config";
	return sString;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
