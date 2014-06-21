/*********************************************************\
 *  File: DrawHelpers.h                                  *
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


#ifndef __PLRENDERER_DRAWHELPERS_H__
#define __PLRENDERER_DRAWHELPERS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLMath/Vector2.h>
#include <PLMath/Matrix4x4.h>
#include <PLGraphics/Color/Color4.h>
#include "PLRenderer/PLRenderer.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class Font;
	class TextureBuffer;
	class SamplerStates;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract draw helpers interface
*
*  @remarks
*    If you want for example to draw a simple point or line with just one line of code, use this
*    offered draw helpers. If you want to draw huge point clouds and so on, use vertex buffers
*    and draw them all at once using a single draw call - this is the more efficient and universal way!
*    The 2D draw helper space looks like this
*
*    (0, 0)
*       |------
*       |
*       |    (Width, Height)
*
*    The origin is at the upper left which is also usually true for GUI systems.
*/
class DrawHelpers {


	//[-------------------------------------------------------]
	//[ Public virtual DrawHelpers functions                  ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ 2D mode                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Begin 2D mode
		*
		*  @param[in] fX1
		*    Virtual screen x start position
		*  @param[in] fY1
		*    Virtual screen y start position
		*  @param[in] fX2
		*    Virtual screen x end position
		*  @param[in] fY2
		*    Virtual screen y end position
		*
		*  @note
		*    - If all parameters are 0.0f, the current viewport settings will be used
		*    - The projection matrix is set to an orthographic matrix
		*    - The projection and view matrix will be reset to the old states
		*      by calling End2DMode(), so be careful when using matrix or render state
		*      operations withing Begin2DMode() and End2DMode() - or better don't do it!
		*    - If the renderer is already in the 2D mode the old is ended automatically
		*      before the new one is started
		*    - If bitmaps, texts etc. are drawn without an active 2D mode the 2D
		*      mode is set automatically using the default values
		*    - When drawing lines, points etc. note that you should go sure that no for instance
		*      no textures are used else its possible that your lines will be e.g. black
		*/
		virtual void Begin2DMode(float fX1 = 0.0f, float fY1 = 0.0f, float fX2 = 1.0f, float fY2 = 1.0f) = 0;

		/**
		*  @brief
		*    Returns whether the renderer is currently in the 2D mode or not
		*
		*  @return
		*    'true' if the renderer is currently in the 2D mode, else 'false'
		*/
		virtual bool Is2DMode() const = 0;

		/**
		*  @brief
		*    Returns the virtual 2D mode screen size
		*
		*  @param[out] fX1
		*    Will receive the virtual screen x start position
		*  @param[out] fY1
		*    Will receive the virtual screen y start position
		*  @param[out] fX2
		*    Will receive the virtual screen x end position
		*  @param[out] fY2
		*    Will receive the virtual screen y end position
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe the 2D mode isn't active)
		*/
		virtual bool Get2DMode(float &fX1, float &fY1, float &fX2, float &fY2) const = 0;

		/**
		*  @brief
		*    End 2D mode
		*
		*  @see
		*    - Begin2DMode()
		*/
		virtual void End2DMode() = 0;

		/**
		*  @brief
		*    Returns the z value to be used for 2D rendering
		*
		*  @return
		*    Z value
		*/
		virtual float Get2DZValue() const = 0;

		/**
		*  @brief
		*    Sets the Z Value to be used for 2D rendering
		*
		*  @param[in] fZValue
		*    Z value
		*/
		virtual void Set2DZValue(float fZValue = 0.0f) = 0;

		/**
		*  @brief
		*    Returns the 2D mode object space to clip space matrix
		*
		*  @return
		*    2D mode object space to clip space matrix
		*/
		virtual const PLMath::Matrix4x4 &GetObjectSpaceToClipSpaceMatrix() const = 0;

		//[-------------------------------------------------------]
		//[ Text                                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Draws a text at a given 2D position
		*
		*  @param[in] cFont
		*    Font to use
		*  @param[in] sText
		*    Text to draw
		*  @param[in] cColor
		*    Text color
		*  @param[in] vPosition
		*    Text screen position
		*  @param[in] nFlags
		*    Draw flags, see Font::EDrawFlags
		*  @param[in] vScale
		*    Font scale
		*  @param[in] vBias
		*    Font bias (position offset)
		*/
		virtual void DrawText(Font &cFont, const PLCore::String &sText, const PLGraphics::Color4 &cColor, const PLMath::Vector2 &vPosition, PLCore::uint32 nFlags = 0, const PLMath::Vector2 &vScale = PLMath::Vector2::One, const PLMath::Vector2 &vBias = PLMath::Vector2::Zero) = 0;

		/**
		*  @brief
		*    Draws a text at a given 3D position
		*
		*  @param[in] cFont
		*    Font to use
		*  @param[in] sText
		*    Text to draw
		*  @param[in] cColor
		*    Text color
		*  @param[in] vPosition
		*    Text object space position
		*  @param[in] mObjectSpaceToClipSpace
		*    Object space to clip space matrix
		*  @param[in] nFlags
		*    Draw flags, see Font::EDrawFlags
		*  @param[in] vScale
		*    Font scale
		*  @param[in] vBias
		*    Font bias (position offset)
		*
		*  @note
		*    - "vPosition.To2DCoordinate(mObjectSpaceToClipSpace, cRenderer.GetViewport())" is used to calculate the screen space position
		*/
		virtual void DrawText(Font &cFont, const PLCore::String &sText, const PLGraphics::Color4 &cColor, const PLMath::Vector3 &vPosition, const PLMath::Matrix4x4 &mObjectSpaceToClipSpace, PLCore::uint32 nFlags = 0, const PLMath::Vector2 &vScale = PLMath::Vector2::One, const PLMath::Vector2 &vBias = PLMath::Vector2::Zero) = 0;

		/**
		*  @brief
		*    Returns the width of a text
		*
		*  @param[in] cFont
		*    Font to use
		*  @param[in] sText
		*    Text to 'draw'
		*
		*  @return
		*    The width of a text
		*/
		virtual float GetTextWidth(Font &cFont, const PLCore::String &sText) const = 0;

		/**
		*  @brief
		*    Returns the height of a font
		*
		*  @param[in] cFont
		*    Font to use
		*
		*  @return
		*    The height of a font
		*/
		virtual float GetTextHeight(Font &cFont) const = 0;


		//[-------------------------------------------------------]
		//[ Image                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Draws an image using a given 2D position
		*
		*  @param[in] cTextureBuffer
		*    Texture buffer holding the image to draw
		*  @param[in] cSamplerStates
		*    Sampler states
		*  @param[in] vPos
		*    Image position
		*  @param[in] vSize
		*    Image size
		*  @param[in] cColor
		*    Color to use
		*  @param[in] fAlphaReference
		*    Alpha test reference value (0-1), all texels below the value will be discarded, if >= 1, no alpha test will be performed
		*  @param[in] vTextureCoordinate
		*    Normalized texture coordinate (0..1)
		*  @param[in] vTextureCoordinateSize
		*    Normalized texture coordinate size (0..1)
		*  @param[in] mTexture
		*    Texture matrix
		*
		*  @note
		*    - Usually used together with Begin2DMode()
		*    - Use counter clockwise culling or deactivate culling
		*    - If vSize = (0 0), the image size is used
		*/
		virtual void DrawImage(TextureBuffer &cTextureBuffer, SamplerStates &cSamplerStates, const PLMath::Vector2 &vPos, const PLMath::Vector2 &vSize = PLMath::Vector2::Zero, const PLGraphics::Color4 &cColor = PLGraphics::Color4::White,
							   float fAlphaReference = 1.0f, const PLMath::Vector2 &vTextureCoordinate = PLMath::Vector2::Zero, const PLMath::Vector2 &vTextureCoordinateSize = PLMath::Vector2::One, const PLMath::Matrix4x4 &mTexture = PLMath::Matrix4x4::Identity) = 0;

		/**
		*  @brief
		*    Draws an image using a given 3D position
		*
		*  @param[in] cTextureBuffer
		*    Texture buffer holding the image to draw
		*  @param[in] cSamplerStates
		*    Sampler states
		*  @param[in] vPos
		*    Image position
		*  @param[in] mObjectSpaceToClipSpace
		*    Object space to clip space matrix
		*  @param[in] vSize
		*    Image size
		*  @param[in] cColor
		*    Color to use
		*  @param[in] fAlphaReference
		*    Alpha test reference value (0-1), all texels below the value will be discarded, if >= 1, no alpha test will be performed
		*  @param[in] vTextureCoordinate
		*    Normalized texture coordinate (0..1)
		*  @param[in] vTextureCoordinateSize
		*    Normalized texture coordinate size (0..1)
		*  @param[in] mTexture
		*    Texture matrix
		*
		*  @note
		*    - The image is drawn on the x/y plane and 'looks' into the positive z axis
		*    - If vSize = (0 0), the image size is used
		*/
		virtual void DrawImage(TextureBuffer &cTextureBuffer, SamplerStates &cSamplerStates, const PLMath::Vector3 &vPos, const PLMath::Matrix4x4 &mObjectSpaceToClipSpace, const PLMath::Vector2 &vSize = PLMath::Vector2::Zero, const PLGraphics::Color4 &cColor = PLGraphics::Color4::White,
							   float fAlphaReference = 1.0f, const PLMath::Vector2 &vTextureCoordinate = PLMath::Vector2::Zero, const PLMath::Vector2 &vTextureCoordinateSize = PLMath::Vector2::One, const PLMath::Matrix4x4 &mTexture = PLMath::Matrix4x4::Identity) = 0;


		//[-------------------------------------------------------]
		//[ Primitive                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Draws a single colored point using a given 2D position
		*
		*  @param[in] cColor
		*    Color to use
		*  @param[in] vPosition
		*    2D point position
		*  @param[in] fSize
		*    Point size
		*
		*  @note
		*    - For drawing many points you should use Renderer::DrawPrimitives() with Primitive::PointList
		*      for better performance. This way you also have more control over 'how' the points
		*      are rendered. (vertex color and so on)
		*    - You should use the draw functions for points and lines only for debug
		*      purposes because often they are not fast and in general not supported
		*      by each API! (OpenGL supports this function well :)
		*
		*  @see
		*    - Begin2DMode()
		*/
		virtual void DrawPoint(const PLGraphics::Color4 &cColor, const PLMath::Vector2 &vPosition, float fSize = 1.0f) = 0;

		/**
		*  @brief
		*    Draws a single colored point using a given 3D position
		*
		*  @param[in] cColor
		*    Color to use
		*  @param[in] vPosition
		*    3D object space point position
		*  @param[in] mObjectSpaceToClipSpace
		*    Object space to clip space matrix
		*  @param[in] fSize
		*    Point size
		*
		*  @see
		*    - DrawPoint() above
		*/
		virtual void DrawPoint(const PLGraphics::Color4 &cColor, const PLMath::Vector3 &vPosition, const PLMath::Matrix4x4 &mObjectSpaceToClipSpace, float fSize = 1.0f) = 0;

		/**
		*  @brief
		*    Draws a single colored line using a given 2D start position and 2D end position
		*
		*  @param[in] cColor
		*    Color to use
		*  @param[in] vStartPosition
		*    2D start position of the line
		*  @param[in] vEndPosition
		*    2D end position of the line
		*  @param[in] fWidth
		*    Width of the line
		*
		*  @note
		*    - For drawing many lines you should use Renderer::DrawPrimitives() with Primitive::LineList
		*      for better performance. This way you also have more control over 'how' the lines
		*      are rendered. (vertex color and so on)
		*
		*  @see
		*    - Begin2DMode()
		*    - DrawPoint()
		*/
		virtual void DrawLine(const PLGraphics::Color4 &cColor, const PLMath::Vector2 &vStartPosition, const PLMath::Vector2 &vEndPosition, float fWidth = 1.0f) = 0;

		/**
		*  @brief
		*    Draws a single colored line using a given 3D start position and 3D end position
		*
		*  @param[in] cColor
		*    Color to use
		*  @param[in] vStartPosition
		*    3D object space start position of the line
		*  @param[in] vEndPosition
		*    3D object space end position of the line
		*  @param[in] mObjectSpaceToClipSpace
		*    Object space to clip space matrix
		*  @param[in] fWidth
		*    Width of the line
		*
		*  @see
		*    - DrawLine() above
		*/
		virtual void DrawLine(const PLGraphics::Color4 &cColor, const PLMath::Vector3 &vStartPosition, const PLMath::Vector3 &vEndPosition, const PLMath::Matrix4x4 &mObjectSpaceToClipSpace, float fWidth = 1.0f) = 0;

		/**
		*  @brief
		*    Draws a colored triangle
		*
		*  @param[in] cColor
		*    Color to use
		*  @param[in] vV1
		*    Object space triangle vertex 1
		*  @param[in] vV2
		*    Object space triangle vertex 2
		*  @param[in] vV3
		*    Object space triangle vertex 3
		*  @param[in] mObjectSpaceToClipSpace
		*    Object space to clip space matrix
		*  @param[in] fWidth
		*    Width of the line, if 0, the box is not drawn using lines
		*
		*  @note
		*    - This function should be used for debugging proposes only!
		*      Therefore you can use this only to visualize something without to be able
		*      to define normals, texture coordinates etc.
		*/
		virtual void DrawTriangle(const PLGraphics::Color4 &cColor, const PLMath::Vector3 &vV1, const PLMath::Vector3 &vV2, const PLMath::Vector3 &vV3, const PLMath::Matrix4x4 &mObjectSpaceToClipSpace, float fWidth = 0.0f) = 0;

		/**
		*  @brief
		*    Draws a colored 2D quad
		*
		*  @param[in] cColor
		*    Color to use
		*  @param[in] vPos
		*    2D position
		*  @param[in] vSize
		*    2D size
		*  @param[in] fWidth
		*    Width of the line, if 0, the quad is not drawn using lines
		*
		*  @see
		*    - DrawTriangle()
		*/
		virtual void DrawQuad(const PLGraphics::Color4 &cColor, const PLMath::Vector2 &vPos, const PLMath::Vector2 &vSize, float fWidth = 0.0f) = 0;

		/**
		*  @brief
		*    Draws a colored quad using given 3D positions
		*
		*  @param[in] cColor
		*    Color to use
		*  @param[in] vV1
		*    Object space quad vertex 1
		*  @param[in] vV2
		*    Object space quad vertex 2
		*  @param[in] vV3
		*    Object space quad vertex 3
		*  @param[in] vV4
		*    Object space quad vertex 4
		*  @param[in] mObjectSpaceToClipSpace
		*    Object space to clip space matrix
		*  @param[in] fWidth
		*    Width of the line, if 0, the quad is not drawn using lines
		*
		*  @see
		*    - DrawTriangle()
		*/
		virtual void DrawQuad(const PLGraphics::Color4 &cColor, const PLMath::Vector3 &vV1, const PLMath::Vector3 &vV2, const PLMath::Vector3 &vV3, const PLMath::Vector3 &vV4, const PLMath::Matrix4x4 &mObjectSpaceToClipSpace, float fWidth = 0.0f) = 0;

		/**
		*  @brief
		*    Draws a gradient colored 2D quad
		*
		*  @param[in] cColor1
		*    First color to use
		*  @param[in] cColor2
		*    Second color to use
		*  @param[in] fAngle
		*    Clockwise angle (in radian) of the gradient color. 0° means from left to right.
		*  @param[in] vPos
		*    2D position
		*  @param[in] vSize
		*    2D size
		*
		*  @see
		*    - DrawTriangle()
		*/
		virtual void DrawGradientQuad(const PLGraphics::Color4 &cColor1, const PLGraphics::Color4 &cColor2, float fAngle, const PLMath::Vector2 &vPos, const PLMath::Vector2 &vSize) = 0;

		/**
		*  @brief
		*    Draws a gradient colored quad using given 3D positions
		*
		*  @param[in] cColor1
		*    First color to use
		*  @param[in] cColor2
		*    Second color to use
		*  @param[in] fAngle
		*    Clockwise angle (in radian) of the gradient color. 0° means from left to right.
		*  @param[in] vV1
		*    Object space quad vertex 1
		*  @param[in] vV2
		*    Object space quad vertex 2
		*  @param[in] vV3
		*    Object space quad vertex 3
		*  @param[in] vV4
		*    Object space quad vertex 4
		*  @param[in] mObjectSpaceToClipSpace
		*    Object space to clip space matrix
		*
		*  @see
		*    - DrawTriangle()
		*/
		virtual void DrawGradientQuad(const PLGraphics::Color4 &cColor1, const PLGraphics::Color4 &cColor2, float fAngle, const PLMath::Vector3 &vV1, const PLMath::Vector3 &vV2, const PLMath::Vector3 &vV3, const PLMath::Vector3 &vV4, const PLMath::Matrix4x4 &mObjectSpaceToClipSpace) = 0;

		/**
		*  @brief
		*    Draws a colored box
		*
		*  @param[in] cColor
		*    Color to use
		*  @param[in] vMin
		*    Minimum object space position
		*  @param[in] vMax
		*    Maximum object space position
		*  @param[in] mObjectSpaceToClipSpace
		*    Object space to clip space matrix
		*  @param[in] fWidth
		*    Width of the line, if 0, the box is not drawn using lines
		*
		*  @see
		*    - DrawTriangle()
		*/
		virtual void DrawBox(const PLGraphics::Color4 &cColor, const PLMath::Vector3 &vMin, const PLMath::Vector3 &vMax, const PLMath::Matrix4x4 &mObjectSpaceToClipSpace, float fWidth = 0.0f) = 0;

		/**
		*  @brief
		*    Draws a colored plane
		*
		*  @param[in] cColor
		*    Color to use
		*  @param[in] vN
		*    Plane normal (must be normalized)
		*  @param[in] fD
		*    Plane d factor
		*  @param[in] mObjectSpaceToClipSpace
		*    Object space to clip space matrix
		*  @param[in] fSize
		*    Plane size, a plane has an infinite size but often it's useful to limit the visible size...
		*  @param[in] fLineWidth
		*    Width of the line, if 0, draw no line to origin
		*
		*  @see
		*    - DrawTriangle()
		*/
		virtual void DrawPlane(const PLGraphics::Color4 &cColor, const PLMath::Vector3 &vN, float fD, const PLMath::Matrix4x4 &mObjectSpaceToClipSpace, float fSize = 10000.0f, float fLineWidth = 1.0f) = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		PLRENDERER_API DrawHelpers();

		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~DrawHelpers();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		DrawHelpers(const DrawHelpers &cSource);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		DrawHelpers &operator =(const DrawHelpers &cSource);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


#endif // __PLRENDERER_DRAWHELPERS_H__
