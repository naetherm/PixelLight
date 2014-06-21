/*********************************************************\
 *  File: PLPluginInfo.h                                 *
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


#ifndef __PLPLUGININFO_H__
#define __PLPLUGININFO_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
//#include "PLPluginPlatformInfo.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class File;
	class XmlElement;
	class XmlDocument;
	class Class;
}


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Class to parse source and header files to generate information which gets saved in .plugins files
*
*  @verbatim
*    Usage example:
*    String sLibraryName    = "MyExamplePlugin";							// Name of the compiled library
*    String sMainModulePath = "<projectPath>/source/MyExamplePlugin.cpp";	// Path to the source file which holds the pl_plugin_module definition
*    String sIncludePath    = "<projectPath>/include/";						// Path where the include files of the project are located
*
*    PLPluginInfo cPluginInfo;
*    cPluginInfo.SetPluginFileVersion("1");				// Set version of the plugin file
*    cPluginInfo.SetDelayed(true);						// Set delayed value
*    cPluginInfo.SetActive(true);						// Set active value
*    cPluginInfo.SetPLVersion("PixelLight 1.0.1-R1");	// Set version string of PixelLight
*    cPluginInfo.SetLibraryName(sLibraryName);			// Set the name of the library to which the sources gets compiled
*    cPluginInfo.ParseMainModuleFile(sMainModulePath);	// Let the class parse the main module source file for an pl_plugin_module pl_module_end block
*    cPluginInfo.(sIncludePath);		// Let the class parse all include files at the given path for pl_class pl_class_end blocks
*
*    cPluginInfo.Save(File::StandardOutput); // Print the result to the standard output
*
*    Example output:
*    <?xml version="1.0" ?>
*    <Plugin Version="1" PixelLightVersion="PixelLight 1.0.1-R1">
*       <Active>1</Active>
*       <Delayed>1</Delayed>
*       <Name>MyExample</Name>
*       <Vendor>Copyright (C) 2002-2013 by The PixelLight Team</Vendor>
*       <License>"MIT License" which is also known as "X11 License" or "MIT X License" (mit-license.org)</License>
*       <Description>Example Plugin Module Description</Description>
*       <Platform Name="Win32">
*           <Library Type="Release">MyExamplePlugin.dll</Library>
*           <Library Type="Debug">MyExamplePluginD.dll</Library>
*       </Platform>
*       <Platform Name="Win64">
*           <Library Type="Release">MyExamplePlugin64.dll</Library>
*           <Library Type="Debug">MyExamplePluginD64.dll</Library>
*       </Platform>
*       <Platform Name="Linux">
*           <Library Type="Release">libMyExamplePlugin.so</Library>
*           <Library Type="Debug">libMyExamplePluginLD.so</Library>
*       </Platform>
*       <Classes>
*           <Class Name="MyExamplePluginClass" Namespace="MyExamplePlugin Class Description" BaseClassName="PLRenderer::RendererBackend" Description="Example Plugin" HasConstructor="1" HasDefaultConstructor="0" />
*    </Plugin>
*  @endverbatim
*/
class PLPluginInfo {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLPluginInfo();

		/**
		*  @brief
		*    Destructor
		*/
		~PLPluginInfo();

		/**
		*  @brief
		*    Sets the name of the library
		*
		*  @param[in] sLibraryName
		*    Name of the library
		*/
		void SetLibraryName(const PLCore::String &sLibraryName);

		/**
		*  @brief
		*    Sets a library suffix, this string is appended to the library name
		*
		*  @param[in] sLibrarySuffix
		*    Library suffix
		*/
		void SetLibrarySuffix(const PLCore::String &sLibrarySuffix);

		/**
		*  @brief
		*    Sets the version of the plugin file
		*
		*  @param[in] sPluginVersion
		*    Version of the plugin file
		*/
		void SetPluginFileVersion(const PLCore::String &sPluginVersion);

		/**
		*  @brief
		*    Sets the PixelLight version string
		*
		*  @param[in] sPLVersion
		*    PixelLight version string
		*/
		void SetPLVersion(const PLCore::String &sPLVersion);

		/**
		*  @brief
		*    Set active flag, this flag indicates if the plugin is active or not
		*
		*  @param[in] bActive
		*    Active state
		*
		*  @note
		*    - This value can be overwritten by the pl_module_active definition in the main source file of the parsed project
		*/
		void SetActive(bool bActive);

		/**
		*  @brief
		*    Set delayed flag, this flag indicates if delayed loading should be used for this plugin
		*
		*  @param[in] bDelayed
		*    Delayed state
		*
		*  @note
		*    - This value can be overwritten by the pl_module_delayed definition in the main source file of the parsed project
		*/
		void SetDelayed(bool bDelayed);

		/**
		*  @brief
		*    Loads the given module and retrieves plugin info from it
		*
		*  @param[in] sModulePath
		*    Path to the module to load
		*/
		bool CreateFromModule(const PLCore::String &sModulePath);

		/**
		*  @brief
		*    Saves the parsed information to the given file
		*
		*  @param[in] sFilename
		*    Name of the file to write into
		*/
		void Save(const PLCore::String &sFilename) const;

		/**
		*  @brief
		*    Saves the parsed information to the given file
		*
		*  @param[out] cFile
		*    File to write in, must be opened and writable
		*/
		void Save(PLCore::File &cFile) const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cOther
		*    Source to copy from
		*/
		PLPluginInfo(const PLPluginInfo &Other);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cOther
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		PLPluginInfo &operator =(const PLPluginInfo &cOther);


		/**
		*  @brief
		*    Adds an XmlTextElement child to an XmlElement
		*
		*  @param[out] cParent
		*    Parent to add to
		*  @param[in]  sName
		*    XML element name
		*  @param[in]  sValue
		*    XML element value
		*/
		void AddTextXmlElement(PLCore::XmlElement &cParent, const PLCore::String &sName, const PLCore::String &sValue) const;


		/**
		*  @brief
		*    Appends the parsed information about a plugin to the given XML document
		*
		*  @param[out] cDocument
		*    XML document to append to
		*/
		void AppendInformation(PLCore::XmlDocument &cDocument) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::String						m_sPluginFileVersion;	/**< Plugin version string */
		PLCore::String						m_sPLVersion;			/**< PixelLight version string */
		PLCore::String						m_sModuleName;			/**< Name of the plugin module */
		PLCore::String						m_sLibraryName;			/**< Name of the plugin library file */
		bool								m_bActive;				/**< Flag if the plugin is active */
		bool								m_bDelayed;				/**< Flag if the delayed loading should be used */
		PLCore::String						m_sDescription;			/**< Plugin description */
		PLCore::String						m_sVendor;				/**< Plugin vendor information */
		PLCore::String						m_sLicense;				/**< Plugin license information */
		PLCore::String						m_sPluginName;			/**< Name of the plugin */
		PLCore::String						m_sModuleVersion;		/**< Version of the module */
		PLCore::Array<const PLCore::Class*>	m_lstClasses;		/**< List of PLPLuginClassInfo instances */


};


#endif // __PLPLUGININFO_H__
