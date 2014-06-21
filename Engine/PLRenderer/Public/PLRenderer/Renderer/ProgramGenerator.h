/*********************************************************\
 *  File: ProgramGenerator.h                             *
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


#ifndef __PLRENDERER_PROGRAMGENERATOR_H__
#define __PLRENDERER_PROGRAMGENERATOR_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/String/String.h>
#include <PLCore/Container/Array.h>
#include <PLCore/Container/HashMap.h>
#include <PLCore/Base/Event/EventHandler.h>
#include "PLRenderer/PLRenderer.h"


//[-------------------------------------------------------]
//[ Macro definitions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Adds a vertex shader flag to the program flags
*
*  @param[in] PROGRAMFLAGS
*    Program flags were to add the vertex shader flag
*  @param[in] FLAG
*    Flag to add
*/
#define PL_ADD_VS_FLAG(PROGRAMFLAGS, FLAG) PROGRAMFLAGS.AddVertexShaderFlag(static_cast<PLCore::uint32>(FLAG), #FLAG);

/**
*  @brief
*    Adds a fragment shader flag to the program flags
*
*  @param[in] PROGRAMFLAGS
*    Program flags were to add the fragment shader flag
*  @param[in] FLAG
*    Flag to add
*/
#define PL_ADD_FS_FLAG(PROGRAMFLAGS, FLAG) PROGRAMFLAGS.AddFragmentShaderFlag(static_cast<PLCore::uint32>(FLAG), #FLAG);


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Renderer;
class Program;
class VertexShader;
class FragmentShader;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Program generator for "Über-Shader"
*
*  @remarks
*    The program generator takes as input vertex shader and fragment shader source codes within a defined shader language and
*    returns dynamically composed program instances using given program flags. The generated programs are internally cached so
*    during runtime, they need to be dynamically compiled only once.
*/
class ProgramGenerator {


	//[-------------------------------------------------------]
	//[ Public classes                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Class holding program flags describing the requested program features
		*
		*  @remarks
		*    In order to tell the program generator the requested program features, an instance of this class is used.
		*    This is done by using program flags, which internally will be added in front of the shader source code
		*    as #define preprocessor directives. The goal of the class design was to to make the implementation as
		*    performant as possible due the heavy usage of this class during rendering. Therefore definition strings are just
		*    stored as pointer to "char" instead of "PLCore::String" to avoid any overhead produced by strings. As a result,
		*    this definitions strings must stay valid until "ProgramGenerator::GetProgram()" was called with the program flags.
		*    To reduce dynamic memory allocations during rendering, it's recommended to just use one program flags instance during
		*    rendering. When starting the definition of current program flags, call "ProgramFlags::Reset()" to reset the current
		*    states - this method doesn't produce much overhead. If "m_cProgramFlags" is an instance of "ProgramGenerator", vertex
		*    shader program flags are usually added by using "PL_ADD_VS_FLAG(m_cProgramFlags, VS_TWOSIDED)" and program flags
		*    for the fragment shader by using "PL_ADD_FS_FLAG(m_cProgramFlags, FS_ALPHATEST)". The actual flag values "VS_TWOSIDED"
		*    and "FS_ALPHATEST" are implementation depended and usually defined as enumeration values like "VS_TWOSIDED=1<<0",
		*    "VS_ANYTHING=1<<1" and so on. Up to 32 flags per shader type are supported. Flags are only added once.
		*/
		class Flags {
			public:
				Flags() :
					m_nVertexShaderFlags(0),
					m_nFragmentShaderFlags(0)
				{
					m_lstVertexShaderDefinitions.Resize(32, false);
					m_lstFragmentShaderDefinitions.Resize(32, false);
				}
				inline void Reset()
				{
					m_nVertexShaderFlags   = 0;
					m_nFragmentShaderFlags = 0;
					m_lstVertexShaderDefinitions.Reset();
					m_lstFragmentShaderDefinitions.Reset();
				}
				inline void AddVertexShaderFlag(PLCore::uint32 nFlag, const char *pszDefinition)
				{
					// Is this flag already set?
					if (!(m_nVertexShaderFlags & nFlag)) {
						// Nope, set it right now
						m_nVertexShaderFlags |= nFlag;
						m_lstVertexShaderDefinitions.Add(pszDefinition);
					}
				}
				inline void AddFragmentShaderFlag(PLCore::uint32 nFlag, const char *pszDefinition)
				{
					// Is this flag already set?
					if (!(m_nFragmentShaderFlags & nFlag)) {
						// Nope, set it right now
						m_nFragmentShaderFlags |= nFlag;
						m_lstFragmentShaderDefinitions.Add(pszDefinition);
					}
				}
				inline PLCore::uint32 GetVertexShaderFlags() const
				{
					return m_nVertexShaderFlags;
				}
				inline const PLCore::Array<const char *> &GetVertexShaderDefinitions() const
				{
					return m_lstVertexShaderDefinitions;
				}
				inline PLCore::uint32 GetFragmentShaderFlags() const
				{
					return m_nFragmentShaderFlags;
				}
				inline const PLCore::Array<const char *> &GetFragmentShaderDefinitions() const
				{
					return m_lstFragmentShaderDefinitions;
				}
			private:
				PLCore::uint32				 m_nVertexShaderFlags;
				PLCore::Array<const char *>  m_lstVertexShaderDefinitions;
				PLCore::uint32				 m_nFragmentShaderFlags;
				PLCore::Array<const char *>  m_lstFragmentShaderDefinitions;
		};
		struct GeneratedProgram {
			Program		   *pProgram;				/**< The program, always valid! */
			PLCore::uint32  nVertexShaderFlags;		/**< Vertex shader flags used to generate the program */
			PLCore::uint32  nFragmentShaderFlags;	/**< Fragment shader flags used to generate the program */
			void		   *pUserData;				/**< User data for the generated program, can be a null pointer, is destroyed automatically of the generated
														 program is destroyed, may also become a null pointer if for example the program became dirty */
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
		*    Renderer to use
		*  @param[in] sShaderLanguage
		*    The name of the shader language the shaders are using (for example "GLSL" or "Cg")
		*  @param[in] sVertexShader
		*    Vertex shader ("Über-Shader") source code to use, usually blank ASCII
		*  @param[in] sVertexShaderProfile
		*    Vertex shader profile to use (for example "arbvp1" or "glslv" when using Cg, e.g. "130" when using GLSL), if empty string, a default profile will be used which usually
		*    tries to use the best available profile that runs on most hardware
		*  @param[in] sFragmentShader
		*    Fragment shader ("Über-Shader") source code to use, usually blank ASCII
		*  @param[in] sFragmentShaderProfile
		*    Fragment shader profile to use (for example "arbfp1" or "glslf" when using Cg, e.g. "130" when using GLSL), if empty string, a default profile will be used which usually
		*    tries to use the best available profile that runs on most hardware
		*
		*  @note
		*    - When using GLSL, the profile is the GLSL version to use
		*/
		PLRENDERER_API ProgramGenerator(Renderer &cRenderer, const PLCore::String &sShaderLanguage,
										const PLCore::String &sVertexShader, const PLCore::String &sVertexShaderProfile,
										const PLCore::String &sFragmentShader, const PLCore::String &sFragmentShaderProfile);

		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API ~ProgramGenerator();

		/**
		*  @brief
		*    Returns the used renderer
		*
		*  @return
		*    The used renderer
		*/
		inline Renderer &GetRenderer() const;

		/**
		*  @brief
		*    Returns the name of the used shader language
		*
		*  @return
		*    The name of the used shader language
		*/
		inline PLCore::String GetShaderLanguage() const;

		/**
		*  @brief
		*    Returns the used vertex shader source code
		*
		*  @return
		*    The used vertex shader source code
		*/
		inline PLCore::String GetVertexShaderSourceCode() const;

		/**
		*  @brief
		*    Returns the used fragment shader source code
		*
		*  @return
		*    The used fragment shader source code
		*/
		inline PLCore::String GetFragmentShaderSourceCode() const;

		/**
		*  @brief
		*    Returns a program
		*
		*  @param[in] cFlags
		*    Program flags describing the requested program features
		*
		*  @return
		*    Generated program, can be a null pointer
		*/
		PLRENDERER_API GeneratedProgram *GetProgram(const Flags &cFlags);

		/**
		*  @brief
		*    Clears the cache of the program generator
		*
		*  @remarks
		*    All already generated and cached vertex shaders, fragment shaders and linked programs will be destroyed. Don't
		*    call this method frequently, else the shader programs must be regenerated to often which will have an impact on
		*    the performance.
		*/
		PLRENDERER_API void ClearCache();


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
		ProgramGenerator(const ProgramGenerator &cSource);

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
		ProgramGenerator &operator =(const ProgramGenerator &cSource);

		/**
		*  @brief
		*    Called when a program became dirty
		*
		*  @param[in] pProgram
		*    Program which became dirty
		*/
		void OnDirty(Program *pProgram);


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<Program*> EventHandlerDirty;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Input
		Renderer	   *m_pRenderer;				/**< Renderer to use, always valid! */
		PLCore::String  m_sShaderLanguage;			/**< The name of the shader language the shaders are using (for example "GLSL" or "Cg") */
		PLCore::String  m_sVertexShader;			/**< Vertex shader ("Über-Shader") source code to use, usually blank ASCII */
		PLCore::String  m_sVertexShaderProfile;		/**< Vertex shader profile to use, if empty string, a default profile will be used which usually
														 tries to use the best available profile that runs on most hardware */
		PLCore::String  m_sFragmentShader;			/**< Fragment shader ("Über-Shader") source code to use, usually blank ASCII */
		PLCore::String  m_sFragmentShaderProfile;	/**< Fragment shader profile to use, if empty string, a default profile will be used which usually
														 tries to use the best available profile that runs on most hardware */
		// Cache
		PLCore::Array<VertexShader*>					   m_lstVertexShaders;		/**< List of generated vertex shader instances */
		PLCore::HashMap<PLCore::uint32, VertexShader*>     m_mapVertexShaders;		/**< Program flags -> vertex shader instance */
		PLCore::Array<FragmentShader*>					   m_lstFragmentShaders;	/**< List of generated fragment shader instances */
		PLCore::HashMap<PLCore::uint32, FragmentShader*>   m_mapFragmentShaders;	/**< Program flags -> fragment shader instance */
		PLCore::Array<GeneratedProgram*>				   m_lstPrograms;			/**< List of generated program instances */
		PLCore::HashMap<PLCore::uint64, GeneratedProgram*> m_mapPrograms;			/**< Program flags -> program instance */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLRenderer/Renderer/ProgramGenerator.inl"


#endif // __PLRENDERER_PROGRAMGENERATOR_H__
