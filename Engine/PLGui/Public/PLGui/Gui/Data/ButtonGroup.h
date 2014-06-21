/*********************************************************\
 *  File: ButtonGroup.h                                  *
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


#ifndef __PLGUI_BUTTONGROUP_H__
#define __PLGUI_BUTTONGROUP_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Event/Event.h>
#include "PLGui/PLGui.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
class AbstractToggleButton;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Manages a button group
*/
class ButtonGroup {


	//[-------------------------------------------------------]
	//[ Public events                                         ]
	//[-------------------------------------------------------]
	public:
		PLCore::Event<AbstractToggleButton*> EventActivate;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLGUI_API ButtonGroup();

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~ButtonGroup();

		/**
		*  @brief
		*    Check if the button activation is exclusive
		*
		*  @return
		*    'true', if the button activation is exclusive, else 'false'
		*
		*  @remarks
		*    If the button activation is exclusive, only one button can be active at the same time
		*/
		PLGUI_API bool IsExclusive() const;

		/**
		*  @brief
		*    Set if the button activation is exclusive
		*
		*  @param[in] bExclusive
		*    'true', if the button activation is exclusive, else 'false'
		*/
		PLGUI_API void SetExclusive(bool bExclusive);

		/**
		*  @brief
		*    Activates a certain button
		*
		*  @param[in] cButton
		*    Button that is being activated
		*/
		PLGUI_API void Activate(AbstractToggleButton &cButton);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		bool				  m_bExclusive;	/**< If 'true', only one button can be activated at the same time */
		AbstractToggleButton *m_pButton;	/**< Button that is currently activated */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_BUTTONGROUP_H__
