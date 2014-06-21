/*********************************************************\
 *  File: IndexBuffer.h                                  *
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


#ifndef __PLRENDEREROPENGL_INDEXBUFFER_H__
#define __PLRENDEREROPENGL_INDEXBUFFER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLRenderer/Renderer/IndexBuffer.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    OpenGL index buffer
*/
class IndexBuffer : public PLRenderer::IndexBuffer {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Renderer;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		virtual ~IndexBuffer();

		/**
		*  @brief
		*    Makes this index buffer to the renderers current index buffer
		*
		*  @return
		*   'true' if all went fine, else 'false'
		*
		*  @note
		*    - This function is used by the renderer when SetIndexBuffer() was called
		*/
		bool MakeCurrent();

		/**
		*  @brief
		*    Returns the dynamic data (none IBO)
		*
		*  @return
		*    Dynamic data, can be a null pointer (if no IBO if used!)
		*/
		void *GetDynamicData() const;


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
		IndexBuffer(PLRenderer::Renderer &cRenderer);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::uint32  m_nIndexBuffer;		/**< OpenGL index buffer (IBO) */
		void		   *m_pData;			/**< Dynamic buffer, can be a null pointer (none IBO) */
		void		   *m_pLockedData;		/**< Locked data, can be a null pointer */
		bool			m_bLockReadOnly;	/**< Read only lock? */
		bool			m_bUpdateIBO;		/**< Do we need to update the IBO? */
		PLCore::uint32  m_nUsageAPI;		/**< API usage setting */


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
	//[ Private virtual PLRenderer::Resource functions        ]
	//[-------------------------------------------------------]
	private:
		virtual void BackupDeviceData(PLCore::uint8 **ppBackup) override;
		virtual void RestoreDeviceData(PLCore::uint8 **ppBackup) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL


#endif // __PLRENDEREROPENGL_INDEXBUFFER_H__
