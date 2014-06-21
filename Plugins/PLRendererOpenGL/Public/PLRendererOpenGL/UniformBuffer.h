/*********************************************************\
 *  File: UniformBuffer.h                                *
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


#ifndef __PLRENDEREROPENGL_UNIFORMBUFFER_H__
#define __PLRENDEREROPENGL_UNIFORMBUFFER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Renderer/UniformBuffer.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    OpenGL uniform buffer
*/
class UniformBuffer : public PLRenderer::UniformBuffer {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class ShaderLanguageGLSL;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~UniformBuffer();

		/**
		*  @brief
		*    Returns the dynamic data (none UBO)
		*
		*  @return
		*    Dynamic data, can be a null pointer (if no UBO if used!)
		*/
		void *GetDynamicData() const;

		/**
		*  @brief
		*    Returns the OpenGL uniform buffer
		*
		*  @return
		*    The OpenGL uniform buffer, 0 on error
		*/
		PLCore::uint32 GetOpenGLUniformBuffer() const;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cRenderer
		*    Owner renderer
		*  @param[in] sShaderLanguage
		*    The name of the shader language the uniform buffer is using (for example "GLSL" or "Cg")
		*/
		UniformBuffer(PLRenderer::Renderer &cRenderer, const PLCore::String &sShaderLanguage);

		/**
		*  @brief
		*    Makes this uniform buffer to the renderers current uniform buffer
		*
		*  @return
		*   'true' if all went fine, else 'false'
		*/
		bool MakeCurrent();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLCore::String  m_sShaderLanguage;	/**< The name of the shader language the uniform buffer is using (for example "GLSL" or "Cg") */
		PLCore::uint32  m_nUniformBuffer;	/**< OpenGL uniform buffer (UBO) */
		void		   *m_pData;			/**< Dynamic buffer, can be a null pointer (none UBO) */
		void		   *m_pLockedData;		/**< Locked data, can be a null pointer */
		bool		    m_bLockReadOnly;	/**< Read only lock? */
		bool			m_bUpdateUBO;		/**< Do we need to update the UBO? */
		PLCore::uint32  m_nUsageAPI;		/**< API usage setting */


	//[-------------------------------------------------------]
	//[ Public virtual PLRenderer::UniformBuffer functions    ]
	//[-------------------------------------------------------]
	public:
		virtual PLCore::String GetShaderLanguage() const override;


	//[-------------------------------------------------------]
	//[ Public virtual PLRenderer::Buffer functions           ]
	//[-------------------------------------------------------]
	public:
		virtual bool IsAllocated() const override;
		virtual bool Allocate(PLCore::uint32 nElements, PLRenderer::Usage::Enum nUsage = PLRenderer::Usage::Dynamic, bool bManaged = true, bool bKeepData = false) override;
		virtual bool Clear() override;
		virtual void *Lock(PLCore::uint32 nFlag = PLRenderer::Lock::ReadWrite) override;
		virtual void *GetData() override;
		virtual bool Unlock() override;


	//[-------------------------------------------------------]
	//[ Protected virtual PLRenderer::Resource functions      ]
	//[-------------------------------------------------------]
	protected:
		virtual void BackupDeviceData(PLCore::uint8 **ppBackup) override;
		virtual void RestoreDeviceData(PLCore::uint8 **ppBackup) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL


#endif // __PLRENDEREROPENGL_UNIFORMBUFFER_H__
