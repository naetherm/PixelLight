/*********************************************************\
 *  File: ShaderCompositionID.inl                        *
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


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLVolumeRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
inline ShaderCompositionID::~ShaderCompositionID()
{
}

/**
*  @brief
*    Returns the CRC32 checksum of this shader composition ID
*/
inline PLCore::uint32 ShaderCompositionID::GetChecksum() const
{
	return m_nChecksumCRC32;
}

/**
*  @brief
*    Returns the shader composition data the ID consists of
*/
inline const ShaderCompositionID::SData &ShaderCompositionID::GetData() const
{
	return m_sData;
}

/**
*  @brief
*    Returns whether or not this shader composition ID is valid
*/
bool ShaderCompositionID::IsValid() const
{
	// If this ID is valid, all pointers are no null pointer (the following formating is intended)
	return
		m_sData.pShaderLanguage						&&
		// 1.0 - Ray Setup
		m_sData.pShaderFunctionRaySetupClass		&&
		// 1.1 - Clip Ray
		m_sData.pShaderFunctionClipRayClass			&&
		// 1.2 - Jitter Position
		m_sData.pShaderFunctionJitterPositionClass	&&
		// 2.0 - Ray Traversal
		m_sData.pShaderFunctionRayTraversalClass	&&
		// 2.1 - Clip Position
		m_sData.pShaderFunctionClipPositionClass	&&
		// 2.2 - Reconstruction
		m_sData.pShaderFunctionReconstructionClass	&&
		// 2.2 - Fetch Scalar
		m_sData.pShaderFunctionFetchScalarClass		&&
		// 2.3 - Shading
		m_sData.pShaderFunctionShadingClass			&&
		// 2.4 - Classification
		m_sData.pShaderFunctionClassificationClass	&&
		// 2.5 - Gradient
		m_sData.pShaderFunctionGradientClass		&&
		// 2.5 - Gradient Input
		m_sData.pShaderFunctionGradientInputClass	&&
		// 2.6 - Illumination
		m_sData.pShaderFunctionIlluminationClass
	;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer
