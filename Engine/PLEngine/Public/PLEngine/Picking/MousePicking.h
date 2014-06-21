/*********************************************************\
 *  File: MousePicking.h                                 *
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


#ifndef __PLENGINE_MOUSEPICKING_H__
#define __PLENGINE_MOUSEPICKING_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLEngine/Picking/Picking.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class Frontend;
}
namespace PLMath {
	class Vector2i;
}
namespace PLScene {
	class SNCamera;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLEngine {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Class offering scene picking functionality by using the mouse
*/
class MousePicking : public Picking {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cFrontend
		*    Frontend instance used to determine the mouse position
		*  @param[in] pCamera
		*    Camera to perform the picking with, if a null pointer no picking is possible
		*/
		PL_API MousePicking(PLCore::Frontend &cFrontend, PLScene::SNCamera *pCamera = nullptr);

		/**
		*  @brief
		*    Destructor
		*/
		PL_API virtual ~MousePicking();

		/**
		*  @brief
		*    Returns the frontend instance used to determine the mouse position
		*
		*  @return
		*    The frontend instance used to determine the mouse position
		*/
		PL_API PLCore::Frontend &GetFrontend() const;

		/**
		*  @brief
		*    Returns the camera to perform the picking with
		*
		*  @return
		*    The camera to perform the picking with, if a null pointer no picking is possible
		*/
		PL_API PLScene::SNCamera *GetCamera() const;

		/**
		*  @brief
		*    Sets the camera to perform the picking with
		*
		*  @param[in] pCamera
		*    The camera to perform the picking with, if a null pointer no picking is possible
		*/
		PL_API void SetCamera(PLScene::SNCamera *pCamera);

		/**
		*  @brief
		*    Performs picking by using the current camera and current mouse position within the widget
		*
		*  @param[out] cPickingResult
		*    Receives the picking result if all went fine
		*  @param[in]  fMaxDistance
		*    Maximum picking distance, if negative there's no maximum picking distance (in the same scene container the camera is in)
		*  @param[in]  nCull
		*    Cull mode (see "PLRenderer::Cull")
		*
		*  @return
		*    'true' if anything has been picked, else 'false'
		*
		*  @note
		*    - For performance reasons we recommend to always set a maximum picking distance
		*    - The function will only work if a widget and camera is set ('SetWidget()' and 'GetCamera()')
		*/
		PL_API bool PerformMousePicking(PickingResult &cPickingResult, float fMaxDistance = -1.0f, PLRenderer::Cull::Enum nCull = PLRenderer::Cull::CCW);

		/**
		*  @brief
		*    Performs picking by using the current camera and the given mouse position
		*
		*  @param[out] cPickingResult
		*    Receives the picking result if all went fine
		*  @param[in]  vMousePos
		*    Position of the mouse inside the widget the picking takes place
		*  @param[in]  fMaxDistance
		*    Maximum picking distance, if negative there's no maximum picking distance (in the same scene container the camera is in)
		*  @param[in]  nCull
		*    Cull mode (see "PLRenderer::Cull")
		*
		*  @return
		*    'true' if anything has been picked, else 'false'
		*
		*  @note
		*    - For performance reasons we recommend to always set a maximum picking distance
		*    - The function will only work if a widget and camera is set ('SetWidget()' and 'GetCamera()')
		*/
		PL_API bool PerformMousePicking(PickingResult &cPickingResult, const PLMath::Vector2i &vMousePos, float fMaxDistance = -1.0f, PLRenderer::Cull::Enum nCull = PLRenderer::Cull::CCW);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when a camera was destroyed
		*/
		void OnCameraDestroy();


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<> EventHandlerCameraDestroy;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::Frontend  *m_pFrontend;	/**< Frontend instance used to determine the mouse position, always valid! */
		PLScene::SNCamera *m_pCamera;	/**< Camera to perform the picking with, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine


#endif // __PLENGINE_MOUSEPICKING_H__
