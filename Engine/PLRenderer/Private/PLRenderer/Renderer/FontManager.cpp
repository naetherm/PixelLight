/*********************************************************\
 *  File: FontManager.cpp                                *
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
#include <PLCore/File/File.h>
#include <PLCore/Tools/LoadableManager.h>
#include "PLRenderer/Renderer/ResourceHandler.h"
#include "PLRenderer/Renderer/FontTexture.h"
#include "PLRenderer/Renderer/FontManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the default texture font
*/
Font *FontManager::GetDefaultFontTexture()
{
	// Set default font texture (internally not initialized until it's first real usage)
	if (!m_bDefaultFontTextureSet)
		SetDefaultFontTexture(GetFontTexture("Data/Fonts/LinLibertine_Re-2.7.9.9.otf", 15));

	// Return the default front texture
	return reinterpret_cast<FontTexture*>(m_pDefaultFontTextureHandler->GetResource());
}

/**
*  @brief
*    Sets the default texture font
*/
void FontManager::SetDefaultFontTexture(FontTexture *pFont)
{
	m_bDefaultFontTextureSet = true;
	m_pDefaultFontTextureHandler->SetResource(pFont);
}

/**
*  @brief
*    Returns a texture font
*/
FontTexture *FontManager::GetFontTexture(const String &sFilename, uint32 nSize, uint32 nResolution)
{
	// Check the parameters
	if (sFilename.GetLength() && nSize) {
		// Try to open the file
		File cFile;
		if (LoadableManager::GetInstance()->OpenFile(cFile, sFilename, false)) {
			// Get the absolute font filename
			const String sAbsFilename = cFile.GetUrl().GetUrl();

			// Is this font already registered?
			Iterator<FontTexture*> cIterator = m_lstFontTexture.GetIterator();
			while (cIterator.HasNext()) {
				FontTexture *pFont = cIterator.Next();
				if (pFont->GetFilename() == sAbsFilename && pFont->GetSize() == nSize && pFont->GetResolution() == nResolution)
					return pFont; // Return the found font
			}

			// Create the font
			FontTexture *pFont = reinterpret_cast<FontTexture*>(CreateFontTexture(cFile, nSize, nResolution));
			if (pFont)
				m_lstFontTexture.Add(pFont);

			// Return the created font
			return pFont;
		} else {
			PL_LOG(Error, "Failed to open font file '" + sFilename + '\'')
		}
	}

	// Error!
	return nullptr;
}

/**
*  @brief
*    Clears all texture fonts
*/
void FontManager::ClearFontTexture()
{
	// Texture fonts unregister within the FontTexture destructor
	while (m_lstFontTexture.GetNumOfElements())
		delete m_lstFontTexture[0];
}


//[-------------------------------------------------------]
//[ Public virtual FontManager functions                  ]
//[-------------------------------------------------------]
/**
*  @brief
*    Creates a texture font
*/
Font *FontManager::CreateFontTexture(const String &sFilename, uint32 nSize, uint32 nResolution)
{
	Font *pFont = nullptr;

	// Try to open the file
	File cFile;
	if (LoadableManager::GetInstance()->OpenFile(cFile, sFilename, false)) {
		// Create the font
		pFont = CreateFontTexture(cFile, nSize, nResolution);
	} else {
		PL_LOG(Error, "Failed to open font file '" + sFilename + '\'')
	}

	// Return the created font
	return pFont;
}


//[-------------------------------------------------------]
//[ Protected functions                                   ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
FontManager::FontManager(Renderer &cRenderer) :
	m_pRenderer(&cRenderer),
	m_pDefaultFontTextureHandler(new ResourceHandler()),
	m_bDefaultFontTextureSet(false)
{
}

/**
*  @brief
*    Destructor
*/
FontManager::~FontManager()
{
	// Cleanup
	delete m_pDefaultFontTextureHandler;
	ClearFontTexture();
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
FontManager::FontManager(const FontManager &cSource) :
	m_pRenderer(nullptr),
	m_pDefaultFontTextureHandler(nullptr),
	m_bDefaultFontTextureSet(false)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
FontManager &FontManager::operator =(const FontManager &cSource)
{
	// No implementation because the copy constructor is never used
	return *this;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
