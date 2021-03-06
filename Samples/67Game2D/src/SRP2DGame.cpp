/*********************************************************\
 *  File: SRP2DGame.cpp                                  *
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
#include <PLCore/Tools/Localization.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/FontManager.h>
#include <PLRenderer/Renderer/SamplerStates.h>
#include <PLRenderer/Renderer/DrawHelpers.h>
#include <PLRenderer/Effect/EffectManager.h>
#include <PLRenderer/Texture/TextureManager.h>
#include <PLScene/Scene/SceneContainer.h>
#include <PLScene/Visibility/SQCull.h>
#include "SNGun.h"
#include "SNBomb.h"
#include "SRP2DGame.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLRenderer;
using namespace PLScene;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(SRP2DGame, "", PLScene::SceneRendererPass, "Scene renderer pass 2D game")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
pl_class_metadata_end(SRP2DGame)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
SRP2DGame::SRP2DGame()
{
}

/**
*  @brief
*    Destructor
*/
SRP2DGame::~SRP2DGame()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Draws an image
*/
bool SRP2DGame::DrawImage(Renderer &cRenderer, const String &sName, const Vector2 &vPos, const Vector2 &vSize,
						  const Vector2 &vTextureCoordinate, const Vector2 &vTextureCoordinateSize) const
{
	// Get the texture
	const Texture *pTexture = cRenderer.GetRendererContext().GetTextureManager().LoadResource(sName);
	if (pTexture && pTexture->GetTextureBuffer()) {
		// To get the perfect look, we want to see some pixels - so, do not perform texture filtering!
		SamplerStates cSamplerStates;
		cSamplerStates.Set(Sampler::MagFilter, TextureFiltering::None);
		cSamplerStates.Set(Sampler::MinFilter, TextureFiltering::None);
		cSamplerStates.Set(Sampler::MipFilter, TextureFiltering::None);

		// Draw image
		cRenderer.GetDrawHelpers().DrawImage(*pTexture->GetTextureBuffer(), cSamplerStates, vPos, vSize, Color4::White, 0.5f, vTextureCoordinate, vTextureCoordinateSize);

		// Done
		return true;
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneRendererPass functions  ]
//[-------------------------------------------------------]
void SRP2DGame::Draw(Renderer &cRenderer, const SQCull &cCullQuery)
{
	// Set render states
	cRenderer.GetRendererContext().GetEffectManager().Use();
	cRenderer.SetRenderState(RenderState::ZEnable,      false);
	cRenderer.SetRenderState(RenderState::ZWriteEnable, false);

	// Begin 2D mode
	DrawHelpers &cDrawHelpers = cRenderer.GetDrawHelpers();
	cDrawHelpers.Begin2DMode(0, 0, 320, 200);

		// Draw the background
		DrawImage(cRenderer, "Data/Textures/2DGame/Background.tga", Vector2::Zero);

		// Get the scene container
		SceneContainer &cSceneContainer = cCullQuery.GetSceneContainer();

		// Draw solid
		for (uint32 i=0; i<cSceneContainer.GetNumOfElements(); i++) {
			SceneNode *pSceneNode = cSceneContainer.GetByIndex(i);
			if (pSceneNode->IsVisible()) {
				// SNProjectile
				if (pSceneNode->IsInstanceOf("SNProjectile")) {
					// Draw the projectile
					DrawImage(cRenderer, "Data/Textures/2DGame/Projectile.tga", pSceneNode->GetTransform().GetPosition() - Vector3(3.0f, 3.0f, 0.0f));

				// SNBomb
				} else if (pSceneNode->IsInstanceOf("SNBomb")) {
					// Draw the bomb (original image dimension: 188x18)
					char nFrame = static_cast<SNBomb*>(pSceneNode)->GetFrame();
					DrawImage(cRenderer, "Data/Textures/2DGame/Bomb.tga", pSceneNode->GetTransform().GetPosition() - Vector3(8.0f, 8.0f, 0.0f), Vector2(16.0f, 16.0f),
							  Vector2(static_cast<float>(1+nFrame*17)/188.0f, 0.0f), Vector2(16.0f/188.0f, 1.0f));

				// SNUFO
				} else if (pSceneNode->IsInstanceOf("SNUFO")) {
					// Draw the UFO
					DrawImage(cRenderer, "Data/Textures/2DGame/UFO.tga", pSceneNode->GetTransform().GetPosition() - Vector3(25.0f, 10.0f, 0.0f));

				// SNGun
				} else if (pSceneNode->IsInstanceOf("SNGun")) {
					// Draw the gun
					char nFrame = static_cast<SNGun*>(pSceneNode)->GetFrame();
					DrawImage(cRenderer, "Data/Textures/2DGame/Gun.tga", pSceneNode->GetTransform().GetPosition() - Vector3(18.0f, 7.0f, 0.0f), Vector2(36.0f, 14.0f),
							  Vector2(static_cast<float>(1+nFrame*38)/495.0f, 0.0f), Vector2(36.0f/495.0f, 1.0f));
				}
			}
		}

		// Draw debug - if we want to visualize some debug information :)
		for (uint32 i=0; i<cSceneContainer.GetNumOfElements(); i++) {
			SceneNode *pSceneNode = cSceneContainer.GetByIndex(i);
			if (pSceneNode->IsVisible() && (pSceneNode->GetDebugFlags() & SceneNode::DebugEnabled) && (pSceneNode->GetDrawFunctionFlags() & SceneNode::UseDrawDebug))
				pSceneNode->DrawDebug(cRenderer);
		}

	// End 2D mode
	cDrawHelpers.End2DMode();

	// Get the font
	Font *pFont = cRenderer.GetFontManager().GetDefaultFontTexture();
	if (pFont) {
		// Setup render states
		cRenderer.GetRendererContext().GetEffectManager().Use();
		cRenderer.SetRenderState(RenderState::ZEnable,      false);
		cRenderer.SetRenderState(RenderState::ZWriteEnable, false);

		// Begin 2D mode
		cDrawHelpers.Begin2DMode();

			// Draw texts
			cDrawHelpers.DrawText(*pFont, PLT("Keys:"),						 Color4::White, Vector2(0.01f, 0.01f));
			cDrawHelpers.DrawText(*pFont, PLT("a/d = Gun control"),			 Color4::White, Vector2(0.06f, 0.03f));
			cDrawHelpers.DrawText(*pFont, PLT("Space = Start projectile"),   Color4::White, Vector2(0.06f, 0.05f));
			cDrawHelpers.DrawText(*pFont, PLT("r = Restart game"),			 Color4::White, Vector2(0.06f, 0.07f));
			cDrawHelpers.DrawText(*pFont, PLT("p = Toggle post processing"), Color4::White, Vector2(0.06f, 0.09f));

		// End 2D mode
		cDrawHelpers.End2DMode();
	}
}
