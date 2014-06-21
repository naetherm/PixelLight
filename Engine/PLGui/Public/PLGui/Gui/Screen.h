/*********************************************************\
 *  File: Screen.h                                       *
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


#ifndef __PLGUI_SCREEN_H__
#define __PLGUI_SCREEN_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/String/String.h>
#include <PLMath/Vector2i.h>
#include "PLGui/PLGui.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Gui;
class Widget;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Screen class
*
*  @remarks
*    This class represents a physical or logical screen (e.g. a monitor device)
*/
class Screen {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Gui;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pGui
		*    Pointer to the owner GUI
		*/
		PLGUI_API Screen(Gui *pGui);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API ~Screen();

		/**
		*  @brief
		*    Get owner GUI
		*
		*  @return
		*    GUI
		*/
		PLGUI_API Gui *GetGui() const;

		/**
		*  @brief
		*    Get screen name
		*
		*  @return
		*    Name of screen (this depends on the system)
		*/
		PLGUI_API PLCore::String GetName() const;

		/**
		*  @brief
		*    Set screen name
		*
		*  @param[in] sName
		*    Name of screen
		*/
		PLGUI_API void SetName(const PLCore::String &sName);

		/**
		*  @brief
		*    Get screen position
		*
		*  @return
		*    Screen position (upper/left corner)
		*/
		PLGUI_API PLMath::Vector2i GetPos() const;

		/**
		*  @brief
		*    Set screen position
		*
		*  @param[in] vPos
		*    Screen size (x, y)
		*/
		PLGUI_API void SetPos(const PLMath::Vector2i &vPos);

		/**
		*  @brief
		*    Get screen size
		*
		*  @return
		*    Screen size (width, height)
		*/
		PLGUI_API PLMath::Vector2i GetSize() const;

		/**
		*  @brief
		*    Set screen size
		*
		*  @param[in] vSize
		*    Screen size (width, height)
		*/
		PLGUI_API void SetSize(const PLMath::Vector2i &vSize);

		/**
		*  @brief
		*    Check if this is the default screen
		*
		*  @return
		*    'true', if the screen is the default screen
		*/
		PLGUI_API bool IsDefault() const;

		/**
		*  @brief
		*    Set if this is the default screen
		*
		*  @param[in] bDefault
		*    'true', if the screen is the default screen
		*/
		PLGUI_API void SetDefault(bool bDefault);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Gui				 *m_pGui;			/**< Pointer to GUI instance */
		mutable Widget	 *m_pDesktopWidget;	/**< Desktop widget */
		bool			  m_bDefault;		/**< 'true' if the screen is the default screen */
		PLCore::String	  m_sName;			/**< Screen name */
		PLMath::Vector2i  m_vPos;			/**< Screen position */
		PLMath::Vector2i  m_vSize;			/**< Screen size */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_SCREEN_H__
