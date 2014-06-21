/*********************************************************\
 *  File: ConsoleDefaultCommands.h                       *
 *      Console default execution function headers
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


#ifndef __PLENGINE_COMPOSITING_CONSOLEDEFAULTCOMMANDS_H__
#define __PLENGINE_COMPOSITING_CONSOLEDEFAULTCOMMANDS_H__
#pragma once


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLEngine {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class ConsoleCommand;


//[-------------------------------------------------------]
//[ Global functions                                      ]
//[-------------------------------------------------------]
// Standard commands
extern void PLConsoleCommandHelp(ConsoleCommand &cCommand);				/**< Show console help text */
extern void PLConsoleCommandList(ConsoleCommand &cCommand);				/**< List all console commands */
extern void PLConsoleCommandBulkyList(ConsoleCommand &cCommand);		/**< Detailed list all console commands */
extern void PLConsoleCommandClear(ConsoleCommand &cCommand);			/**< Clear console history */
extern void PLConsoleCommandAbout(ConsoleCommand &cCommand);			/**< Display some info about the engine */
extern void PLConsoleCommandVersion(ConsoleCommand &cCommand);			/**< Version information */
extern void PLConsoleCommandFPS(ConsoleCommand &cCommand);				/**< Toggle FPS display */
extern void PLConsoleCommandFPSLimit(ConsoleCommand &cCommand);			/**< Sets the FPS limit, 0 if there's no FPS limitation */
extern void PLConsoleCommandProfiling(ConsoleCommand &cCommand);		/**< Activates/deactivates the profiling system */
extern void PLConsoleCommandNextProfile(ConsoleCommand &cCommand);		/**< Selects the next profile group */
extern void PLConsoleCommandPreviousProfile(ConsoleCommand &cCommand);	/**< Selects the previous profile group */
// Debug commands
extern void PLConsoleCommandWireframes(ConsoleCommand &cCommand);		/**< Toggle render wireframe mode */
extern void PLConsoleCommandPoints(ConsoleCommand &cCommand);			/**< Toggle render point mode */
extern void PLConsoleCommandCoordinateAxis(ConsoleCommand &cCommand);	/**< Toggle coordinate axis visualization */
extern void PLConsoleCommandXZPlane(ConsoleCommand &cCommand);			/**< Toggle xz-plane visualization */
extern void PLConsoleCommandXYPlane(ConsoleCommand &cCommand);			/**< Toggle xy-plane visualization */
extern void PLConsoleCommandYZPlane(ConsoleCommand &cCommand);			/**< Toggle yz-plane visualization */
extern void PLConsoleCommandSetLogLevel(ConsoleCommand &cCommand);		/**< Set log level */
extern void PLConsoleCommandPause(ConsoleCommand &cCommand);			/**< Toggle pause modus */
extern void PLConsoleCommandTimeScale(ConsoleCommand &cCommand);		/**< Time scale */


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine


#endif // __PLENGINE_COMPOSITING_CONSOLEDEFAULTCOMMANDS_H__
