/*********************************************************\
 *  File: ScriptBinding_PL_Log.h                         *
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


#ifndef __PLSCRIPTBINDINGS_PL_LOG_H__
#define __PLSCRIPTBINDINGS_PL_LOG_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Script/ScriptBinding.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class Log;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLScriptBindings {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Log script binding class
*/
class ScriptBinding_PL_Log : public PLCore::ScriptBinding {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(pl_rtti_export, ScriptBinding_PL_Log, "PLScriptBindings", PLCore::ScriptBinding, "Log script binding class")
		// Properties
		pl_properties
			pl_property("Namespace",	"PL.Log")
		pl_properties_end
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
		// Methods
		pl_method_1(OutputAlways,	pl_ret_type(void),	PLCore::String,	"Writes the given string into the log ('always' log level)",	"")
		pl_method_1(OutputCritical,	pl_ret_type(void),	PLCore::String,	"Writes the given string into the log ('critical' log level)",	"")
		pl_method_1(OutputError,	pl_ret_type(void),	PLCore::String,	"Writes the given string into the log ('error' log level)",		"")
		pl_method_1(OutputWarning,	pl_ret_type(void),	PLCore::String,	"Writes the given string into the log ('warning' log level)",	"")
		pl_method_1(OutputInfo,		pl_ret_type(void),	PLCore::String,	"Writes the given string into the log ('info' log level)",		"")
		pl_method_1(OutputDebug,	pl_ret_type(void),	PLCore::String,	"Writes the given string into the log ('debug' log level)",		"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI methods                                   ]
	//[-------------------------------------------------------]
	public:
		void OutputAlways(PLCore::String sText);
		void OutputCritical(PLCore::String sText);
		void OutputError(PLCore::String sText);
		void OutputWarning(PLCore::String sText);
		void OutputInfo(PLCore::String sText);
		void OutputDebug(PLCore::String sText);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		ScriptBinding_PL_Log();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ScriptBinding_PL_Log();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::Log *m_pLog;	/**< Log instance, always valid */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScriptBindings


#endif // __PLSCRIPTBINDINGS_PL_LOG_H__
