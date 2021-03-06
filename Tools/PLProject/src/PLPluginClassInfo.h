/*********************************************************\
 *  File: PLPluginClassInfo.h                            *
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


#ifndef __PLPLUGINCLASSINFO_H__
#define __PLPLUGINCLASSINFO_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/String/String.h>
#include <PLCore/Container/HashMap.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class XmlElement;
}


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Class to parse an pl_class..pl_class_end block
*/
class PLPluginClassInfo {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLPluginClassInfo();

		/**
		*  @brief
		*    Destructor
		*/
		~PLPluginClassInfo();

		/**
		*  @brief
		*    Appends the parsed information to the given XML element
		*
		*  @param[in] cParent
		*   XML element to append the parsed information to
		*/
		void Save(PLCore::XmlElement &cParent) const;

		/**
		*  @brief
		*    Parse the given pl_class pl_class_end block
		*
		*  @param[in] sPLClassBlock
		*    Block to parse
		*/
		void ParsePLClassBlock(const PLCore::String &sPLClassBlock);

		/**
		*  @brief
		*    Parse the given pl_class_metadata pl_class_metadata_end block
		*
		*  @param[in] sPLClassMetadataBlock
		*    Block to parse
		*/
		void ParsePLClassMetadataBlock(const PLCore::String &sPLClassMetadataBlock);


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
		PLPluginClassInfo(const PLPluginClassInfo &cOther);

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
		PLPluginClassInfo &operator =(const PLPluginClassInfo &cOther);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::String									m_sClassName;				/**< Name of the class */
		PLCore::String									m_sNamespace;				/**< Namespace in which the class is in */
		PLCore::String									m_sBaseClass;				/**< Name of the base class */
		PLCore::String									m_sDescription;				/**< Class description */
		bool											m_bHasConstructor;			/**< Flag which indicates whether or not the class has at least one constructor known to the RTTI */
		bool											m_bHasDefaultConstructor;	/**< Flag which indicates whether or not the class has a default constructor known to the RTTI */
		PLCore::HashMap<PLCore::String, PLCore::String> m_mapProperties;			/**< List of class properties and their value */


};


#endif // __PLPLUGINCLASSINFO_H__
