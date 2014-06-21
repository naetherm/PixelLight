/*********************************************************\
 *  File: PLPhysicsNewton.h                              *
 *      Main header file of this project
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


/**
*  @mainpage PLPhysicsNewton
*
*  @section intro_sec Introduction
*
*  This is the PLPhysicsNewton reference. Some parts of the class documentation are taken from the
*  Newton documentation. The plugin is no Newton wrapper, it only offers plugins which will make your
*  work using PixelLight and Newton more comfortable.
*
*  Newton uses the matrixes in row-major/left-handed order (this is the way DirectX stores matrices).
*  If you are using PL matrices (column-major/right-handed, like OpenGL does) you will need to transpose
*  the matrices into a local array, before you pass them to Newton.
*/


#ifndef __PLPHYSICSNEWTON_PLPHYSICSNEWTON_H__
#define __PLPHYSICSNEWTON_PLPHYSICSNEWTON_H__
#pragma once


//[-------------------------------------------------------]
//[ OS definitions                                        ]
//[-------------------------------------------------------]
// Windows platform
#ifdef WIN32
	#include "PLPhysicsNewton/PLPhysicsNewtonWindows.h"
#endif

// Linux platform
#ifdef LINUX
	#include "PLPhysicsNewton/PLPhysicsNewtonLinux.h"
#endif


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
namespace Newton {
	#include <Newton/Newton.h>
}


#endif // __PLPHYSICSNEWTON_PLPHYSICSNEWTON_H__
