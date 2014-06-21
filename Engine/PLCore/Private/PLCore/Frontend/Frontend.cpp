/*********************************************************\
 *  File: Frontend.cpp                                   *
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
#include "PLCore/Runtime.h"
#include "PLCore/Log/Log.h"
#include "PLCore/Base/Class.h"
#include "PLCore/System/System.h"
#include "PLCore/Frontend/FrontendImpl.h"
#include "PLCore/Frontend/FrontendContext.h"
#include "PLCore/Frontend/Frontend.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(Frontend)


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Run the frontend
*/
int Frontend::Run(const FrontendContext &cFrontendContext, bool bUrgentMessageAllowed)
{
	int nResult = -1;	// Error by default

	// Create a frontend implementation instance
	FrontendImpl *pFrontendImpl = CreateFrontendImplementation(cFrontendContext);
	if (pFrontendImpl) {
		// Create a frontend instance
		Frontend *pFrontend = FrontendImpl::CreateFrontend(cFrontendContext, *pFrontendImpl);
		if (pFrontend) {
			// Do the frontend life cycle thing - let the world know that we have been created
			pFrontendImpl->OnCreate();

			// Let the world know that this frontend is now going to run
			pFrontendImpl->OnRun(cFrontendContext.GetExecutableFilename(), cFrontendContext.GetArguments());

			// Let the frontend implementation run
			nResult = pFrontendImpl->Run(cFrontendContext.GetExecutableFilename(), cFrontendContext.GetArguments());

			// Do the frontend life cycle thing - let the world know that we're going to die
			pFrontendImpl->OnDestroy();

			// Destroy the frontend
			delete pFrontend;
		} else {
			// Error!
			if (bUrgentMessageAllowed)
				System::GetInstance()->UrgentMessage("Failed to create the frontend instance (\"" + cFrontendContext.GetFrontend() + "\")");
		}

		// Destroy the frontend implementation
		delete pFrontendImpl;
	} else {
		// Error!
		if (bUrgentMessageAllowed)
			System::GetInstance()->UrgentMessage("Failed to create the frontend implementation instance \"" + cFrontendContext.GetFrontendImplementation() + "\".\n\n" \
												 "-> Is the PixelLight runtime registered correctly? (see e.g. \"PixelLightBuild.pdf\" for details)\n" \
												 "-> If yes, is there a RTTI class with the given name?\n" \
												 "-> If yes, if the RTTI class within a plugin, is this plugin active and is it possible to load it?");
	}

	// Done
	return nResult;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Frontend::Frontend(const FrontendContext &cFrontendContext, FrontendImpl &cFrontendImpl) :
	m_cFrontendContext(cFrontendContext),
	m_pFrontendImpl(&cFrontendImpl)
{
	// Set frontend
	m_pFrontendImpl->m_pFrontend = this;
}

/**
*  @brief
*    Destructor
*/
Frontend::~Frontend()
{
}


//[-------------------------------------------------------]
//[ Protected virtual Frontend functions                  ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the frontend is run
*/
void Frontend::OnRun(const String &sExecutableFilename, const Array<String> &lstArguments)
{
	// No default implementation
}


//[-------------------------------------------------------]
//[ Private static functions                              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Creates a implementation frontend instance
*/
FrontendImpl *Frontend::CreateFrontendImplementation(const FrontendContext &cFrontendContext)
{
	// Get the frontend implementation RTTI class
	const Class *pClass = ClassManager::GetInstance()->GetClass(cFrontendContext.GetFrontendImplementation());
	if (pClass && pClass->IsDerivedFrom("PLCore::FrontendImpl")) {
		// Create the frontend RTTI class instance
		Object *pObject = cFrontendContext.GetFrontendImplementationConstructor().GetLength() ? pClass->Create(cFrontendContext.GetFrontendImplementationConstructor(), cFrontendContext.GetFrontendImplementationConstructorParameters()) : pClass->Create();
		if (pObject) {
			// Write down a log message
			PL_LOG(Info, "Using frontend implementation '" + pClass->GetClassName() + "': " + pClass->GetDescription())

			// Set parameters for the instanced frontend implementation RTTI class
			if (cFrontendContext.GetFrontendImplementationParameters().GetLength())
				pObject->SetValues(cFrontendContext.GetFrontendImplementationParameters());

			// Done
			return static_cast<FrontendImpl*>(pObject);
		} else {
			// Error!
			PL_LOG(Error, "Failed instancing frontend implementation '" + pClass->GetClassName() + "': " + pClass->GetDescription())
		}
	} else {
		// Error!
		PL_LOG(Error, "Frontend '" + cFrontendContext.GetFrontendImplementation() + "' is no valid frontend implementation RTTI class")
	}

	// Error!
	return nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
