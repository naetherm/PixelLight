/*********************************************************\
 *  File: SNConsole.h                                    *
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


#ifndef __PLENGINE_COMPOSITING_STANDARDCONSOLE_H__
#define __PLENGINE_COMPOSITING_STANDARDCONSOLE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]


// [TODO] Use FS own functions for this if implemented...
#ifdef WIN32
	#include <PLCore/+Windows/PLCoreWindowsIncludes.h>
#endif


#include <PLRenderer/Material/MaterialHandler.h>
#include "PLEngine/Compositing/Console/SNConsoleBase.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLEngine {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Standard console implementation
*/
class SNConsole : public SNConsoleBase {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const PLCore::String StandardMaterial; /**< Standard console material */


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PL_RTTI_EXPORT, SNConsole, "PLEngine", PLEngine::SNConsoleBase, "Standard console implementation")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PL_API SNConsole();

		/**
		*  @brief
		*    Destructor
		*/
		PL_API virtual ~SNConsole();

		// Get
		PL_API float GetPos() const;
		PL_API float GetMaxPos() const;
		PL_API PLRenderer::Material *GetMaterial();

		// Set
		PL_API void SetPos(float fNewPos);
		PL_API void SetMaxPos(float fMax);
		PL_API void SetAlpha(float fNewAlpha);

		// Overridden virtual
		PL_API PLCore::String GetDescription() const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when the scene node needs to be updated
		*/
		void OnUpdate();


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<> EventHandlerUpdate;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		int							m_nRow;			/**< Current log row */
		PLCore::uint32				m_nLastRow;		/**< Last log row */
		float						m_fPos;			/**< Y position (between 0 and 1) */
		float						m_fMaxPos;		/**< Maximum position (between 0..1) */
		float						m_fAlpha;		/**< Transparency (255 = full opacity) */
		PLRenderer::MaterialHandler	m_cMaterial;	/**< Console material */


		// [TODO] Use FS own functions for this if implemented...
		// Log view
		#ifdef WIN32
			FILETIME  m_LastTime;
			DWORD	  m_nLastFileSizeHigh;
			DWORD	  m_nLastFileSizeLow;
		#endif


		PLCore::Array<PLCore::String> m_lstLogLines;	/**< Log lines */


	//[-------------------------------------------------------]
	//[ Public virtual PLScene::SceneNode functions           ]
	//[-------------------------------------------------------]
	public:
		PL_API virtual void DrawPost(PLRenderer::Renderer &cRenderer, const PLScene::VisNode *pVisNode = nullptr) override;


	//[-------------------------------------------------------]
	//[ Private virtual PLScene::SceneNode functions          ]
	//[-------------------------------------------------------]
	private:
		virtual void InitFunction() override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine


#endif // __PLENGINE_COMPOSITING_STANDARDCONSOLE_H__
