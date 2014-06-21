/*********************************************************\
 *  File: SNConsole.cpp                                  *
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
#include <PLCore/File/File.h>
#include <PLCore/Tools/Timing.h>
#include <PLInput/Input/InputManager.h>
#include <PLInput/Input/Devices/Keyboard.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/Font.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Renderer/FontManager.h>
#include <PLRenderer/Renderer/SamplerStates.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLRenderer/Material/MaterialManager.h>
#include <PLScene/Scene/SceneContext.h>
#include "PLEngine/Application/EngineApplication.h"
#include "PLEngine/Compositing/Console/SNConsole.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLInput;
using namespace PLRenderer;
using namespace PLScene;
namespace PLEngine {


//[-------------------------------------------------------]
//[ Public definitions                                    ]
//[-------------------------------------------------------]
const String SNConsole::StandardMaterial = "Data/Effects/PLConsole.plfx";


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(SNConsole)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SNConsole::SNConsole() :
	EventHandlerUpdate(&SNConsole::OnUpdate, this),
	m_nRow(0),
	m_nLastRow(0),
	m_fPos(0.0f),
	m_fMaxPos(0.7f),
	m_fAlpha(0.0f)
{
	// [TODO] Use FS own functions for this if implemented...
	#ifdef WIN32
		m_LastTime.dwLowDateTime  = 0;
		m_LastTime.dwHighDateTime = 0;
		m_nLastFileSizeHigh = 0;
		m_nLastFileSizeLow  = 0;
	#endif

	// Initialize
	PL_LOG(Info, "Initialize console " + GetDescription())

	// Use 28 log lines
	m_lstLogLines.Resize(28);

	// Set draw function flags
	SetDrawFunctionFlags(static_cast<uint8>(GetDrawFunctionFlags() | UseDrawPost));
}

/**
*  @brief
*    Destructor
*/
SNConsole::~SNConsole()
{
}

float SNConsole::GetPos() const
{
	return m_fPos;
}

float SNConsole::GetMaxPos() const
{
	return m_fMaxPos;
}

Material *SNConsole::GetMaterial()
{
	return m_cMaterial.GetResource();
}

void SNConsole::SetPos(float fNewPos)
{
	m_fPos = fNewPos;
}

void SNConsole::SetMaxPos(float fMax)
{
	m_fMaxPos = fMax;
}

void SNConsole::SetAlpha(float fNewAlpha)
{
	m_fAlpha = fNewAlpha;
}

String SNConsole::GetDescription() const
{
	static const String sString = "PixelLight console v1.0";
	return sString;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node needs to be updated
*/
void SNConsole::OnUpdate()
{
	// Ignore time scale
	float fTimeDiff = Timing::GetInstance()->GetTimeDifference()/Timing::GetInstance()->GetTimeScaleFactor();

	// Log row
	m_nLastRow = m_nRow;
	if (m_nState == Active || m_nState == Activating) {
		// Check if input is active
		// [TODO] Don't use devices directly, use a virtual controller instead
		CoreApplication *pCoreApplication = CoreApplication::GetApplication();
		if (pCoreApplication && pCoreApplication->IsInstanceOf("PLEngine::EngineApplication")) {
			Controller *pController = reinterpret_cast<Controller*>(static_cast<EngineApplication*>(pCoreApplication)->GetInputController());
			if ((pController && pController->GetActive()) || !pController) {
				// Get keyboard input device
				Keyboard *pKeyboard = InputManager::GetInstance()->GetKeyboard();
				if (pKeyboard) {
					if (pKeyboard->PageUp.IsHit())
						m_nRow += 27;
					if (pKeyboard->PageDown.IsHit()) {
						m_nRow -= 27;
						if (m_nRow < 0)
							m_nRow = 0;
					}
				}
			}
		}
	}

	// Read keyboard
	ProcessKeyMessage();

	// Check blending
	if (m_nState == Active || m_nState == Activating) {
		m_fAlpha += fTimeDiff*1000;
		if (m_fAlpha > 225)
			m_fAlpha = 225;
	}
	if (m_nState == Inactive || m_nState == Deactivating) {
		m_fAlpha -= fTimeDiff*1000;
		if (m_fAlpha < 0)
			m_fAlpha = 0;
		if (m_nState == Inactive)
			return;
	}

	// Activate the console
	if (m_nState == Activating) {
		m_fPos += 0.015f * fTimeDiff*2;
		if (m_fPos >= m_fMaxPos) {
			m_fPos   = m_fMaxPos;
			m_nState = Active;
		}
	}

	// Deactivate the console
	if (m_nState == Deactivating) {
		m_fPos -= 0.015f * fTimeDiff*2;
		if (m_fPos <= 0.0f || !m_fAlpha) {
			m_fPos   = 0.0f;
			m_nState = Inactive;
		}
	}

	if (m_nState == Deactivating) {
		m_nRow = -1;
	} else {
		// Row must always be above 0
		if (m_nRow < 0)
			m_nRow = 0;

		// In here, we ALWAYS want to be up to date - so, flush the log right now
		Log::GetInstance()->Flush();

		// Was the log file changed and we need a sync?
		#ifdef WIN32
			// Check for log update
			// [TODO] Use FS own functions for this if implemented...
			if (static_cast<int>(m_nLastRow) == m_nRow) {
				WIN32_FIND_DATA File;
				HANDLE hFind = FindFirstFile(Log::GetInstance()->GetFilename(), &File);
				if (hFind != INVALID_HANDLE_VALUE) {
					// We need to close the find handle
					FindClose(hFind);

					// Compare file time
					long nResult = CompareFileTime(&m_LastTime, &File.ftLastWriteTime);
					if (nResult >= 0 && m_nLastFileSizeHigh == File.nFileSizeHigh && m_nLastFileSizeLow == File.nFileSizeLow)
						return; // Nothing to do :)
					m_LastTime			= File.ftLastWriteTime;
					m_nLastFileSizeHigh = File.nFileSizeHigh;
					m_nLastFileSizeLow  = File.nFileSizeLow;
				}
			}
		#endif

		// Update log information
		File cFile(Log::GetInstance()->GetFilename());
		if (cFile.Open(File::FileRead)) {
			// Find current offset by skipping the desired number of rows from down wards
			int nRow = 0;
			uint32 nOffset = 0;
			do {
				// Update the offset
				nOffset++;

				// Read a log line by reading until a '\n' is found
				do {
					// Set the current file pointer position
					if (!cFile.Seek(-static_cast<int>(nOffset), File::SeekEnd)) {
						// We reached the beginning of the file :)
						m_nRow = nRow-1;
						break;
					}

					// Update the offset
					nOffset++;
				} while (static_cast<char>(cFile.GetC()) != '\n');
				if (nRow < m_nRow+1)
					nRow++;
			} while (nRow < m_nRow+1);

			// Read in the log lines
			static const uint32 BufferSize = 256;
			char szBuffer[BufferSize];
			for (uint32 nLine=0; nLine<m_lstLogLines.GetNumOfElements(); nLine++) {
				// Read a log line by reading until a '\n' is found
				int i;
				String sLine;
				do {
					// Read a log line by reading until a '\n' is found or the buffer is full
					i = BufferSize - 1;
					do {
						// Set the current file pointer position
						if (!cFile.Seek(-static_cast<int>(nOffset), File::SeekEnd)) {
							// We reached the beginning of the file :)
							m_nRow = nRow-1;
							break;
						}

						// Read a character
						szBuffer[i] = static_cast<char>(cFile.GetC());

						// Update the offsets
						i--;
						nOffset++;
					} while (szBuffer[i+1] != '\n' && i >= 0);

					// Add the buffer content to the line text
					if (i != BufferSize-1) {
						if (szBuffer[i+1] == '\n') {
							if (i < BufferSize-2)
								sLine.Insert(&szBuffer[i+2], 0, BufferSize-(i+2));
						} else {
							sLine.Insert(&szBuffer[i+1], 0, BufferSize-(i+1));
						}
					} else {
						// We are done :)
						break;
					}
				} while (szBuffer[i+1] != '\n');

				// Set read line text
				m_lstLogLines[nLine] = sLine;
			}

			// Close the log file
			cFile.Close();
		}
	}
}


//[-------------------------------------------------------]
//[ Public virtual PLScene::SceneNode functions           ]
//[-------------------------------------------------------]
void SNConsole::DrawPost(Renderer &cRenderer, const VisNode *pVisNode)
{
	// Check whether the console is inactive
	if (m_nState != Inactive) {
		DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();

		// Call base implementation
		SNConsoleBase::DrawPost(cRenderer, pVisNode);

		// Backup renderer states
		uint32 nFixedFillModeBackup = cRenderer.GetRenderState(RenderState::FixedFillMode);
		uint32 nFillModeBackup      = cRenderer.GetRenderState(RenderState::FillMode);

		// Set new renderer states
		cRenderer.SetRenderState(RenderState::FixedFillMode, Fill::Solid);
		cRenderer.SetRenderState(RenderState::FillMode,      Fill::Solid);

		// Draw the background material
		Material *pMaterial = m_cMaterial.GetResource();
		if (pMaterial) {
			SamplerStates cSamplerStates;
			for (uint32 i=0; i<pMaterial->GetNumOfPasses(); i++) {
				pMaterial->SetupPass(i);
				TextureBuffer *pTextureBuffer = cRenderer.GetTextureBuffer(0);
				if (pTextureBuffer) {
					// Begin 2D mode
					cDrawHelpers.Begin2DMode();

						// Draw image
						cRenderer.SetRenderState(RenderState::CullMode, Cull::None);
						cDrawHelpers.DrawImage(*pTextureBuffer, cSamplerStates, Vector2::Zero, Vector2::One, Color4(0.5f, 0.5f, 0.5f, m_fAlpha/255*0.6f));

					// End 2D mode
					cDrawHelpers.End2DMode();
				}
			}
		}

		// Get the font
		Font *pFont = cRenderer.GetFontManager().GetDefaultFontTexture();
		if (pFont) {
			// Set render states
			cRenderer.GetRendererContext().GetEffectManager().Use();
			cRenderer.SetRenderState(RenderState::ZEnable,      false);
			cRenderer.SetRenderState(RenderState::ZWriteEnable, false);

			// Begin 2D mode
			static const float VirtualWidth  = 800.0f;
			static const float VirtualHeight = 600.0f;
			cDrawHelpers.Begin2DMode(0.0f, 0.0f, VirtualWidth, VirtualHeight);

				// Draw current console line
				const float fY = 100*((255-m_fAlpha)/255);
				cDrawHelpers.DrawText(*pFont, ">>" + m_sFoundCommand, Color4(0.5f, 0.5f, 0.5f, m_fAlpha/255), Vector2(5, 480-fY));
				const Color4 cColor(1.0f, 1.0f, 1.0f, m_fAlpha/255);
				cDrawHelpers.DrawText(*pFont, ">>" + m_sCommand, cColor, Vector2(5, 480-fY));

				// Draw cursor
				if ((Timing::GetInstance()->GetPastTime()/500)%2) {
					const float fX = m_nCursor ? cDrawHelpers.GetTextWidth(*pFont, ">>" + m_sCommand.GetSubstring(0, m_nCursor)) : cDrawHelpers.GetTextWidth(*pFont, ">>");
					cDrawHelpers.DrawText(*pFont, '|', cColor, Vector2(5 + fX*VirtualWidth, 480-fY));
				}

				// Draw selection
				if (m_nSelStart != -1 && m_nSelEnd != -1 && m_nSelStart != m_nSelEnd) {
					// Get the start and end of the selection
					uint32 nSelStart = m_nSelStart;
					uint32 nSelEnd   = m_nSelEnd;
					if (nSelStart > nSelEnd) {
						nSelStart = m_nSelEnd;
						nSelEnd   = m_nSelStart;
					}

					// Draw selection
					float fX = 5+cDrawHelpers.GetTextWidth(*pFont, ">>")*VirtualWidth;
					if (nSelStart)
						fX += cDrawHelpers.GetTextWidth(*pFont, m_sCommand.GetSubstring(0, nSelStart))*VirtualWidth;
					const float fHeight = cDrawHelpers.GetTextHeight(*pFont)*VirtualHeight;
					cDrawHelpers.DrawLine(cColor,
										  Vector2(fX, 482-fY+fHeight),
										  Vector2(fX+cDrawHelpers.GetTextWidth(*pFont, m_sCommand.GetSubstring(nSelStart, nSelEnd-nSelStart))*VirtualWidth, 482-fY+fHeight),
										  1.0f);
				}

				// Print console description
				cDrawHelpers.DrawText(*pFont, GetDescription(), cColor, Vector2(400, 500-fY), Font::CenterText);

				// Draw the log
				for (uint32 i=0; i<m_lstLogLines.GetNumOfElements(); i++)
					cDrawHelpers.DrawText(*pFont, m_lstLogLines[i], cColor, Vector2(5, 460 - fY - i*15.0f), 0, Vector2(0.8f, 0.8f));

			// End 2D mode
			cDrawHelpers.End2DMode();

			// Reset renderer states
			cRenderer.SetRenderState(RenderState::FillMode,		 nFillModeBackup);
			cRenderer.SetRenderState(RenderState::FixedFillMode, nFixedFillModeBackup);
		}
	}
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneNode functions          ]
//[-------------------------------------------------------]
void SNConsole::InitFunction()
{
	// Call base implementation
	SNConsoleBase::InitFunction();

	// Load material
	m_cMaterial.SetResource(GetSceneContext()->GetRendererContext().GetMaterialManager().LoadResource(StandardMaterial));

	// Connect/disconnect event handler
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext)
		pSceneContext->EventUpdate.Connect(EventHandlerUpdate);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine
