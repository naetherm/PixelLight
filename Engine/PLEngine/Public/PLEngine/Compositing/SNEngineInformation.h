/*********************************************************\
 *  File: SNEngineInformation.h                          *
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


#ifndef __PLENGINE_COMPOSITING_ENGINEINFORMATION_H__
#define __PLENGINE_COMPOSITING_ENGINEINFORMATION_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLScene/Scene/SceneNode.h>
#include "PLEngine/PLEngine.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class MaterialHandler;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLEngine {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Engine information scene node
*
*  @remarks
*    This scene node can draw different engine/debug information.
*    The drawn depends on the config group 'EngineDebugConfig':
*    ShowFPS, ShowCoordinateAxis, ShowXZPlane, ShowXYPlane, ShowYZPlane
*
*  @note
*    - Should only be used within a scene root container
*    - If profiling is active, you can use the page-up/page-down keys to select the
*      next/previous group
*/
class SNEngineInformation : public PLScene::SceneNode {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Information flags
		*/
		enum EInfoFlags {
			FPS            = 1<<0,	/**< Draw FPS */
			CoordinateAxis = 1<<1,	/**< Draw coordinate axis */
			Planes         = 1<<2,	/**< Draw planes */
			Profiling      = 1<<3	/**< Draw profiling */
		};
		pl_enum(EInfoFlags)
			pl_enum_value(FPS,				"Draw FPS")
			pl_enum_value(CoordinateAxis,	"Draw coordinate axis")
			pl_enum_value(Planes,			"Draw planes")
			pl_enum_value(Profiling,		"Draw profiling")
		pl_enum_end


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PL_RTTI_EXPORT, SNEngineInformation, "PLEngine", PLScene::SceneNode, "Engine information scene node")
		// Attributes
		pl_attribute(InfoFlags,			pl_flag_type(EInfoFlags),	FPS|CoordinateAxis|Planes|Profiling,	ReadWrite,	DirectValue,	"Information flags",	"")
		pl_attribute(ProfilingMaterial,	PLCore::String,				"Data/Effects/PLProfiling.plfx",		ReadWrite,	GetSet,			"Profiling material",	"Type='Material Effect Image TextureAni'")
			// Overwritten SceneNode attributes
		pl_attribute(Flags,				pl_flag_type(EFlags),		NoCulling,								ReadWrite,	GetSet,			"Flags",				"")
		// Constructors
		pl_constructor_0(DefaultConstructor,	"Default constructor",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PL_API PLCore::String GetProfilingMaterial() const;
		PL_API void SetProfilingMaterial(const PLCore::String &sValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PL_API SNEngineInformation();

		/**
		*  @brief
		*    Destructor
		*/
		PL_API virtual ~SNEngineInformation();

		/**
		*  @brief
		*    Returns the handler of the profiling material
		*
		*  @return
		*    Handler of the profiling material
		*/
		PL_API const PLRenderer::MaterialHandler &GetProfilingMaterialHandler();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Draws the information of the current selected profile group
		*
		*  @param[in] cRenderer
		*    The used renderer
		*/
		void DrawProfiling(PLRenderer::Renderer &cRenderer);

		/**
		*  @brief
		*    Called when the scene node needs to be updated
		*/
		void OnUpdate();


	//[-------------------------------------------------------]
	//[ Private event handlers                                ]
	//[-------------------------------------------------------]
	private:
		PLCore::EventHandler<> EventHandlerUpdate;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PLCore::String				 m_sProfilingMaterial;	/**< Profiling material */
		bool						 m_bProfilingMaterial;	/**< Profiling material loaded? */
		PLRenderer::MaterialHandler *m_pProfilingMaterial;	/**< Profiling material (ALWAYS valid!) */


	//[-------------------------------------------------------]
	//[ Public virtual PLScene::SceneNode functions           ]
	//[-------------------------------------------------------]
	public:
		PL_API virtual void DrawPost(PLRenderer::Renderer &cRenderer, const PLScene::VisNode *pVisNode = nullptr) override;


	//[-------------------------------------------------------]
	//[ Protected virtual PLScene::SceneNode functions        ]
	//[-------------------------------------------------------]
	protected:
		PL_API virtual void OnActivate(bool bActivate) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLEngine


#endif // __PLENGINE_COMPOSITING_ENGINEINFORMATION_H__
