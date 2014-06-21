/*********************************************************\
 *  File: ParameterManager.h                             *
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


#ifndef __PLRENDERER_MATERIAL_PARAMETERMANAGER_H__
#define __PLRENDERER_MATERIAL_PARAMETERMANAGER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Event/Event.h>
#include "PLRenderer/Renderer/Parameters.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class XmlElement;
}
namespace PLRenderer {
	class Parameter;
	class RendererContext;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Parameter manager
*
*  @remarks
*    The implementation of the 'SetParameter()' functions automatically create 'missing' parameters,
*    if you don't want this, use for example 'IsParameter()' before calling 'SetParameter()'.
*/
class ParameterManager : public Parameters {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class Effect;
	friend class Material;
	friend class Parameter;


	//[-------------------------------------------------------]
	//[ Events                                                ]
	//[-------------------------------------------------------]
	public:
		PLCore::Event<Parameter&> EventParameterChanged;	/**< Parameter changed (created, destroyed, value changed) event, changed parameter as parameter */


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the owner renderer context
		*
		*  @return
		*    Reference to the owner renderer context
		*/
		inline RendererContext &GetRendererContext() const;

		/**
		*  @brief
		*    Gets the number of parameters
		*
		*  @return
		*    Number of parameters
		*/
		inline PLCore::uint32 GetNumOfParameters() const;

		/**
		*  @brief
		*    Creates a new parameter
		*
		*  @param[in] nType
		*    Type of the new parameter, NEVER UnknownDataType!
		*  @param[in] sName
		*    Name of the new parameter, if there's already a parameter with this name this
		*    parameter is returned instead creating a new one
		*
		*  @return
		*    The created parameter, a null pointer on error (maybe unknown data type?)
		*/
		PLRENDERER_API Parameter *CreateParameter(EDataType nType, const PLCore::String &sName);

		/**
		*  @brief
		*    Gets a parameter by index
		*
		*  @param[in] nIndex
		*    Parameter index
		*
		*  @return
		*    The requested parameter, a null pointer on error
		*/
		inline Parameter *GetParameter(PLCore::uint32 nIndex) const;

		/**
		*  @brief
		*    Gets a parameter by name
		*
		*  @param[in] sName
		*    Parameter name
		*
		*  @return
		*    The requested parameter, a null pointer on error
		*/
		inline Parameter *GetParameter(const PLCore::String &sName) const;

		/**
		*  @brief
		*    Add all parameters to a given XML element
		*
		*  @param[out] cElement
		*    XML element were the parameters should be added
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe invalid XML element)
		*/
		PLRENDERER_API bool AddParametersToXMLElement(PLCore::XmlElement &cElement) const;

		/**
		*  @brief
		*    Adds a parameter from a given XML element
		*
		*  @param[in] cElement
		*    XML element were the parameter should be added from
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*    (maybe invalid XML element or it's no known parameter type or no parameter name given)
		*
		*  @note
		*    - If there's already a parameter with this name, this parameter to add is ignored (returns false)
		*/
		PLRENDERER_API bool AddParameterFromXMLElement(const PLCore::XmlElement &cElement);

		/**
		*  @brief
		*    Removes a parameter by index
		*
		*  @param[in] nIndex
		*    Index of the parameter name to remove
		*
		*  @return
		*    'true' if all went fine and the parameter was removed, else 'false'
		*    (maybe unknown parameter?)
		*/
		PLRENDERER_API bool RemoveParameter(PLCore::uint32 nIndex);

		/**
		*  @brief
		*    Removes a parameter by name
		*
		*  @param[in] sName
		*    Name of the parameter name to remove
		*
		*  @return
		*    'true' if all went fine and the parameter was removed, else 'false'
		*    (maybe unknown parameter?)
		*/
		PLRENDERER_API bool RemoveParameter(const PLCore::String &sName);

		/**
		*  @brief
		*    Removes all parameters
		*/
		PLRENDERER_API void RemoveAllParameters();


	//[-------------------------------------------------------]
	//[ Public virtual ParameterManager functions             ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Reloads all textures
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*/
		PLRENDERER_API virtual bool ReloadTextures() const;

		/**
		*  @brief
		*    Called when a parameter has been changed (created, destroyed, value changed)
		*
		*  @param[in] cParameter
		*    Changed parameter
		*
		*  @note
		*    - The default implementation just emits the "EventParameterChanged"-event
		*/
		PLRENDERER_API virtual void OnParameterChange(Parameter &cParameter) const;


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cRendererContext
		*    Renderer context to use
		*/
		ParameterManager(RendererContext &cRendererContext);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ParameterManager();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		RendererContext							    *m_pRendererContext;	/**< Renderer context to use, always valid! */
		PLCore::Array<Parameter*>				     m_lstParameters;
		PLCore::HashMap<PLCore::String, Parameter*>  m_mapParameters;


	//[-------------------------------------------------------]
	//[ Public virtual Parameters functions                   ]
	//[-------------------------------------------------------]
	public:
		PLRENDERER_API virtual bool IsParameter(const PLCore::String &sName) const override;
		PLRENDERER_API virtual PLCore::String GetParameterString(const PLCore::String &sName) const override;
		PLRENDERER_API virtual bool SetParameterString(const PLCore::String &sName, const PLCore::String &sValue) override;
		// Integer
		PLRENDERER_API virtual bool GetParameter1i(const PLCore::String &sName, int &nX) const override;
		PLRENDERER_API virtual bool SetParameter1i(const PLCore::String &sName, int nX) override;
		// Integer2
		PLRENDERER_API virtual bool GetParameter2i(const PLCore::String &sName, int &nX, int &nY) const override;
		PLRENDERER_API virtual bool GetParameter2iv(const PLCore::String &sName, int nValue[]) const override;
		PLRENDERER_API virtual bool SetParameter2i(const PLCore::String &sName, int nX, int nY) override;
		PLRENDERER_API virtual bool SetParameter2iv(const PLCore::String &sName, const int nValue[]) override;
		// Integer3
		PLRENDERER_API virtual bool GetParameter3i(const PLCore::String &sName, int &nX, int &nY, int &nZ) const override;
		PLRENDERER_API virtual bool GetParameter3iv(const PLCore::String &sName, int nValue[]) const override;
		PLRENDERER_API virtual bool SetParameter3i(const PLCore::String &sName, int nX, int nY, int nZ) override;
		PLRENDERER_API virtual bool SetParameter3iv(const PLCore::String &sName, const int nValue[]) override;
		// Integer4
		PLRENDERER_API virtual bool GetParameter4i(const PLCore::String &sName, int &nX, int &nY, int &nZ, int &nW) const override;
		PLRENDERER_API virtual bool GetParameter4iv(const PLCore::String &sName, int nValue[]) const override;
		PLRENDERER_API virtual bool SetParameter4i(const PLCore::String &sName, int nX, int nY, int nZ, int nW) override;
		PLRENDERER_API virtual bool SetParameter4iv(const PLCore::String &sName, const int nValue[]) override;
		// Float
		PLRENDERER_API virtual bool GetParameter1f(const PLCore::String &sName, float &fX) const override;
		PLRENDERER_API virtual bool SetParameter1f(const PLCore::String &sName, float fX) override;
		// Float2
		PLRENDERER_API virtual bool GetParameter2f(const PLCore::String &sName, float &fX, float &fY) const override;
		PLRENDERER_API virtual bool GetParameter2fv(const PLCore::String &sName, float fValue[]) const override;
		PLRENDERER_API virtual bool SetParameter2f(const PLCore::String &sName, float fX, float fY) override;
		PLRENDERER_API virtual bool SetParameter2fv(const PLCore::String &sName, const float fValue[]) override;
		// Float3
		PLRENDERER_API virtual bool GetParameter3f(const PLCore::String &sName, float &fX, float &fY, float &fZ) const override;
		PLRENDERER_API virtual bool GetParameter3fv(const PLCore::String &sName, float fValue[]) const override;
		PLRENDERER_API virtual bool SetParameter3f(const PLCore::String &sName, float fX, float fY, float fZ) override;
		PLRENDERER_API virtual bool SetParameter3fv(const PLCore::String &sName, const float fValue[]) override;
		// Float4
		PLRENDERER_API virtual bool GetParameter4f(const PLCore::String &sName, float &fX, float &fY, float &fZ, float &fW) const override;
		PLRENDERER_API virtual bool GetParameter4fv(const PLCore::String &sName, float fValue[]) const override;
		PLRENDERER_API virtual bool SetParameter4f(const PLCore::String &sName, float fX, float fY, float fZ, float fW) override;
		PLRENDERER_API virtual bool SetParameter4fv(const PLCore::String &sName, const float fValue[]) override;
		// Double
		PLRENDERER_API virtual bool GetParameter1d(const PLCore::String &sName, double &fX) const override;
		PLRENDERER_API virtual bool SetParameter1d(const PLCore::String &sName, double fX) override;
		// Double2
		PLRENDERER_API virtual bool GetParameter2d(const PLCore::String &sName, double &fX, double &fY) const override;
		PLRENDERER_API virtual bool GetParameter2dv(const PLCore::String &sName, double fValue[]) const override;
		PLRENDERER_API virtual bool SetParameter2d(const PLCore::String &sName, double fX, double fY) override;
		PLRENDERER_API virtual bool SetParameter2dv(const PLCore::String &sName, const double fValue[]) override;
		// Double3
		PLRENDERER_API virtual bool GetParameter3d(const PLCore::String &sName, double &fX, double &fY, double &fZ) const override;
		PLRENDERER_API virtual bool GetParameter3dv(const PLCore::String &sName, double fValue[]) const override;
		PLRENDERER_API virtual bool SetParameter3d(const PLCore::String &sName, double fX, double fY, double fZ) override;
		PLRENDERER_API virtual bool SetParameter3dv(const PLCore::String &sName, const double fValue[]) override;
		// Double4
		PLRENDERER_API virtual bool GetParameter4d(const PLCore::String &sName, double &fX, double &fY, double &fZ, double &fW) const override;
		PLRENDERER_API virtual bool GetParameter4dv(const PLCore::String &sName, double fValue[]) const override;
		PLRENDERER_API virtual bool SetParameter4d(const PLCore::String &sName, double fX, double fY, double fZ, double fW) override;
		PLRENDERER_API virtual bool SetParameter4dv(const PLCore::String &sName, const double fValue[]) override;
		// Float3x3
		PLRENDERER_API virtual bool GetParameterFloat3x3(const PLCore::String &sName, float fValue[]) const override;
		PLRENDERER_API virtual bool SetParameterFloat3x3(const PLCore::String &sName, const float fValue[]) override;
		// Float3x4
		PLRENDERER_API virtual bool GetParameterFloat3x4(const PLCore::String &sName, float fValue[]) const override;
		PLRENDERER_API virtual bool SetParameterFloat3x4(const PLCore::String &sName, const float fValue[]) override;
		// Float4x4
		PLRENDERER_API virtual bool GetParameterMatrixfv(const PLCore::String &sName, float fValue[]) const override;
		PLRENDERER_API virtual bool SetParameterMatrixfv(const PLCore::String &sName, const float fValue[]) override;
		// Double4x4
		PLRENDERER_API virtual bool GetParameterMatrixdv(const PLCore::String &sName, double fValue[]) const override;
		PLRENDERER_API virtual bool SetParameterMatrixdv(const PLCore::String &sName, const double fValue[]) override;
		// Texture buffer
		PLRENDERER_API virtual PLRenderer::TextureBuffer *GetParameterTextureBuffer(const PLCore::String &sName) const override;
		PLRENDERER_API virtual int SetParameterTextureBuffer(const PLCore::String &sName, PLRenderer::TextureBuffer *pTextureBuffer) override;
		PLRENDERER_API virtual PLRenderer::Texture *GetParameterTexture(const PLCore::String &sName) const override;
		PLRENDERER_API virtual int SetParameterTexture(const PLCore::String &sName, const PLCore::String &sFilename) override;
		PLRENDERER_API virtual int SetParameterTexture(const PLCore::String &sName, PLRenderer::Texture *pTexture) override;


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLRenderer/Material/ParameterManager.inl"


#endif // __PLRENDERER_MATERIAL_PARAMETERMANAGER_H__
