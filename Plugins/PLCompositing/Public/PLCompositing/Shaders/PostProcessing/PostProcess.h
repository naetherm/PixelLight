/*********************************************************\
 *  File: PostProcess.h                                  *
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


#ifndef __PLCOMPOSITING_POSTPROCESSING_POSTPROCESS_H__
#define __PLCOMPOSITING_POSTPROCESSING_POSTPROCESS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Container/Element.h>
#include <PLCore/Container/ElementHandler.h>
#include <PLCore/Container/ElementManager.h>
#include <PLMath/Vector3.h>
#include <PLCompositing/PLCompositing.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLRenderer {
	class Material;
	class MaterialHandler;
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
*    Post process class
*/
class PostProcess : public PLCore::Element<PostProcess> {


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] sName
		*    Element name to set
		*  @param[in] cManager
		*    Post process manager using this element
		*/
		PLCOM_API PostProcess(const PLCore::String &sName, PostProcessManager &cManager);

		/**
		*  @brief
		*    Destructor
		*/
		PLCOM_API virtual ~PostProcess();

		/**
		*  @brief
		*    Returns whether the post process is active or not
		*
		*  @return
		*    'true' if the post process is active, else 'false'
		*/
		PLCOM_API bool IsActive() const;

		/**
		*  @brief
		*    Sets whether the post process is active or not
		*
		*  @param[in] bActive
		*    'true' if the post process is active, else 'false'
		*/
		PLCOM_API void SetActive(bool bActive = true);

		/**
		*  @brief
		*    Returns the post process material
		*
		*  @return
		*    Post process material, can be a null pointer
		*/
		PLCOM_API PLRenderer::Material *GetMaterial() const;

		/**
		*  @brief
		*    Sets the post process material
		*
		*  @param[in] pMaterial
		*    Post process material to set, can be a null pointer
		*/
		PLCOM_API void SetMaterial(PLRenderer::Material *pMaterial);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool						 m_bActive;				/**< Is this post process currently active? */
		PLRenderer::MaterialHandler *m_pMaterialHandler;	/**< Handler holding the used material (always valid!) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCompositing


#endif // __PLCOMPOSITING_POSTPROCESSING_POSTPROCESS_H__
