/*********************************************************\
 *  File: FixedFunctionsTextureStageStates.h             *
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


#ifndef __PLRENDERER_FIXEDFUNCTIONSTEXTURESTAGESTATES_H__
#define __PLRENDERER_FIXEDFUNCTIONSTEXTURESTAGESTATES_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Object.h>
#include "PLRenderer/Renderer/FixedFunctions.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Fixed functions texture stage states RTTI wrapper class
*/
class FixedFunctionsTextureStageStates : public PLCore::Object {


	//[-------------------------------------------------------]
	//[ RTTI interface                                        ]
	//[-------------------------------------------------------]
	pl_class(PLRENDERER_RTTI_EXPORT, FixedFunctionsTextureStageStates, "PLRenderer", PLCore::Object, "Fixed functions texture stage states RTTI wrapper class")
		// Attributes
		pl_attribute(ColorTexEnv,	pl_enum_type(FixedFunctions::TextureEnvironment::Enum),	FixedFunctions::TextureEnvironment::Modulate,	ReadWrite,	GetSet,	"Color texture environment mode",		"")
		pl_attribute(AlphaTexEnv,	pl_enum_type(FixedFunctions::TextureEnvironment::Enum),	FixedFunctions::TextureEnvironment::Modulate,	ReadWrite,	GetSet,	"Alpha texture environment mode",		"")
		pl_attribute(TexGen,		pl_enum_type(FixedFunctions::TexCoordGen::Enum),		FixedFunctions::TexCoordGen::None,				ReadWrite,	GetSet,	"Texture coordinate generation mode",	"")
	pl_class_end


	//[-------------------------------------------------------]
	//[ Public RTTI get/set functions                         ]
	//[-------------------------------------------------------]
	public:
		PLRENDERER_API FixedFunctions::TextureEnvironment::Enum GetColorTexEnv() const;
		PLRENDERER_API void SetColorTexEnv(FixedFunctions::TextureEnvironment::Enum nValue);
		PLRENDERER_API FixedFunctions::TextureEnvironment::Enum GetAlphaTexEnv() const;
		PLRENDERER_API void SetAlphaTexEnv(FixedFunctions::TextureEnvironment::Enum nValue);
		PLRENDERER_API FixedFunctions::TexCoordGen::Enum GetTexGen() const;
		PLRENDERER_API void SetTexGen(FixedFunctions::TexCoordGen::Enum nValue);


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		PLRENDERER_API FixedFunctionsTextureStageStates();

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		PLRENDERER_API FixedFunctionsTextureStageStates(const FixedFunctionsTextureStageStates &cSource);

		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~FixedFunctionsTextureStageStates();

		/**
		*  @brief
		*    Retrieves a texture stage state value
		*
		*  @param[in] nState
		*    State variable that is being queried. This parameter can
		*    be any member of the texture stage enumerated type.
		*
		*  @return
		*    The value of the queried texture stage state variable
		*/
		PLRENDERER_API PLCore::uint32 Get(FixedFunctions::TextureStage::Enum nState) const;

		/**
		*  @brief
		*    Sets a single texture stage state parameter
		*
		*  @param[in] nState
		*    State variable that is being modified. This parameter
		*    can be any member of the texture stage enumerated type.
		*  @param[in] nValue
		*    New value for the texture stage state to be set. The meaning of
		*    this parameter is dependent on the value specified for nState.
		*    For example, if nState is FixedFunctions::TextureStage::ColorTexEnv, the second parameter
		*    must be one member of the texture environment enumerated type. (e.g. FixedFunctions::TextureEnvironment::Modulate)
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLRENDERER_API bool Set(FixedFunctions::TextureStage::Enum nState, PLCore::uint32 nValue);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    This class
		*/
		PLRENDERER_API FixedFunctionsTextureStageStates &operator =(const FixedFunctionsTextureStageStates &cSource);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Exported variables
		PLCore::uint32 m_nTSS[FixedFunctions::TextureStage::Number];	/**< List of texture stage states (see FixedFunctions::TextureStage) */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


#endif // __PLRENDERER_FIXEDFUNCTIONSTEXTURESTAGESTATES_H__
