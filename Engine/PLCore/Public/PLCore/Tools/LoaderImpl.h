/*********************************************************\
 *  File: LoaderImpl.h                                   *
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


#ifndef __PLCORE_LOADER_IMPL_H__
#define __PLCORE_LOADER_IMPL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Base/Object.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract loader implementation base class, derive your concrete loader implementations from this class
*
*  @remarks
*    A loader implementation is ONLY responsible for loading & saving, NOT for opening/closing
*    concrete file objects or for clearing the given loadable!
*    There's only one instance of each loader within the system, do NOT use class member variables,
*    do ONLY use local variables else you may get in trouble when using multi-threading.
*    By default, the loadable system looks for "Load" and "LoadParams" RTTI methods for loading, and
*    "Save" and "SaveParams" for saving. For example "Load" is used if there are no special user provided
*    parameters, "LoadParams" is used if a loader supports special format dependent parameters and
*    the user explicitly set them. Please note that it's also possible to add methods with other names,
*    when loading a loadable, this method name can be used explicitly. The first method parameter has to
*    be a reference to the loadable, the second parameter has to be a reference to the file to operate on.
*
*    Each loader should have the following properties:
*    - "Type":    Loader type, usually only defined once within the abstract loader base class
*    - "Formats": File format extensions this loader can load in (for example: "bmp" or "jpg,jpeg")
*    - "Load":    "1" if loading is implemented, else "0"
*    - "Save":    "1" if saving is implemented, else "0"
*
*    Special file format case: There are file formats which don't have any filename extension or
*    even expect that it's data is within a provided directory (example: DICOM). As "Format", write
*    ",", ",dcm,DCM,dicom,DICOM" and so on.
*/
class LoaderImpl : public Object {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Loader;


	//[-------------------------------------------------------]
	//[ Public static data                                    ]
	//[-------------------------------------------------------]
	public:
		static PLCORE_API const String UnknownFormatVersion;			/**< 'Unknown format version' string */
		static PLCORE_API const String DeprecatedFormatVersion;			/**< 'Deprecated format version' string */
		static PLCORE_API const String NoLongerSupportedFormatVersion;	/**< 'No longer supported format version' string */
		static PLCORE_API const String InvalidFormatVersion;			/**< 'Invalid format version' string */


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCORE_RTTI_EXPORT, LoaderImpl, "PLCore", PLCore::Object, "Abstract loader implementation base class, derive your concrete loader implementations from this class")
		// Properties
		pl_properties
			pl_property("Type",		"Unknown")
			pl_property("Formats",	"")
			pl_property("Load",		"0")
			pl_property("Save",		"0")
		pl_properties_end
	pl_class_end


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		PLCORE_API LoaderImpl();

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~LoaderImpl();


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_LOADER_IMPL_H__
