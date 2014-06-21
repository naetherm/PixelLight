/*********************************************************\
 *  File: ModBlend.h                                     *
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


#ifndef __PLGUI_MODBLEND_H__
#define __PLGUI_MODBLEND_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLGui/Modifiers/Modifier.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Timer;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Modifier that blends a widget in and out according to the mouse cursor
*/
class ModBlend : public Modifier {


	//[-------------------------------------------------------]
	//[ Class definition                                      ]
	//[-------------------------------------------------------]
	pl_class(PLGUI_RTTI_EXPORT, ModBlend, "PLGui", PLGui::Modifier, "Modifier that blends a widget in and out according to the mouse cursor")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
		// Slots
		pl_slot_0(OnTimer,	"Timer callback",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLGUI_API ModBlend();

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~ModBlend();

		/**
		*  @brief
		*    Get blend animation time
		*
		*  @return
		*    Time in milliseconds for the blend animation
		*/
		PLGUI_API PLCore::uint64 GetBlendTime() const;

		/**
		*  @brief
		*    Set blend animation time
		*
		*  @param[in] nBlendTime
		*    Time in milliseconds for the blend animation
		*/
		PLGUI_API void SetBlendTime(PLCore::uint64 nBlendTime);


	//[-------------------------------------------------------]
	//[ Protected virtual Modifier functions                  ]
	//[-------------------------------------------------------]
	protected:
		virtual void OnAttach(Widget &cWidget) override;
		virtual void OnDetach(Widget &cWidget) override;


	//[-------------------------------------------------------]
	//[ Protected virtual WidgetFunctions functions           ]
	//[-------------------------------------------------------]
	protected:
		virtual void OnMouseOver(bool bMouseOver) override;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Called when the timer has fired
		*/
		void OnTimer();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLCore::uint64  m_nTimeout;		/**< Timeout for blend animation (in milliseconds) */
		float		    m_fTrans;		/**< Current transparency (0.0f - 1.0f) */
		float		    m_fTransDest;	/**< Destination transparency (0.0f - 1.0f) */
		Timer		   *m_pTimer;		/**< Timer used for fading */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_MODBLEND_H__
