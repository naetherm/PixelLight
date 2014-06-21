/*********************************************************\
 *  File: PLGuiLinuxIncludes.h
 *      Linux includes for PLGui
\*********************************************************/


#ifndef __PLGUI_LINUXINCLUDE_H__
#define __PLGUI_LINUXINCLUDE_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>


//[-------------------------------------------------------]
//[ Public definitions                                    ]
//[-------------------------------------------------------]
/**
*  @brief
*    _NET_WM state actions
*/
#define _NET_WM_STATE_REMOVE			0    /**< Remove/unset property */
#define _NET_WM_STATE_ADD				1    /**< Add/set property */
#define _NET_WM_STATE_TOGGLE			2    /**< Toggle property  */

/**
*  @brief
*    XEMBED messages
*/
#define XEMBED_EMBEDDED_NOTIFY					 0	/**< Window has been embedded */
#define XEMBED_WINDOW_ACTIVATE					 1	/**< Window has been activated */
#define XEMBED_WINDOW_DEACTIVATE  				 2	/**< Window has been de-activated */
#define XEMBED_REQUEST_FOCUS		 			 3	/**< Window requests to get the focus */
#define XEMBED_FOCUS_IN 	 					 4	/**< Window has got the focus */
#define XEMBED_FOCUS_OUT  						 5	/**< Window has lost the focus */
#define XEMBED_FOCUS_NEXT 						 6	/**< Window has reached the end of it's logical tab (tabbing forward) */
#define XEMBED_FOCUS_PREV 						 7	/**< Window has reached the beginning of it's logical tab (tabbing backwards) */
#define XEMBED_MODALITY_ON 						10	/**< Window becomes shadowed by a modal dialog */
#define XEMBED_MODALITY_OFF 					11	/**< Window has been released from a modal dialog */
#define XEMBED_REGISTER_ACCELERATOR				12	/**< Register a key accelerator */
#define XEMBED_UNREGISTER_ACCELERATOR			13	/**< Unregister a key accelerator */
#define XEMBED_ACTIVATE_ACCELERATOR				14	/**< An accelerator has been activated */

/**
*  @brief
*    XEMBED flags
*/
#define XEMBED_MAPPED                     (1 << 0)	/**< Client is visible */

/**
*  @brief
*    XEMBED options for XEMBED_FOCUS_IN
*/
#define XEMBED_FOCUS_CURRENT					 0	/**< Do not move logical focus */
#define XEMBED_FOCUS_FIRST 						 1	/**< Activate first item in list */
#define XEMBED_FOCUS_LAST						 2	/**< Activate last item in list */

/**
*  @brief
*    XEMBED modifier keys for XEMBED_FOCUS_IN
*/
#define XEMBED_MODIFIER_SHIFT			  (1 << 0)	/**< Shift key */
#define XEMBED_MODIFIER_CONTROL			  (1 << 1)	/**< Ctrl key */
#define XEMBED_MODIFIER_ALT				  (1 << 2)	/**< ALT key */
#define XEMBED_MODIFIER_SUPER			  (1 << 3)	/**< Super key */
#define XEMBED_MODIFIER_HYPER			  (1 << 4)	/**< Hyper key */

/**
*  @brief
*    System tray messages
*/
#define SYSTEM_TRAY_REQUEST_DOCK				 0	/**< Dock into systray */
#define SYSTEM_TRAY_BEGIN_MESSAGE				 1	/**< Show message */
#define SYSTEM_TRAY_CANCEL_MESSAGE				 2	/**< Hide message */

/**
*  @brief
*    Mouse buttons
*/
#ifdef Button1
	namespace PLGuiLinuxIncludes {
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
			Button1 = PLGuiLinuxIncludes::X11_Button1,
			Button2 = PLGuiLinuxIncludes::X11_Button2,
			Button3 = PLGuiLinuxIncludes::X11_Button3,
			Button4 = PLGuiLinuxIncludes::X11_Button4,
			Button5 = PLGuiLinuxIncludes::X11_Button5
		};
	}
#endif

	
/**
*  @brief
*    Misc
*/
#ifdef None
	namespace PLGuiLinuxIncludes {
		enum {
			X11_None = None,
			X11_Always = Always,
			X11_Above = Above,
			X11_Success = Success
		};
	}

	#undef None
	#undef Always
	#undef Above
	#undef Success

	namespace XLib {
		enum {
			None = PLGuiLinuxIncludes::X11_None,
			Always = PLGuiLinuxIncludes::X11_Always,
			Above = PLGuiLinuxIncludes::X11_Above,
			Success = PLGuiLinuxIncludes::X11_Success
		};
	}
#endif


#endif // __PLGUI_LINUXINCLUDE_H__
