/*********************************************************\
 *  File: Gui.cpp                                        *
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
#include <PLCore/Base/ClassManager.h>
#include <PLCore/Base/Class.h>
#include "PLGui/Gui/Gui.h"
#include "PLGui/Gui/Screen.h"
#include "PLGui/Gui/Base/GuiMessage.h"
#include "PLGui/Gui/Base/MessageFilter.h"
#include "PLGui/Gui/Resources/Timer.h"
#include "PLGui/Themes/Theme.h"
#include "PLGui/Themes/ThemeDesktop.h"
#include "PLGui/Widgets/Widget.h"
#include "PLGui/Widgets/Popups/Tooltip.h"
#if defined(WIN32)
	#include "PLGui/Backend/+Windows/GuiWindows.h"
#elif defined(ANDROID)
	#include "PLGui/Backend/+Android/GuiAndroid.h"
#elif defined(LINUX)
	#include "PLGui/Backend/+Linux/GuiLinux.h"
#endif

// [DEBUG]
//#include <PLCore/System/System.h>
//#include <PLCore/System/Console.h>


//[-------------------------------------------------------]
//[ Disable warnings                                      ]
//[-------------------------------------------------------]
PL_WARNING_PUSH
PL_WARNING_DISABLE(4355) // "'this' : used in base member initializer list"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
namespace PLGui {


//[-------------------------------------------------------]
//[ Class implementation                                  ]
//[-------------------------------------------------------]
pl_implement_class(Gui)


//[-------------------------------------------------------]
//[ Static data                                           ]
//[-------------------------------------------------------]
/**
*  @brief
*    Provides access to static instance of system GUI
*/
class SystemGui : public Singleton<SystemGui> {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Singleton<SystemGui>;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the instance of the system GUI
		*
		*  @return
		*    The instance of the system GUI, null pointer on terrible error
		*/
		Gui *GetGui() const
		{
			return m_pGui;
		}


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*/
		SystemGui() : m_pGui(new Gui())
		{
		}

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		SystemGui(const SystemGui &cSource) : m_pGui(new Gui())
		{
		}

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SystemGui()
		{
			delete m_pGui;
		}

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		SystemGui &operator =(const SystemGui &cSource)
		{
			return *this;
		}


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		Gui *m_pGui;	/**< GUI instance, always valid! */


};


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get system GUI
*/
Gui *Gui::GetSystemGui()
{
	// Return pointer to system GUI
	return SystemGui::GetInstance()->GetGui();
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Gui::Gui(const String &sGui) :
	m_pGuiImpl(nullptr),
	m_bShutdown(false),
	m_pTheme(nullptr),
	m_nHoverTime(2000),
	m_cFontManager(*this),
	m_cCursorManager(*this),
	m_cClipBoard(*this),
	m_bActive(true),
	m_nNextWidgetID(1),
	m_pDefaultScreen(nullptr),
	m_bMouseVisible(true),
	m_pRootWidget(nullptr),
	m_pMouseOverWidget(nullptr),
	m_pMouseOverWidgetNew(nullptr),
	m_pHoverTimer(nullptr),
	m_pFocusWidget(nullptr),
	m_pFocusWidgetNew(nullptr),
	m_pTooltip(nullptr),
	EventHandlerOnHoverTimer(&Gui::OnHoverTimer, this)
{
	// Check name of GUI backend
	if (sGui.GetLength()) {
		// Find GUI class
		const Class *pClass = ClassManager::GetInstance()->GetClass(sGui);
		if (pClass && pClass->IsDerivedFrom("PLGui::GuiImpl")) {
			// Create the specified GUI implementation
			m_pGuiImpl = static_cast<GuiImpl*>(pClass->Create(Params<Object*, Gui*>(this)));
		}
	} else {
		// Create system GUI
		#if defined(WIN32)
			m_pGuiImpl = new GuiWindows(this);
		#elif defined(ANDROID)
			m_pGuiImpl = new GuiAndroid(this);
		#elif defined(LINUX)
			m_pGuiImpl = new GuiLinux(this);
		#else
			// Unknown system
			#error "Unsupported platform"
		#endif
	}

	// Get list of screens
	m_pGuiImpl->EnumerateScreens(m_lstScreens);
	for (uint32 i=0; i<m_lstScreens.GetNumOfElements(); i++) {
		// Save default screen
		Screen *pScreen = m_lstScreens[i];
		if (pScreen->IsDefault()) {
			m_pDefaultScreen = pScreen;
		}
	}

	// Create dummy root widget
	m_pRootWidget = new Widget(this);

	// Initialize ClipBoard
	m_cClipBoard.InitClipBoard();

	// Create theme
	m_pTheme = new ThemeDesktop(*this);
}

/**
*  @brief
*    Destructor
*/
Gui::~Gui()
{
	// Clean up
	Shutdown();
}

/**
*  @brief
*    Get implementation
*/
GuiImpl *Gui::GetImpl() const
{
	// Return GUI implementation
	return m_pGuiImpl;
}

/**
*  @brief
*    Shut down GUI and clean up
*/
void Gui::Shutdown()
{
	// Already shut down?
	if (!m_bShutdown) {
		// Shutting down
		m_bShutdown = true;

		// Destroy all top-level widgets
		while (!m_lstTopLevelWidgets.IsEmpty()) {
			// Destroy widget
			m_lstTopLevelWidgets[0]->Destroy();

			// Process destroy-messages
			ProcessMessages();
		}

		// Clean up
		DeleteDestroyedWidgets();

		// Delete wrapper-widgets
		for (uint32 i=0; i<m_lstWrapperWidgets.GetNumOfElements(); i++) {
			delete m_lstWrapperWidgets[i];
		}

		// Delete root widget
		delete m_pRootWidget;

		// Destroy hover timer
		if (m_pHoverTimer) {
			delete m_pHoverTimer;
			m_pHoverTimer = nullptr;
		}

		// Destroy theme
		if (m_pTheme) {
			delete m_pTheme;
			m_pTheme = nullptr;
		}

		// Destroy screens
		for (uint32 i=0; i<m_lstScreens.GetNumOfElements(); i++) {
			delete m_lstScreens[i];
		}

		// Destroy message filters
		for (uint32 i=0; i<m_lstMessageFilters.GetNumOfElements(); i++) {
			delete m_lstMessageFilters[i];
		}

		// Destroy GUI implementation
		if (m_pGuiImpl) delete m_pGuiImpl;
	}
}

//[-------------------------------------------------------]
//[ Control and message loop                              ]
//[-------------------------------------------------------]
/**
*  @brief
*    Check if the GUI is still active
*/
bool Gui::IsActive() const
{
	// Return active-flag
	return m_bActive;
}

/**
*  @brief
*    Exit application
*/
void Gui::Exit()
{
	// Post OnExit()-Message
	PostMessage(GuiMessage::OnExit());
}

/**
*  @brief
*    Check if there are system messages waiting
*/
bool Gui::HasPendingMessages() const
{
	// Check if messages are waiting
	return m_pGuiImpl->HasPendingMessages();
}

/**
*  @brief
*    Process all waiting messages, blocks if no messages are waiting
*/
void Gui::ProcessMessages()
{
	// Clean up
	DeleteDestroyedWidgets();

	// Wait for the first message
	if (m_bActive) {
		m_pGuiImpl->ProcessMessage();
	}

	// Then process until no messages are waiting anymore
	while (m_pGuiImpl->HasPendingMessages()) {
		// [DEBUG]
//		System::GetInstance()->GetConsole().Print("ProcessMessage\n");

		// Process system messages
		m_pGuiImpl->ProcessMessage();
	}

	// [DEBUG]
//	System::GetInstance()->GetConsole().Print("--------------------------------------------------\n");
}

/**
*  @brief
*    Post message to the message queue
*/
void Gui::PostMessage(const GuiMessage &cMessage)
{
	// Post message to message queue
	m_pGuiImpl->PostMessage(cMessage);
}

/**
*  @brief
*    Send and process a message directly
*/
void Gui::SendMessage(const GuiMessage &cMessage)
{
	// Pass message to message filters
	for (uint32 i=0; i<m_lstMessageFilters.GetNumOfElements(); i++) {
		// Pass message to filter
		m_lstMessageFilters[i]->AddMessage(cMessage);
	}

	// Get widget
	Widget *pWidget = cMessage.GetWidget();

	// Process message
	switch (cMessage.GetType()) {
		// Exit application
		case MessageOnExit:
			// Set flag to leave application
			m_bActive = false;

			// Skip passing message to widget
			return;

		// Timer fired
		case MessageOnTimer:
		{
			// Get timer
			Timer *pTimer = cMessage.GetTimer();

			// Discard message, if the timer has already been destroyed
			if (pTimer && m_lstTimers.IsElement(pTimer)) {
				// Fire timer
				pTimer->Fire();
			}

			// Skip passing message to widget
			return;
		}

		// Widget has been destroyed
		case MessageOnDestroy:
			// Remove from parent widget
			if (pWidget->GetParent()) {
				pWidget->GetParent()->RemoveChild(pWidget);
			}

			// Remove from list of top-level widgets
			if (m_lstTopLevelWidgets.IsElement(pWidget)) {
				m_lstTopLevelWidgets.Remove(pWidget);
			}

			// Add widget to list of destroyed widgets
			m_lstDestroyedWidgets.Add(pWidget);

			// Pass message on to widget
			break;

		// Mouse has entered a widget
		case MessageOnMouseEnter:
			// Set new mouse-over widget
			if (m_pMouseOverWidget != pWidget) {
				// Update mouse-over widget
				m_pMouseOverWidgetNew = pWidget;
				UpdateMouseOverWidget();
			}
			break;

		// Mouse has left a widget
		case MessageOnMouseLeave:
			// Reset mouse-over widget
			if (m_pMouseOverWidget == pWidget) {
				// Update mouse-over widget
				m_pMouseOverWidgetNew = nullptr;
				UpdateMouseOverWidget();
			}
			break;

		// Widget has got the focus
		case MessageOnGetFocus:
			// Set focus widget
			if (m_pFocusWidget != pWidget) {
				// Update focus widget
				m_pFocusWidgetNew = pWidget;
				UpdateFocusWidget();
			}
			break;

		// Widget has lost the focus
		case MessageOnLooseFocus:
			// Set focus widget
			if (m_pFocusWidget == pWidget) {
				// Update focus widget
				m_pFocusWidgetNew = nullptr;
				UpdateFocusWidget();
			}
			break;

		case MessageOnUnknown:
		case MessageOnInternalMessage:
		case MessageOnWakeup:
		case MessageOnThemeChanged:
		case MessageOnUpdateContent:
		case MessageOnUpdateChildWidget:
		case MessageOnAddChildWidget:
		case MessageOnRemoveChildWidget:
		case MessageOnClose:
		case MessageOnCreate:
		case MessageOnShow:
		case MessageOnHide:
		case MessageOnEnable:
		case MessageOnDisable:
		case MessageOnActivate:
		case MessageOnDrawBackground:
		case MessageOnDraw:
		case MessageOnMove:
		case MessageOnSize:
		case MessageOnWindowState:
		case MessageOnEnterFullscreen:
		case MessageOnLeaveFullscreen:
		case MessageOnPreferredSize:
		case MessageOnAdjustContent:
		case MessageOnMouseOver:
		case MessageOnMouseMove:
		case MessageOnMouseHover:
		case MessageOnMousePosUpdate:
		case MessageOnMouseButtonDown:
		case MessageOnMouseButtonUp:
		case MessageOnMouseButtonClick:
		case MessageOnMouseButtonDoubleClick:
		case MessageOnMouseWheel:
		case MessageOnKeyDown:
		case MessageOnKeyUp:
		case MessageOnHotkey:
		case MessageOnDrop:
		case MessageOnUserMessage:
		default:
			// Nothing to do in here
			break;
	}

	// Pass message to widget
	if (pWidget) {
		pWidget->OnMessage(cMessage);
	}
}

/**
*  @brief
*    Wakeup message loop
*/
void Gui::WakeUp()
{
	// Post wakeup message
	PostMessage(GuiMessage::OnWakeup());
}

/**
*  @brief
*    Get message filters
*/
const Container<MessageFilter*> &Gui::GetMessageFilters() const
{
	// Return filter list
	return m_lstMessageFilters;
}

/**
*  @brief
*    Add message filter
*/
void Gui::AddMessageFilter(MessageFilter *pFilter)
{
	// Add filter to list
	m_lstMessageFilters.Add(pFilter);
}

/**
*  @brief
*    Remove and destroy message filter
*/
void Gui::RemoveMessageFilter(MessageFilter *pFilter)
{
	// Check parameter
	if (pFilter && m_lstMessageFilters.IsElement(pFilter)) {
		// Remove filter to list
		m_lstMessageFilters.Remove(pFilter);

		// Destroy filter
		delete pFilter;
	}
}

/**
*  @brief
*    Returns the root widget
*/
Widget *Gui::GetRootWidget() const
{
	// Return root widget
	return m_pRootWidget;
}

//[-------------------------------------------------------]
//[ Screens                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get list of screens (monitors)
*/
const Container<Screen*> &Gui::GetScreens() const
{
	// Return screen list
	return m_lstScreens;
}

/**
*  @brief
*    Get screen by name
*/
Screen *Gui::GetScreen(const String &sName) const
{
	// Find screen with that name
	for (uint32 i=0; i<m_lstScreens.GetNumOfElements(); i++) {
		Screen *pScreen = m_lstScreens[i];
		if (pScreen->GetName() == sName) {
			return pScreen;
		}
	}

	// Not found
	return nullptr;
}

/**
*  @brief
*    Get default screen
*/
Screen *Gui::GetDefaultScreen() const
{
	// Return default screen
	return m_pDefaultScreen;
}

//[-------------------------------------------------------]
//[ Resources                                             ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get font manager
*/
FontManager &Gui::GetFontManager()
{
	// Return font manager
	return m_cFontManager;
}

/**
*  @brief
*    Get cursor manager
*/
CursorManager &Gui::GetCursorManager()
{
	// Return cursor manager
	return m_cCursorManager;
}

/**
*  @brief
*    Get clipboard
*/
ClipBoard &Gui::GetClipBoard()
{
	// Return clipboard
	return m_cClipBoard;
}

//[-------------------------------------------------------]
//[ Gui info and state                                    ]
//[-------------------------------------------------------]
/**
*  @brief
*    Check if the GUI has a taskbar
*/
bool Gui::HasTaskbar() const
{
	// Return if the GUI has a taskbar
	return m_pGuiImpl->HasTaskbar();
}

/**
*  @brief
*    Check if the mouse cursor is visible or not
*/
bool Gui::IsMouseVisible() const
{
	// Return visible-flag
	return m_bMouseVisible;
}

/**
*  @brief
*    Set whether the mouse cursor is visible or not
*/
void Gui::SetMouseVisible(bool bVisible)
{
	// Set visible-flag
	m_bMouseVisible = bVisible;

	// Show or hide mouse cursor
	m_pGuiImpl->SetMouseVisible(bVisible);
}

/**
*  @brief
*    Get widget that the mouse is currently in
*/
Widget *Gui::GetMouseOverWidget() const
{
	// Return mouse over widget
	return m_pMouseOverWidget;
}

/**
*  @brief
*    Get widget that currently has the focus
*/
Widget *Gui::GetFocusWidget() const
{
	// Return focus widget
	return m_pFocusWidget;
}

/**
*  @brief
*    Display a tooltip
*/
void Gui::ShowTooltip(const PLMath::Vector2i &vPos, const String &sTooltip)
{
	// Create tooltip window
	if (!m_pTooltip) {
		m_pTooltip = new Tooltip(GetRootWidget());
	}

	// Show tooltip
	m_pTooltip->SetText(sTooltip);
	m_pTooltip->ShowTooltip(vPos);
}

//[-------------------------------------------------------]
//[ Theme                                                 ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get theme
*/
Theme *Gui::GetTheme() const
{
	// Return theme
	return m_pTheme;
}

/**
*  @brief
*    Set theme
*/
void Gui::SetTheme(Theme *pTheme)
{
	// If theme valid?
	if (pTheme) {
		// Destroy old theme
		if (m_pTheme) {
			delete m_pTheme;
			m_pTheme = nullptr;
		}

		// Set new theme
		m_pTheme = pTheme;

		// Apply theme to all top-level widgets
		for (uint32 i=0; i<m_lstTopLevelWidgets.GetNumOfElements(); i++) {
			m_lstTopLevelWidgets[i]->SetTheme(m_pTheme);
		}
	}
}

/**
*  @brief
*    Set theme
*/
void Gui::SetTheme(const String &sClass, const String &sOptions)
{
	// Find theme class
	const Class *pClass = ClassManager::GetInstance()->GetClass(sClass);
	if (!pClass) {
		// Try it with "PLGui::<Class>"
		pClass = ClassManager::GetInstance()->GetClass("PLGui::" + sClass);
	}

	// Class found?
	if (pClass && pClass->IsDerivedFrom("PLGui::Theme")) {
		// Create theme
		Theme *pTheme = static_cast<Theme*>(pClass->Create());
		if (pTheme) {
			// Set options
			if (sOptions.GetLength()) {
				pTheme->SetValues(sOptions);
			}

			// Set theme
			SetTheme(pTheme);
		}
	}
}

//[-------------------------------------------------------]
//[ Options                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get time after which a hove event occurs
*/
uint64 Gui::GetHoverTime() const
{
	// Return hover timeout
	return m_nHoverTime;
}

/**
*  @brief
*    Set time after which a hove event occurs
*/
void Gui::SetHoverTime(uint64 nTimeout)
{
	// Set hover timeout
	m_nHoverTime = nTimeout;
	if (m_nHoverTime == 0 && m_pHoverTimer) {
		// Destroy hover timer
		delete m_pHoverTimer;
		m_pHoverTimer = nullptr;
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Get new widget ID
*/
uint32 Gui::GetUniqueWidgetID()
{
	// Return and increase widget ID
	return m_nNextWidgetID++;
}

/**
*  @brief
*    Delete destroyed widgets
*/
void Gui::DeleteDestroyedWidgets()
{
	// Loop through lift of destroyed widgets
	for (uint32 i=0; i<m_lstDestroyedWidgets.GetNumOfElements(); i++) {
		// Get widget
		Widget *pWidget = m_lstDestroyedWidgets[i];

		// Remove widget from any list or reference
		if (m_lstTopLevelWidgets.IsElement(pWidget)) m_lstTopLevelWidgets.Remove(pWidget);
		if (m_lstWrapperWidgets.IsElement(pWidget))	 m_lstTopLevelWidgets.Remove(pWidget);
		if (m_pMouseOverWidget    == pWidget)		 m_pMouseOverWidget    = nullptr;
		if (m_pMouseOverWidgetNew == pWidget)		 m_pMouseOverWidgetNew = nullptr;
		if (m_pFocusWidget		  == pWidget)		 m_pFocusWidget		   = nullptr;
		if (m_pFocusWidgetNew	  == pWidget)		 m_pFocusWidgetNew	   = nullptr;

		// Delete widget
		delete pWidget;
	}

	// Clear list
	m_lstDestroyedWidgets.Clear();
}

/**
*  @brief
*    Add widget to list of top-level widgets
*/
void Gui::AddTopLevelWidget(Widget *pWidget)
{
	// Check if widget is already in the list
	if (!m_lstTopLevelWidgets.IsElement(pWidget) && !pWidget->m_bManaged) {
		// Add widget to list
		m_lstTopLevelWidgets.Add(pWidget);
	}
}

/**
*  @brief
*    Add widget to list of wrapper widgets
*/
void Gui::AddWrapperWidget(Widget *pWidget)
{
	// Check if widget is already in the list
	if (!m_lstWrapperWidgets.IsElement(pWidget)) {
		// Add widget to list
		m_lstWrapperWidgets.Add(pWidget);
	}
}

/**
*  @brief
*    Add timer to list of active timers
*/
void Gui::AddTimer(Timer *pTimer)
{
	// Check if timer is already in the list
	if (!m_lstTimers.IsElement(pTimer)) {
		// Add timer to list
		m_lstTimers.Add(pTimer);
	}
}

/**
*  @brief
*    Remove timer from list of active timers
*/
void Gui::RemoveTimer(Timer *pTimer)
{
	// Check if timer is in the list
	if (m_lstTimers.IsElement(pTimer)) {
		// Remove timer from list
		m_lstTimers.Remove(pTimer);
	}
}

/**
*  @brief
*    Update mouse-over widget
*/
void Gui::UpdateMouseOverWidget()
{
	// Has the mouse-over widget changed?
	if (m_pMouseOverWidget != m_pMouseOverWidgetNew) {
		// Get old mouse-over widget
		Widget *m_pMouseOverWidgetOld = m_pMouseOverWidget;

		// Save new mouse-over widget
		m_pMouseOverWidget = m_pMouseOverWidgetNew;

		// Get common parent of old mouse-over widget and the new one
		Widget *pParent = (m_pMouseOverWidgetOld && m_pMouseOverWidgetNew) ? m_pMouseOverWidgetNew->GetCommonParent(*m_pMouseOverWidgetOld) : nullptr;

		// Choose where to stop (include parent itself only when necessary)
		Widget *pStop = pParent;
		if (m_pMouseOverWidgetOld == pParent || m_pMouseOverWidgetNew == pParent) {
			pStop = (pParent ? pParent->GetParent() : nullptr);
		}

		// Inform old widgets, that they are no longer mouse-over widgets
		Widget *pWidget = m_pMouseOverWidgetOld;
		while (pWidget && pWidget != pStop) {
			// Send mouse-over message
			SendMessage(GuiMessage::OnMouseOver(pWidget, false));

			// Next widget
			pWidget = pWidget->GetParent();
		}

		// Inform new widgets, that they are now mouse-over widgets
		pWidget = m_pMouseOverWidgetNew;
		while (pWidget && pWidget != pStop) {
			// Send mouse-over message
			SendMessage(GuiMessage::OnMouseOver(pWidget, true));

			// Next widget
			pWidget = pWidget->GetParent();
		}

		// Start hover timer
		if (m_pMouseOverWidget) {
			// Create timer if it has not yet been created
			if (!m_pHoverTimer && m_nHoverTime > 0) {
				m_pHoverTimer = new Timer(*this);
				m_pHoverTimer->EventFire.Connect(EventHandlerOnHoverTimer);
			}

			// Start timer
			if (!m_pHoverTimer->IsActive())
				m_pHoverTimer->StartOnce(m_nHoverTime);
		}
	}
}

/**
*  @brief
*    Update focus widget
*/
void Gui::UpdateFocusWidget()
{
	// Has the focus widget changed?
	if (m_pFocusWidget != m_pFocusWidgetNew) {
		// Get old focus widget
		Widget *m_pFocusWidgetOld = m_pFocusWidget;

		// Save new focus widget
		m_pFocusWidget = m_pFocusWidgetNew;

		// Get common parent of old focus widget and the new one
		Widget *pParent = (m_pFocusWidgetOld && m_pFocusWidgetNew) ? m_pFocusWidgetNew->GetCommonParent(*m_pFocusWidgetOld) : nullptr;

		// Choose where to stop (include parent itself only when necessary)
		Widget *pStop = pParent;
		if (m_pFocusWidgetOld == pParent || m_pFocusWidgetNew == pParent) {
			pStop = (pParent ? pParent->GetParent() : nullptr);
		}

		// Inform old widgets, that they are no longer activated
		Widget *pWidget = m_pFocusWidgetOld;
		while (pWidget && pWidget != pStop) {
			// Send activate message
			SendMessage(GuiMessage::OnActivate(pWidget, false));

			// Next widget
			pWidget = pWidget->GetParent();
		}

		// Inform new widgets, that they are now activated
		pWidget = m_pFocusWidgetNew;
		while (pWidget && pWidget != pStop) {
			// Send activate message
			SendMessage(GuiMessage::OnActivate(pWidget, true));

			// Next widget
			pWidget = pWidget->GetParent();
		}
	}
}

/**
*  @brief
*    Callback for hover timer
*/
void Gui::OnHoverTimer()
{
	// Check if mouse-over window is valid
	if (m_pMouseOverWidget) {
		// Send OnHover message
		SendMessage(GuiMessage::OnMouseHover(m_pMouseOverWidget));
	}
}

/**
*  @brief
*    Create a widget implementation
*/
WidgetImpl *Gui::CreateWidgetImpl(Widget &cWidget) const
{
	// Create widget implementation
	return m_pGuiImpl->CreateWidgetImpl(cWidget);
}

/**
*  @brief
*    Create a graphics implementation
*/
GraphicsImpl *Gui::CreateGraphicsImpl(Graphics &cGraphics) const
{
	// Create graphics implementation
	return m_pGuiImpl->CreateGraphicsImpl(cGraphics);
}

/**
*  @brief
*    Create an image implementation
*/
ImageImpl *Gui::CreateImageImpl(Image &cImage) const
{
	// Create graphics implementation
	return m_pGuiImpl->CreateImageImpl(cImage);
}

/**
*  @brief
*    Create a font implementation
*/
FontImpl *Gui::CreateFontImpl(Font &cFont) const
{
	// Create font implementation
	return m_pGuiImpl->CreateFontImpl(cFont);
}

/**
*  @brief
*    Create a cursor implementation
*/
CursorImpl *Gui::CreateCursorImpl(Cursor &cCursor) const
{
	// Create cursor implementation
	return m_pGuiImpl->CreateCursorImpl(cCursor);
}

/**
*  @brief
*    Create a tray-icon implementation
*/
TrayIconImpl *Gui::CreateTrayIconImpl(TrayIcon &cTrayIcon) const
{
	// Create tray-icon implementation
	return m_pGuiImpl->CreateTrayIconImpl(cTrayIcon);
}

/**
*  @brief
*    Create a clipboard implementation
*/
ClipBoardImpl *Gui::CreateClipBoardImpl(ClipBoard &cClipBoard) const
{
	// Create clipboard implementation
	return m_pGuiImpl->CreateClipBoardImpl(cClipBoard);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGui


//[-------------------------------------------------------]
//[ Compiler settings                                     ]
//[-------------------------------------------------------]
PL_WARNING_POP
