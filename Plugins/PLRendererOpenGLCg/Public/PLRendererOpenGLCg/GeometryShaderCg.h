/*********************************************************\
 *  File: GeometryShaderCg.h                             *
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


#ifndef __PLRENDEREROPENGLCG_GEOMETRYSHADERCG_H__
#define __PLRENDEREROPENGLCG_GEOMETRYSHADERCG_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <Cg/cgGL.h>
#include <PLRenderer/Renderer/GeometryShader.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRendererOpenGLCg {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    OpenGL Cg renderer geometry shader resource
*/
class GeometryShaderCg : public PLRenderer::GeometryShader {


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
		virtual ~GeometryShaderCg();

		/**
		*  @brief
		*    Returns the Cg profile
		*
		*  @return
		*    The Cg profile, can be CG_PROFILE_UNKNOWN
		*/
		CGprofile GetCgProfile() const;

		/**
		*  @brief
		*    Returns the Cg geometry program
		*
		*  @return
		*    The Cg geometry program, can be a null pointer, do not destroy it!
		*/
		CGprogram GetCgGeometryProgram() const;


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
		GeometryShaderCg(PLRenderer::Renderer &cRenderer);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		EInputPrimitiveType	 m_nInputPrimitiveType;		/**< Input primitive type */
		EOutputPrimitiveType m_nOutputPrimitiveType;	/**< Output primitive type */
		PLCore::uint32	 	 m_nNumOfOutputVertices;	/**< Num of output vertices */
		CGprofile			 m_pCgProfile;				/**< Used Cg profile, can be CG_PROFILE_UNKNOWN */
		PLCore::String		 m_sArguments;				/**< Optional shader compiler arguments */
		PLCore::String	 	 m_sEntry;					/**< User defined entry point */
		CGprogram			 m_pCgGeometryProgram;		/**< Cg geometry program, can be a null pointer */


	//[-------------------------------------------------------]
	//[ Public virtual PLRenderer::GeometryShader functions   ]
	//[-------------------------------------------------------]
	public:
		virtual EInputPrimitiveType GetInputPrimitiveType() const override;
		virtual EOutputPrimitiveType GetOutputPrimitiveType() const override;
		virtual PLCore::uint32 GetNumOfOutputVertices() const override;
		virtual bool SetSourceCode(const PLCore::String &sSourceCode, EInputPrimitiveType nInputPrimitiveType, EOutputPrimitiveType nOutputPrimitiveType, PLCore::uint32 nNumOfOutputVertices, const PLCore::String &sProfile = "", const PLCore::String &sArguments = "", const PLCore::String &sEntry = "") override;


	//[-------------------------------------------------------]
	//[ Public virtual PLRenderer::Shader functions           ]
	//[-------------------------------------------------------]
	public:
		virtual PLCore::String GetShaderLanguage() const override;
		virtual PLCore::String GetSourceCode() const override;
		virtual PLCore::String GetProfile() const override;
		virtual PLCore::String GetArguments() const override;
		virtual PLCore::String GetEntry() const override;
		virtual bool SetSourceCode(const PLCore::String &sSourceCode, const PLCore::String &sProfile = "", const PLCore::String &sArguments = "", const PLCore::String &sEntry = "") override;


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


#endif // __PLRENDEREROPENGLCG_GEOMETRYSHADERCG_H__
