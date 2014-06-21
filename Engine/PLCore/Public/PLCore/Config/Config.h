/*********************************************************\
 *  File: Config.h                                       *
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


#ifndef __PLCORE_CONFIG_H__
#define __PLCORE_CONFIG_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Base/Object.h"
#include "PLCore/Tools/Loadable.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Configuration group
*
*  @remarks
*    This is the base class ALL configuration group are derived from. Derive your configuration
*    groups from this new configuration group 'collection'. By doing it in this way, you are able
*    to save this own configuration group into separate configuration files.
*
*  @verbatim
*   Example:
*   cConfig.Save("Config/All.cfg") will save ALL available configurations into one
*   file.
*   cConfig.Save("Config/Core.cfg", "CoreConfigGroup") will ONLY will save
*   available configurations derived from 'CoreConfigGroup' into one file.
*
*   class CoreConfigGroup : public ConfigGroup {
*     pl_class(CoreConfigGroup, "PLCore", PLCore::ConfigGroup, "PLCore configuration classes")
*     pl_class_end
*   };
*
*   class CoreConfig : public CoreConfigGroup {
*     pl_class(CoreConfig, "PLCore", PLCore::CoreConfigGroup, "PLCore general configuration classes")
*       pl_constructor_0(DefaultConstructor, "Default constructor", "")
*       pl_attribute(Blabla, bool, true, ReadWrite, DirectValue, "Blabla setting", "")
*     pl_class_end
*   };
*  @endverbatim
*/
class ConfigGroup : public Object {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Config;


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCORE_RTTI_EXPORT, ConfigGroup, "PLCore", PLCore::Object, "Configuration group")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		PLCORE_API ConfigGroup();

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~ConfigGroup();


};

/**
*  @brief
*    Configuration class
*
*  @remarks
*    Configuration class. If this configuration class is created it
*    will register all RTTI classes which are in the group "PLCore::ConfigGroup"
*    automatically to it's configuration system.\n
*
*    Usage example:\n
*  @verbatim
*    // Somewhere deep within your custom configuration class header...
*    class ConfigTest : public ConfigGroup {
*      pl_class(ConfigTest, "PLCore", PLCore::ConfigGroup, "Test config class")
*        pl_constructor_0(DefaultConstructor, "Default constructor", "")
*        pl_attribute(Detail, float, 1.0f, ReadWrite, DirectValue, "Detail description text", "")
*      pl_class_end
*    };
*    // Somewhere in your custom configuration class source code
*    pl_implement_class(ConfigTest)
*    // Set ConfigTest variable value
*    cConfig.SetVar("ConfigTest", "Detail", "2.0");
*    // Get ConfigTest variable value
*    float fDetail = cConfig.GetVar("ConfigTest", "Detail").GetFloat();
*  @endverbatim
*/
class Config : public Loadable {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLCORE_API Config();

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~Config();

		/**
		*  @brief
		*    Returns the value of a configuration class variable
		*
		*  @param[in] sClass
		*    Configuration class name (no empty string)
		*  @param[in] sVariable
		*    Name of the variable (no empty string)
		*
		*  @return
		*    Configuration variable value, empty string on error so
		*    this pointer is never a null pointer!
		*/
		PLCORE_API String GetVar(const String &sClass, const String &sVariable);

		/**
		*  @brief
		*    Returns the value of a configuration class variable
		*
		*  @param[in] sClass
		*    Configuration class name (no empty string)
		*  @param[in] sVariable
		*    Name of the variable (no empty string)
		*
		*  @return
		*    Configuration variable value
		*
		*  @note
		*    - If the variable type is PL_VAR_FLAGS, the flags will be read out
		*    - If the variable type is PL_VAR_ENUM, the enum value will be read out
		*/
		PLCORE_API int GetVarInt(const String &sClass, const String &sVariable);

		/**
		*  @brief
		*    Set the value of a configuration class variable
		*
		*  @param[in] sClass
		*    Configuration class name (no empty string)
		*  @param[in] sVariable
		*    Name of the variable (no empty string)
		*  @param[in] sValue
		*    Variable value
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLCORE_API bool SetVar(const String &sClass, const String &sVariable, const String &sValue = "");

		/**
		*  @brief
		*    Set default configuration settings
		*
		*  @param[in] sClass
		*    Configuration class name, if empty string, set ALL settings to default values
		*  @param[in] sVariable
		*    Name of the variable, if empty string, set all variables
		*    of the given configuration class to default values
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLCORE_API bool SetDefault(const String &sClass = "", const String &sVariable = "");

		/**
		*  @brief
		*    Returns the number of configuration class instances
		*
		*  @return
		*    The number of configuration class instances
		*/
		inline uint32 GetNumOfClasses() const;

		/**
		*  @brief
		*    Returns a configuration class instance by index
		*
		*  @param[in] nIndex
		*    Index of the configuration class instance to return
		*
		*  @return
		*    The requested configuration class instance, a null pointer on error
		*/
		inline ConfigGroup *GetClassByIndex(uint32 nIndex) const;

		/**
		*  @brief
		*    Returns a configuration class instance
		*
		*  @param[in] sName
		*    Configuration class name
		*
		*  @return
		*    The requested configuration class instance, a null pointer if the class wasn't found
		*
		*  @remarks
		*    You can use\n
		*    List<Class*> lstClasses;\n
		*    System::GetInstance()->GetClassList(&lstClasses, "PLConfig");\n
		*    to get a list of all configuration plugins.
		*/
		PLCORE_API ConfigGroup *GetClass(const String &sName);


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
		Config(const Config &cSource);

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
		Config &operator =(const Config &cSource);

		/**
		*  @brief
		*    Set a class to default configuration settings
		*
		*  @param[in] sClass
		*    Configuration class name (no empty string)
		*  @param[in] sVariable
		*    Name of the variable, if empty string, set all variables
		*    of the given configuration class to default values
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool SetClassDefault(const String &sClass, const String &sVariable);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Array<ConfigGroup*>			  m_lstConfig;
		HashMap<String, ConfigGroup*> m_mapConfig;


	//[-------------------------------------------------------]
	//[ Public virtual Loadable functions                     ]
	//[-------------------------------------------------------]
	public:
		PLCORE_API virtual String GetLoadableTypeName() const override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Config/Config.inl"


#endif // __PLCORE_CONFIG_H__
