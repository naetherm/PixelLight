/*********************************************************\
 *  File: FrontendImpl.cpp                               *
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
#include "PLCore/Log/Log.h"
#include "PLCore/Base/Class.h"
#include "PLCore/Frontend/Frontend.h"
#include "PLCore/Frontend/FrontendContext.h"
#include "PLCore/Frontend/FrontendImpl.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(FrontendImpl)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
FrontendImpl::FrontendImpl() :
	m_pFrontend(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
FrontendImpl::~FrontendImpl()
{
}


//[-------------------------------------------------------]
//[ Protected virtual AbstractLifecycle functions         ]
//[-------------------------------------------------------]
void FrontendImpl::OnCreate()
{
	// Call virtual function from frontend
	if (m_pFrontend)
		m_pFrontend->OnCreate();
}

void FrontendImpl::OnRestart()
{
	// Call virtual function from frontend
	if (m_pFrontend)
		m_pFrontend->OnRestart();
}

bool FrontendImpl::OnStart()
{
	// Call virtual function from frontend
	return m_pFrontend ? m_pFrontend->OnStart() : true;
}

void FrontendImpl::OnResume()
{
	// Call virtual function from frontend
	if (m_pFrontend)
		m_pFrontend->OnResume();
}

void FrontendImpl::OnPause()
{
	// Call virtual function from frontend
	if (m_pFrontend)
		m_pFrontend->OnPause();
}

void FrontendImpl::OnStop()
{
	// Call virtual function from frontend
	if (m_pFrontend)
		m_pFrontend->OnStop();
}

void FrontendImpl::OnDestroy()
{
	// Call virtual function from frontend
	if (m_pFrontend)
		m_pFrontend->OnDestroy();
}


//[-------------------------------------------------------]
//[ Protected virtual AbstractFrontend functions          ]
//[-------------------------------------------------------]
void FrontendImpl::OnSize()
{
	// Call virtual function from frontend
	if (m_pFrontend)
		m_pFrontend->OnSize();
}

void FrontendImpl::OnFullscreenMode()
{
	// Call virtual function from frontend
	if (m_pFrontend)
		m_pFrontend->OnFullscreenMode();
}

void FrontendImpl::OnDraw()
{
	// Call virtual function from frontend
	if (m_pFrontend)
		m_pFrontend->OnDraw();
}

void FrontendImpl::OnUpdate()
{
	// Call virtual function from frontend
	if (m_pFrontend)
		m_pFrontend->OnUpdate();
}

void FrontendImpl::OnDrop(const Container<String> &lstFiles)
{
	// Call virtual function from frontend
	if (m_pFrontend)
		m_pFrontend->OnDrop(lstFiles);
}


//[-------------------------------------------------------]
//[ Protected virtual FrontendImpl functions              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the frontend is run
*/
void FrontendImpl::OnRun(const String &sExecutableFilename, const Array<String> &lstArguments)
{
	// Call virtual function from frontend
	if (m_pFrontend)
		m_pFrontend->OnRun(sExecutableFilename, lstArguments);
}

/**
*  @brief
*    Called when the frontend should run
*/
int FrontendImpl::Run(const String &sExecutableFilename, const Array<String> &lstArguments)
{
	// Nothing to do in here
	return 0;
}


//[-------------------------------------------------------]
//[ Protected static functions                            ]
//[-------------------------------------------------------]
/**
*  @brief
*    Creates a frontend instance
*/
Frontend *FrontendImpl::CreateFrontend(const FrontendContext &cFrontendContext, FrontendImpl &cFrontendImpl)
{
	// Get the frontend RTTI class
	const Class *pClass = ClassManager::GetInstance()->GetClass(cFrontendContext.GetFrontend());
	if (pClass && pClass->IsDerivedFrom("PLCore::Frontend")) {
		// Create the frontend RTTI class instance
		Object *pObject = nullptr;
		if (cFrontendContext.GetFrontendConstructor().GetLength())
			pObject = pClass->Create(cFrontendContext.GetFrontendConstructor(), "FrontendContext=\"" + Type<const FrontendContext&>::ConvertToString(cFrontendContext) + "\" FrontendImpl=\"" + Type<FrontendImpl&>::ConvertToString(cFrontendImpl) + "\" " + cFrontendContext.GetFrontendConstructorParameters());
		else
			pObject = pClass->Create(Params<Object*, const FrontendContext&, FrontendImpl&>(cFrontendContext, cFrontendImpl));

		// Do we now have an instance?
		if (pObject) {
			// Cast the pointer to a frontend pointer
			Frontend *pFrontend = static_cast<Frontend*>(pObject);

			// Write down a log message
			PL_LOG(Info, "Using frontend '" + pClass->GetClassName() + "': " + pClass->GetDescription())

			// Set parameters for the instanced frontend RTTI class
			if (cFrontendContext.GetFrontendParameters().GetLength())
				pObject->SetValues(cFrontendContext.GetFrontendParameters());

			// Done
			return pFrontend;
		} else {
			// Error!
			PL_LOG(Error, "Failed instancing frontend '" + pClass->GetClassName() + "': " + pClass->GetDescription())
		}
	} else {
		// Error!
		PL_LOG(Error, "Frontend '" + cFrontendContext.GetFrontend() + "' is no valid frontend RTTI class")
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Correct frontend position and size settings
*/
void FrontendImpl::CorrectPositionSize(int &nX, int &nY, uint32 &nWidth, uint32 &nHeight, int nScreenLeft, int nScreenTop, int nScreenWidth, int nScreenHeight, uint32 nMinWidth, uint32 nMinHeight)
{
	// The frontend position shouldn't leave the visible screen
	if (nX < nScreenLeft)
		nX = nScreenLeft;
	if (nY < nScreenTop)
		nY = nScreenTop;

	// The frontend position shouldn't be outside the visible screen
	if (nX > nScreenWidth-static_cast<int>(nMinHeight))
		nX = nScreenWidth-nMinHeight;
	if (nY > static_cast<int>(nScreenHeight-nMinHeight))
		nY = nScreenHeight-nMinHeight;

	// The width of the frontend shouldn't leave the visible screen
	int nDelta = (nX + nWidth) - nScreenWidth;
	if (nDelta > 0)
		nWidth = (nWidth-nDelta > 0) ? (nWidth-nDelta) : nMinHeight;

	// The height of the frontend shouldn't leave the visible screen
	nDelta = (nY + nHeight) - nScreenHeight;
	if (nDelta > 0)
		nHeight = (nHeight-nDelta > 0) ? (nHeight-nDelta) : nMinHeight;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
