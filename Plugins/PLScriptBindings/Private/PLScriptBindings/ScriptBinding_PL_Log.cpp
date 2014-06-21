/*********************************************************\
 *  File: ScriptBinding_PL_Log.cpp                       *
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
#include "PLScriptBindings/ScriptBinding_PL_Log.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLScriptBindings {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(ScriptBinding_PL_Log)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
void ScriptBinding_PL_Log::OutputAlways(String sText)
{
	m_pLog->Output(Log::Always, sText);
}

void ScriptBinding_PL_Log::OutputCritical(String sText)
{
	m_pLog->Output(Log::Critical, sText);
}

void ScriptBinding_PL_Log::OutputError(String sText)
{
	m_pLog->Output(Log::Error, sText);
}

void ScriptBinding_PL_Log::OutputWarning(String sText)
{
	m_pLog->Output(Log::Warning, sText);
}

void ScriptBinding_PL_Log::OutputInfo(String sText)
{
	m_pLog->Output(Log::Info, sText);
}

void ScriptBinding_PL_Log::OutputDebug(String sText)
{
	m_pLog->Output(Log::Debug, sText);
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ScriptBinding_PL_Log::ScriptBinding_PL_Log() :
	m_pLog(Log::GetInstance())
{
}

/**
*  @brief
*    Destructor
*/
ScriptBinding_PL_Log::~ScriptBinding_PL_Log()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScriptBindings
