/*********************************************************\
 *  File: Graphics.h                                     *
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


#ifndef __PLGUI_GRAPHICS_H__
#define __PLGUI_GRAPHICS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/List.h>
#include <PLMath/Vector2i.h>
#include "PLGui/PLGui.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class String;
}
namespace PLGraphics {
	class Color4;
}
namespace PLGui {
	class Gui;
	class GraphicsHint;
	class GraphicsImpl;
	class Image;
	class Font;
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
*    Graphics class
*
*  @note
*    - Implementation of the bridge design pattern, this class is the abstraction
*/
class Graphics {


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
		*    This function creates a new graphics implementation by calling the appropriate functions
		*    from cGui. This is useful e.g. for offscreen graphics objects. The implementation will be
		*    deleted along with the graphics object itself.
		*/
		PLGUI_API Graphics(Gui &cGui);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cGui
		*    Owner GUI
		*  @param[in] cGraphicsImpl
		*    Reference to graphics implementation
		*
		*  @remarks
		*    This function takes an already created graphics implementation and embeds it inside
		*    the graphics object. This implementation will not be deleted automatically!
		*/
		PLGUI_API Graphics(Gui &cGui, GraphicsImpl &cGraphicsImpl);

		/**
		*  @brief
		*    Destructor
		*/
		PLGUI_API ~Graphics();

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
		*    Pointer to platform  specific implementation
		*/
		PLGUI_API GraphicsImpl *GetImpl() const;

		/**
		*  @brief
		*    Get hints
		*
		*  @return
		*    List of hints
		*/
		PLGUI_API const PLCore::List<GraphicsHint*> &GetHints();

		/**
		*  @brief
		*    Add a hint
		*
		*  @param[in] pHint
		*    GraphicsHint for the GUI renderer
		*/
		PLGUI_API void AddHint(GraphicsHint *pHint);

		/**
		*  @brief
		*    Draws a line
		*
		*  @param[in] cColor
		*    Color of the line
		*  @param[in] vPos1
		*    Start position
		*  @param[in] vPos2
		*    End position
		*  @param[in] nWidth
		*    Line width
		*/
		PLGUI_API void DrawLine(const PLGraphics::Color4 &cColor, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, PLCore::uint32 nWidth = 1);

		/**
		*  @brief
		*    Draws a rectangle
		*
		*  @param[in] cColor
		*    Color of the line
		*  @param[in] vPos1
		*    First corner
		*  @param[in] vPos2
		*    Second corner
		*  @param[in] nRoundX
		*    Rounded border in X direction
		*  @param[in] nRoundY
		*    Rounded border in Y direction
		*  @param[in] nWidth
		*    Line width
		*/
		PLGUI_API void DrawRect(const PLGraphics::Color4 &cColor, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, PLCore::uint32 nRoundX = 0, PLCore::uint32 nRoundY = 0, PLCore::uint32 nWidth = 1);

		/**
		*  @brief
		*    Draws a filled box
		*
		*  @param[in] cColor
		*    Color of the box
		*  @param[in] vPos1
		*    First corner
		*  @param[in] vPos2
		*    Second corner
		*  @param[in] nRoundX
		*    Rounded border in X direction
		*  @param[in] nRoundY
		*    Rounded border in Y direction
		*/
		PLGUI_API void DrawBox(const PLGraphics::Color4 &cColor, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2, PLCore::uint32 nRoundX = 0, PLCore::uint32 nRoundY = 0);

		/**
		*  @brief
		*    Draws a filled box with a color gradient
		*
		*  @param[in] cColor1
		*    First color
		*  @param[in] cColor2
		*    Second color
		*  @param[in] fAngle
		*    Clockwise angle of the gradient color (in degrees). 0° means from left to right.
		*  @param[in] vPos1
		*    First corner
		*  @param[in] vPos2
		*    Second corner
		*/
		PLGUI_API void DrawGradientBox(const PLGraphics::Color4 &cColor1, const PLGraphics::Color4 &cColor2, float fAngle, const PLMath::Vector2i &vPos1, const PLMath::Vector2i &vPos2);

		/**
		*  @brief
		*    Draws an image
		*
		*  @param[in] cImage
		*    Image to draw
		*  @param[in] vPos
		*    Image position
		*  @param[in] vSize
		*    Image size
		*
		*  @note
		*    - If vSize = (0 0), the image size is used
		*/
		PLGUI_API void DrawImage(const Image &cImage, const PLMath::Vector2i &vPos, const PLMath::Vector2i &vSize);

		/**
		*  @brief
		*    Draws an tiled image
		*
		*  @param[in] cImage
		*    Image to draw
		*  @param[in] vPos
		*    Image position
		*  @param[in] vSize
		*    Image size
		*
		*  @note
		*    - If vSize = (0 0), the image size is used
		*/
		PLGUI_API void DrawTiledImage(const Image &cImage, const PLMath::Vector2i &vPos, const PLMath::Vector2i &vSize);

		/**
		*  @brief
		*    Draws a text
		*
		*  @param[in] cFont
		*    Font to use
		*  @param[in] cTextColor
		*    Text color
		*  @param[in] cBkColor
		*    Background color
		*  @param[in] vPos
		*    Text position
		*  @param[in] sText
		*    Text
		*/
		PLGUI_API void DrawText(const Font &cFont, const PLGraphics::Color4 &cTextColor, const PLGraphics::Color4 &cBkColor, const PLMath::Vector2i &vPos, const PLCore::String &sText);

		/**
		*  @brief
		*    Gets the width of a given text in the current font
		*
		*  @param[in] cFont
		*    Font to use
		*  @param[in] sText
		*    Text to get the width of
		*
		*  @return
		*    The width of the given text using the given font
		*/
		PLGUI_API PLCore::uint32 GetTextWidth(const Font &cFont, const PLCore::String &sText);

		/**
		*  @brief
		*    Gets the height of a given text in the current font
		*
		*  @param[in] cFont
		*    Font to use
		*  @param[in] sText
		*    Text to get the height of
		*
		*  @return
		*    The height of the given text using the given font
		*/
		PLGUI_API PLCore::uint32 GetTextHeight(const Font &cFont, const PLCore::String &sText);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Gui							*m_pGui;			/**< GUI instance */
		GraphicsImpl				*m_pGraphicsImpl;	/**< Graphics implementation object */
		bool						 m_bManageImpl;		/**< If 'true', m_pGraphicsImpl will be destroyed in destructor */
		PLCore::List<GraphicsHint*>	 m_lstHints;		/**< Rendering hints */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


#endif // __PLGUI_GRAPHICS_H__
