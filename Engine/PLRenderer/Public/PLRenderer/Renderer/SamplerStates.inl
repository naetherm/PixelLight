/*********************************************************\
 *  File: SamplerStates.inl                              *
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
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Tools/Tools.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
// Address modes
inline TextureAddressing::Enum SamplerStates::GetAddressU() const
{
	return static_cast<TextureAddressing::Enum>(m_nSS[Sampler::AddressU]);
}

inline void SamplerStates::SetAddressU(TextureAddressing::Enum nValue)
{
	m_nSS[Sampler::AddressU] = nValue;
}

inline TextureAddressing::Enum SamplerStates::GetAddressV() const
{
	return static_cast<TextureAddressing::Enum>(m_nSS[Sampler::AddressV]);
}

inline void SamplerStates::SetAddressV(TextureAddressing::Enum nValue)
{
	m_nSS[Sampler::AddressV] = nValue;
}

inline TextureAddressing::Enum SamplerStates::GetAddressW() const
{
	return static_cast<TextureAddressing::Enum>(m_nSS[Sampler::AddressW]);
}

inline void SamplerStates::SetAddressW(TextureAddressing::Enum nValue)
{
	m_nSS[Sampler::AddressW] = nValue;
}

// Filter
inline TextureFiltering::Enum SamplerStates::GetMagFilter() const
{
	return static_cast<TextureFiltering::Enum>(m_nSS[Sampler::MagFilter]);
}

inline void SamplerStates::SetMagFilter(TextureFiltering::Enum nValue)
{
	m_nSS[Sampler::MagFilter] = nValue;
}

inline TextureFiltering::Enum SamplerStates::GetMinFilter() const
{
	return static_cast<TextureFiltering::Enum>(m_nSS[Sampler::MinFilter]);
}

inline void SamplerStates::SetMinFilter(TextureFiltering::Enum nValue)
{
	m_nSS[Sampler::MinFilter] = nValue;
}

inline TextureFiltering::Enum SamplerStates::GetMipFilter() const
{
	return static_cast<TextureFiltering::Enum>(m_nSS[Sampler::MipFilter]);
}

inline void SamplerStates::SetMipFilter(TextureFiltering::Enum nValue)
{
	m_nSS[Sampler::MipFilter] = nValue;
}

// Filter
inline float SamplerStates::GetMipmapLODBias() const
{
	return PLCore::Tools::UInt32ToFloat(m_nSS[Sampler::MipmapLODBias]);
}

inline void SamplerStates::SetMipmapLODBias(float fValue)
{
	m_nSS[Sampler::MipmapLODBias] = PLCore::Tools::FloatToUInt32(fValue);
}

inline PLCore::uint32 SamplerStates::GetMaxMapLevel() const
{
	return m_nSS[Sampler::MaxMapLevel];
}

inline void SamplerStates::SetMaxMapLevel(int nValue)
{
	m_nSS[Sampler::MaxMapLevel] = nValue;
}

inline PLCore::uint32 SamplerStates::GetMaxAnisotropy() const
{
	return m_nSS[Sampler::MaxAnisotropy];
}

inline void SamplerStates::SetMaxAnisotropy(PLCore::uint32 nValue)
{
	m_nSS[Sampler::MaxAnisotropy] = nValue;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Retrieves a sampler-state value
*/
inline PLCore::uint32 SamplerStates::Get(Sampler::Enum nState) const
{
	// Check whether the state is a valid sampler member
	return (nState < Sampler::Number) ? m_nSS[nState] : 0;
}

/**
*  @brief
*    Sets a single sampler-state parameter
*/
inline bool SamplerStates::Set(Sampler::Enum nState, PLCore::uint32 nValue)
{
	// Check whether the state is a valid sampler member
	if (nState >= Sampler::Number)
		return false; // Error!

	// Set sampler state value
	m_nSS[nState] = nValue;

	// Done
	return true;
}

/**
*  @brief
*    Copy operator
*/
inline SamplerStates &SamplerStates::operator =(const SamplerStates &cSource)
{
	// Copy states
	for (PLCore::uint32 i=0; i<Sampler::Number; i++)
		m_nSS[i] = cSource.m_nSS[i];

	// Return this
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
