/*********************************************************\
 *  File: ScriptBinding_PL_Timing.cpp                    *
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
#include <PLCore/Tools/Timing.h>
#include "PLScriptBindings/ScriptBinding_PL_Timing.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLScriptBindings {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(ScriptBinding_PL_Timing)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
float ScriptBinding_PL_Timing::GetTimeDifference()
{
	return m_pTiming->GetTimeDifference();
}

float ScriptBinding_PL_Timing::GetFramesPerSecond()
{
	return m_pTiming->GetFramesPerSecond();
}

bool ScriptBinding_PL_Timing::IsPaused()
{
	return m_pTiming->IsPaused();
}

void ScriptBinding_PL_Timing::Pause(bool bPause)
{
	m_pTiming->Pause(bPause);
}

float ScriptBinding_PL_Timing::GetTimeScaleFactor()
{
	return m_pTiming->GetTimeScaleFactor();
}

bool ScriptBinding_PL_Timing::SetTimeScaleFactor(float fFactor)
{
	return m_pTiming->SetTimeScaleFactor(fFactor);
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ScriptBinding_PL_Timing::ScriptBinding_PL_Timing() :
	m_pTiming(Timing::GetInstance())
{
}

/**
*  @brief
*    Destructor
*/
ScriptBinding_PL_Timing::~ScriptBinding_PL_Timing()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLScriptBindings
