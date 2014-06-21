/*********************************************************\
 *  File: ProgramCg.h                                    *
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


#ifndef __PLRENDEREROPENGLCG_PROGRAMCG_H__
#define __PLRENDEREROPENGLCG_PROGRAMCG_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <Cg/cg.h>
#include <PLCore/Container/HashMap.h>
#include <PLRenderer/Renderer/ResourceHandler.h>
#include <PLRendererOpenGL/Program.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRendererOpenGLCg {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    OpenGL Cg renderer program resource
*/
class ProgramCg : public PLRendererOpenGL::Program {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class ShaderLanguageCg;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ProgramCg();

		/**
		*  @brief
		*    Returns the Cg combined program
		*
		*  @param[in] bAutomaticLink
		*    If 'true', the program is automatically linked if it's not yet linked
		*
		*  @return
		*    The Cg combined program, do not destroy it!
		*/
		CGprogram GetCgCombinedProgram(bool bAutomaticLink = true);

		/**
		*  @brief
		*    Returns whether or not the program is currently linked
		*
		*  @return
		*    'true' if the program is currently linked, else 'false'
		*/
		bool IsLinked() const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cRenderer
		*    Owner renderer
		*/
		ProgramCg(PLRenderer::Renderer &cRenderer);

		/**
		*  @brief
		*    Denotes that a program relink is required
		*/
		void RelinkRequired();

		/**
		*  @brief
		*    Builds the attribute information
		*/
		void BuildAttributeInformation();

		/**
		*  @brief
		*    Destroys the attribute information
		*/
		void DestroyAttributeInformation();

		/**
		*  @brief
		*    Builds the uniform information
		*/
		void BuildUniformInformation();

		/**
		*  @brief
		*    Destroys the uniform information
		*/
		void DestroyUniformInformation();

		/**
		*  @brief
		*    Builds the uniform block information
		*/
		void BuildUniformBlockInformation();

		/**
		*  @brief
		*    Destroys the uniform block information
		*/
		void DestroyUniformBlockInformation();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Program
		CGprogram															m_pCgCombinedProgram;					/**< Cg combined program, can be a null pointer */
		bool																m_bGLSL;								/**< Is it an OpenGL GLSL program? If yes, we need some hacks to make it work properly. :/ */
		bool																m_bLinked;								/**< Is the Cg combined program currently linked? */
		bool																m_bLinkedFailed;						/**< Cg combined program link failed? */
		// Shaders
		PLRenderer::ResourceHandler											m_cVertexShaderHandler;					/**< Vertex shader resource handler */
		PLRenderer::ResourceHandler											m_cTessellationControlShaderHandler;	/**< Tessellation control shader resource handler */
		PLRenderer::ResourceHandler											m_cTessellationEvaluationShaderHandler;	/**< Tessellation evaluation shader resource handler */
		PLRenderer::ResourceHandler											m_cGeometryShaderHandler;				/**< Geometry shader resource handler */
		PLRenderer::ResourceHandler											m_cFragmentShaderHandler;				/**< Fragment shader resource handler */
		// Attributes
		bool																m_bAttributeInformationBuild;			/**< Attribute information already build? */
		PLCore::Array<PLRenderer::ProgramAttribute*>						m_lstAttributes;						/**< List of attributes */
		PLCore::HashMap<PLCore::String, PLRenderer::ProgramAttribute*>		m_mapAttributes;						/**< Hash map of names -> attributes */
		// Uniforms
		bool																m_bUniformInformationBuild;				/**< Uniform information already build? */
		PLCore::Array<PLRenderer::ProgramUniform*>							m_lstUniforms;							/**< List of uniforms */
		PLCore::HashMap<PLCore::String, PLRenderer::ProgramUniform*>		m_mapUniforms;							/**< Hash map of names -> uniforms */
		// Uniform blocks
		bool																m_bUniformBlockInformationBuild;		/**< Uniform block information already build? */
		PLCore::Array<PLRenderer::ProgramUniformBlock*>						m_lstUniformBlocks;						/**< List of uniform blocks */
		PLCore::HashMap<PLCore::String, PLRenderer::ProgramUniformBlock*>	m_mapUniformBlocks;						/**< Hash map of names -> uniform blocks */


	//[-------------------------------------------------------]
	//[ Public virtual PLRenderer::Program functions          ]
	//[-------------------------------------------------------]
	public:
		virtual PLCore::String GetShaderLanguage() const override;
		virtual PLRenderer::VertexShader *GetVertexShader() const override;
		virtual bool SetVertexShader(PLRenderer::VertexShader *pVertexShader) override;
		virtual PLRenderer::TessellationControlShader *GetTessellationControlShader() const override;
		virtual bool SetTessellationControlShader(PLRenderer::TessellationControlShader *pTessellationControlShader) override;
		virtual PLRenderer::TessellationEvaluationShader *GetTessellationEvaluationShader() const override;
		virtual bool SetTessellationEvaluationShader(PLRenderer::TessellationEvaluationShader *pTessellationEvaluationShader) override;
		virtual PLRenderer::GeometryShader *GetGeometryShader() const override;
		virtual bool SetGeometryShader(PLRenderer::GeometryShader *pGeometryShader) override;
		virtual PLRenderer::FragmentShader *GetFragmentShader() const override;
		virtual bool SetFragmentShader(PLRenderer::FragmentShader *pFragmentShader) override;
		virtual bool IsValid() override;
		virtual PLCore::String GetCompiledProgram() override;
		virtual const PLCore::Array<PLRenderer::ProgramAttribute*> &GetAttributes() override;
		virtual PLRenderer::ProgramAttribute *GetAttribute(const PLCore::String &sName) override;
		virtual const PLCore::Array<PLRenderer::ProgramUniform*> &GetUniforms() override;
		virtual PLRenderer::ProgramUniform *GetUniform(const PLCore::String &sName) override;
		virtual const PLCore::Array<PLRenderer::ProgramUniformBlock*> &GetUniformBlocks() override;
		virtual PLRenderer::ProgramUniformBlock *GetUniformBlock(const PLCore::String &sName) override;


	//[-------------------------------------------------------]
	//[ Protected virtual PLRenderer::Program functions       ]
	//[-------------------------------------------------------]
	protected:
		virtual bool MakeCurrent() override;
		virtual bool UnmakeCurrent() override;


	//[-------------------------------------------------------]
	//[ Private virtual PLRenderer::Resource functions        ]
	//[-------------------------------------------------------]
	private:
		virtual void BackupDeviceData(PLCore::uint8 **ppBackup) override;
		virtual void RestoreDeviceData(PLCore::uint8 **ppBackup) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGLCg


#endif // __PLRENDEREROPENGLCG_PROGRAMCG_H__
