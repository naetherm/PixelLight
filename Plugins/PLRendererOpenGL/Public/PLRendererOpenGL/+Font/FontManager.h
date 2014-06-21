/*********************************************************\
 *  File: FontManager.h                                  *
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


#ifndef __PLRENDEREROPENGL_FONTMANAGER_H__
#define __PLRENDEREROPENGL_FONTMANAGER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Renderer/ProgramGenerator.h>
#include <PLRenderer/Renderer/Backend/FontManagerBackend.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
typedef struct FT_LibraryRec_ *FT_Library;
namespace PLRenderer {
	class VertexBuffer;
	class ProgramUniform;
	class ProgramAttribute;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    OpenGL renderer font manager
*/
class FontManager : public PLRenderer::FontManagerBackend {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Direct pointers to uniforms & attributes of a generated program
		*/
		struct GeneratedProgramUserData {
			// Vertex shader attributes
			PLRenderer::ProgramAttribute *pVertexPosition;
			// Vertex shader uniforms
			PLRenderer::ProgramUniform *pGlyphSizePenPosition;
			PLRenderer::ProgramUniform *pTextureCoordinateMinMax;
			PLRenderer::ProgramUniform *pObjectSpaceToClipSpaceMatrix;
			// Fragment shader uniforms
			PLRenderer::ProgramUniform *pGlyphMap;
			PLRenderer::ProgramUniform *pColor;
		};


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cRenderer
		*    Owner renderer
		*/
		FontManager(PLRenderer::Renderer &cRenderer);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FontManager();

		/**
		*  @brief
		*    Returns the FreeType library object
		*
		*  @return
		*    The FreeType library object, a null pointer on error (in case of an terrible error), do not delete the instance!
		*/
		FT_Library *GetFTLibrary() const;

		/**
		*  @brief
		*    Returns the program for font rendering
		*
		*  @param[in] ppGeneratedProgramUserData
		*    If not a null pointer, this receives a pointer to an GeneratedProgramUserData instance for fast direct uniform access
		*
		*  @return
		*    The program for font rendering, can be a null pointer
		*
		*  @note
		*    - The program is using the default shader language of the renderer
		*    - The program has already proper vertex attributes set
		*/
		PLRenderer::Program *GetProgram(GeneratedProgramUserData **ppGeneratedProgramUserData = nullptr);


	//[-------------------------------------------------------]
	//[ Public virtual PLRenderer::FontManager functions      ]
	//[-------------------------------------------------------]
	public:
		virtual PLRenderer::Font *CreateFontTexture(PLCore::File &cFile, PLCore::uint32 nSize = 12, PLCore::uint32 nResolution = 96) override;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Returns the vertex buffer used for font rendering
		*
		*  @return
		*    The vertex buffer used for font rendering, a null pointer on error
		*/
		PLRenderer::VertexBuffer *GetVertexBuffer();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		FT_Library							*m_pFTLibrary;			/**< FreeType library object, a null pointer on error (in case of an terrible error) */
		PLRenderer::ProgramGenerator		*m_pProgramGenerator;	/**< Program generator, can be a null pointer */
		PLRenderer::ProgramGenerator::Flags	 m_cProgramFlags;		/**< Program flags as class member to reduce dynamic memory allocations */
		PLRenderer::VertexBuffer			*m_pVertexBuffer;		/**< Vertex buffer used for font rendering, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL


#endif // __PLRENDEREROPENGL_FONTMANAGER_H__
