/*********************************************************\
 *  File: FrontendOpenGL.cpp                             *
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


// [TODO] Remove those libs if you don't use this frontend test (MS Windows)
// opengl32.lib
// glu32.lib


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Frontend/FrontendImpl.h"
#include "PLCore/Frontend/FrontendOpenGL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(FrontendOpenGL, "PLCore", PLCore::Frontend, "Simple test frontend that uses OpenGL")
	// Constructors
	pl_constructor_2_metadata(ParameterConstructor,	const FrontendContext&, FrontendImpl&,	"Parameter constructor. Frontend context this frontend is using as first parameter, frontend implementation this frontend is using as second parameter.",	"")
pl_class_metadata_end(FrontendOpenGL)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
FrontendOpenGL::FrontendOpenGL(const FrontendContext &cFrontendContext, FrontendImpl &cFrontendImpl) : Frontend(cFrontendContext, cFrontendImpl),
	#ifdef WIN32
		m_hDC(nullptr),
		m_hRC(nullptr),
	#endif
	m_fAngle(0.0f)
{
	// Nothing to do in here
}

/**
*  @brief
*    Destructor
*/
FrontendOpenGL::~FrontendOpenGL()
{
	// Nothing to do in here
}


//[-------------------------------------------------------]
//[ Public virtual Frontend functions                     ]
//[-------------------------------------------------------]
bool FrontendOpenGL::IsRunning() const
{
	// The frontend is running
	return true;
}


//[-------------------------------------------------------]
//[ Protected virtual AbstractLifecycle functions         ]
//[-------------------------------------------------------]
void FrontendOpenGL::OnCreate()
{
	// Nothing to do in here
}

void FrontendOpenGL::OnRestart()
{
	// Nothing to do in here
}

bool FrontendOpenGL::OnStart()
{
	#ifdef WIN32
		// Get the native window handle
		const HWND hWnd = reinterpret_cast<HWND>(GetImpl()->GetNativeWindowHandle());
		if (hWnd) {
			// Get device context
			m_hDC = GetDC(hWnd);
			if (m_hDC) {
				// Set up pixel format for OpenGL window
				static const PIXELFORMATDESCRIPTOR pfd = {
					sizeof(PIXELFORMATDESCRIPTOR),	// Size Of This Pixel Format Descriptor
					1,								// Version Number
					PFD_DRAW_TO_WINDOW |			// Format Must Support Window
					PFD_SUPPORT_OPENGL |			// Format Must Support OpenGL
					PFD_DOUBLEBUFFER,				// Must Support Double Buffering
					PFD_TYPE_RGBA,					// Request An RGBA Format
					32,								// Select Our Color Depth
					0, 0, 0, 0, 0, 0,				// Color Bits Ignored
					0,								// No Alpha Buffer
					0,								// Shift Bit Ignored
					0,								// No Accumulation Buffer
					0, 0, 0, 0,						// Accumulation Bits Ignored
					16,								// 16Bit Z-Buffer (Depth Buffer)
					0,								// No Stencil Buffer
					0,								// No Auxiliary Buffer
					PFD_MAIN_PLANE,					// Main Drawing Layer
					0,								// Reserved
					0, 0, 0							// Layer Masks Ignored
				};

				// Set pixel format
				int nPixelFormat = ChoosePixelFormat(m_hDC, &pfd);
				SetPixelFormat(m_hDC, nPixelFormat, &pfd);

				// Create rendering context
				m_hRC = wglCreateContext(m_hDC);
				wglMakeCurrent(m_hDC, m_hRC);

				// Initialize OpenGL window
				ResizeGL();
				InitGL();
			}
		}
	#endif

	// Done
	return true;
}

void FrontendOpenGL::OnResume()
{
	// Nothing to do in here
}

void FrontendOpenGL::OnPause()
{
	// Nothing to do in here
}

void FrontendOpenGL::OnStop()
{
	#ifdef WIN32
		// Release rendering context
		if (m_hRC) {
			wglMakeCurrent(nullptr, nullptr);
			wglDeleteContext(m_hRC);
			m_hRC = nullptr;
		}

		// Release device context
		if (m_hDC) {
			// Get the native window handle
			const HWND hWnd = reinterpret_cast<HWND>(GetImpl()->GetNativeWindowHandle());
			if (hWnd)
				ReleaseDC(hWnd, m_hDC);

			// Reset device context pointer
			m_hDC = nullptr;
		}
	#endif
}

void FrontendOpenGL::OnDestroy()
{
	// Nothing to do in here
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Initialize OpenGL
*/
void FrontendOpenGL::InitGL()
{
	// Set up OpenGL rendering modes
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

/**
*  @brief
*    Resize GL scene to fit the current window size
*/
void FrontendOpenGL::ResizeGL()
{
	// Is there a valid render context?
	if (m_hRC) {
		// Get size
		const uint32 nWidth  = GetWidth();
		const uint32 nHeight = GetHeight();
		if (nWidth > 0 && nHeight > 0) {
			// Set viewport
			glViewport(0, 0, nWidth, nHeight);

			// Set projection matrix
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(45.0f, static_cast<GLfloat>(nWidth) /static_cast<GLfloat>(nHeight), 0.1f, 100.0f);

			// Switch back to model/view matrix
			glMatrixMode(GL_MODELVIEW);
		}
	}
}

/**
*  @brief
*    Draw scene
*/
void FrontendOpenGL::DrawGL()
{
	// Is there a valid render context?
	if (m_hRC) {
		// Set scale factor for our rectangle
		static float fScale = 0.6f;

		// Clear buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Rotate and scale view
		glLoadIdentity();
		glRotatef(m_fAngle, 0.0f, 0.0f, 1.0f);
		glScalef(fScale, fScale, 1.0f);

		// Draw a colored rectangle
		glBegin(GL_QUADS);
			// 0
			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex3f(-1.0f, -1.0f, -1.0f);

			// 1
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(-1.0f,  1.0f, -1.0f);

			// 2
			glColor3f(0.0f, 0.0f, 0.0f);
			glVertex3f( 1.0f,  1.0f, -1.0f);

			// 3
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f( 1.0f, -1.0f, -1.0f);
		glEnd();
	}
}


//[-------------------------------------------------------]
//[ Protected virtual AbstractFrontend functions          ]
//[-------------------------------------------------------]
void FrontendOpenGL::OnSize()
{
	// Adjust size of scene
	ResizeGL();
}

void FrontendOpenGL::OnFullscreenMode()
{
	// Nothing to do in here
}

void FrontendOpenGL::OnDraw()
{
	// Draw OpenGL scene
	DrawGL();

	// Swap buffers (double buffering)
	#ifdef WIN32
		if (m_hDC)
			SwapBuffers(m_hDC);
	#endif

	// Issue a redraw request
	Redraw();
}

void FrontendOpenGL::OnUpdate()
{
	// Rotate our rectangle
	m_fAngle += 0.1f;	// To keep things simple in here, no frame rate independent timing
	if (m_fAngle >= 360.0f)
		m_fAngle -= 360.0f;
}

void FrontendOpenGL::OnDrop(const Container<String> &lstFiles)
{
	// Nothing to do in here
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore
