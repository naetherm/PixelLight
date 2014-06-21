/*********************************************************\
 *  File: PLGraphicsLinuxInclude.h                       *
 *      Linux includes for PLGraphics
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


#ifndef __PLGRAPHICS_LINUXINCLUDE_H__
#define __PLGRAPHICS_LINUXINCLUDE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <X11/Xlib.h>


//[-------------------------------------------------------]
//[ Public definitions                                    ]
//[-------------------------------------------------------]
/**
*  @brief
*    Mouse buttons
*/
#ifdef Button1
	namespace PLGraphicsLinuxIncludes {
		enum {
			X11_Button1 = Button1,
			X11_Button2 = Button2,
			X11_Button3 = Button3,
			X11_Button4 = Button4,
			X11_Button5 = Button5
		};
	}

	#undef Button1
	#undef Button2
	#undef Button3
	#undef Button4
	#undef Button5

	namespace XLib {
		enum {
			Button1 = PLGraphicsLinuxIncludes::X11_Button1,
			Button2 = PLGraphicsLinuxIncludes::X11_Button2,
			Button3 = PLGraphicsLinuxIncludes::X11_Button3,
			Button4 = PLGraphicsLinuxIncludes::X11_Button4,
			Button5 = PLGraphicsLinuxIncludes::X11_Button5
		};
	}
#endif

	
/**
*  @brief
*    Misc
*/
#ifdef None
	namespace PLGraphicsLinuxIncludes {
		enum {
			X11_None = None,
			X11_Always = Always
		};
	}

	#undef None
	#undef Always

	namespace XLib {
		enum {
			None = PLGraphicsLinuxIncludes::X11_None,
			Always = PLGraphicsLinuxIncludes::X11_Always
		};
	}
#endif


#endif // __PLGRAPHICS_LINUXINCLUDE_H__
