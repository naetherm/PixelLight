/*********************************************************\
 *  File: FrontendPixelLight.cpp                         *
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
#include "PLCore/Reflection/Class.h"
#include "PLCore/System/System.h"
#include "PLCore/Frontend/FrontendImpl.h"
#include "PLCore/Frontend/FrontendContext.h"
#include "PLCore/Frontend/FrontendApplication.h"
#include "PLCore/Frontend/FrontendPixelLight.h"


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_begin_class(FrontendPixelLight, PLCore)
	pl_base_class(PLCore::Frontend)
	pl_desc("PixelLight frontend")
	//pl_ctor(const PLCore::FrontendContext&, PLCore::FrontendImpl*)
	//	pl_desc("Parameter constructor. Frontend context this frontend is using as first parameter, frontend implementation this frontend is using as second parameter.")

	/*pl_property(ApplicationClass)
		pl_desc("Name of the frontend application RTTI class to use")
		pl_default(PLCore::String("Application"))

	pl_property(ApplicationConstructor)
		pl_desc("Name of the frontend application RTTI class constructor to use")

	pl_property(ApplicationConstructorParameters)
		pl_desc("Parameters for the frontend application RTTI class constructor")

	pl_property(ApplicationParameters)
		pl_desc("Parameters for the frontend application RTTI class instance")*/
	
pl_end_class()


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
FrontendPixelLight::FrontendPixelLight(const FrontendContext &cFrontendContext, FrontendImpl *pFrontendImpl) : Frontend(cFrontendContext, *pFrontendImpl),
	m_pFrontendApplication(nullptr),
	m_bFrontendApplicationInitialized(false)
{
}

/**
*  @brief
*    Destructor
*/
FrontendPixelLight::~FrontendPixelLight()
{
}


//[-------------------------------------------------------]
//[ Public virtual Frontend functions                     ]
//[-------------------------------------------------------]
bool FrontendPixelLight::IsRunning() const
{
	return (m_pFrontendApplication && m_bFrontendApplicationInitialized && m_pFrontendApplication->IsRunning());
}


//[-------------------------------------------------------]
//[ Protected virtual Frontend functions                  ]
//[-------------------------------------------------------]
void FrontendPixelLight::OnRun(const String &sExecutableFilename, const Array<String> &lstArguments)
{
	// Fill application context
	if (m_pFrontendApplication) {
		ApplicationContext &cApplicationContext = m_pFrontendApplication->m_cApplicationContext;
		cApplicationContext.SetStartupDirectory(System::GetInstance()->GetCurrentDir());
		cApplicationContext.SetExecutableFilename(sExecutableFilename);
		cApplicationContext.SetArguments(lstArguments);
	}
}


//[-------------------------------------------------------]
//[ Protected virtual AbstractLifecycle functions         ]
//[-------------------------------------------------------]
void FrontendPixelLight::OnCreate()
{
	PL_TODO(ananta, "Construction now works in a different way, adapt this code!")

	// Get the frontend application RTTI class
	//const Class *pClass = ClassManager::GetInstance()->GetClass(ApplicationClass.GetString());
	//if (pClass && pClass->IsDerivedFrom("PLCore::FrontendApplication")) {
	//	// Create the frontend application RTTI class instance
	//	if (ApplicationConstructor.GetString().GetLength())
	//		m_pFrontendApplication = static_cast<FrontendApplication*>(pClass->Create(ApplicationConstructor.GetString(), "Frontend=\"" + Type<Frontend&>::ConvertToString(*this) + "\" " + ApplicationConstructorParameters.GetString()));
	//	else
	//		m_pFrontendApplication = static_cast<FrontendApplication*>(pClass->Create(Params<Object*, Frontend&>(*this)));
	//	if (m_pFrontendApplication) {
	//		// Set parameters for the instanced frontend application RTTI class
	//		if (ApplicationParameters.GetString().GetLength())
	//			m_pFrontendApplication->SetValues(ApplicationParameters.GetString());

	//		// Do the frontend life cycle thing - let the world know that we have been created
	//		m_pFrontendApplication->OnCreate();
	//	}
	//}
}

void FrontendPixelLight::OnRestart()
{
	// Do the frontend life cycle thing
	if (m_pFrontendApplication)
		m_pFrontendApplication->OnRestart();
}

bool FrontendPixelLight::OnStart()
{
	// Do the frontend life cycle thing
	if (m_pFrontendApplication && m_pFrontendApplication->OnStart()) {
		// Frontend application successfully initialized (the following "OnInit()" is just application-specific, not framework specific)
		m_bFrontendApplicationInitialized = true;

		// Call application-specific initialization routine
		m_pFrontendApplication->OnInit();

		// Done
		return true;
	} else {
		// Error!
		return false;
	}
}

void FrontendPixelLight::OnResume()
{
	// Do the frontend life cycle thing
	if (m_pFrontendApplication)
		m_pFrontendApplication->OnResume();
}

void FrontendPixelLight::OnPause()
{
	// Do the frontend life cycle thing
	if (m_pFrontendApplication)
		m_pFrontendApplication->OnPause();
}

void FrontendPixelLight::OnStop()
{
	if (m_pFrontendApplication && m_bFrontendApplicationInitialized) {
		// Call application-specific de-initialization routine
		m_pFrontendApplication->OnDeInit();

		// Do the frontend life cycle thing
		m_pFrontendApplication->OnStop();

		// Frontend application is no longer initialized
		m_bFrontendApplicationInitialized = false;
	}
}

void FrontendPixelLight::OnDestroy()
{
	// Destroy the frontend application instance
	if (m_pFrontendApplication) {
		// Do the frontend life cycle thing - let the world know that we're going to die
		m_pFrontendApplication->OnDestroy();

		// Destroy the frontend application instance
		delete m_pFrontendApplication;
		m_pFrontendApplication = nullptr;
	}
}


//[-------------------------------------------------------]
//[ Protected virtual AbstractFrontend functions          ]
//[-------------------------------------------------------]
void FrontendPixelLight::OnSize()
{
	// Call virtual function from application (don't check "m_bFrontendApplicationInitialized" in here because it's valid that this method is called during the initialization process)
	if (m_pFrontendApplication)
		m_pFrontendApplication->OnSize();
}

void FrontendPixelLight::OnFullscreenMode()
{
	// Call virtual function from application (don't check "m_bFrontendApplicationInitialized" in here because it's valid that this method is called during the initialization process)
	if (m_pFrontendApplication)
		m_pFrontendApplication->OnFullscreenMode();
}

void FrontendPixelLight::OnDraw()
{
	// Call virtual function from application
	if (m_pFrontendApplication && m_bFrontendApplicationInitialized)
		m_pFrontendApplication->OnDraw();
}

void FrontendPixelLight::OnUpdate()
{
	// Call virtual function from application
	if (m_pFrontendApplication && m_bFrontendApplicationInitialized)
		m_pFrontendApplication->OnUpdate();
}

void FrontendPixelLight::OnDrop(const Container<String> &lstFiles)
{
	// Call virtual function from application
	if (m_pFrontendApplication && m_bFrontendApplicationInitialized)
		m_pFrontendApplication->OnDrop(lstFiles);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
