/*********************************************************\
 *  File: FrontendOpenGL.h                               *
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


#ifndef __PLCORE_FRONTEND_OPENGL_H__
#define __PLCORE_FRONTEND_OPENGL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Frontend/Frontend.h"
#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Simple test frontend that uses OpenGL
*
*  @remarks
*    This is a test frontend which uses OpenGL to display a spinning colored
*    rectangle. It can be used easily to test the frontend integration when
*    developing a new backend.
*/
class FrontendOpenGL : public Frontend {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCORE_RTTI_EXPORT, FrontendOpenGL, "PLCore", PLCore::Frontend, "Simple test frontend that uses OpenGL")
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
		PLCORE_API FrontendOpenGL(const FrontendContext &cFrontendContext, FrontendImpl &cFrontendImpl);

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~FrontendOpenGL();


	//[-------------------------------------------------------]
	//[ Public virtual Frontend functions                     ]
	//[-------------------------------------------------------]
	public:
		PLCORE_API virtual bool IsRunning() const override;


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
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Initialize OpenGL
		*/
		void InitGL();

		/**
		*  @brief
		*    Resize GL scene to fit the current window size
		*/
		void ResizeGL();

		/**
		*  @brief
		*    Draw scene
		*/
		void DrawGL();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Platform specific
		#ifdef WIN32
			HDC   m_hDC;	/**< Device context, can be a null pointer */
			HGLRC m_hRC;	/**< OpenGL rendering context, can be a null pointer */
		#endif

		// Platform independent
		float m_fAngle;	/**< Current rotation angle of the rectangle */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


#endif // __PLCORE_FRONTEND_OPENGL_H__
