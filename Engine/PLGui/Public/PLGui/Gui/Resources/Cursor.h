/*********************************************************\
 *  File: Cursor.h                                       *
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


#ifndef __PLGUI_CURSOR_H__
#define __PLGUI_CURSOR_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/String/String.h>
#include "PLGui/PLGui.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLMath {
	class Vector2i;
}
namespace PLGui {
	class Gui;
	class CursorImpl;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGui {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Cursor class
*
*  @note
*    - Implementation of the bridge design pattern, this class is the abstraction
*/
class Cursor {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cGui
		*    Owner GUI
		*
		*  @remarks
		*    Loads the standard mouse cursor
		*/
		PLGUI_API Cursor(Gui &cGui);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cGui
		*    Owner GUI
		*  @param[in] nCursor
		*    Cursor ID
		*
		*  @remarks
		*    Loads one of the predefined mouse cursors. If you pass CursorCustom here, it will be ignored
		*    and the standard mouse cursor will be used instead. To load a custom cursor, see the other
		*    constructor and pass a filename.
		*/
		PLGUI_API Cursor(Gui &cGui, EMouseCursor nCursor);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cGui
		*    Owner GUI
		*  @param[in] sFilename
		*    Name of image file
		*  @param[in] vHotspot
		*    Hotspot of cursor
		*
		*  @remarks
		*    Loads a custom cursor from an image. The mouse cursor ID will be set to CursorCustom.
		*/
		PLGUI_API Cursor(Gui &cGui, const PLCore::String &sFilename, const PLMath::Vector2i &vHotspot);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cCursor
		*    Cursor that is copied
		*/
		PLGUI_API Cursor(const Cursor &cCursor);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API ~Cursor();

		/**
		*  @brief
		*    Assignment operator
		*
		*  @param[in] cCursor
		*    Cursor that is copied
		*/
		PLGUI_API Cursor &operator =(const Cursor &cCursor);

		/**
		*  @brief
		*    Get owner GUI
		*
		*  @return
		*    Pointer to GUI object (never a null pointer)
		*/
		PLGUI_API Gui *GetGui() const;

		/**
		*  @brief
		*    Get implementation
		*
		*  @return
		*    Pointer to platform specific implementation
		*/
		PLGUI_API CursorImpl *GetImpl() const;

		/**
		*  @brief
		*    Load cursor
		*
		*  @param[in] nCursor
		*    Cursor ID
		*/
		PLGUI_API void Load(EMouseCursor nCursor);

		/**
		*  @brief
		*    Load cursor
		*
		*  @param[in] sFilename
		*    Name of image file
		*  @param[in] vHotspot
		*    Hotspot of cursor
		*/
		PLGUI_API void Load(const PLCore::String &sFilename, const PLMath::Vector2i &vHotspot);

		/**
		*  @brief
		*    Get cursor ID
		*
		*  @return
		*    Cursor ID or CursorCustom, if no standard cursor is used
		*/
		PLGUI_API EMouseCursor GetCursorID() const;

		/**
		*  @brief
		*    Get cursor filename
		*
		*  @return
		*    Cursor filename
		*/
		PLGUI_API PLCore::String GetFilename() const;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Gui			   *m_pGui;			/**< Pointer to GUI */
		CursorImpl	   *m_pCursorImpl;	/**< Cursor implementation */
		EMouseCursor	m_nMouseCursor;	/**< Cursor ID (CustomCursor if a custom image has been loaded) */
		PLCore::String  m_sFilename;	/**< Image filename */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_CURSOR_H__
