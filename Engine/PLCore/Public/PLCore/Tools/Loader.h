/*********************************************************\
 *  File: Loader.h                                       *
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


#ifndef __PLCORE_LOADER_H__
#define __PLCORE_LOADER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/String/String.h"
#include "PLCore/Container/Array.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Class;
class Loadable;
class LoaderImpl;
class LoadableType;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Loader class
*
*  @note
*    - Implementation of the proxy design pattern
*/
class Loader {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Loadable;
	friend class LoadableType;
	friend class LoadableManager;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Loader implementation class
		*
		*  @return
		*    Loader implementation class
		*/
		inline const Class &GetClass() const;

		/**
		*  @brief
		*    Returns the loader implementation
		*
		*  @return
		*    The loader implementation, a null pointer on error
		*
		*  @note
		*    - If required, this method creates an instance of the loader implementation class
		*/
		PLCORE_API LoaderImpl *GetImpl();

		/**
		*  @brief
		*    Returns the loadable type
		*
		*  @return
		*    The loadable type, a null pointer on (terrible) error
		*/
		inline LoadableType *GetType() const;

		/**
		*  @brief
		*    Returns the name of the loadable type
		*
		*  @return
		*    The name of the loadable type
		*/
		PLCORE_API String GetTypeName() const;

		/**
		*  @brief
		*    Returns the number of supported formats
		*
		*  @return
		*    The number of supported formats
		*/
		inline uint32 GetNumOfFormats();

		/**
		*  @brief
		*    Returns a supported format
		*
		*  @param[in] nIndex
		*    Format index
		*
		*  @return
		*    The requested supported format, empty string on error
		*/
		inline String GetFormat(uint32 nIndex);

		/**
		*  @brief
		*    Checks if a format is supported in a list of extensions
		*
		*  @param[in] sExtension
		*    Extension of loadable
		*
		*  @return
		*    'true' if the format is supported, else 'false'
		*/
		PLCORE_API bool IsFormatSupported(const String &sExtension);

		/**
		*  @brief
		*    Returns the formats string
		*
		*  @return
		*    The formats string
		*/
		PLCORE_API String GetFormats() const;

		/**
		*  @brief
		*    Returns the description string
		*
		*  @return
		*    The description string
		*/
		PLCORE_API String GetDescription() const;

		/**
		*  @brief
		*    Returns the whether loading is supported
		*
		*  @return
		*    'true' if loading is supported, else 'false'
		*/
		PLCORE_API bool CanLoad() const;

		/**
		*  @brief
		*    Returns the whether saving is supported
		*
		*  @return
		*    'true' if saving is supported, else 'false'
		*/
		PLCORE_API bool CanSave() const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Default constructor
		*/
		Loader();

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cClass
		*    Loader implementation class, must be derived from "PLCore::LoaderImpl"
		*/
		Loader(const Class &cClass);

		/**
		*  @brief
		*    Destructor
		*/
		~Loader();

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
		Loader &operator =(const Loader &cSource);

		/**
		*  @brief
		*    Initializes the formats list
		*
		*  @note
		*    - If the list is already initialized, nothing happens
		*/
		PLCORE_API void InitFormatsList();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const Class	  *m_pClass;		/**< Loader implementation class, must be derived from "PLCore::LoaderImpl", always valid! */
		LoaderImpl	  *m_pLoaderImpl;	/**< Loader implementation class instance, can be a null pointer */
		LoadableType  *m_pLoadableType;	/**< Loadable type, can be a null pointer */
		Array<String>  m_lstFormats;	/**< List of parsed formats */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLCore/Tools/Loader.inl"


#endif // __PLCORE_LOADER_H__
