/*********************************************************\
 *  File: RagdollLoaderPL.h                              *
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


#ifndef __PLPHYSICS_SCENENODES_RAGDOLLLOADER_PL_H__
#define __PLPHYSICS_SCENENODES_RAGDOLLLOADER_PL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLPhysics/SceneNodes/RagdollLoader.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLPhysics {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class SNRagdoll;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Ragdoll loader implementation for the PixelLight ragdoll XML file format
*/
class RagdollLoaderPL : public RagdollLoader {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLPHYSICS_RTTI_EXPORT, RagdollLoaderPL, "PLPhysics", PLPhysics::RagdollLoader, "Ragdoll loader implementation for the PixelLight ragdoll XML file format")
		// Properties
		pl_properties
			pl_property("Formats",	"ragdoll,RAGDOLL")
			pl_property("Load",		"1")
			pl_property("Save",		"1")
		pl_properties_end
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
		// Methods
		pl_method_2(Load,		pl_ret_type(bool),	SNRagdoll&,	PLCore::File&,			"Load method. By default, attach to world is allowed.",					"")
		pl_method_3(LoadParams,	pl_ret_type(bool),	SNRagdoll&,	PLCore::File&,	bool,	"Load method. Parameters: Attach to world allowed state as 'bool'.",	"")
		pl_method_2(Save,		pl_ret_type(bool),	SNRagdoll&,	PLCore::File&,			"Save method",															"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI methods                                   ]
	//[-------------------------------------------------------]
	public:
		PLPHYSICS_API bool Load(SNRagdoll &cSNRagdoll, PLCore::File &cFile);
		PLPHYSICS_API bool LoadParams(SNRagdoll &cSNRagdoll, PLCore::File &cFile, bool bAllowAttachToWorld);
		PLPHYSICS_API bool Save(SNRagdoll &cSNRagdoll, PLCore::File &cFile);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLPHYSICS_API RagdollLoaderPL();

		/**
		*  @brief
		*    Destructor
		*/
		PLPHYSICS_API virtual ~RagdollLoaderPL();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Loader implementation for format version 1
		*
		*  @param[in] cSNRagdoll
		*    Ragdoll to load into
		*  @param[in] cRagdollElement
		*    Ragdoll XML element to read the data from
		*  @param[in] bAllowAttachToWorld
		*    Allow attach to world?
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool LoadV1(SNRagdoll &cSNRagdoll, const PLCore::XmlElement &cRagdollElement, bool bAllowAttachToWorld) const;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLPhysics


#endif // __PLPHYSICS_SCENENODES_RAGDOLLLOADER_PL_H__
