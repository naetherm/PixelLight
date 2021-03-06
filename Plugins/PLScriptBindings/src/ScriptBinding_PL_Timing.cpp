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
pl_class_metadata(ScriptBinding_PL_Timing, "PLScriptBindings", PLCore::ScriptBinding, "Timing script binding class")
	// Properties
	pl_properties
		pl_property("Namespace",	"PL.Timing")
	pl_properties_end
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
	// Methods
	pl_method_0_metadata(GetTimeDifference,		pl_ret_type(float),			"Returns the past time since last frame (seconds)",																																																																						"")
	pl_method_0_metadata(GetFramesPerSecond,	pl_ret_type(float),			"Returns the current frames per second (FPS)",																																																																							"")
	pl_method_0_metadata(IsPaused,				pl_ret_type(bool),			"Returns 'true' when the timing is paused, else 'false'. If the timing is paused scene nodes, particles etc. are not updated. The timing will still be updated.",																																										"")
	pl_method_1_metadata(Pause,					pl_ret_type(void),	bool,	"Set pause mode. 'true' as first parameter when timing should be pause, else 'false'.",																																																													"")
	pl_method_0_metadata(GetTimeScaleFactor,	pl_ret_type(float),			"Returns the time scale factor. The global time scale factor should only be manipulated for debugging. A factor of <= 0 is NOT allowed because this may cause problems in certain situations, pause the timer instead by hand! Do NOT make the factor 'too' (for example > 4) extreme, this may cause problems in certain situations!",	"")
	pl_method_1_metadata(SetTimeScaleFactor,	pl_ret_type(bool),	float,	"Sets the time scale factor. Time scale as first parameter (a factor of <= 0 is NOT allowed!). Returns 'true' if all went fine, else 'false' (maybe the given factor is <= 0?).",																																						"")
pl_class_metadata_end(ScriptBinding_PL_Timing)


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
