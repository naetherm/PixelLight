/*********************************************************\
 *  File: ShaderCompositionID.h                          *
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


#ifndef __PLVOLUMERENDERER_SHADERCOMPOSITIONID_H__
#define __PLVOLUMERENDERER_SHADERCOMPOSITIONID_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLVolumeRenderer/PLVolumeRenderer.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class Class;
}
namespace PLRenderer {
	class ShaderLanguage;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolumeRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Shader composition ID class
*/
class ShaderCompositionID {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class ShaderCompositorPC;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Shader composition data the ID consists of
		*
		*  @note
		*    - In here it's safe to store direct pointers, their address will not change during runtime
		*    - If this ID is valid, all pointers are no null pointer
		*/
		struct SData {
			PLRenderer::ShaderLanguage	*pShaderLanguage;
			// 1.0 - Ray Setup
			const PLCore::Class			*pShaderFunctionRaySetupClass;
			// 1.1 - Clip Ray
			const PLCore::Class			 *pShaderFunctionClipRayClass;
			PLCore::uint8				 nNumOfClipPlanes;
			PLCore::uint8				 nNumOfDepthTextures;
			// 1.2 - Jitter Position
			const PLCore::Class			*pShaderFunctionJitterPositionClass;
			// 2.0 - Ray Traversal
			const PLCore::Class			*pShaderFunctionRayTraversalClass;
			// 2.1 - Clip Position
			const PLCore::Class			*pShaderFunctionClipPositionClass;
			// 2.2 - Reconstruction
			const PLCore::Class			*pShaderFunctionReconstructionClass;
			// 2.2 - Fetch Scalar
			const PLCore::Class			*pShaderFunctionFetchScalarClass;
			// 2.3 - Shading
			const PLCore::Class			*pShaderFunctionShadingClass;
			// 2.4 - Classification
			const PLCore::Class			*pShaderFunctionClassificationClass;
			// 2.5 - Gradient
			const PLCore::Class			*pShaderFunctionGradientClass;
			// 2.5 - Gradient Input
			const PLCore::Class			*pShaderFunctionGradientInputClass;
			// 2.6 - Illumination
			const PLCore::Class			*pShaderFunctionIlluminationClass;
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLVOLUMERENDERER_API ShaderCompositionID();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		PLVOLUMERENDERER_API ShaderCompositionID(const ShaderCompositionID &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~ShaderCompositionID();

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
		PLVOLUMERENDERER_API ShaderCompositionID &operator =(const ShaderCompositionID &cSource);

		/**
		*  @brief
		*    Check for equality
		*
		*  @param[in] cOther
		*    Other shader composition ID to compare with
		*
		*  @return
		*    'true' if the two shader composition IDs are equal, else 'false'
		*/
		PLVOLUMERENDERER_API bool operator ==(const ShaderCompositionID &cOther) const;

		/**
		*  @brief
		*    Returns the CRC32 checksum of this shader composition ID
		*
		*  @return
		*    The CRC32 checksum of this shader composition ID
		*/
		inline PLCore::uint32 GetChecksum() const;

		/**
		*  @brief
		*    Returns the shader composition data the ID consists of
		*
		*  @return
		*    The shader composition data the ID consists of
		*/
		inline const SData &GetData() const;

		/**
		*  @brief
		*    Returns whether or not this shader composition ID is valid
		*
		*  @return
		*    'true' if this shader composition ID is valid, else 'false'
		*
		*  @note
		*    - If this ID is valid, all pointers are no null pointer
		*/
		inline bool IsValid() const;

		/**
		*  @brief
		*    Returns a human readable representation of this shader composition ID
		*
		*  @return
		*    Human readable representation of this shader shader composition ID
		*
		*  @note
		*    - For debugging
		*/
		PLVOLUMERENDERER_API PLCore::String ToString() const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Calculates the CRC32 checksum
		*/
		void CalculateChecksum();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::uint32 m_nChecksumCRC32;	/**< CRC32 checksum */
		SData		   m_sData;				/**< Shader composition data the ID consists of */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLVolumeRenderer/ShaderCompositionID.inl"


#endif // __PLVOLUMERENDERER_SHADERCOMPOSITIONID_H__
