/*********************************************************\
 *  File: Buffer.h                                       *
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


#ifndef __PLRENDERER_BUFFER_H__
#define __PLRENDERER_BUFFER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLRenderer/Renderer/Types.h"
#include "PLRenderer/Renderer/Resource.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract renderer buffer resource base class
*/
class Buffer : public Resource {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~Buffer();

		/**
		*  @brief
		*    Returns the number of buffer elements
		*
		*  @return
		*    Number of buffer elements
		*/
		inline PLCore::uint32 GetNumOfElements() const;

		/**
		*  @brief
		*    Returns the buffer size (in bytes)
		*
		*  @return
		*    Buffer size (in bytes)
		*/
		inline PLCore::uint32 GetSize() const;

		/**
		*  @brief
		*    Returns the usage flag
		*
		*  @return
		*    Usage flag
		*/
		inline Usage::Enum GetUsage() const;

		/**
		*  @brief
		*    Returns whether the buffer is managed or not
		*
		*  @return
		*    'true' if the buffer is managed, else 'false'
		*
		*  @see
		*    - Allocate()
		*/
		inline bool IsManaged() const;

		/**
		*  @brief
		*    Returns the lock count
		*
		*  @return
		*    0 if the buffer is currently unlocked, else the lock count is returned
		*/
		inline PLCore::uint16 GetLockCount() const;


	//[-------------------------------------------------------]
	//[ Public virtual Buffer functions                       ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns whether the buffer is allocated or not
		*
		*  @return
		*    'true' if the buffer is allocated, else 'false'
		*/
		virtual bool IsAllocated() const = 0;

		/**
		*  @brief
		*    Allocates the buffer
		*
		*  @param[in] nElements
		*    Number of elements
		*  @param[in] nUsage
		*    Usage flag indicating the expected application usage pattern of the data store
		*  @param[in] bManaged
		*    If the render buffer is managed, for instance shadow buffers (system memory copies that
		*    allow for faster reads) are used internally.
		*  @param[in] bKeepData
		*    Should the current buffer content be restored after the buffer was reallocated?
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - If the buffer is already allocated, it is reallocated if required
		*    - When allocating a vertex buffer, you first have to define the vertex attributes
		*      before allocating the vertex buffer
		*    - After an index buffer is allocated, you can't edit the index element type
		*    - nUsage is useful for drivers because it enables them to decide where to place the buffer. In general, static
		*      buffers are placed in video memory and dynamic buffers are placed in accelerated graphics port (AGP)
		*      memory.
		*    - If the usage is 'Usage::Software', 'bManaged' will have no effect because the data is only hold
		*      within the system memory
		*    - It the buffer is still locked, it's forced to be unlocked immediately if all went fine
		*/
		virtual bool Allocate(PLCore::uint32 nElements, Usage::Enum nUsage = Usage::Dynamic, bool bManaged = true, bool bKeepData = false) = 0;

		/**
		*  @brief
		*    Clears the buffer
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @note
		*    - It the buffer is still locked, it's forced to be unlocked immediately if all went fine
		*/
		virtual bool Clear() = 0;

		/**
		*  @brief
		*    Locks the buffer
		*
		*  @param[in] nFlag
		*    Access flag which describes the operations which should be
		*    performed on the locked buffer. This can be any member of the
		*    lock enumeration type.
		*
		*  @return
		*    The data of the locked buffer, a null pointer on error
		*    (Maybe the buffer isn't allocated?)
		*
		*  @note
		*    - Each time you lock a buffer, you have to unlock it after you finished
		*      your work, too!
		*    - If the buffer is currently locked, do not call functions like Clear(), Allocate()
		*      etc., such functions will force the buffer to be unlocked and your pointers
		*      will become invalid!
		*    - Do NOT delete the returned data!
		*/
		virtual void *Lock(PLCore::uint32 nFlag = Lock::ReadWrite) = 0;

		/**
		*  @brief
		*    Returns the buffer data
		*
		*  @return
		*    The data of the locked buffer, a null pointer on error
		*    (Maybe the buffer isn't locked?)
		*
		*  @note
		*    - This function will only work if the buffer is locked (see Lock())
		*    - Note that the vertex element type size depends on the used API.
		*      For instance color, OpenGL will use 4*float to save the color value,
		*      but Direct3D will handle colors as PLCore::uint32! Therefore you should use this
		*      function carefully! We recommend to use the special functions provided by the
		*      vertex buffer interface to manipulate the vertex buffer data.
		*    - Do NOT delete the returned data!
		*/
		virtual void *GetData() = 0;

		/**
		*  @brief
		*    Unlocks the buffer
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*    (Maybe the buffer isn't locked?)
		*
		*  @see
		*    - Lock()
		*/
		virtual bool Unlock() = 0;


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
		*  @param[in] nType
		*    Resource type
		*/
		PLRENDERER_API Buffer(Renderer &cRenderer, EType nType);

		/**
		*  @brief
		*    Forces immediately buffer unlock
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*    (Maybe the buffer isn't locked?)
		*/
		inline bool ForceUnlock();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLCore::uint32 m_nElements;			/**< Number of buffer elements */
		PLCore::uint32 m_nSize;				/**< Buffer size (in bytes) */
		Usage::Enum	   m_nUsage;			/**< Usage flag */
		bool		   m_bManaged;			/**< Is the buffer managed? */
		PLCore::uint16 m_nLockCount;		/**< Lock count */
		PLCore::uint64 m_nLockStartTime;	/**< Time where the buffer was locked */


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
		Buffer(const Buffer &cSource);

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
		Buffer &operator =(const Buffer &cSource);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLRenderer/Renderer/Buffer.inl"


#endif // __PLRENDERER_BUFFER_H__
