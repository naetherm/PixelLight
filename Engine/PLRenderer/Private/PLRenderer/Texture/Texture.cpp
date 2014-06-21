/*********************************************************\
 *  File: Texture.cpp                                    *
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
#include <PLCore/Log/Log.h>
#include <PLCore/Xml/Xml.h>
#include <PLCore/File/Url.h>
#include <PLCore/Tools/Loader.h>
#include <PLCore/Tools/LoaderImpl.h>
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include <PLGraphics/Image/ImageEffects.h>
#include "PLRenderer/RendererContext.h"
#include "PLRenderer/Renderer/TextureBuffer.h"
#include "PLRenderer/Renderer/ResourceHandler.h"
#include "PLRenderer/Texture/TextureManager.h"
#include "PLRenderer/Texture/Texture.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Destructor
*/
Texture::~Texture()
{
	// First at all: Unload the texture
	Unload();

	// Delete renderer texture buffer resource handler
	delete m_pTextureBufferHandler;
}

/**
*  @brief
*    Returns the texture buffer
*/
TextureBuffer *Texture::GetTextureBuffer() const
{
	return static_cast<TextureBuffer*>(m_pTextureBufferHandler->GetResource());
}

/**
*  @brief
*    Sets the texture buffer
*/
void Texture::SetTextureBuffer(TextureBuffer *pTextureBuffer)
{
	if (m_pTextureBufferHandler->GetResource() != pTextureBuffer) {
		DestroyTextureBuffer();
		m_pTextureBufferHandler->SetResource(pTextureBuffer);

		// Get the texture buffer size and backup it as original size so the behaviour
		// is the same as when loading a texture instead of just setting a texture buffer
		m_vOriginalSize = pTextureBuffer ? pTextureBuffer->GetUniformSize() : Vector3i::Zero;
	}
}

/**
*  @brief
*    Makes the texture to the current renderer texture
*/
bool Texture::Bind(uint32 nStage) const
{
	if (m_pTextureBufferHandler->GetResource()) {
		TextureBuffer *pTextureBuffer = static_cast<TextureBuffer*>(m_pTextureBufferHandler->GetResource());
		pTextureBuffer->GetRenderer().SetTextureBuffer(nStage, pTextureBuffer);

		// Done
		return true;
	} else {
		// Set a null pointer texture
		GetTextureManager().GetRendererContext().GetRenderer().SetTextureBuffer(nStage, nullptr);

		// Error!
		return false;
	}
}


//[-------------------------------------------------------]
//[ Public virtual Texture functions                      ]
//[-------------------------------------------------------]
bool Texture::IsAnimated() const
{
	// Not animated
	return false;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Texture::Texture(TextureManager &cManager, const String &sName) : PLCore::Resource<Texture>(sName, &cManager),
	m_pTextureBufferHandler(new ResourceHandler()),
	m_bShareTextureBuffer(false),
	m_nCompressionHint(Default)
{
}

/**
*  @brief
*    Returns a correct texture size
*/
uint32 Texture::GetCorrectTextureSize(uint32 nSize, uint32 nMinSize, uint32 nMaxSize, bool bRectangle) const
{
	if (bRectangle) {
		uint32 i = Math::GetNearestPowerOfTwo(nSize);
		if (i != nSize)
			nSize = i;
	}
	if (nSize < nMinSize)
		nSize = nMinSize;
	if (nSize > nMaxSize)
		nSize = nMaxSize;

	// Return the correct size
	return nSize;
}

/**
*  @brief
*    Destroys the used texture buffer
*/
void Texture::DestroyTextureBuffer()
{
	// If this is the last handler using this texture buffer, kill the texture buffer
	if (!m_bShareTextureBuffer) {
		TextureBuffer *pTextureBuffer = static_cast<TextureBuffer*>(m_pTextureBufferHandler->GetResource());
		if (pTextureBuffer) {
			if (pTextureBuffer->GetNumOfHandlers() <= 1)
				delete pTextureBuffer;
		}
	}
	m_pTextureBufferHandler->SetResource(nullptr);
}


//[-------------------------------------------------------]
//[ Public virtual PLCore::Resource functions             ]
//[-------------------------------------------------------]
bool Texture::LoadByFilename(const String &sFilename, const String &sParams, const String &sMethod)
{
	// Unload texture
	Unload();

	// Check parameter
	if (sFilename.GetLength()) {
		// Log output
		PL_LOG(Debug, "Load texture: " + sFilename)

		// Load the image
		Image cImage;
		if (cImage.LoadByFilename(sFilename, sParams, sMethod)) {
			// Get the image buffer
			ImageBuffer *pImageBuffer = cImage.GetBuffer();
			if (pImageBuffer) {
				// Get texture quality
				float fTextureQuality = GetTextureManager().GetTextureQuality();
				if (fTextureQuality <= 0.0f) {
					// If texture quality is 0 no textures at all are loaded beside the default one.
					// All other textures will use this default texture...
					if (sFilename != TextureManager::Default) {
						// Log output
						PL_LOG(Debug, "Texture quality is 0, using default texture instead of '" + sFilename + "'...")

						// Use the default texture
						const Texture *pTexture = GetTextureManager().GetByName(TextureManager::Default);
						if (pTexture && pTexture->GetTextureBuffer()) {
							// This texture is just sharing a texture buffer, so when this texture get's destroyed the texture buffer stays alive
							m_bShareTextureBuffer = true;
							pTexture->GetTextureBuffer()->AddHandler(*m_pTextureBufferHandler);

							// Done
							return true;
						} else {
							// Error!
							return false;
						}
					} else {
						fTextureQuality = 1.0f;
					}
				}

				// Check renderer
				Renderer &cRenderer = GetTextureManager().GetRendererContext().GetRenderer();

				// Settings
				// Take the next lower valid texture size? (0 = Texture manager settings, 1 = fit higher, 2 = fit lower)
				char nTextureFitLower = 0;
				// Rectangle texture (no power of 2 limitation)
				bool bRectangleTexture = false;
				// Are mipmaps allowed?
				bool bMipmapsAllowed = GetTextureManager().AreTextureMipmapsAllowed();
				// Is it allowed to resizing the texture?
				bool bAllowResize = true;
				// Force a given size? (-1 = no)
				int nForceWidth = -1, nForceHeight = -1, nForceDepth = -1;
				// The minimum allowed size for this texture (resizing)
				int nMinTextureBufferSize[3];
				int nTempMinTextureBufferSize[3];
				nTempMinTextureBufferSize[Vector3::Y] = nTempMinTextureBufferSize[Vector3::X] = 4;
				nTempMinTextureBufferSize[Vector3::Z] = 1;
				int nMaxTextureBufferSize[3];
				nMaxTextureBufferSize[Vector3::X] = cRenderer.GetCapabilities().nMaxTextureBufferSize;
				nMaxTextureBufferSize[Vector3::Z] = nMaxTextureBufferSize[Vector3::Y] = nMaxTextureBufferSize[Vector3::X];
				nMinTextureBufferSize[Vector3::X] = nTempMinTextureBufferSize[Vector3::X];
				nMinTextureBufferSize[Vector3::Y] = nTempMinTextureBufferSize[Vector3::Y];
				nMinTextureBufferSize[Vector3::Z] = nTempMinTextureBufferSize[Vector3::Z];
				// Color key
				int nCKR = 0, nCKG = 0, nCKB = 0, nCKTolerance = 0;
				bool bColorKey = false;

				// Backup the given filename
				m_sUrl = cImage.GetUrl();

				// Load 'plt'-file
				const String sFile = Url(m_sUrl).CutExtension() + ".plt";

				// Load XML document - Look for a 'plt'-file with more information about the image
				XmlDocument cDocument;
				if (cDocument.Load(sFile)) {
					// Get texture element
					const XmlElement *pTextureElement = cDocument.GetFirstChildElement("Texture");
					if (pTextureElement) {
						// Get the format version
						const int nVersion = pTextureElement->GetAttribute("Version").GetInt();

						// Unknown
						if (nVersion > 1) {
							PL_LOG(Error, cDocument.GetValue() + ": " + LoaderImpl::UnknownFormatVersion)

						// 1 (current) or ""/0 (same format as 1)
						} else if (nVersion == 1 || nVersion == 0) {
							// [DEPRECATED]
							if (nVersion == 0)
								PL_LOG(Warning, cDocument.GetValue() + ": " + LoaderImpl::DeprecatedFormatVersion)

							// Iterate through all general elements
							String sValue;
							const XmlElement *pElement = pTextureElement->GetFirstChildElement("General");
							while (pElement) {
								// Mipmaps
								sValue = pElement->GetAttribute("Mipmaps");
								if (sValue.GetLength())
									bMipmapsAllowed = sValue.GetBool();

								// Compression
								sValue = pElement->GetAttribute("Compression");
								if (sValue.GetLength()) {
									static const String sFormat[] = {"Default", "DXT1", "DXT3", "DXT5", "DXT5_xGxR", "LATC1", "LATC2", "LATC2_XYSwizzle", "None"};
									for (uint32 nFormat=0; nFormat<=None; nFormat++) {
										if (sValue == sFormat[nFormat]) {
											m_nCompressionHint = static_cast<ECompressionFormat>(nFormat);
											break;
										}
									}
								}

								// FitLower
								sValue = pElement->GetAttribute("FitLower");
								if (sValue.GetLength())
									nTextureFitLower = sValue.GetBool() ? 2 : 1;

								// Rectangle
								sValue = pElement->GetAttribute("Rectangle");
								if (sValue.GetLength())
									bRectangleTexture = sValue.GetBool();

								// Next element, please
								pElement = pElement->GetNextSiblingElement("General");
							}

							// Iterate through all resize elements
							pElement = pTextureElement->GetFirstChildElement("Resize");
							while (pElement) {
								// Active
								sValue = pElement->GetAttribute("Active");
								if (sValue.GetLength())
									bAllowResize = sValue.GetBool();

								// MinWidth
								if (pElement->QueryIntAttribute("MinWidth", &nMinTextureBufferSize[Vector3::X]) == XmlBase::Success) {
									uint32 nValue = GetCorrectTextureSize(nMinTextureBufferSize[Vector3::X], nTempMinTextureBufferSize[Vector3::X], nMaxTextureBufferSize[Vector3::X], bRectangleTexture);
									if (static_cast<uint32>(nMinTextureBufferSize[Vector3::X]) != nValue) {
										PL_LOG(Warning, '\'' + sFilename + "': " + nMinTextureBufferSize[Vector3::X] + " is an invalid texture width! " + nValue + " will be used instead.")
										nMinTextureBufferSize[Vector3::X] = nValue;
									}
								}

								// MinHeight
								if (pElement->QueryIntAttribute("MinHeight", &nMinTextureBufferSize[Vector3::Y]) == XmlBase::Success) {
									uint32 nValue = GetCorrectTextureSize(nMinTextureBufferSize[Vector3::Y], nTempMinTextureBufferSize[Vector3::Y], nMaxTextureBufferSize[Vector3::Y], bRectangleTexture);
									if (static_cast<uint32>(nMinTextureBufferSize[Vector3::Y]) != nValue) {
										PL_LOG(Warning, '\'' + sFilename + "': " + nMinTextureBufferSize[Vector3::Y] + " is an invalid texture height! " + nValue + " will be used instead.")
										nMinTextureBufferSize[Vector3::Y] = nValue;
									}
								}

								// MinDepth
								if (pElement->QueryIntAttribute("MinDepth", &nMinTextureBufferSize[Vector3::Z]) == XmlBase::Success) {
									uint32 nValue = GetCorrectTextureSize(nMinTextureBufferSize[Vector3::Z], nTempMinTextureBufferSize[Vector3::Z], nMaxTextureBufferSize[Vector3::Z], bRectangleTexture);
									if (static_cast<uint32>(nMinTextureBufferSize[Vector3::Z]) != nValue) {
										PL_LOG(Warning, '\'' + sFilename + "': " + nMinTextureBufferSize[Vector3::Z] + " is an invalid texture depth! " + nValue + " will be used instead.")
										nMinTextureBufferSize[Vector3::Z] = nValue;
									}
								}

								// Next element, please
								pElement = pElement->GetNextSiblingElement("Resize");
							}

							// Iterate through all force size elements
							pElement = pTextureElement->GetFirstChildElement("ForceSize");
							while (pElement) {
								// Width
								if (pElement->QueryIntAttribute("Width", &nForceWidth) == XmlBase::Success) {
									uint32 nValue = GetCorrectTextureSize(nForceWidth, nTempMinTextureBufferSize[Vector3::X], nMaxTextureBufferSize[Vector3::X], bRectangleTexture);
									if (nForceWidth != static_cast<int>(nValue)) {
										PL_LOG(Warning, '\'' + sFilename + "': " + nForceWidth + " is an invalid texture width! " + nValue + " will be used instead.")
										nForceWidth = nValue;
									}
								}

								// Height
								if (pElement->QueryIntAttribute("Height", &nForceHeight) == XmlBase::Success) {
									uint32 nValue = GetCorrectTextureSize(nForceHeight, nTempMinTextureBufferSize[Vector3::Y], nMaxTextureBufferSize[Vector3::Y], bRectangleTexture);
									if (nForceHeight != static_cast<int>(nValue)) {
										PL_LOG(Warning, '\'' + sFilename + "': " + nForceHeight + " is an invalid texture height! " + nValue + " will be used instead.")
										nForceHeight = nValue;
									}
								}

								// Depth
								if (pElement->QueryIntAttribute("Depth", &nForceDepth) == XmlBase::Success) {
									uint32 nValue = GetCorrectTextureSize(nForceDepth, nTempMinTextureBufferSize[Vector3::Z], nMaxTextureBufferSize[Vector3::Z], bRectangleTexture);
									if (nForceDepth != static_cast<int>(nValue)) {
										PL_LOG(Warning, '\'' + sFilename + "': " + nForceDepth + " is an invalid texture depth! " + nValue + " will be used instead.")
										nForceDepth = nValue;
									}
								}

								// Next element, please
								pElement = pElement->GetNextSiblingElement("ForceSize");
							}

							// Iterate through all color key elements
							pElement = pTextureElement->GetFirstChildElement("ColorKey");
							while (pElement) {
								bColorKey = true;

								// R
								pElement->QueryIntAttribute("R", &nCKR);

								// G
								pElement->QueryIntAttribute("G", &nCKG);

								// B
								pElement->QueryIntAttribute("B", &nCKB);

								// Tolerance
								pElement->QueryIntAttribute("Tolerance", &nCKTolerance);

								// Next element, please
								pElement = pElement->GetNextSiblingElement("ColorKey");
							}

						// No longer supported format version
						} else if (nVersion >= 0) {
							PL_LOG(Warning, cDocument.GetValue() + ": " + LoaderImpl::NoLongerSupportedFormatVersion)

						// Invalid format version (negative!)
						} else {
							PL_LOG(Error, cDocument.GetValue() + ": " + LoaderImpl::InvalidFormatVersion)
						}

						// We can only reduce the quality if this is allowed for this texture
						if (!bAllowResize)
							fTextureQuality = 1.0f;
					} else {
						PL_LOG(Error, '\'' + cDocument.GetValue() + "': " + "Can't find 'Texture' element")
					}
				}

				// Rectangle textures supported?
				if (!cRenderer.GetCapabilities().bTextureBufferRectangle)
					bRectangleTexture = false;	// Sorry, no rectangle textures supported!

				// We do NOT support palettes within textures!
				if (pImageBuffer->GetColorFormat() == ColorPalette) {
					// Apply effect
					cImage.ApplyEffect(ImageEffects::RemovePalette());

					// Update the image buffer pointer
					pImageBuffer = cImage.GetBuffer();
				}

				// Perform requested image manipulations
				// Get width
				uint32 nWidth = pImageBuffer->GetSize().x;
				m_vOriginalSize.x = nWidth;
				if (nWidth > 1) {
					nWidth = static_cast<uint32>(nWidth*fTextureQuality);
					if (static_cast<int>(nWidth) < nMinTextureBufferSize[Vector3::X])
						nWidth = nMinTextureBufferSize[Vector3::X];
				}
				// Get height
				uint32 nHeight = pImageBuffer->GetSize().y;
				m_vOriginalSize.y = nHeight;
				if (nHeight > 1) {
					nHeight = static_cast<uint32>(nHeight*fTextureQuality);
					if (static_cast<int>(nHeight) < nMinTextureBufferSize[Vector3::Y])
						nHeight = nMinTextureBufferSize[Vector3::Y];
				}
				// Get depth
				uint32 nDepth = pImageBuffer->GetSize().z;
				m_vOriginalSize.z = nDepth;
				if (nDepth > 1) {
					nDepth = static_cast<uint32>(nDepth*fTextureQuality);
					if (static_cast<int>(nDepth) < nMinTextureBufferSize[Vector3::Z])
						nDepth = nMinTextureBufferSize[Vector3::Z];
				}

				// By default, allow resize if it's required (we MUST scale if the texture dimension is not supported)
				bool bAllowResizeWidth  = true;
				bool bAllowResizeHeight = true;
				bool bAllowResizeDepth  = true;
				if (nForceWidth > -1) {
					nWidth			  = nForceWidth;
					bAllowResizeWidth = false;
				}
				if (nForceHeight > -1) {
					nHeight			   = nForceHeight;
					bAllowResizeHeight = false;
				}
				if (nForceDepth > -1) {
					nDepth			  = nForceDepth;
					bAllowResizeDepth = false;
				}

				// Resize
				bool bError = false;
				if (bRectangleTexture) {
					uint32 nMaxSize = cRenderer.GetCapabilities().nMaxRectangleTextureBufferSize;

					// Check if the texture size is correct
					if (nWidth > nMaxSize || nHeight > nMaxSize) {
						PL_LOG(Warning, '\'' + sFilename + "': Rectangle texture size " + nWidth + 'x' + nHeight + " isn't correct! (max: " + nMaxSize + 'x' + nMaxSize + ')')

						// Correct texture size
						if (nWidth > nMaxSize) {
							if (bAllowResizeWidth)
								nWidth = nMaxSize;
							else
								bError = true;
						}
						if (nHeight > nMaxSize) {
							if (bAllowResizeHeight)
								nHeight = nMaxSize;
							else
								bError = true;
						}
						if (nDepth > nMaxSize) {
							if (bAllowResizeDepth)
								nDepth = nMaxSize;
							else
								bError = true;
						}
					}
				} else {
					// Get maximum size
					uint32 nMaxSize;
					if (cImage.GetNumOfParts() == 6)
						nMaxSize = cRenderer.GetCapabilities().nMaxCubeTextureBufferSize;
					else {
						if (nDepth > 1)
							nMaxSize = cRenderer.GetCapabilities().nMax3DTextureBufferSize;
						else
							nMaxSize = cRenderer.GetCapabilities().nMaxTextureBufferSize;
					}

					// Check if the texture size is correct
					if (m_vOriginalSize.x > static_cast<int>(nMaxSize) || m_vOriginalSize.y > static_cast<int>(nMaxSize) || m_vOriginalSize.z > static_cast<int>(nMaxSize) ||
						(!cRenderer.GetCapabilities().bTextureBufferNonPowerOfTwo && (!Math::IsPowerOfTwo(m_vOriginalSize.x) || !Math::IsPowerOfTwo(m_vOriginalSize.y) || !Math::IsPowerOfTwo(m_vOriginalSize.z)))) {
						// Write a performance warning into the log if the original texture size is not optimal
						if (m_vOriginalSize.z == 1)
							PL_LOG(Warning, '\'' + sFilename + "': Texture size " + m_vOriginalSize.x + 'x' + m_vOriginalSize.y + " isn't correct! (max: " + nMaxSize + 'x' + nMaxSize + ')')
						else
							PL_LOG(Warning, '\'' + sFilename + "': Texture size " + m_vOriginalSize.x + 'x' + m_vOriginalSize.y + 'x' + m_vOriginalSize.z + " isn't correct! (max: " + nMaxSize + 'x' + nMaxSize + 'x' + nMaxSize + ')')
					}
					if (nWidth > nMaxSize || nHeight > nMaxSize || nDepth > nMaxSize ||
						(!cRenderer.GetCapabilities().bTextureBufferNonPowerOfTwo && (!Math::IsPowerOfTwo(nWidth) || !Math::IsPowerOfTwo(nHeight) || !Math::IsPowerOfTwo(nDepth)))) {
						// Correct texture size
						bool bTextureFit;
						if (nTextureFitLower == 2)
							bTextureFit = 1;
						else if (nTextureFitLower == 1)
							bTextureFit = 0;
						else
							bTextureFit = static_cast<TextureManager*>(GetManager())->GetTextureFit();

						// Check width
						if (!cRenderer.GetCapabilities().bTextureBufferNonPowerOfTwo && !Math::IsPowerOfTwo(nWidth)) {
							if (bAllowResizeWidth)
								nWidth = Math::GetNearestPowerOfTwo(nWidth, bTextureFit);
							else
								bError = true;
						}
						if (nWidth > nMaxSize) {
							if (bAllowResizeWidth)
								nWidth = nMaxSize;
							else
								bError = true;
						}

						// Check height
						if (!cRenderer.GetCapabilities().bTextureBufferNonPowerOfTwo && !Math::IsPowerOfTwo(nHeight)) {
							if (bAllowResizeHeight)
								nHeight = Math::GetNearestPowerOfTwo(nHeight, bTextureFit);
							else
								bError = true;
						}
						if (nHeight > nMaxSize) {
							if (bAllowResizeHeight)
								nHeight = nMaxSize;
							else
								bError = true;
						}

						// Check depth
						if (!cRenderer.GetCapabilities().bTextureBufferNonPowerOfTwo && !Math::IsPowerOfTwo(nDepth)) {
							if (bAllowResizeDepth)
								nDepth = Math::GetNearestPowerOfTwo(nDepth, bTextureFit);
							else
								bError = true;
						}
						if (nDepth > nMaxSize) {
							if (bAllowResizeDepth)
								nDepth = nMaxSize;
							else
								bError = true;
						}
					}
				}

				// Scale
				if (!bError) {
					// Scale the image if required
					if (nWidth  != static_cast<uint32>(pImageBuffer->GetSize().x) ||
						nHeight != static_cast<uint32>(pImageBuffer->GetSize().y) ||
						nDepth  != static_cast<uint32>(pImageBuffer->GetSize().z)) {
						if (nDepth == 1)
							PL_LOG(Debug, '\'' + sFilename + "': Scale texture dimension from " + m_vOriginalSize.x + 'x' + m_vOriginalSize.y + " to " + nWidth + 'x' + nHeight)
						else
							PL_LOG(Debug, '\'' + sFilename + "': Scale texture dimension from " + m_vOriginalSize.x + 'x' + m_vOriginalSize.y + 'x' + m_vOriginalSize.z + " to " + nWidth + 'x' + nHeight + 'x' + nDepth)

						// Apply scale - we can scale by using another mipmap as base map :D
						cImage.ApplyEffect(ImageEffects::Scale(Vector3i(nWidth, nHeight, nDepth), true));

						// Update the image buffer pointer
						pImageBuffer = cImage.GetBuffer();
					}

					// Check compression format
					if (m_nCompressionHint == Default) {
						switch (pImageBuffer->GetCompression()) {
							case CompressionDXT1:
								m_nCompressionHint = DXT1;
								break;

							case CompressionDXT3:
								m_nCompressionHint = DXT3;
								break;

							case CompressionDXT5:
								m_nCompressionHint = DXT5;
								break;

							case CompressionLATC1:
								m_nCompressionHint = LATC1;
								break;

							case CompressionLATC2:
								m_nCompressionHint = LATC2;
								break;

							case CompressionNone:
							default:
								m_nCompressionHint = None;
								break;
						}
					}

					// Color key
					if (bColorKey) {
						// Apply color key
						cImage.ApplyEffect(ImageEffects::ColorKey(Color3(static_cast<uint8>(nCKR), static_cast<uint8>(nCKG), static_cast<uint8>(nCKB)), static_cast<float>(nCKTolerance)/255.0f));

						// Update the image buffer pointer
						pImageBuffer = cImage.GetBuffer();
					}

					// Setup texture flags
					uint32 nTextureFlags = bMipmapsAllowed ? TextureBuffer::Mipmaps : 0;

					// Is texture compression allowed in general? (for none floating point formats only)
					TextureBuffer::EPixelFormat nInternalFormat = TextureBuffer::Unknown;
					if (pImageBuffer->GetDataFormat() != DataHalf && pImageBuffer->GetDataFormat() != DataFloat && GetTextureManager().IsTextureCompressionAllowed()) {
						// Check if texture compression should be used internally and the image is currently not compressed
						switch (m_nCompressionHint) {
							case Default:
								// This case is handled during texture loading above
								break;

							case DXT1:
								nTextureFlags   |= TextureBuffer::Compression;
								nInternalFormat  = TextureBuffer::DXT1;
								break;

							case DXT3:
								nTextureFlags   |= TextureBuffer::Compression;
								nInternalFormat  = TextureBuffer::DXT3;
								break;

							case DXT5:
							case DXT5_xGxR:
								nTextureFlags   |= TextureBuffer::Compression;
								nInternalFormat  = TextureBuffer::DXT5;
								break;

							case LATC1:
								nTextureFlags   |= TextureBuffer::Compression;
								nInternalFormat  = TextureBuffer::LATC1;
								break;

							case LATC2:
							case LATC2_XYSwizzle:
								nTextureFlags   |= TextureBuffer::Compression;
								nInternalFormat  = TextureBuffer::LATC2;
								break;

							case None:
							default:
								// No texture compression is used
								break;
						}
					} else {
						// If 'DXT5 xGxR' or 'LATC2_XYSwizzle' is used, the data is also uncompressed... BUT the data is STILL swizzled!
						if (m_nCompressionHint != Texture::DXT5_xGxR && m_nCompressionHint != Texture::LATC2_XYSwizzle)
							m_nCompressionHint = None; // Do never ever use texture compression!
					}

					// Currently, GrayscaleA and RGB is not supported by half/float formats, so, just convert to RGBA so we're still able to use the image data as texture!
					if ((pImageBuffer->GetDataFormat() == DataHalf || pImageBuffer->GetDataFormat() == DataFloat) && (pImageBuffer->GetColorFormat() != ColorGrayscale || pImageBuffer->GetColorFormat() != ColorRGB))
						cImage.ApplyEffect(ImageEffects::Convert((pImageBuffer->GetDataFormat() == DataHalf) ? DataHalf : DataFloat, ColorRGBA));

					// Create the renderer texture buffer resource
					TextureBuffer *pTextureBuffer;
					if (cImage.GetNumOfParts() == 6) {
						// Cube texture buffer
						pTextureBuffer = reinterpret_cast<TextureBuffer*>(cRenderer.CreateTextureBufferCube(cImage, nInternalFormat, nTextureFlags));
					} else if (nDepth != 1) {
						// 3D texture buffer
						pTextureBuffer = reinterpret_cast<TextureBuffer*>(cRenderer.CreateTextureBuffer3D(cImage, nInternalFormat, nTextureFlags));
					} else if (nWidth == 1 || nHeight == 1) {
						// 1D texture buffer
						pTextureBuffer = reinterpret_cast<TextureBuffer*>(cRenderer.CreateTextureBuffer1D(cImage, nInternalFormat, nTextureFlags));
					} else {
						// 2D/rectancle texture buffer
						if (bRectangleTexture)
							pTextureBuffer = reinterpret_cast<TextureBuffer*>(cRenderer.CreateTextureBufferRectangle(cImage, nInternalFormat, nTextureFlags));
						else
							pTextureBuffer = reinterpret_cast<TextureBuffer*>(cRenderer.CreateTextureBuffer2D(cImage, nInternalFormat, nTextureFlags));
					}

					// Renderer texture created?
					if (pTextureBuffer) {
						pTextureBuffer->AddHandler(*m_pTextureBufferHandler);

						// Backup the given filename
						m_sFilename = sFilename;

						// Done
						return true;
					} else {
						PL_LOG(Error, '\'' + sFilename + "': Can't create texture buffer!")
					}
				} else {
					PL_LOG(Error, '\'' + sFilename + "': Error: Invalid texture dimension!")
				}
			} else {
				PL_LOG(Error, "Image '" + sFilename + "' was opened successfully, but there's no image data?! (unknown RTTI load method provided?)")
			}
		} else {
			PL_LOG(Error, "Can't open '" + sFilename + '\'')
		}
	}

	// Reset the absolute filename of this loadable
	m_sUrl = "";

	// Error!
	return false;
}

bool Texture::SaveByFilename(const String &sFilename, const String &sParams, const String &sMethod)
{
	// [TODO] Implement me
	return false;
}

bool Texture::SaveByFile(File &cFile, const String &sParams, const String &sMethod)
{
	// [TODO] Implement me
	return false;
}

bool Texture::Unload()
{
	// Is there a texture loaded?
	if (m_pTextureBufferHandler->GetResource()) {
		// Delete texture buffer
		PL_LOG(Debug, "Delete texture: " + GetName())
		DestroyTextureBuffer();

		// Reset additional information
		m_nCompressionHint = Default;
		m_vOriginalSize    = Vector3i::Zero;
	}

	// Call base implementation
	return PLCore::Resource<Texture>::Unload();
}

String Texture::GetLoadableTypeName() const
{
	static const String sString = "Image";
	return sString;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
