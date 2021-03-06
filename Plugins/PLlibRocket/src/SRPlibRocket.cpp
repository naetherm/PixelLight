/*********************************************************\
 *  File: SRPlibRocket.cpp                               *
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
#include <Rocket/Core.h>
#include "PLlibRocket/RenderInterfacePL.h"
#include "PLlibRocket/SRPlibRocket.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLRenderer;
using namespace PLScene;
namespace PLlibRocket {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SRPlibRocket, "PLlibRocket", PLScene::SceneRendererPass, "libRocket scene renderer pass")
	// Constructors
	pl_constructor_2_metadata(ParameterConstructor,	Rocket::Core::Context&,	RenderInterfacePL&,	"Parameter constructor",	"")
pl_class_metadata_end(SRPlibRocket)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
SRPlibRocket::SRPlibRocket(Rocket::Core::Context &cRocketContext, RenderInterfacePL &cRenderInterfacePL) :
	m_pRocketContext(&cRocketContext),
	m_pRenderInterfacePL(&cRenderInterfacePL)
{
}

/**
*  @brief
*    Destructor
*/
SRPlibRocket::~SRPlibRocket()
{
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneRendererPass functions  ]
//[-------------------------------------------------------]
void SRPlibRocket::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Update libRocket
	m_pRocketContext->Update();

	// Configures PixelLight's rendering system for rendering Rocket
	if (m_pRenderInterfacePL->ConfigureRenderSystem()) {
		// Render libRocket
		m_pRocketContext->Render();
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLlibRocket
