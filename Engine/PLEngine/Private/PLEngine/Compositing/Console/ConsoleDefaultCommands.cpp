/*********************************************************\
 *  File: ConsoleDefaultCommands.cpp                     *
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
#include <PLCore/Runtime.h>
#include <PLCore/Log/Log.h>
#include <PLCore/Tools/Timing.h>
#include <PLCore/Tools/Profiling.h>
#include <PLCore/Application/CoreApplication.h>
#include <PLScene/Scene/SceneContainer.h>
#include "PLEngine/Compositing/Console/ConsoleCommand.h"
#include "PLEngine/Compositing/Console/SNConsole.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLEngine {


//[-------------------------------------------------------]
//[ Global functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Show console help text
*/
void PLConsoleCommandHelp(ConsoleCommand &cCommand)
{
	PL_LOG(Info, "'list' for a list of available commands")
	PL_LOG(Info, "'bulkylist' for a detailed list of available commands")
	PL_LOG(Info, "Press tab for automatic command completion")
	PL_LOG(Info, "Press left/right arrow keys to move the cursor")
	PL_LOG(Info, "  + hold shift to select text")
	PL_LOG(Info, "Press shift and delete/insert key to copy'n'past selected text")
	PL_LOG(Info, "Press up/down arrow keys to select previous commands")
	Log::GetInstance()->Flush();
}

/**
*  @brief
*    List all console commands
*/
void PLConsoleCommandList(ConsoleCommand &cCommand)
{
	if (cCommand.GetConsole())
		cCommand.GetConsole()->List(false);
}

/**
*  @brief
*    Detailed list all console commands
*/
void PLConsoleCommandBulkyList(ConsoleCommand &cCommand)
{
	if (cCommand.GetConsole())
		cCommand.GetConsole()->List(true);
}

/**
*  @brief
*    Clear console history
*/
void PLConsoleCommandClear(ConsoleCommand &cCommand)
{
	if (cCommand.GetConsole())
		cCommand.GetConsole()->ClearCommandHistory();
}

/**
*  @brief
*    Display some info about the engine
*/
void PLConsoleCommandAbout(ConsoleCommand &cCommand)
{
	PL_LOG(Info, "")
	PL_LOG(Info, Runtime::GetVersion().ToString())
	PL_LOG(Info, "Copyright (C) 2002-2013 by The PixelLight Team")
	PL_LOG(Info, "")
	Log::GetInstance()->Flush();
}

/**
*  @brief
*    Version information
*/
void PLConsoleCommandVersion(ConsoleCommand &cCommand)
{
	PL_LOG(Info, "")
	PL_LOG(Info, Runtime::GetVersion().ToString() + String::Format(" compiled at %s / %s", __DATE__, __TIME__))
	if (cCommand.GetConsole())
		PL_LOG(Info, cCommand.GetConsole()->GetDescription())
	PL_LOG(Info, "")
	Log::GetInstance()->Flush();
}

/**
*  @brief
*    Toggle FPS display
*/
void PLConsoleCommandFPS(ConsoleCommand &cCommand)
{
	CoreApplication *pApplication = CoreApplication::GetApplication();
	if (pApplication) {
		const bool bValue = pApplication->GetConfig().GetVar("PLEngine::DebugConfig", "ShowFPS").GetBool();
		pApplication->GetConfig().SetVar("PLEngine::DebugConfig", "ShowFPS", String::Format("%d", !bValue));
	}
}

/**
*  @brief
*    Sets the FPS limit, 0 if there's no FPS limitation
*/
void PLConsoleCommandFPSLimit(ConsoleCommand &cCommand)
{
	Timing::GetInstance()->SetFPSLimit(cCommand.GetVar(0).f);
}

/**
*  @brief
*    Activates/deactivates the profiling system
*/
void PLConsoleCommandProfiling(ConsoleCommand &cCommand)
{
	Profiling::GetInstance()->SetActive(!Profiling::GetInstance()->IsActive());
}

/**
*  @brief
*    Selects the next profile group
*/
void PLConsoleCommandNextProfile(ConsoleCommand &cCommand)
{
	Profiling::GetInstance()->SelectNextGroup();
}

/**
*  @brief
*    Selects the previous profile group
*/
void PLConsoleCommandPreviousProfile(ConsoleCommand &cCommand)
{
	Profiling::GetInstance()->SelectPreviousGroup();
}

/**
*  @brief
*    Toggle coordinate axis visualization
*/
void PLConsoleCommandCoordinateAxis(ConsoleCommand &cCommand)
{
	CoreApplication *pApplication = CoreApplication::GetApplication();
	if (pApplication) {
		const bool bValue = pApplication->GetConfig().GetVar("PLEngine::DebugConfig", "ShowCoordinateAxis").GetBool();
		pApplication->GetConfig().SetVar("PLEngine::DebugConfig", "ShowCoordinateAxis", String::Format("%d", !bValue));
	}
}

/**
*  @brief
*    Toggle xz-plane visualization
*/
void PLConsoleCommandXZPlane(ConsoleCommand &cCommand)
{
	CoreApplication *pApplication = CoreApplication::GetApplication();
	if (pApplication) {
		const bool bValue = pApplication->GetConfig().GetVar("PLEngine::DebugConfig", "ShowXZPlane").GetBool();
		pApplication->GetConfig().SetVar("PLEngine::DebugConfig", "ShowXZPlane", String::Format("%d", !bValue));
	}
}

/**
*  @brief
*    Toggle xy-plane visualization
*/
void PLConsoleCommandXYPlane(ConsoleCommand &cCommand)
{
	CoreApplication *pApplication = CoreApplication::GetApplication();
	if (pApplication) {
		const bool bValue = pApplication->GetConfig().GetVar("PLEngine::DebugConfig", "ShowXYPlane").GetBool();
		pApplication->GetConfig().SetVar("PLEngine::DebugConfig", "ShowXYPlane", String::Format("%d", !bValue));
	}
}

/**
*  @brief
*    Toggle yz-plane visualization
*/
void PLConsoleCommandYZPlane(ConsoleCommand &cCommand)
{
	CoreApplication *pApplication = CoreApplication::GetApplication();
	if (pApplication) {
		const bool bValue = pApplication->GetConfig().GetVar("PLEngine::DebugConfig", "ShowYZPlane").GetBool();
		pApplication->GetConfig().SetVar("PLEngine::DebugConfig", "ShowYZPlane", String::Format("%d", !bValue));
	}
}

/**
*  @brief
*    Set log level
*/
void PLConsoleCommandSetLogLevel(ConsoleCommand &cCommand)
{
	String sString = cCommand.GetVar(0).s;
	if (sString == "Quiet")
		Log::GetInstance()->SetLogLevel(Log::Quiet);
	else if (sString == "Always")
		Log::GetInstance()->SetLogLevel(Log::Always);
	else if (sString == "Critical")
		Log::GetInstance()->SetLogLevel(Log::Critical);
	else if (sString == "Error")
		Log::GetInstance()->SetLogLevel(Log::Error);
	else if (sString == "Warning")
		Log::GetInstance()->SetLogLevel(Log::Warning);
	else if (sString == "Info")
		Log::GetInstance()->SetLogLevel(Log::Info);
	else if (sString == "Debug")
		Log::GetInstance()->SetLogLevel(Log::Debug);
	else
		PL_LOG(Error, "Unknown log level '" + sString + "'. Must be Quiet, Always, Critical, Error, Warning, Info or Debug!")
}

/**
*  @brief
*    Toggle pause modus
*/
void PLConsoleCommandPause(ConsoleCommand &cCommand)
{
	Timing::GetInstance()->Pause(!Timing::GetInstance()->IsPaused());
}

/**
*  @brief
*    Time scale
*/
void PLConsoleCommandTimeScale(ConsoleCommand &cCommand)
{
	float fFactor = cCommand.GetVar(0).f;
	if (!Timing::GetInstance()->SetTimeScaleFactor(fFactor))
		PL_LOG(Error, String("Failed to set '") + fFactor + "' as time scale factor (note, a factor of <= 0 is NOT allowed)")
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine
