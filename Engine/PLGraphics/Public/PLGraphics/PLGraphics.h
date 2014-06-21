/*********************************************************\
 *  File: PLGraphics.h                                   *
 *      Main header file of this project
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


/**
*  @mainpage PLGraphics
*
*  @section intro_sec Introduction
*
*  This is the PLGraphics reference.
*/


#ifndef __PLGRAPHICS_PLGRAPHICS_H__
#define __PLGRAPHICS_PLGRAPHICS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/PLCore.h>


//[-------------------------------------------------------]
//[ Import/Export                                         ]
//[-------------------------------------------------------]
#ifdef PLGRAPHICS_EXPORTS
	// To export classes, methods and variables
	#define PLGRAPHICS_API			PL_GENERIC_API_EXPORT

	// To export RTTI elements
	#define PLGRAPHICS_RTTI_EXPORT	PL_GENERIC_RTTI_EXPORT
#else
	// To import classes, methods and variables
	#define PLGRAPHICS_API			PL_GENERIC_API_IMPORT

	// To import RTTI elements
	#define PLGRAPHICS_RTTI_EXPORT	PL_GENERIC_RTTI_IMPORT
#endif


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLGraphics {


//[-------------------------------------------------------]
//[ Definitions                                           ]
//[-------------------------------------------------------]
/**
*  @brief
*    Test images
*/
enum ETestImage {
	TestImage2DSimple = 0,	/**< Simple blue test image (256x256, RGB) */
	TestImage2DGradient,	/**< Gradient colored test image (256x256, RGB) */
	TestImage2DCheckers,	/**< Two-colored checkers test image (256x256, RGB) */
	TestImageCubeMap		/**< Cube map test */
};

/**
*  @brief
*    Consistency checks
*/
enum ECheckConsistency {
	CheckOk = 0,					/**< No problems */
	CheckImageBufferInvalidSize,	/**< ImageBuffer has an invalid size (<= 0) */
	CheckImageBufferNoData,			/**< ImageBuffer has no data at all */
	CheckPaletteNotAssigned,		/**< ImageBuffer has color type palette but no palette assigned */
	CheckPaletteEmpty,				/**< ImageBuffer has color type palette but palette is empty */
	CheckInconsistentDataFormat,	/**< ImageBuffers do not all have the same data format */
	CheckInconsistentColorFormat,	/**< ImageBuffers do not all have the same color format */
	CheckPartEmpty,					/**< Part contains no image buffers */
	CheckImageEmpty,				/**< Image contains no parts */
	CheckCubemapSideMissing,		/**< Cubemap does not have all 6 sides defined */
	CheckCubemapSideInvalid,		/**< Cubemap has an image part that is not a cubemap side */
	CheckCubemapSideDouble,			/**< Cubemap has more than one image part with the same side ID */
	CheckMipmapInconsistent,		/**< Mipmaps are not defined properly (everyone has to be smaller than the last) */
	CheckMipmapsNotPowerOfTwo,		/**< Not all mipmaps have a size that is power-of-two */
	CheckMipmapsNot1x1,				/**< Mipmaps have not been defined down to 1x1 */
};

/**
*  @brief
*    Semantics of image part
*
*  @remarks
*    ID used to identify parts of an image. Please note that the part ID is purely optional and can be chosen freely.
*    This enumeration contains some predefined semantics but is not a complete list, so you can choose any other value
*    as well. Therefore please always use PLCore::uint32 as a data type for semantics and not EImagePart directly.
*/
enum EImagePart {
	ImagePartStatic = 0,	/**< A single image */
	ImagePartCubeSidePosX,	/**< Side of cubemap: positive x */
	ImagePartCubeSideNegX,	/**< Side of cubemap: negative x */
	ImagePartCubeSidePosY,	/**< Side of cubemap: positive y */
	ImagePartCubeSideNegY,	/**< Side of cubemap: negative y */
	ImagePartCubeSidePosZ,	/**< Side of cubemap: positive z */
	ImagePartCubeSideNegZ	/**< Side of cubemap: negative z */
};

/**
*  @brief
*    Data format
*/
enum EDataFormat {
	DataByte = 0,			/**< Byte (8 bit) */
	DataWord,				/**< Word (16 bit) */
	DataHalf,				/**< Half (16 bit float) */
	DataFloat,				/**< IEEE float (32 bit) */
	DataDouble				/**< Double (64 bit) */
};

/**
*  @brief
*    Color format
*/
enum EColorFormat {
	ColorRGB = 0,			/**< RGB */
	ColorRGBA,				/**< RGBA */
	ColorBGR,				/**< BGR */
	ColorBGRA,				/**< BGRA */
	ColorGrayscale,			/**< Gray scale image */
	ColorGrayscaleA,		/**< Gray scale image with alpha */
	ColorPalette			/**< Color palette */
};

/**
*  @brief
*    Compression types
*/
enum ECompression {
	CompressionNone = 0,	/**< No compression */
	CompressionDXT1,		/**< DXT1 compression (known as BC1 in DirectX 10, RGB compression: 8:1, 8 bytes per block) */
	CompressionDXT3,		/**< DXT3 compression (known as BC2 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) */
	CompressionDXT5,		/**< DXT5 compression (known as BC3 in DirectX 10, RGBA compression: 4:1, 16 bytes per block) */
	CompressionLATC1,		/**< 1 component texture compression (also known as 3DC+/ATI1N, known as BC4 in DirectX 10, 8 bytes per block) */
	CompressionLATC2		/**< 2 component texture compression (luminance & alpha compression 4:1 -> normal map compression, also known as 3DC/ATI2N, known as BC5 in DirectX 10, 16 bytes per block) */
};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}


#endif // __PLGRAPHICS_PLGRAPHICS_H__
