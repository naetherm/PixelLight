/*********************************************************\
 *  File: CgContext.h                                    *
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


#ifndef __PLRENDEREROPENGLCG_CGCONTEXT_H__
#define __PLRENDEREROPENGLCG_CGCONTEXT_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <Cg/cgGL.h>
#include <PLCore/String/String.h>
#include <PLCore/Core/AbstractContext.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRendererOpenGLCg {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Cg context - Static class with some useful OpenGL Cg shader tool methods
*/
class CgContext : public PLCore::AbstractContext {


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Adds a Cg context reference
		*
		*  @note
		*    - Should be done when creating for example a vertex shader
		*    - If this is the first reference, the internal Cg context is created automatically
		*/
		static void AddCgContextReference();

		/**
		*  @brief
		*    Releases a Cg context reference
		*
		*  @note
		*    - Should be done when destroying for example a vertex shader
		*    - If this is the last reference, the internal Cg context is destroyed automatically
		*/
		static void ReleaseCgContextReference();

		/**
		*  @brief
		*    Returns the Cg context
		*
		*  @return
		*    The Cg context, can be a null pointer
		*/
		static CGcontext GetCgContext();

		/**
		*  @brief
		*    Creates a Cg program
		*
		*  @param[in] pCgProfile
		*    Cg profile the created Cg program should use
		*  @param[in] sSourceCode
		*    Shader source code, usually blank ASCII code
		*  @param[in] sArguments
		*    Optional shader compiler arguments
		*  @param[in] sEntry
		*    Entry point, if empty string, "main" is used as default
		*
		*  @return
		*    The creates Cg program, a null pointer on error
		*
		*  @note
		*    - There must be at least one reference to the Cg context
		*/
		static CGprogram CreateCgProgram(CGprofile pCgProfile, const PLCore::String &sSourceCode, const PLCore::String &sArguments, const PLCore::String &sEntry);


	//[-------------------------------------------------------]
	//[ Private static Cg callback functions                  ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Internal Cg error callback function
		*/
		static void CgErrorCallback();


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static CGcontext	  m_pCgContext;			/**< Cg shader program context, can be a null pointer */
		static PLCore::uint32 m_nCgContexCounter;	/**< Cg context counter */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGLCg


#endif // __PLRENDEREROPENGLCG_CGCONTEXT_H__
