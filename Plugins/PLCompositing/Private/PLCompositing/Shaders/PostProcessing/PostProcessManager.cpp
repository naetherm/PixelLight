/*********************************************************\
 *  File: PostProcessManager.cpp                         *
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
#include <PLCore/Tools/Loader.h>
#include "PLCompositing/Shaders/PostProcessing/PostProcess.h"
#include "PLCompositing/Shaders/PostProcessing/PostProcessLoader.h"
#include "PLCompositing/Shaders/PostProcessing/PostProcessManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
namespace PLCompositing {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
PostProcessManager::PostProcessManager(RendererContext &cRendererContext) :
	m_pRendererContext(&cRendererContext),
	m_nTextureFormat(TextureBuffer::R8G8B8A8)
{
}

/**
*  @brief
*    Destructor
*/
PostProcessManager::~PostProcessManager()
{
}

/**
*  @brief
*    Returns the used renderer context
*/
RendererContext &PostProcessManager::GetRendererContext() const
{
	return *m_pRendererContext;
}

/**
*  @brief
*    Returns the texture format
*/
TextureBuffer::EPixelFormat PostProcessManager::GetTextureFormat() const
{
	return m_nTextureFormat;
}

/**
*  @brief
*    Sets the texture format
*/
void PostProcessManager::SetTextureFormat(TextureBuffer::EPixelFormat nFormat)
{
	m_nTextureFormat = nFormat;
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Loadable functions             ]
//[-------------------------------------------------------]
bool PostProcessManager::Unload()
{
	// Remove all post processes
	Clear();

	// Reset texture format
	m_nTextureFormat = TextureBuffer::R8G8B8A8;

	// Call base implementation
	return Loadable::Unload();
}

String PostProcessManager::GetLoadableTypeName() const
{
	static const String sString = "PostProcess";
	return sString;
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::Loadable functions            ]
//[-------------------------------------------------------]
bool PostProcessManager::CallLoadable(File &cFile, Loader &cLoader, const String &sMethod, const String &sParams)
{
	// Get the loader implementation
	LoaderImpl *pLoaderImpl = cLoader.GetImpl();
	if (pLoaderImpl) {
		// Load
		if (sParams.GetLength()) {
			pLoaderImpl->CallMethod(sMethod, "Param0=\"" + Type<PostProcessManager&>::ConvertToString(*this) + "\" Param1=\"" + Type<File&>::ConvertToString(cFile) + "\" " + sParams);
			return true;
		} else {
			Params<bool, PostProcessManager&, File&> cParams(*this, cFile);
			pLoaderImpl->CallMethod(sMethod, cParams);
			return cParams.Return;
		}
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Private virtual PLCore::ElementManager functions      ]
//[-------------------------------------------------------]
PostProcess *PostProcessManager::CreateElement(const String &sName)
{
	return new PostProcess(sName, *this);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing
