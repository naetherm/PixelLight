/*********************************************************\
 *  File: SNTerrain.h                                    *
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


#ifndef __PLNATURE_SCENENODE_TERRAIN_H__
#define __PLNATURE_SCENENODE_TERRAIN_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Tools/Loadable.h>
#include <PLScene/Scene/SceneNode.h>
#include "PLNature/PLNature.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLGraphics {
	class Image;
}
namespace PLNature {
	class GMMSurface;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLNature {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Terrain scene node
*/
class SNTerrain : public PLScene::SceneNode, public PLCore::Loadable {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	// [TODO] Add additional functions to the interface and remove this
	friend class TerrainLoaderPL;


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLNATURE_RTTI_EXPORT, SNTerrain, "PLNature", PLScene::SceneNode, "Terrain scene node")
		// Attributes
		pl_attribute(Filename,	PLCore::String, "",	ReadWrite,	GetSet,	"Terrain filename",	"Type='Terrain'")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLNATURE_API PLCore::String GetFilename() const;
		PLNATURE_API void SetFilename(const PLCore::String &sValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLNATURE_API SNTerrain();

		/**
		*  @brief
		*    Destructor
		*/
		PLNATURE_API virtual ~SNTerrain();

		/**
		*  @brief
		*    Returns the GMM surface
		*
		*  @return
		*    The terrain GMM surface, can be a null pointer
		*/
		PLNATURE_API GMMSurface *GetGMMSurface() const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Loads the height map
		*
		*  @param[in]  sHeightMap
		*    Filename of the height map to load
		*  @param[out] cHeightMap
		*    Receives the loaded height map
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		bool LoadHeightMap(const PLCore::String &sHeightMap, PLGraphics::Image &cHeightMap);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::String  m_sFilename;	/**< Terrain filename */
		float		   *m_pfHeightMap;	/**< Height map, can be a null pointer */
		GMMSurface	   *m_pGMMSurface;	/**< Terrain GMM surface, can be a null pointer */


	//[-------------------------------------------------------]
	//[ Public virtual PLScene::SceneNode functions           ]
	//[-------------------------------------------------------]
	public:
		PLNATURE_API virtual void DrawSolid(PLRenderer::Renderer &cRenderer, const PLScene::VisNode *pVisNode = nullptr) override;


	//[-------------------------------------------------------]
	//[ Private virtual PLScene::SceneNode functions          ]
	//[-------------------------------------------------------]
	private:
		virtual void InitFunction() override;


	//[-------------------------------------------------------]
	//[ Public virtual PLCore::Loadable functions             ]
	//[-------------------------------------------------------]
	public:
		PLNATURE_API virtual bool Unload() override;
		PLNATURE_API virtual PLCore::String GetLoadableTypeName() const override;


	//[-------------------------------------------------------]
	//[ Private virtual PLCore::Loadable functions            ]
	//[-------------------------------------------------------]
	private:
		virtual bool CallLoadable(PLCore::File &cFile, PLCore::Loader &cLoader, const PLCore::String &sMethod, const PLCore::String &sParams) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLNature


#endif // __PLNATURE_SCENENODE_TERRAIN_H__
