/*********************************************************\
 *  File: GMMSurfaceLevel.cpp                            *
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
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Renderer/IndexBuffer.h>
#include "PLNature/Terrain/GMMSurfaceLevel.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
namespace PLNature {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
GMMSurfaceLevel::GMMSurfaceLevel(Renderer &cRenderer) :
	m_pRenderer(&cRenderer),
	m_nLevels(0),
	m_pBodies(nullptr),
	m_pLinkers(nullptr)
{
}

/**
*  @brief
*    Destructor
*/
GMMSurfaceLevel::~GMMSurfaceLevel()
{
	if (m_pBodies) {
		for (uint32 i=0; i<m_nLevels; i++) {
			if (m_pBodies[i].nIndexBufferSize) {
				delete [] m_pBodies[i].pnIndexBuffer;
				if (m_pBodies[i].pIndexBuffer)
					delete m_pBodies[i].pIndexBuffer;
			}
			if (m_pBodies[i].nCornerIndexSize) {
				delete [] m_pBodies[i].pnCornerIndexBuffer;
				if (m_pBodies[i].pCornerIndexBuffer)
					delete m_pBodies[i].pCornerIndexBuffer;
			}
		}
		delete [] m_pBodies;
	}
	if (m_pLinkers) {
		for (uint32 i=0; i<m_nLevels; i++) {
			for (uint32 j=0; j<4; j++) {
				if (m_pLinkers[i].pnIndexBuffer[j])
					delete [] m_pLinkers[i].pnIndexBuffer[j];
				if (m_pLinkers[i].pIndexBuffer[j])
					delete m_pLinkers[i].pIndexBuffer[j];
			}
		}
		delete [] m_pLinkers;
	}
}

/**
*  @brief
*    Builds the bodies
*/
void GMMSurfaceLevel::BuildBodies(uint32 nLevels, uint32 nPatchSize)
{
	m_nLevels = nLevels;
	uint32 v = nLevels ? 16 : 1;
	m_pBodies = new BodyPiece[v];

	for (uint32 nVar=0; nVar<v; nVar++) {
		uint32 nCutRight  = (nVar & (1<<Right))  ? 1 : 0;
		uint32 nCutTop    = (nVar & (1<<Top))    ? 1 : 0;
		uint32 nCutLeft   = (nVar & (1<<Left))   ? 1 : 0;
		uint32 nCutBottom = (nVar & (1<<Bottom)) ? 1 : 0;

		const int n     = nPatchSize+1;
		const int nStep = nPatchSize/(1<<nLevels);
		int nSizeX = (nPatchSize/nStep);
		if (nCutLeft)
			--nSizeX;
		if (nCutRight)
			--nSizeX;
		int nSizeY = (nPatchSize/nStep);
		if (nCutTop)
			--nSizeY;
		if (nCutBottom)
			--nSizeY;
		m_pBodies[nVar].pIndexBuffer = nullptr;

		if (nSizeY > 0 && nSizeX > 0) {
			m_pBodies[nVar].nIndexBufferSize = nSizeX*(2*(nSizeY+1)+1);
			m_pBodies[nVar].pnIndexBuffer    = new uint32[m_pBodies[nVar].nIndexBufferSize];

			{
				int nStrtX = nCutLeft*nStep;
				int nEndX  = nPatchSize-(nCutRight*nStep);
				int nStrtY = nCutTop*nStep;
				int nEndY  = nPatchSize-(nCutBottom*nStep);
				int i = 0;
				int X = nStrtX;
				int Y = 0;
				while (X<nEndX) {
					for (Y=nEndY; Y>=nStrtY; Y-=nStep) {
						m_pBodies[nVar].pnIndexBuffer[i++] = (X*n)+Y;
						m_pBodies[nVar].pnIndexBuffer[i++] = ((X+nStep)*n)+Y;
					}
					m_pBodies[nVar].pnIndexBuffer[i++] = ((X += nStep)*n)+nStrtY;

					if (X >= nEndX)
						break;

					for (Y=nStrtY; Y<=nEndY; Y+=nStep) {
						m_pBodies[nVar].pnIndexBuffer[i++] = (X*n)+Y;
						m_pBodies[nVar].pnIndexBuffer[i++] = ((X+nStep)*n)+Y;
					}
					m_pBodies[nVar].pnIndexBuffer[i++] = ((X += nStep)*n)+nEndY;
				}
			}

			// Create index buffer
			IndexBuffer *pIndexBuffer = m_pRenderer->CreateIndexBuffer();
			m_pBodies[nVar].pIndexBuffer = pIndexBuffer;
			uint32 nIndices = m_pBodies[nVar].nIndexBufferSize;
			pIndexBuffer->Allocate(nIndices, Usage::Static, false);
			if (pIndexBuffer->Lock(Lock::WriteOnly)) {
				uint32 *pIndexT = m_pBodies[nVar].pnIndexBuffer;
				for (uint32 i=0; i<nIndices; i++) {
					pIndexBuffer->SetData(i, *pIndexT);
					pIndexT++;
				}

				// Unlock the index buffer
				pIndexBuffer->Unlock();
			}
		} else {
			m_pBodies[nVar].nIndexBufferSize = 0;
		}

		bool nCorner[8];
		uint32 nNumCorn = 0;

		if (nVar==2 || nVar==3 || nVar==10 || nVar==11)  { nCorner[0] = true; nNumCorn++;} else nCorner[0] = false;
		if (nVar>=4 && nVar<=7)							 { nCorner[1] = true; nNumCorn++;} else nCorner[1] = false;
		if (nVar==8 || nVar==10 || nVar==12 || nVar==14) { nCorner[2] = true; nNumCorn++;} else nCorner[2] = false;
		if (nVar==1 || nVar==5 ||  nVar==9  || nVar==13) { nCorner[3] = true; nNumCorn++;} else nCorner[3] = false;
		if (nVar==4 || nVar==5 ||  nVar==12 || nVar==13) { nCorner[4] = true; nNumCorn++;} else nCorner[4] = false;
		if (nVar==2 || nVar==6 ||  nVar==10 || nVar==14) { nCorner[5] = true; nNumCorn++;} else nCorner[5] = false;
		if (nVar==1 || nVar==3 ||  nVar==5  || nVar==7)	 { nCorner[6] = true; nNumCorn++;} else nCorner[6] = false;
		if (nVar>=8 && nVar<=11)						 { nCorner[7] = true; nNumCorn++;} else nCorner[7] = false;

		if (nNumCorn) {
			m_pBodies[nVar].nCornerIndexSize    = nNumCorn*3;
			m_pBodies[nVar].pnCornerIndexBuffer = new uint32[m_pBodies[nVar].nCornerIndexSize];

			{
				uint32 i = 0;
				if (nCorner[0]) {
					m_pBodies[nVar].pnCornerIndexBuffer[i++] = 0;
					m_pBodies[nVar].pnCornerIndexBuffer[i++] = nStep;
					m_pBodies[nVar].pnCornerIndexBuffer[i++] = nStep*n+nStep;
				}
				if (nCorner[1]) {
					m_pBodies[nVar].pnCornerIndexBuffer[i++] = nPatchSize;
					m_pBodies[nVar].pnCornerIndexBuffer[i++] = nStep*n+nPatchSize;
					m_pBodies[nVar].pnCornerIndexBuffer[i++] = nStep*n+nPatchSize-nStep;
				}
				if (nCorner[2]) {
					m_pBodies[nVar].pnCornerIndexBuffer[i++] = (nPatchSize*n)+nPatchSize;
					m_pBodies[nVar].pnCornerIndexBuffer[i++] = (nPatchSize*n)+nPatchSize-nStep;
					m_pBodies[nVar].pnCornerIndexBuffer[i++] = ((nPatchSize-nStep)*n)+nPatchSize-nStep;
				}
				if (nCorner[3]) {
					m_pBodies[nVar].pnCornerIndexBuffer[i++] = nPatchSize*n;
					m_pBodies[nVar].pnCornerIndexBuffer[i++] = ((nPatchSize-nStep)*n);
					m_pBodies[nVar].pnCornerIndexBuffer[i++] = ((nPatchSize-nStep)*n)+nStep;
				}
				if (nCorner[4]) {
					m_pBodies[nVar].pnCornerIndexBuffer[i++] = 0;
					m_pBodies[nVar].pnCornerIndexBuffer[i++] = (nStep*n)+nStep;
					m_pBodies[nVar].pnCornerIndexBuffer[i++] = nStep*n;
				}
				if (nCorner[5]) {
					m_pBodies[nVar].pnCornerIndexBuffer[i++] = nPatchSize*n;
					m_pBodies[nVar].pnCornerIndexBuffer[i++] = ((nPatchSize-nStep)*n)+nStep;
					m_pBodies[nVar].pnCornerIndexBuffer[i++] = (nPatchSize*n)+nStep;
				}
				if (nCorner[6]) {
					m_pBodies[nVar].pnCornerIndexBuffer[i++] = (nPatchSize*n)+nPatchSize;
					m_pBodies[nVar].pnCornerIndexBuffer[i++] = ((nPatchSize-nStep)*n)+nPatchSize-nStep;
					m_pBodies[nVar].pnCornerIndexBuffer[i++] = ((nPatchSize-nStep)*n)+nPatchSize;
				}
				if (nCorner[7]) {
					m_pBodies[nVar].pnCornerIndexBuffer[i++] = nPatchSize;
					m_pBodies[nVar].pnCornerIndexBuffer[i++] = (nStep*n)+nPatchSize-nStep;
					m_pBodies[nVar].pnCornerIndexBuffer[i++] = nPatchSize-nStep;
				}
			}

			// Create index buffer
			IndexBuffer *pIndexBuffer = m_pRenderer->CreateIndexBuffer();
			m_pBodies[nVar].pCornerIndexBuffer = pIndexBuffer;
			uint32 nIndices = m_pBodies[nVar].nCornerIndexSize;
			pIndexBuffer->Allocate(nIndices, Usage::Static, false);
			if (pIndexBuffer->Lock(Lock::WriteOnly)) {
				uint32 *pIndexT = m_pBodies[nVar].pnCornerIndexBuffer;
				for (uint32 i=0; i<nIndices; i++) {
					pIndexBuffer->SetData(i, *pIndexT);
					pIndexT++;
				}
				pIndexBuffer->Unlock();
			}
		} else {
			m_pBodies[nVar].pnCornerIndexBuffer = nullptr;
			m_pBodies[nVar].nCornerIndexSize    = 0;
			m_pBodies[nVar].pCornerIndexBuffer  = nullptr;
		}
	}
}

/**
*  @brief
*    Builds the linkers
*/
void GMMSurfaceLevel::BuildLinkers(uint32 nLevels, uint32 nPatchSize)
{
	if (nLevels) {
		m_pLinkers = new LinkingPiece[nLevels];

		const uint32 StripsHigh = 1<<nLevels;
		const uint32 HighStep   = nPatchSize/StripsHigh;

		for (uint32 nLowLevel=0; nLowLevel<nLevels; nLowLevel++) {
			for (uint32 nSide=0; nSide<4; nSide++) {
				const uint32 nStripsLow = 1<<nLowLevel;
				const uint32 nLowStep   = nPatchSize/nStripsLow;
				const uint32 nBuffSize  = nStripsLow*(3+ (nLowStep/(2*HighStep)-1)*6) + (nStripsLow-1)*6;

				int    n			= nPatchSize+1;
				uint32 nOffset		= 0;
				bool   nSwitchOrder = false;

				m_pLinkers[nLowLevel].pnIndexBuffer[nSide] = new uint32[nBuffSize];
				m_pLinkers[nLowLevel].pIndexBuffer[nSide] = nullptr;
				uint32 *pBuffer = m_pLinkers[nLowLevel].pnIndexBuffer[nSide];

				// Horizontal piece
				if (1 == nSide || 3 == nSide) {
					int f = 1;
					if (3 == nSide) {
						nOffset = nPatchSize;
						f = -f;
						nSwitchOrder = true;
					}

					uint32 nNum = 0;
					for (uint32 i=nOffset; i<nOffset+nPatchSize*n; i+=nLowStep*n) {
						uint32 j = i;

						if (nSwitchOrder)
							pBuffer[nNum++] = f*HighStep + (i+(nLowStep/2)*n);
						pBuffer[nNum++] = i;
						if (!nSwitchOrder)
							pBuffer[nNum++] = f*HighStep + (i+(nLowStep/2)*n);
						pBuffer[nNum++] = i+nLowStep*n;

						for (j=i+HighStep*n; j<i+(nLowStep/2)*n; j+=HighStep*n) {
							if (nSwitchOrder)
								pBuffer[nNum++] = f*HighStep+ j+HighStep*n;
							pBuffer[nNum++] = f*HighStep+ j;
							if (!nSwitchOrder)
								pBuffer[nNum++] = f*HighStep+ j+HighStep*n;
							pBuffer[nNum++] = i;
						}
						for (j=i+(nLowStep-HighStep)*n; j>i+(nLowStep/2)*n; j-=HighStep*n) {
							if (nSwitchOrder)
								pBuffer[nNum++] = i+nLowStep*n;
							pBuffer[nNum++] = f*HighStep+ j;
							if (!nSwitchOrder)
								pBuffer[nNum++] = i+nLowStep*n;
							pBuffer[nNum++] = f*HighStep+ j-HighStep*n;
						}
						if (i<nOffset+nPatchSize*n && i>nOffset) {
							if (nSwitchOrder)
								pBuffer[nNum++]  = i;
							pBuffer[nNum++] = (f*HighStep)+i;
							if (!nSwitchOrder)
								pBuffer[nNum++] = i;
							pBuffer[nNum++] = (f*HighStep)+(i-HighStep*n);

							if (nSwitchOrder)
								pBuffer[nNum++]  = i;
							pBuffer[nNum++] = (f*HighStep)+(i+HighStep*n);
							if (!nSwitchOrder)
								pBuffer[nNum++] = i;
							pBuffer[nNum++] = (f*HighStep)+i;
						}
					}
					m_pLinkers[nLowLevel].nIndexBufferSize[nSide] = nNum;
				}

				// Vertical piece
				if (0 == nSide || 2 == nSide) {
					if (!nSide) {
						nOffset = nPatchSize*n;
						n = -n;
						nSwitchOrder = true;
					}

					uint32 nNum = 0;
					for (uint32 i=nOffset; i<nPatchSize+nOffset; i+=nLowStep) {
						if (nSwitchOrder)
							pBuffer[nNum++] = HighStep*n + (i+nLowStep/2);
						pBuffer[nNum++] = i+nLowStep;
						if (!nSwitchOrder)
							pBuffer[nNum++] = HighStep*n + (i+nLowStep/2);
						pBuffer[nNum++] = i;

						for (uint32 j=i+HighStep; j<i+nLowStep/2; j+=HighStep) {
							if (nSwitchOrder)
								pBuffer[nNum++] = HighStep*n+ j+HighStep;
							pBuffer[nNum++] = i;
							if (!nSwitchOrder)
								pBuffer[nNum++] = HighStep*n+ j+HighStep;
							pBuffer[nNum++] = HighStep*n+ j;
						}
						for (uint32 j=i+nLowStep-HighStep; j>i+nLowStep/2; j-=HighStep) {
							if (nSwitchOrder)
								pBuffer[nNum++]  = i+nLowStep;
							pBuffer[nNum++] = HighStep*n+ j-HighStep;
							if (!nSwitchOrder)
								pBuffer[nNum++] = i+nLowStep;
							pBuffer[nNum++] = HighStep*n+ j;
						}
						if (i<nPatchSize+nOffset && i>nOffset) {
							if (nSwitchOrder)
								pBuffer[nNum++]  = i;
							pBuffer[nNum++] = HighStep*n+ i-HighStep;
							if (!nSwitchOrder)
								pBuffer[nNum++] = i;
							pBuffer[nNum++] = HighStep*n+ i;

							if (nSwitchOrder)
								pBuffer[nNum++]  = HighStep*n+ i;
							pBuffer[nNum++] = HighStep*n+ i+HighStep;
							if (!nSwitchOrder)
								pBuffer[nNum++] = HighStep*n+ i;
							pBuffer[nNum++] = i;
						}
					}
					m_pLinkers[nLowLevel].nIndexBufferSize[nSide] = nNum;
				}

				// Create index buffer
				IndexBuffer *pIndexBuffer = m_pRenderer->CreateIndexBuffer();
				m_pLinkers[nLowLevel].pIndexBuffer[nSide] = pIndexBuffer;
				uint32 nIndices = nBuffSize;
				pIndexBuffer->Allocate(nIndices, Usage::Static, false);
				if (pIndexBuffer->Lock(Lock::WriteOnly)) {
					uint32 *pIndexT = m_pLinkers[nLowLevel].pnIndexBuffer[nSide];
					for (uint32 i=0; i<nIndices; i++) {
						pIndexBuffer->SetData(i, *pIndexT);
						pIndexT++;
					}

					// Unlock the index buffer
					pIndexBuffer->Unlock();
				}
			}
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLNature
