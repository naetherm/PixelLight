/*********************************************************\
 *  File: SNMPostProcess.h                               *
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


#ifndef __PLSCENE_SCENENODEMODIFIER_POSTPROCESS_H__
#define __PLSCENE_SCENENODEMODIFIER_POSTPROCESS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Event/EventHandler.h>
#include <PLScene/Scene/SceneNodeModifier.h>
#include "PLCompositing/PLCompositing.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class Parameter;
}
namespace PLCompositing {
	class PostProcessManager;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCompositing {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract camera scene node post process modifier class
*
*  @note
*    - This class is NOT derived from SNMDraw because in fact, nothing is 'drawn over' the owner scene node!
*/
class SNMPostProcess : public PLScene::SceneNodeModifier {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLCOM_RTTI_EXPORT, SNMPostProcess, "PLCompositing", PLScene::SceneNodeModifier, "Abstract camera scene node post process modifier class")
		// Properties
		pl_properties
			pl_property("SceneNodeClass",	"PLScene::SNCamera")
		pl_properties_end
		// Attributes
		pl_attribute(Filename,		PLCore::String,	"",		ReadWrite,	GetSet,			"Filename of the post process to use",														"Type='PostProcess'")
		pl_attribute(EffectWeight,	float,			1.0f,	ReadWrite,	DirectValue,	"Effect weight, 0 means that this effect has no influence, 1 for the intended influence",	"Min=0 Max=1")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLCOM_API PLCore::String GetFilename() const;
		PLCOM_API void SetFilename(const PLCore::String &sValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the post process manager
		*
		*  @note
		*    - If the post process manager is not initialized yet, this method will initialize it
		*/
		PLCOM_API PostProcessManager &GetPostProcessManager();

		/**
		*  @brief
		*    Returns a post process effect parameter
		*
		*  @param[in] sName
		*    Parameter name
		*  @param[in] nIndex
		*    Post process index
		*  @param[in] bActiveOnly
		*    Do only take active post processes into account?
		*
		*  @return
		*    The requested post process parameter, a null pointer if no such parameter was found or 'bActiveOnly' is used
		*/
		PLCOM_API PLRenderer::Parameter *GetParameter(const PLCore::String &sName, PLCore::uint32 nIndex = 0, bool bActiveOnly = false);

		/**
		*  @brief
		*    Returns all post process effect parameters of a given parameter name
		*
		*  @param[in]  sName
		*    Parameter name
		*  @param[out] lstParameters
		*    Will receive all parameters with the given name (the array is not cleared before filling it)
		*  @param[in]  bActiveOnly
		*    Do only add active post process parameters into the output array?
		*
		*  @return
		*    'true' if all went fine and at least one parameter with the give name was found, else 'false'
		*
		*  @remarks
		*    This function loops through all (or just active) post processes and adds the material parameter
		*    of a given name into the output array.
		*/
		PLCOM_API bool GetParameters(const PLCore::String &sName, PLCore::Array<PLRenderer::Parameter*> &lstParameters, bool bActiveOnly = false);


	//[-------------------------------------------------------]
	//[ Public virtual SNMPostProcess functions               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Sets all current parameter values as current post process settings
		*
		*  @note
		*    - The default implementation sets "EffectWeight" of all post processes to 1, except of the last one - this is set to m_fEffectWeight
		*    - Called before the post process is applied (synchronize post process effect and RTTI parameters)
		*/
		PLCOM_API virtual void SetParameters();


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cSceneNode
		*    Owner scene node
		*/
		PLCOM_API SNMPostProcess(PLScene::SceneNode &cSceneNode);

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API virtual ~SNMPostProcess();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		PLCore::String m_sFilename;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PostProcessManager *m_pPostProcessManager;	/**< Post process manager, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing


#endif // __PLSCENE_SCENENODEMODIFIER_POSTPROCESS_H__
