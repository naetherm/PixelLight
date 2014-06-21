/*********************************************************\
 *  File: GuiPL.cpp                                      *
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
#include <PLCore/System/MutexGuard.h>
#include <PLCore/System/CriticalSection.h>
#include <PLCore/Frontend/Frontend.h>
#include <PLCore/Frontend/FrontendApplication.h>
#include <PLMath/Rectangle.h>
#include <PLGui/Gui/Gui.h>
#include <PLGui/Gui/Screen.h>
#include <PLGui/Gui/Base/Keys.h>
#include <PLGui/Gui/Base/GuiMessage.h>
#include <PLGui/Gui/Resources/Timer.h>
#include <PLGui/Gui/Resources/Graphics.h>
#include <PLGui/Application/GuiApplication.h>
#include <PLGui/Widgets/Widget.h>
#include <PLInput/Input/InputManager.h>
#include <PLInput/Input/Virtual/VirtualController.h>
#include <PLInput/Input/Devices/Mouse.h>
#include <PLInput/Input/Devices/Keyboard.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/Renderer.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Effect/EffectManager.h>
#include "PLFrontendPLGui/Compositing/WidgetPL.h"
#include "PLFrontendPLGui/Compositing/GraphicsPL.h"
#include "PLFrontendPLGui/Compositing/ImagePL.h"
#include "PLFrontendPLGui/Compositing/FontPL.h"
#include "PLFrontendPLGui/Compositing/CursorPL.h"
#include "PLFrontendPLGui/Compositing/ClipBoardPL.h"
#include "PLFrontendPLGui/Compositing/GuiPL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGui;
using namespace PLInput;
namespace PLFrontendPLGui {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(GuiPL)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
GuiPL::GuiPL(Gui *pGui) : GuiImpl(pGui),
	InputSemantic(this),
	EventHandlerTimer(&GuiPL::OnTimer, this),
	m_pGui(pGui),
	m_vScreenSize(1024, 768),
	m_pRenderer(nullptr),
	m_pMessageQueueCriticalSection(new CriticalSection()),
	m_pMouseOver(nullptr),
	m_pInputController(nullptr),
	m_pKeyTimer(new Timer(*pGui)),
	m_vSystemMousePos(Vector2i::NegativeOne)
{
	{ // Keyboard
		// Get keyboard input device
		Keyboard *pKeyboard = InputManager::GetInstance()->GetKeyboard();
		if (pKeyboard) {
			m_lstKeys.      Resize(pKeyboard->GetButtons().GetNumOfElements(), true, true);
			m_lstRepeatKeys.Resize(pKeyboard->GetButtons().GetNumOfElements(), true, true);
		}

		// Connect event handler to the key autorepeat timer and start the timer
		m_pKeyTimer->EventFire.Connect(EventHandlerTimer);
		m_pKeyTimer->Start(150);
	}

	{ // Mouse
		MemoryManager::Set(m_bMouseButtonPressed, 0, sizeof(bool)*NumOfMouseButtons);
	}
}

/**
*  @brief
*    Destructor
*/
GuiPL::~GuiPL()
{
	delete m_pKeyTimer;
	delete m_pMessageQueueCriticalSection;
}

/**
*  @brief
*    Set screen size
*/
void GuiPL::SetScreenSize(const Vector2i &vScreenSize)
{
	m_vScreenSize = vScreenSize;
}

/**
*  @brief
*    Returns the used renderer
*/
PLRenderer::Renderer *GuiPL::GetRenderer() const
{
	return m_pRenderer;
}

/**
*  @brief
*    Get virtual input controller
*/
VirtualController *GuiPL::GetInputController() const
{
	return m_pInputController;
}

/**
*  @brief
*    Set virtual input controller
*/
void GuiPL::SetInputController(VirtualController *pInputController)
{
	m_pInputController = pInputController;
}

/**
*  @brief
*    Renders the ingame GUI
*/
void GuiPL::Render()
{
	// As soon as we draw the first visible top-level widget, we need to setup some initial render states
	bool bFirstVisibleTopLevelWidget = true;

	// Backup render state
	const uint32 nFixedFillModeBackup = m_pRenderer->GetRenderState(PLRenderer::RenderState::FixedFillMode);

	// Iterate through the top-level widgets
	Iterator<Widget*> cIterator = m_lstTopLevelWidgets.GetEndIterator();
	while (cIterator.HasPrevious()) {
		// Get the current top-level widget
		Widget *pTopLevelWidget = cIterator.Previous();

		// Is the widget visible?
		if (pTopLevelWidget->IsVisible()) {
			// Is this the first visible top-level widget?
			if (bFirstVisibleTopLevelWidget) {
				// Setup some initial render states
				m_pRenderer->GetRendererContext().GetEffectManager().Use();
				m_pRenderer->SetRenderState(PLRenderer::RenderState::ScissorTestEnable,	true);						// Use scissor rectangle to clip against widget boundings
				m_pRenderer->SetRenderState(PLRenderer::RenderState::ZEnable,			false);						// We don't need depth test
				m_pRenderer->SetRenderState(PLRenderer::RenderState::ZWriteEnable,		false);						// We don't need depth test
				m_pRenderer->SetRenderState(PLRenderer::RenderState::CullMode,			PLRenderer::Cull::None);	// We don't need backface culling
				m_pRenderer->SetRenderState(PLRenderer::RenderState::FixedFillMode,		PLRenderer::Fill::Solid);	// Even in wireframe mode rendering, we want to see the ingame GUI
				m_pRenderer->GetDrawHelpers().Begin2DMode(0.0f, 0.0f, static_cast<float>(m_vScreenSize.x), static_cast<float>(m_vScreenSize.y));

				// The render states are now set
				bFirstVisibleTopLevelWidget = false;
			}

			// Render the current top-level widgets recursive
			RenderWidget(*pTopLevelWidget, m_pRenderer->GetViewport(), NoTransparency, PLGraphics::Color4::White);
		}
	}

	// Is the mouse cursor visible?
	if (m_pGui->IsMouseVisible()) {
		// [TODO] Draw the mouse cursor
	}

	// Reset render state
	if (!bFirstVisibleTopLevelWidget) {
		m_pRenderer->GetDrawHelpers().End2DMode();
		m_pRenderer->SetScissorRect();
	}
	m_pRenderer->SetRenderState(PLRenderer::RenderState::FixedFillMode, nFixedFillModeBackup);
}

/**
*  @brief
*    Updates the ingame GUI
*/
void GuiPL::Update()
{
	// Check if input is active
	// [TODO] Don't use devices directly, use a virtual controller instead and use "PLInput::InputManager::EventInputControllerFound"
	if ((m_pInputController && m_pInputController->GetActive()) || !m_pInputController) {
		// Update the ingame GUI keyboard
		UpdateKeyboard();

		// Update the ingame GUI mouse
		UpdateMouse();
	}
}


//[-------------------------------------------------------]
//[ Public virtual PLGui::GuiImpl functions               ]
//[-------------------------------------------------------]
bool GuiPL::HasPendingMessages()
{
	// Lock the GUI message queue
	const MutexGuard cMutexGuard(*m_pMessageQueueCriticalSection);

	// Is there anything within the GUI message queue?
	return (m_lstMessageQueue.GetNumOfElements() != 0);
}

void GuiPL::ProcessMessage()
{
	// Lock the GUI message queue
	const MutexGuard cMutexGuard(*m_pMessageQueueCriticalSection);

	// Is there anything within the GUI message queue?
	if (m_lstMessageQueue.GetNumOfElements()) {
		// Pop a message from the queue
		GuiMessage cMessage = m_lstMessageQueue.Get(0);
		m_lstMessageQueue.RemoveAtIndex(0);

		// Pass on the GUI message
		m_pGui->SendMessage(cMessage);
	}
}

void GuiPL::PostMessage(const GuiMessage &cMessage)
{
	// Lock the GUI message queue
	const MutexGuard cMutexGuard(*m_pMessageQueueCriticalSection);

	// Add the GUI message to the internal GUI message queue processed by GuiPL::ProcessMessage()
	m_lstMessageQueue.Add(cMessage);
}

void GuiPL::EnumerateScreens(List<Screen*> &lstScreens)
{
	// Add screen
	Screen *pScreen = new Screen(m_pGui);
	pScreen->SetName   ("Monitor0");
	pScreen->SetPos	   (Vector2i(0, 0));
	pScreen->SetSize   (m_vScreenSize);
	pScreen->SetDefault(true);
	lstScreens.Add(pScreen);
}

Vector2i GuiPL::GetScreenSize() const
{
	// Get system metrics for screen-width and screen-height
	return m_vScreenSize;
}

bool GuiPL::HasTaskbar()
{
	// The ingame GUI doesn't have a taskbar
	return false;
}

void GuiPL::SetMouseVisible(bool bVisible)
{
	// Nothing required in here, the GUI using this implementation, stores the mouse visibility
}

void GuiPL::ListFonts(List<FontInfo> &lstFonts) const
{
	// Not supported, fonts are loaded by using font files
}

WidgetImpl *GuiPL::CreateWidgetImpl(Widget &cWidget) const
{
	// Create a PixelLight ingame GUI widget
	return new WidgetPL(cWidget);
}

GraphicsImpl *GuiPL::CreateGraphicsImpl(Graphics &cGraphics) const
{
	// Create a PixelLight ingame GUI graphics object
	return m_pRenderer ? new GraphicsPL(cGraphics, *m_pRenderer, Vector2i::Zero, NoTransparency, PLGraphics::Color4::White) : nullptr;
}

ImageImpl *GuiPL::CreateImageImpl(Image &cImage) const
{
	// Create a PixelLight ingame GUI image object
	return new ImagePL(cImage);
}

FontImpl *GuiPL::CreateFontImpl(Font &cFont) const
{
	// Create a PixelLight ingame GUI font object
	return new FontPL(cFont);
}

CursorImpl *GuiPL::CreateCursorImpl(Cursor &cCursor) const
{
	// Create a PixelLight ingame GUI cursor object
	return new CursorPL(cCursor);
}

TrayIconImpl *GuiPL::CreateTrayIconImpl(TrayIcon &cTrayIcon) const
{
	// Tray icon not required for an ingame GUI
	return nullptr;
}

ClipBoardImpl *GuiPL::CreateClipBoardImpl(ClipBoard &cClipBoard) const
{
	// Create a PixelLight ingame GUI clipboard object
	return new ClipBoardPL(cClipBoard);
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Add widget to z position sorted list of top-level widgets, by default below all already existing widget
*/
void GuiPL::AddTopLevelWidget(Widget *pWidget)
{
	// Check if widget is already in the list
	if (!m_lstTopLevelWidgets.IsElement(pWidget) && !pWidget->IsManaged()) {
		// Add widget to list
		m_lstTopLevelWidgets.Add(pWidget);
	}
}

/**
*  @brief
*    Render a widget recursive
*/
void GuiPL::RenderWidget(Widget &cWidget, const Rectangle &cParentRectangle, ETransparency nTransparency, PLGraphics::Color4 cTransparencyColor)
{
	// Get the absolute position of the widget (the rendering is working with absolute coordinates)
	const Vector2i vAbsPos = cWidget.GetAbsPos();

	// Scissor rectangle in virtual GUI screen space dimension
	Rectangle cRectangle(static_cast<float>(vAbsPos.x), static_cast<float>(vAbsPos.y), static_cast<float>(vAbsPos.x + cWidget.GetSize().x), static_cast<float>(vAbsPos.y + cWidget.GetSize().y));

	// Transform the scissor rectangle into real viewport coordinates
	const Vector2 vScale(m_pRenderer->GetViewport().GetWidth()/m_vScreenSize.x, m_pRenderer->GetViewport().GetHeight()/m_vScreenSize.y);
	cRectangle.vMin *= vScale;
	cRectangle.vMax *= vScale;

	// Clip the calculated scissor rectangle against the scissor rectangle of the parent widget
	cRectangle.ClipByRectangle(cParentRectangle);

	// Set scissor rectangle to clip against widget boundings
	m_pRenderer->SetScissorRect(&cRectangle);

	// Check for transparency settings?
	if (nTransparency == NoTransparency) {
		// Get the widget implementation
		const WidgetPL *pWidgetPL = static_cast<WidgetPL*>(cWidget.GetImpl());
		if (pWidgetPL && pWidgetPL->m_nTransparency == AlphaTransparency) {
			nTransparency	   = pWidgetPL->m_nTransparency;
			cTransparencyColor = pWidgetPL->m_cTransparencyColor;
		}
	}

	// Create PixelLight ingame GUI graphics object
	GraphicsPL cGraphicsPL(*m_pRenderer, vAbsPos, m_vScreenSize, nTransparency, cTransparencyColor);
	Graphics cGraphics(*m_pGui, cGraphicsPL);

	// There's no erase within the PixelLight ingame GUI, so we don't need to send a OnDrawBackground message

	// Send OnDraw message
	m_pGui->SendMessage(GuiMessage::OnDraw(&cWidget, &cGraphics));

	{ // Recursion part
		// Get list of widget children
		const List<Widget*> &cChildren = static_cast<WidgetPL*>(cWidget.GetImpl())->m_lstChildren;

		// Iterate through the child widgets
		Iterator<Widget*> cIterator = cChildren.GetEndIterator();
		while (cIterator.HasPrevious()) {
			// Get the current child widget
			Widget *pChildWidget = cIterator.Previous();

			// Is the widget visible?
			if (pChildWidget->IsVisible() && !pChildWidget->GetTopmost()) {
				// Render the current child widgets recursive
				RenderWidget(*pChildWidget, cRectangle, nTransparency, cTransparencyColor);
			}
		}
	}

	{ // Topmost recursion part
		// [TODO] Draw topmost widgets in a complete separate draw step, or is the current solution ok?
		// Get list of widget children
		const List<Widget*> &cChildren = static_cast<WidgetPL*>(cWidget.GetImpl())->m_lstTopmostChildren;

		// Iterate through the child widgets
		Iterator<Widget*> cIterator = cChildren.GetEndIterator();
		while (cIterator.HasPrevious()) {
			// Get the current child widget
			Widget *pChildWidget = cIterator.Previous();

			// Is the widget visible?
			if (pChildWidget->IsVisible()) {
				// Render the current child widgets recursive
				RenderWidget(*pChildWidget, cRectangle, nTransparency, cTransparencyColor);
			}
		}
	}
}

/**
*  @brief
*    Called when the timer has fired
*/
void GuiPL::OnTimer()
{
	// Update ALL key repeat states
	m_lstRepeatKeys.SetAll();
}

/**
*  @brief
*    Get the currently pressed key modifiers
*/
uint32 GuiPL::GetKeyModifiers(Keyboard &cKeyboard) const
{
	uint32 nModifiers = 0;
	if (cKeyboard.Alt.IsPressed())		nModifiers |= PLGUIMOD_ALT;
	if (cKeyboard.Control.IsPressed())	nModifiers |= PLGUIMOD_CONTROL;
	if (cKeyboard.Shift.IsPressed())	nModifiers |= PLGUIMOD_SHIFT;
//	if (GetAsyncKeyState(VK_LWIN))	  nModifiers |= PLGUIMOD_WIN;	// [TODO] PLInput key?
//	if (GetAsyncKeyState(VK_RWIN))	  nModifiers |= PLGUIMOD_WIN;	// [TODO] PLInput key?
	return nModifiers;
}

/**
*  @brief
*    Update the ingame GUI keyboard
*/
void GuiPL::UpdateKeyboard()
{
	// Get the focus widget
	Widget *pFocusWidget = m_pGui->GetFocusWidget();
	if (pFocusWidget) {
		// Get keyboard input device
		Keyboard *pKeyboard = InputManager::GetInstance()->GetKeyboard();
		if (pKeyboard) {
			// Check all key states
			for (uint32 i=0; i<pKeyboard->GetButtons().GetNumOfElements(); i++) {
				// Is this key currently pressed?
				if (pKeyboard->GetButtons()[i]->IsPressed()) {
					// Was this key previously pressed?
					if (m_lstKeys.IsSet(i)) {
						// Key repeat?
						if (m_lstRepeatKeys.IsSet(i)) {
							// Update the key repeat state
							m_lstRepeatKeys.Clear(i);

							// Get state of modifier keys
							const uint32 nModifiers = GetKeyModifiers(*pKeyboard);

							// Send OnKeyDown message
							m_pGui->SendMessage(GuiMessage::OnKeyDown(pFocusWidget, i, nModifiers));
						}
					} else {
						// Update the key state
						m_lstKeys.Set(i);
						m_lstRepeatKeys.Clear(i);

						// Get state of modifier keys
						const uint32 nModifiers = GetKeyModifiers(*pKeyboard);

						// Send OnKeyDown message
						m_pGui->SendMessage(GuiMessage::OnKeyDown(pFocusWidget, i, nModifiers));
					}
				} else {
					// Was this key previously pressed?
					if (m_lstKeys.IsSet(i)) {
						// Update the key state
						m_lstKeys.Clear(i);

						// Get state of modifier keys
						const uint32 nModifiers = GetKeyModifiers(*pKeyboard);

						// Send OnKeyUp message
						m_pGui->SendMessage(GuiMessage::OnKeyUp(pFocusWidget, i, nModifiers));
					}
				}
			}
		}
	}
}

/**
*  @brief
*    Update the ingame GUI mouse
*/
void GuiPL::UpdateMouse()
{
	// Check whether or not there's an application instance
	if (CoreApplication::GetApplication() && CoreApplication::GetApplication()->IsInstanceOf("PLCore::FrontendApplication")) {
		// Get the frontend instance
		Frontend &cFrontend = static_cast<FrontendApplication*>(CoreApplication::GetApplication())->GetFrontend();

		// Check if the mouse is currently over the frontend
		if (cFrontend.IsMouseOver()) {
			// Get current mouse cursor position inside the frontend
			const Vector2i vMousePos(cFrontend.GetMousePositionX(), cFrontend.GetMousePositionY());
			if (vMousePos && m_vSystemMousePos != vMousePos) {
				// The system mouse cursor has been moved
				m_vSystemMousePos = vMousePos;

				{ // Calculate the absolute ingame GUI mouse cursor position
					// Calculate normalized mouse position
					const float fNormalizedX = static_cast<float>(m_vSystemMousePos.x)/static_cast<float>(cFrontend.GetWidth());
					const float fNormalizedY = static_cast<float>(m_vSystemMousePos.y)/static_cast<float>(cFrontend.GetHeight());

					// Calculate the mouse position within the virtual GUI space
					m_vMousePos.x = static_cast<int>(fNormalizedX*m_vScreenSize.x);
					m_vMousePos.y = static_cast<int>(fNormalizedY*m_vScreenSize.y);
				}

				// Get the widget implementation
				const WidgetPL *pMouseOverPL = m_pMouseOver ? static_cast<WidgetPL*>(m_pMouseOver->GetImpl()) : nullptr;

				// Currently over a widget that captures the mouse?
				if (pMouseOverPL && pMouseOverPL->m_bCaptureMouse) {
					// Calculate the relative mouse position within the mouse over widget
					const Vector2i vRelativeMousePos = m_vMousePos - m_pMouseOver->GetAbsPos();

					// Send OnMouseMove message
					m_pGui->SendMessage(GuiMessage::OnMouseMove(m_pMouseOver, vRelativeMousePos));
				} else {
					// Find the widget at the current absolute ingame GUI mouse cursor position
					Widget *pWidgetAtPos = FindWidgetAtPos(m_vMousePos);
					if (pWidgetAtPos) {
						// Check if mouse-enter widget has changed
						if (m_pMouseOver != pWidgetAtPos) {
							// Was there a previous mouse-enter widget?
							if (m_pMouseOver) {
								// Send OnMouseLeave message
								m_pGui->SendMessage(GuiMessage::OnMouseLeave(m_pMouseOver));
							}

							// Save mouse-enter widget
							m_pMouseOver = pWidgetAtPos;

							// Send OnMouseEnter message
							m_pGui->SendMessage(GuiMessage::OnMouseEnter(pWidgetAtPos));
						}

						// Calculate the relative mouse position within the mouse over widget
						const Vector2i vRelativeMousePos = m_vMousePos - m_pMouseOver->GetAbsPos();

						// Send OnMouseMove message
						m_pGui->SendMessage(GuiMessage::OnMouseMove(pWidgetAtPos, vRelativeMousePos));
					} else {
						// Is there a mouse-enter widget?
						if (m_pMouseOver) {
							// Send OnMouseLeave message
							m_pGui->SendMessage(GuiMessage::OnMouseLeave(m_pMouseOver));

							// Reset mouse-enter widget
							m_pMouseOver = nullptr;
						}
					}
				}
			}
		} else {
			// Is there a mouse-enter widget?
			if (m_pMouseOver) {
				// Send OnMouseLeave message
				m_pGui->SendMessage(GuiMessage::OnMouseLeave(m_pMouseOver));

				// Reset mouse-enter widget
				m_pMouseOver = nullptr;
			}
		}
	}

	// Get the mouse over widget
	Widget *pMouseOverWidget = m_pGui->GetMouseOverWidget();
	if (pMouseOverWidget) {
		// Get mouse input device
		Mouse *pMouse = InputManager::GetInstance()->GetMouse();
		if (pMouse) {
			// Calculate the relative mouse position within the mouse over widget
			const Vector2i vRelativeMousePos = m_vMousePos - pMouseOverWidget->GetAbsPos();

			// Update mouse status
			const bool bMouseButtonPressed[NumOfMouseButtons] = {
				pMouse->Left.IsPressed(),
				pMouse->Right.IsPressed(),
				pMouse->Middle.IsPressed()
			};
			for (uint32 nButton=0; nButton<NumOfMouseButtons; nButton++) {
				// Get the current mouse button
				const EMouseButton nMouseButton = static_cast<EMouseButton>(static_cast<uint32>(Button0) + nButton);

				// Check for state change
				if (!m_bMouseButtonPressed[nButton] && bMouseButtonPressed[nButton]) {
					// If this is the left mouse button, bring the widget to the foreground
					if (nMouseButton == LeftButton)
						pMouseOverWidget->SetZPos(ZTop, nullptr);

					// Send OnMouseButtonDown message
					m_pGui->SendMessage(GuiMessage::OnMouseButtonDown(pMouseOverWidget, nMouseButton, vRelativeMousePos));
				} else if (m_bMouseButtonPressed[nButton] && !bMouseButtonPressed[nButton]) {
					// Send OnMouseButtonUp message
					m_pGui->SendMessage(GuiMessage::OnMouseButtonUp(pMouseOverWidget, nMouseButton, vRelativeMousePos));

					// [TODO] When to send a click-event, and when not?
					// Send OnMouseButtonClick message
					m_pGui->SendMessage(GuiMessage::OnMouseButtonClick(pMouseOverWidget, nMouseButton, vRelativeMousePos));
				}

				// [TODO] Send OnMouseButtonDoubleClick message
				//pGui->SendMessage(GuiMessage::OnMouseButtonDoubleClick(pWidget, LeftButton, Vector2i(LOWORD(lParam), HIWORD(lParam))));

				// Backup current state
				m_bMouseButtonPressed[nButton] = bMouseButtonPressed[nButton];
			}
		}
	}
/* // [TODO]
			// Mouse wheel
			case WM_MOUSEWHEEL:
				// Send OnMouseWheel message
				pGui->SendMessage(GuiMessage::OnMouseWheel(pWidget, (short)HIWORD(wParam)/120));
				return 0;
*/
}

/**
*  @brief
*    Find the widget at the given absolute ingame GUI mouse cursor position
*/
Widget *GuiPL::FindWidgetAtPos(const Vector2i &vMousePos) const
{
	// By default, no widget was found
	Widget *pMouseAtPos = nullptr;

	// Iterate through the top-level widgets
	Iterator<Widget*> cIterator = m_lstTopLevelWidgets.GetIterator();
	while (cIterator.HasNext() && !pMouseAtPos) {
		// Get the current top-level widget
		Widget *pTopLevelWidget = cIterator.Next();

		// Is the widget visible and enabled?
		if (pTopLevelWidget->IsVisible() && pTopLevelWidget->IsEnabled()) {
			// Get the absolute position and size of the widget
			const Vector2i vAbsPos = pTopLevelWidget->GetAbsPos();
			const Vector2i vSize   = pTopLevelWidget->GetSize();

			// Is the given absolute ingame GUI mouse cursor position within the current top-level widget?
			if (vMousePos.x >= vAbsPos.x         && vMousePos.y >= vAbsPos.y &&
				vMousePos.x <= vAbsPos.x+vSize.x && vMousePos.y <= vAbsPos.y+vSize.y) {
				// Check the current top-level widgets recursive
				pMouseAtPos = FindWidgetAtPos(vMousePos, *pTopLevelWidget);

				// The given position is not over a child of the top-level widget, so, return the top-level widget instead
				if (!pMouseAtPos)
					pMouseAtPos = pTopLevelWidget;
			}
		}
	}

	// Done
	return pMouseAtPos;
}

/**
*  @brief
*    Find the widget at the given absolute ingame GUI mouse cursor position recursive
*/
Widget *GuiPL::FindWidgetAtPos(const Vector2i &vMousePos, Widget &cParentWidget) const
{
	// By default, no widget was found
	Widget *pMouseAtPos = nullptr;

	// Get list of widget children
	const List<Widget*> &cChildren = static_cast<WidgetPL*>(cParentWidget.GetImpl())->m_lstChildren;

	// Iterate through the child widgets
	Iterator<Widget*> cIterator = cChildren.GetIterator();
	while (cIterator.HasNext() && !pMouseAtPos) {
		// Get the current child widget
		Widget *pChildWidget = cIterator.Next();

		// [TODO] IsEnabled() not yet working?
		// Is the widget visible and enabled?
		if (pChildWidget->IsVisible()) {
//		if (pChildWidget->IsVisible() && pChildWidget->IsEnabled()) {
			// Get the absolute position and size of the widget
			const Vector2i vAbsPos = pChildWidget->GetAbsPos();
			const Vector2i vSize   = pChildWidget->GetSize();

			// Is the given absolute ingame GUI mouse cursor position within the current child widget?
			if (vMousePos.x >= vAbsPos.x         && vMousePos.y >= vAbsPos.y &&
				vMousePos.x <= vAbsPos.x+vSize.x && vMousePos.y <= vAbsPos.y+vSize.y) {
				// Check the current child widgets recursive
				pMouseAtPos = FindWidgetAtPos(vMousePos, *pChildWidget);

				// The given position is not over a child of the child widget, so, return the child widget instead
				if (!pMouseAtPos)
					pMouseAtPos = pChildWidget;
			}
		}
	}

	// Done
	return pMouseAtPos;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLFrontendPLGui
