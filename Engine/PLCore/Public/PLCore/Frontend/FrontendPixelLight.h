/*********************************************************\
 *  File: FrontendPixelLight.h                           *
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


#ifndef __PLCORE_FRONTEND_PIXELLIGHT_H__
#define __PLCORE_FRONTEND_PIXELLIGHT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Frontend/Frontend.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class FrontendApplication;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    PixelLight frontend
*
*  @remarks
*    This frontend controls an embedded PixelLight frontend application, meaning application classes
*    derived from "FrontendApplication". Just think of this frontend class as a puppet master
*    (puppet = application class) controlled by another puppet master (the frontend implementation,
*    e.g. a simple native OS window or a browser such as MS Internet Explorer or Mozilla Firefox).
*/
class FrontendPixelLight : public Frontend {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCORE_RTTI_EXPORT, FrontendPixelLight, "PLCore", PLCore::Frontend, "PixelLight frontend")
		// Attributes
		pl_attribute(ApplicationClass,					String,	"Application",	ReadWrite,	DirectValue,	"Name of the frontend application RTTI class to use",				"")
		pl_attribute(ApplicationConstructor,			String,	"",				ReadWrite,	DirectValue,	"Name of the frontend application RTTI class constructor to use",	"")
		pl_attribute(ApplicationConstructorParameters,	String,	"",				ReadWrite,	DirectValue,	"Parameters for the frontend application RTTI class constructor",	"")
		pl_attribute(ApplicationParameters,				String,	"",				ReadWrite,	DirectValue,	"Parameters for the frontend application RTTI class instance",		"")
		#ifdef PLCORE_EXPORTS	// The following is only required when compiling PLCore
			// Constructors
			pl_constructor_2(ParameterConstructor,	const FrontendContext&, FrontendImpl&,	"Parameter constructor. Frontend context this frontend is using as first parameter, frontend implementation this frontend is using as second parameter.",	"")
		#endif
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cFrontendContext
		*    Frontend context to use (just shared, the given instance must stay valid as long as this frontend lives)
		*  @param[in] cFrontendImpl
		*    Frontend implementation instance
		*/
		PLCORE_API FrontendPixelLight(const FrontendContext &cFrontendContext, FrontendImpl &cFrontendImpl);

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~FrontendPixelLight();


	//[-------------------------------------------------------]
	//[ Public virtual Frontend functions                     ]
	//[-------------------------------------------------------]
	public:
		PLCORE_API virtual bool IsRunning() const override;


	//[-------------------------------------------------------]
	//[ Protected virtual Frontend functions                  ]
	//[-------------------------------------------------------]
	protected:
		PLCORE_API virtual void OnRun(const String &sExecutableFilename, const Array<String> &lstArguments);


	//[-------------------------------------------------------]
	//[ Protected virtual AbstractLifecycle functions         ]
	//[-------------------------------------------------------]
	protected:
		PLCORE_API virtual void OnCreate() override;
		PLCORE_API virtual void OnRestart() override;
		PLCORE_API virtual bool OnStart() override;
		PLCORE_API virtual void OnResume() override;
		PLCORE_API virtual void OnPause() override;
		PLCORE_API virtual void OnStop() override;
		PLCORE_API virtual void OnDestroy() override;


	//[-------------------------------------------------------]
	//[ Protected virtual AbstractFrontend functions          ]
	//[-------------------------------------------------------]
	protected:
		PLCORE_API virtual void OnSize() override;
		PLCORE_API virtual void OnFullscreenMode() override;
		PLCORE_API virtual void OnDraw() override;
		PLCORE_API virtual void OnUpdate() override;
		PLCORE_API virtual void OnDrop(const Container<String> &lstFiles) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		FrontendApplication *m_pFrontendApplication;			/**< Frontend application instance, can be a null pointer */
		bool				 m_bFrontendApplicationInitialized;	/**< Frontend application successfully initialized? */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_FRONTEND_PIXELLIGHT_H__
