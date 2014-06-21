/*********************************************************\
 *  File: PGImage.cpp                                    *
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
#include <PLGraphics/Image/Image.h>
#include <PLGraphics/Image/ImageBuffer.h>
#include <PLScene/Scene/SceneContext.h>
#include "PLParticleGroups/PGImage.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMath;
using namespace PLGraphics;
using namespace PLScene;
namespace PLParticleGroups {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(PGImage)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
PGImage::PGImage() :
	ImageFilename(this),
	RedColorKey(this),
	GreenColorKey(this),
	BlueColorKey(this),
	ColorKeyTolerance(this),
	ImageScale(this),
	Material(this),
	Particles(this),
	Flags(this),
	EventHandlerUpdate(&PGImage::OnUpdate, this),
	m_bUpdate(false)
{
	// Overwritten SNParticleGroup variables
	m_sMaterial  = "Data/Effects/PGImage.plfx";
	m_nParticles = 1;

	// Overwritten PLScene::SceneNode variables
	SetFlags(PointSprites | SceneNodeSpaceParticles);
}

/**
*  @brief
*    Destructor
*/
PGImage::~PGImage()
{
}


//[-------------------------------------------------------]
//[ Private virtual PLScene::SceneNode functions          ]
//[-------------------------------------------------------]
void PGImage::InitFunction()
{
	// Call base implementation
	SNParticleGroup::InitFunction();

	// Load image
	Image cImage;
	if (cImage.LoadByFilename(ImageFilename.Get())) {
		// Get the image buffer
		ImageBuffer *pImageBuffer = cImage.GetBuffer();
		if (pImageBuffer) {
			const uint32  nWidth		   = pImageBuffer->GetSize().x;
			const uint32  nHeight		   = pImageBuffer->GetSize().y;
			const uint32  nPixels		   = nWidth*nHeight;
			const uint8  *pData			   = pImageBuffer->GetData();
			const uint32  nColorComponents = pImageBuffer->GetComponentsPerPixel();
			if (nColorComponents == 3 || nColorComponents == 4) {
				// Get total number of required particles
				uint32 nParticles = 0;
				for (uint32 i=0; i<nPixels; i++) {
					// Place particle at this image position?
					uint8 nDifR = static_cast<uint8>(Math::Abs(pData[0]-RedColorKey));
					uint8 nDifG = static_cast<uint8>(Math::Abs(pData[1]-GreenColorKey));
					uint8 nDifB = static_cast<uint8>(Math::Abs(pData[2]-BlueColorKey));
					if (!(nDifR <= ColorKeyTolerance && nDifG <= ColorKeyTolerance && nDifB <= ColorKeyTolerance))
						nParticles++; // A particle here, please
					pData += nColorComponents;
				}
				pData = pImageBuffer->GetData();

				// Create the particles
				InitParticles(nParticles);

				// Setup the particles
				for (int nY=nHeight-1; nY>=0; nY--) {
					for (uint32 nX=0; nX<nWidth; nX++) {
						// Place particle at this image position?
						uint8 nDifR = static_cast<uint8>(Math::Abs(pData[0]-RedColorKey));
						uint8 nDifG = static_cast<uint8>(Math::Abs(pData[1]-GreenColorKey));
						uint8 nDifB = static_cast<uint8>(Math::Abs(pData[2]-BlueColorKey));
						if (nDifR <= ColorKeyTolerance && nDifG <= ColorKeyTolerance && nDifB <= ColorKeyTolerance) {
							// No particle here, please
							pData += nColorComponents;
						} else {
							// Setup particle
							Particle *pParticle = AddParticle();
							if (pParticle) {
								pParticle->vColor.r = pData[0]/255.0f;
								pParticle->vColor.g = pData[1]/255.0f;
								pParticle->vColor.b = pData[2]/255.0f;
								pParticle->vColor.a = nColorComponents == 4 ? pData[3]/255.0f : 1.0f;
								pData += nColorComponents;
								pParticle->fEnergy  = 1.0f;
								pParticle->fCustom2 = 0.5f+Math::GetRandFloat()*2.0f;
								if (Math::GetRandFloat() > 0.5f)
									pParticle->fCustom2 = -pParticle->fCustom2;
								pParticle->fSize = (1.0f+(static_cast<float>(Math::GetRand() % 1000)/500))*ImageScale;
								pParticle->vPos.x = pParticle->vFixPos.x = pParticle->vDistortion.x = static_cast<float>(nX*ImageScale);
								pParticle->vPos.y = pParticle->vFixPos.y = pParticle->vDistortion.y = static_cast<float>(nY*ImageScale);
								pParticle->vPos.z = pParticle->vFixPos.z = pParticle->vDistortion.z = 0.0f;
								pParticle->fCustom1 = pParticle->fSize;
								pParticle->fCustom2 *= 2;
							} else {
								// Error!
								nY = nHeight;
								nX = nWidth;
							}
						}
					}
				}
			}
		}
	}
}

void PGImage::OnActivate(bool bActivate)
{
	// Call the base implementation
	SNParticleGroup::OnActivate(bActivate);

	// Connect/disconnect event handler
	SceneContext *pSceneContext = GetSceneContext();
	if (pSceneContext) {
		if (bActivate)
			pSceneContext->EventUpdate.Connect(EventHandlerUpdate);
		else
			pSceneContext->EventUpdate.Disconnect(EventHandlerUpdate);
	}
}

void PGImage::OnAddedToVisibilityTree(VisNode &cVisNode)
{
	m_bUpdate = true;
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Called when the scene node needs to be updated
*/
void PGImage::OnUpdate()
{
	// If this scene node wasn't drawn at the last frame, we can skip some update stuff
	if ((GetFlags() & ForceUpdate) || m_bUpdate) {
		m_bUpdate = false;

		{ // Update particles
			float fTimeDiff = Timing::GetInstance()->GetTimeDifference();
			Iterator<Particle> cIterator = GetParticleIterator();
			while (cIterator.HasNext()) {
				Particle &cParticle = cIterator.Next();

				cParticle.fSize = cParticle.fCustom1;
				for (int i=0; i<3; i++) {
					if (cParticle.vPos[i] > cParticle.vDistortion[i])
						cParticle.vVelocity[i] -= fTimeDiff*10;
					else {
						if (cParticle.vPos[i] < cParticle.vDistortion[i])
							cParticle.vVelocity[i] += fTimeDiff*10;
					}
					if (cParticle.vVelocity[i] > 100.0f)
						cParticle.vVelocity[i] = 100.0f;
					if (cParticle.vVelocity[i] < -100.0f)
						cParticle.vVelocity[i] = -100.0f;

					cParticle.vPos[i] += cParticle.vVelocity[i]*fTimeDiff;

					if ((cParticle.vPos[i] <= cParticle.vDistortion[i]+0.1f &&
						cParticle.vPos[i] >= cParticle.vDistortion[i]-0.1f) ||
						cParticle.vPos[i] >= cParticle.vFixPos[i]+0.2f ||
						cParticle.vPos[i] <= cParticle.vFixPos[i]-0.2f) {
						if (cParticle.vDistortion[i] != cParticle.vFixPos[i])
							cParticle.vVelocity[i] *= 0.8f;
						if (cParticle.vPos[i] > cParticle.vFixPos[i]+0.2f ||
							cParticle.vPos[i] < cParticle.vFixPos[i]-0.2f)
							cParticle.vDistortion[i] = cParticle.vFixPos[i];
						else {
							if (Math::GetRand() % 2)
								cParticle.vDistortion[i] = static_cast<float>(Math::GetRand() % 100)/1000+cParticle.vFixPos[i];
							else
								cParticle.vDistortion[i] = static_cast<float>(Math::GetRand() % 100)/1000-cParticle.vFixPos[i];
						}
					}
				}
			}
		}

		// We have to recalculate the current axis align bounding box in 'scene node space'
		DirtyAABoundingBox();
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLParticleGroups
