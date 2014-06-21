/*********************************************************\
 *  File: CursorImpl.h                                   *
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


#ifndef __PLGUI_CURSORIMPL_H__
#define __PLGUI_CURSORIMPL_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Core/RefCount.h>
#include <PLCore/String/String.h>
#include "PLGui/PLGui.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLMath {
	class Vector2i;
}
namespace PLGui {
	class Cursor;
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
*    Cursor implementation base class
*
*  @remarks
*    This class is the base class for platform specific cursor implementations
*
*  @note
*    - Implementation of the bridge design pattern, this class is the implementor of the 'Cursor' abstraction
*/
class CursorImpl : public PLCore::RefCount<CursorImpl> {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cCursor
		*    Reference to platform independent cursor
		*/
		PLGUI_API CursorImpl(Cursor &cCursor);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API virtual ~CursorImpl();

		/**
		*  @brief
		*    Get cursor ID
		*
		*  @return
		*    Cursor ID or CursorCustom, if no default cursor is used
		*/
		PLGUI_API EMouseCursor GetCursorID() const;

		/**
		*  @brief
		*    Get image filename
		*
		*  @return
		*    Image filename
		*/
		PLGUI_API PLCore::String GetFilename() const;


	//[-------------------------------------------------------]
	//[ Public virtual CursorImpl functions                   ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Load cursor
		*
		*  @param[in] nCursor
		*    Default cursor ID
		*/
		virtual void Load(EMouseCursor nCursor) = 0;

		/**
		*  @brief
		*    Load cursor
		*
		*  @param[in] sFilename
		*    Name of image file
		*  @param[in] vHotspot
		*    Hotspot of cursor
		*/
		virtual void Load(const PLCore::String &sFilename, const PLMath::Vector2i &vHotspot) = 0;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Cursor		   *m_pCursor;		/**< Pointer to the platform independent cursor object */
		EMouseCursor	m_nMouseCursor;	/**< Default cursor ID (CustomCursor if a custom image has been loaded) */
		PLCore::String  m_sFilename;	/**< Image filename */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_CURSORIMPL_H__
