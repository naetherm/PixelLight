/*********************************************************\
 *  File: ShaderToolsGLSL.cpp                            *
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
#include "PLRendererOpenGL/Extensions.h"
#include "PLRendererOpenGL/ShaderToolsGLSL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRendererOpenGL {


//[-------------------------------------------------------]
//[ Public static methods                                 ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sets and compiles the source code of an OpenGL shader
*/
bool ShaderToolsGLSL::SetSourceCode(GLuint nOpenGLShader, const String &sSourceCode)
{
	// Load the shader source
	const char *pszSourceCode = sSourceCode.GetASCII();
	glShaderSourceARB(nOpenGLShader, 1, &pszSourceCode, nullptr);

	// Compile the shader
	glCompileShaderARB(nOpenGLShader);

	// Check the compile status
	GLint nCompiled = GL_FALSE;
	glGetObjectParameterivARB(nOpenGLShader, GL_OBJECT_COMPILE_STATUS_ARB, &nCompiled);
	if (nCompiled == GL_TRUE) {
		// All went fine, return the shader
		return true;
	} else {
		// Error, failed to compile the shader!
		WriteInformationIntoLog(nOpenGLShader);

		// Error!
		return false;
	}
}

/**
*  @brief
*    Writes program information into the log
*/
void ShaderToolsGLSL::WriteInformationIntoLog(GLuint nOpenGLObject)
{
	// Get the length of the information (including a null termination)
	GLint nInformationLength = 0;
	glGetObjectParameterivARB(nOpenGLObject, GL_OBJECT_INFO_LOG_LENGTH_ARB, &nInformationLength);
	if (nInformationLength > 1) {
		// Allocate memory for the information
		char *pszInformationLog = new char[nInformationLength];

		// Get the information
		glGetInfoLogARB(nOpenGLObject, nInformationLength, nullptr, pszInformationLog);

		// Write the error into the log - the PixelLight string class takes over the control of the allocated string buffer
		String sInformationLog(pszInformationLog, false, nInformationLength-1);	// -1 = excluding the terminating zero
		PL_LOG(Error, sInformationLog)
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRendererOpenGL
