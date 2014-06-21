/*********************************************************\
 *  File: TransferFunction.h                             *
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


#ifndef __PLVOLUME_TRANSFERFUNCTION_H__
#define __PLVOLUME_TRANSFERFUNCTION_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLGraphics/Image/Image.h>
#include "PLVolume/PLVolume.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class Renderer;
	class TextureBuffer;
	class ResourceHandler;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolume {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Transfer function
*/
class TransferFunction : public PLCore::Loadable {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum ETextureBufferType {
			StandardTextureBuffer,		/**< Texture buffer without any special applied features */
			PreMultipliedTextureBuffer,	/**< Texture buffer with pre-multiplied color by their corresponding opacity value in order to avoid color bleeding due to interpolation */
			PreIntegratedTextureBuffer,	/**< Texture buffer with pre-integration */
			// Number of texture buffer types
			NumOfTextureBufferTypes
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLVOLUME_API TransferFunction();

		/**
		*  @brief
		*    Destructor
		*/
		PLVOLUME_API virtual ~TransferFunction();

		/**
		*  @brief
		*    Returns the number of bins
		*
		*  @return
		*    The number of bins
		*/
		PLVOLUME_API PLCore::uint32 GetNumOfBins() const;

		/**
		*  @brief
		*    Returns the renderer texture buffer holding the transfer function
		*
		*  @param[in] cRenderer
		*    Used renderer instance
		*  @param[in] nType
		*    Texture buffer type
		*
		*  @return
		*    Renderer texture buffer holding the transfer function, can be a null pointer
		*
		*  @remarks
		*    The texture type can be "PLRenderer::Resource::TypeTextureBuffer1D".
		*/
		PLVOLUME_API PLRenderer::TextureBuffer *GetTextureBuffer(PLRenderer::Renderer &cRenderer, ETextureBufferType nType = StandardTextureBuffer);


	//[-------------------------------------------------------]
	//[ Public virtual TransferFunction functions             ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the image holding the transfer function
		*
		*  @return
		*    The image holding the transfer function
		*
		*  @note
		*    - This method marks the image data to be dirty, meaning potential change by user, use the constant method version of this method whenever possible to avoid unnecessary recalculations
		*/
		PLVOLUME_API virtual PLGraphics::Image &GetImage();

		/**
		*  @brief
		*    Returns the image holding the transfer function
		*
		*  @return
		*    The image holding the transfer function
		*/
		PLVOLUME_API virtual const PLGraphics::Image &GetConstImage();


	//[-------------------------------------------------------]
	//[ Public virtual PLCore::Loadable functions             ]
	//[-------------------------------------------------------]
	public:
		PLVOLUME_API virtual bool Unload() override;
		PLVOLUME_API virtual PLCore::String GetLoadableTypeName() const override;


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
		TransferFunction(const TransferFunction &cSource);

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
		TransferFunction &operator =(const TransferFunction &cSource);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		PLGraphics::Image			 m_cImage;											/**< Image holding the transfer data (may be derived from data, access it via "GetImage()") */
		// Cached texture buffer
		PLRenderer::ResourceHandler *m_pTextureBufferHandler[NumOfTextureBufferTypes];	/**< Renderer texture buffer holding the transfer function, can be a null pointer (from "m_cImage" derived data, access it via "GetTextureBuffer()") */
		bool						 m_bTextureBufferDirty[NumOfTextureBufferTypes];	/**< Is the renderer texture buffer holding the transfer function marked to be dirty? */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolume


#endif // __PLVOLUME_TRANSFERFUNCTION_H__
