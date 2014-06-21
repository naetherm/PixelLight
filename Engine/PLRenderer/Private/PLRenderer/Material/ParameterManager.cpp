/*********************************************************\
 *  File: ParameterManager.cpp                           *
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
#include <PLCore/Xml/Xml.h>
#include "PLRenderer/RendererContext.h"
#include "PLRenderer/Renderer/TextureBuffer.h"
#include "PLRenderer/Texture/Texture.h"
#include "PLRenderer/Texture/TextureManager.h"
#include "PLRenderer/Material/Parameter.h"
#include "PLRenderer/Material/ParameterManager.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Creates a new parameter
*/
Parameter *ParameterManager::CreateParameter(EDataType nType, const PLCore::String &sName)
{
	// Check parameter
	if (nType >= UnknownDataType)
		return nullptr; // Error!

	// IS there already a parameter with this name?
	Parameter *pParameter = m_mapParameters.Get(sName);
	if (pParameter)
		return pParameter;

	// Create the new parameter
	pParameter = new Parameter(*this, nType, sName);
	m_lstParameters.Add(pParameter);
	m_mapParameters.Add(sName, pParameter);

	// Return the created parameter
	return pParameter;
}

/**
*  @brief
*    Add all parameters to a given XML element
*/
bool ParameterManager::AddParametersToXMLElement(XmlElement &cElement) const
{
	// Are there any parameters we can add?
	const uint32 nNumOfParameters = m_lstParameters.GetNumOfElements();
	if (nNumOfParameters) {
		// Loop through all parameters
		for (uint32 i=0; i<nNumOfParameters; i++) {
			// Get the parameter
			const Parameter *pParameter = m_lstParameters[i];

			// Create the XML element
			XmlElement *pParameterElement = new XmlElement(DataTypeToString(pParameter->m_nType));

			// Name
			if (pParameter->m_sName.GetLength())
				pParameterElement->SetAttribute("Name", pParameter->m_sName);

			// Write parameter value instead
			const PLCore::String sValue = pParameter->GetParameterString();
			if (sValue.GetLength()) {
				XmlText *pValue = new XmlText(sValue);
				pParameterElement->LinkEndChild(*pValue);
			}

			// Link parameter element
			cElement.LinkEndChild(*pParameterElement);
		}
	}

	// Done
	return true;
}

/**
*  @brief
*    Adds a parameter from a given XML element
*/
bool ParameterManager::AddParameterFromXMLElement(const XmlElement &cElement)
{
	// Type
	const EDataType nType = DataTypeFromString(cElement.GetValue());
	if (nType != UnknownDataType) {
		// Name
		const PLCore::String sName = cElement.GetAttribute("Name");
		if (sName.GetLength()) {
			// IS there already a parameter with this name?
			if (m_mapParameters.Get(sName))
				return true; // Jep, ignore this new parameter

			// Create the new parameter right now
			Parameter *pParameter = CreateParameter(nType, sName);
			if (pParameter) {
				// Set value, if there's one
				const XmlNode *pValue = cElement.GetFirstChild();
				if (pValue && pValue->GetType() == XmlNode::Text && pValue->GetValue().GetLength())
					pParameter->SetParameterString(pValue->GetValue());

				// Done
				return true;
			}
		}
	}

	// Error!
	return false;
}

/**
*  @brief
*    Removes a parameter by index
*/
bool ParameterManager::RemoveParameter(uint32 nIndex)
{
	// Get the parameter to remove
	Parameter *pParameter = m_lstParameters[nIndex];
	if (!pParameter)
		return false; // Error!

	// Remove
	m_lstParameters.Remove(pParameter);
	m_mapParameters.Remove(pParameter->m_sName);
	delete pParameter;

	// Done
	return true;
}

/**
*  @brief
*    Removes a parameter by name
*/
bool ParameterManager::RemoveParameter(const PLCore::String &sName)
{
	// Get the parameter to remove
	Parameter *pParameter = m_mapParameters.Get(sName);
	if (!pParameter)
		return false; // Error!

	// Remove
	m_lstParameters.Remove(pParameter);
	delete pParameter;
	m_mapParameters.Remove(sName);

	// Done
	return true;
}

/**
*  @brief
*    Removes all parameters
*/
void ParameterManager::RemoveAllParameters()
{
	for (uint32 i=0; i<m_lstParameters.GetNumOfElements(); i++)
		delete m_lstParameters[i];
	m_lstParameters.Clear();
	m_mapParameters.Clear();
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
ParameterManager::ParameterManager(RendererContext &cRendererContext) :
	m_pRendererContext(&cRendererContext)
{
}

/**
*  @brief
*    Destructor
*/
ParameterManager::~ParameterManager()
{
	RemoveAllParameters();
}


//[-------------------------------------------------------]
//[ Public virtual ParameterManager functions             ]
//[-------------------------------------------------------]
/**
*  @brief
*    Reloads all textures
*/
bool ParameterManager::ReloadTextures() const
{
	// Are there any parameters?
	const uint32 nNumOfParameters = m_lstParameters.GetNumOfElements();
	if (!nNumOfParameters)
		return true; // Work done ;-)

	// Loop through all parameters and collect the textures we have to reload. In this
	// way we avoid reloading the same texture multiple times if it is used by multiple parameters. :)
	List<Texture*> lstTextures;
	for (uint32 i=0; i<nNumOfParameters; i++) {
		// Get the parameter
		const Parameter *pParameter = m_lstParameters[i];

		// Is this a texture?
		if (pParameter->GetType() == TextureBuffer) {
			// Get the texture
			Texture *pTexture = pParameter->GetValueTexture();
			if (pTexture) {
				// Is this texture already within the list?
				if (!lstTextures.IsElement(pTexture))
					lstTextures.Add(pTexture); // Nope, add it right now!
			}
		}
	}

	// Reload the collected textures
	bool bResult = true; // No error by default
	Iterator<Texture*> cIterator = lstTextures.GetIterator();
	while (cIterator.HasNext()) {
		if (!cIterator.Next()->Reload())
			bResult = false; // Error!
	}

	// Done
	return bResult;
}

/**
*  @brief
*    Called when a parameter has been changed (created, destroyed, value changed)
*/
void ParameterManager::OnParameterChange(Parameter &cParameter) const
{
	// Emit event
	EventParameterChanged(cParameter);
}


//[-------------------------------------------------------]
//[ Public virtual Parameters functions                   ]
//[-------------------------------------------------------]
bool ParameterManager::IsParameter(const PLCore::String &sName) const
{
	return (m_mapParameters.Get(sName) != nullptr);
}

String ParameterManager::GetParameterString(const PLCore::String &sName) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	return pParameter ? pParameter->GetParameterString() : "";
}

bool ParameterManager::SetParameterString(const PLCore::String &sName, const PLCore::String &sValue)
{
	Parameter *pParameter = CreateParameter(String, sName);
	return (pParameter && pParameter->SetParameterString(sValue));
}


// Integer
bool ParameterManager::GetParameter1i(const PLCore::String &sName, int &nX) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	return (pParameter && pParameter->GetValue1i(nX));
}

bool ParameterManager::SetParameter1i(const PLCore::String &sName, int nX)
{
	Parameter *pParameter = CreateParameter(Integer, sName);
	return (pParameter && pParameter->SetValue1i(nX));
}

// Integer2
bool ParameterManager::GetParameter2i(const PLCore::String &sName, int &nX, int &nY) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	return (pParameter && pParameter->GetValue2i(nX, nY));
}

bool ParameterManager::GetParameter2iv(const PLCore::String &sName, int nValue[]) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	if (pParameter)
		return pParameter->GetValue2i(nValue[0], nValue[1]);
	else {
		// Error!
		return false;
	}
}

bool ParameterManager::SetParameter2i(const PLCore::String &sName, int nX, int nY)
{
	Parameter *pParameter = CreateParameter(Integer2, sName);
	return (pParameter && pParameter->SetValue2i(nX, nY));
}

bool ParameterManager::SetParameter2iv(const PLCore::String &sName, const int nValue[])
{
	Parameter *pParameter = CreateParameter(Integer2, sName);
	return (pParameter && pParameter->SetValue2iv(nValue));
}

// Integer3
bool ParameterManager::GetParameter3i(const PLCore::String &sName, int &nX, int &nY, int &nZ) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	return (pParameter && pParameter->GetValue3i(nX, nY, nZ));
}

bool ParameterManager::GetParameter3iv(const PLCore::String &sName, int nValue[]) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	if (pParameter)
		return pParameter->GetValue3i(nValue[0], nValue[1], nValue[2]);
	else {
		// Error!
		return false;
	}
}

bool ParameterManager::SetParameter3i(const PLCore::String &sName, int nX, int nY, int nZ)
{
	Parameter *pParameter = CreateParameter(Integer3, sName);
	return (pParameter && pParameter->SetValue3i(nX, nY, nZ));
}

bool ParameterManager::SetParameter3iv(const PLCore::String &sName, const int nValue[])
{
	Parameter *pParameter = CreateParameter(Integer3, sName);
	return (pParameter && pParameter->SetValue3iv(nValue));
}

// Integer4
bool ParameterManager::GetParameter4i(const PLCore::String &sName, int &nX, int &nY, int &nZ, int &nW) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	return (pParameter && pParameter->GetValue4i(nX, nY, nZ, nW));
}

bool ParameterManager::GetParameter4iv(const PLCore::String &sName, int nValue[]) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	if (pParameter)
		return pParameter->GetValue4i(nValue[0], nValue[1], nValue[2], nValue[3]);
	else {
		// Error!
		return false;
	}
}

bool ParameterManager::SetParameter4i(const PLCore::String &sName, int nX, int nY, int nZ, int nW)
{
	Parameter *pParameter = CreateParameter(Integer4, sName);
	return (pParameter && pParameter->SetValue4i(nX, nY, nZ, nW));
}

bool ParameterManager::SetParameter4iv(const PLCore::String &sName, const int nValue[])
{
	Parameter *pParameter = CreateParameter(Integer4, sName);
	return (pParameter && pParameter->SetValue4iv(nValue));
}


// Float
bool ParameterManager::GetParameter1f(const PLCore::String &sName, float &fX) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	return (pParameter && pParameter->GetValue1f(fX));
}

bool ParameterManager::SetParameter1f(const PLCore::String &sName, float fX)
{
	Parameter *pParameter = CreateParameter(Float, sName);
	return (pParameter && pParameter->SetValue1f(fX));
}

// Float2
bool ParameterManager::GetParameter2f(const PLCore::String &sName, float &fX, float &fY) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	return (pParameter && pParameter->GetValue2f(fX, fY));
}

bool ParameterManager::GetParameter2fv(const PLCore::String &sName, float fValue[]) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	if (pParameter)
		return pParameter->GetValue2f(fValue[0], fValue[1]);
	else {
		// Error!
		return false;
	}
}

bool ParameterManager::SetParameter2f(const PLCore::String &sName, float fX, float fY)
{
	Parameter *pParameter = CreateParameter(Float2, sName);
	return (pParameter && pParameter->SetValue2f(fX, fY));
}

bool ParameterManager::SetParameter2fv(const PLCore::String &sName, const float fValue[])
{
	Parameter *pParameter = CreateParameter(Float2, sName);
	return (pParameter && pParameter->SetValue2fv(fValue));
}

// Float3
bool ParameterManager::GetParameter3f(const PLCore::String &sName, float &fX, float &fY, float &fZ) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	return (pParameter && pParameter->GetValue3f(fX, fY, fZ));
}

bool ParameterManager::GetParameter3fv(const PLCore::String &sName, float fValue[]) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	if (pParameter)
		return pParameter->GetValue3f(fValue[0], fValue[1], fValue[2]);
	else {
		// Error!
		return false;
	}
}

bool ParameterManager::SetParameter3f(const PLCore::String &sName, float fX, float fY, float fZ)
{
	Parameter *pParameter = CreateParameter(Float3, sName);
	return (pParameter && pParameter->SetValue3f(fX, fY, fZ));
}

bool ParameterManager::SetParameter3fv(const PLCore::String &sName, const float fValue[])
{
	Parameter *pParameter = CreateParameter(Float3, sName);
	return (pParameter && pParameter->SetValue3fv(fValue));
}

// Float4
bool ParameterManager::GetParameter4f(const PLCore::String &sName, float &fX, float &fY, float &fZ, float &fW) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	return (pParameter && pParameter->GetValue4f(fX, fY, fZ, fW));
}

bool ParameterManager::GetParameter4fv(const PLCore::String &sName, float fValue[]) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	if (pParameter)
		return pParameter->GetValue4f(fValue[0], fValue[1], fValue[2], fValue[3]);
	else {
		// Error!
		return false;
	}
}

bool ParameterManager::SetParameter4f(const PLCore::String &sName, float fX, float fY, float fZ, float fW)
{
	Parameter *pParameter = CreateParameter(Float4, sName);
	return (pParameter && pParameter->SetValue4f(fX, fY, fZ, fW));
}

bool ParameterManager::SetParameter4fv(const PLCore::String &sName, const float fValue[])
{
	Parameter *pParameter = CreateParameter(Float4, sName);
	return (pParameter && pParameter->SetValue4fv(fValue));
}


// Double
bool ParameterManager::GetParameter1d(const PLCore::String &sName, double &fX) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	return (pParameter && pParameter->GetValue1d(fX));
}

bool ParameterManager::SetParameter1d(const PLCore::String &sName, double fX)
{
	Parameter *pParameter = CreateParameter(Double, sName);
	return (pParameter && pParameter->SetValue1d(fX));
}

// Double2
bool ParameterManager::GetParameter2d(const PLCore::String &sName, double &fX, double &fY) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	return (pParameter && pParameter->GetValue2d(fX, fY));
}

bool ParameterManager::GetParameter2dv(const PLCore::String &sName, double fValue[]) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	if (pParameter)
		return pParameter->GetValue2d(fValue[0], fValue[1]);
	else {
		// Error!
		return false;
	}
}

bool ParameterManager::SetParameter2d(const PLCore::String &sName, double fX, double fY)
{
	Parameter *pParameter = CreateParameter(Double2, sName);
	return (pParameter && pParameter->SetValue2d(fX, fY));
}

bool ParameterManager::SetParameter2dv(const PLCore::String &sName, const double fValue[])
{
	Parameter *pParameter = CreateParameter(Double2, sName);
	return (pParameter && pParameter->SetValue2dv(fValue));
}

// Double3
bool ParameterManager::GetParameter3d(const PLCore::String &sName, double &fX, double &fY, double &fZ) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	return (pParameter && pParameter->GetValue3d(fX, fY, fZ));
}

bool ParameterManager::GetParameter3dv(const PLCore::String &sName, double fValue[]) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	if (pParameter)
		return pParameter->GetValue3d(fValue[0], fValue[1], fValue[2]);
	else {
		// Error!
		return false;
	}
}

bool ParameterManager::SetParameter3d(const PLCore::String &sName, double fX, double fY, double fZ)
{
	Parameter *pParameter = CreateParameter(Double3, sName);
	return (pParameter && pParameter->SetValue3d(fX, fY, fZ));
}

bool ParameterManager::SetParameter3dv(const PLCore::String &sName, const double fValue[])
{
	Parameter *pParameter = CreateParameter(Double3, sName);
	return (pParameter && pParameter->SetValue3dv(fValue));
}

// Double4
bool ParameterManager::GetParameter4d(const PLCore::String &sName, double &fX, double &fY, double &fZ, double &fW) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	return (pParameter && pParameter->GetValue4d(fX, fY, fZ, fW));
}

bool ParameterManager::GetParameter4dv(const PLCore::String &sName, double fValue[]) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	if (pParameter)
		return pParameter->GetValue4d(fValue[0], fValue[1], fValue[2], fValue[3]);
	else {
		// Error!
		return false;
	}
}

bool ParameterManager::SetParameter4d(const PLCore::String &sName, double fX, double fY, double fZ, double fW)
{
	Parameter *pParameter = CreateParameter(Double4, sName);
	return (pParameter && pParameter->SetValue4d(fX, fY, fZ, fW));
}

bool ParameterManager::SetParameter4dv(const PLCore::String &sName, const double fValue[])
{
	Parameter *pParameter = CreateParameter(Double4, sName);
	return (pParameter && pParameter->SetValue4dv(fValue));
}


// Float3x3
bool ParameterManager::GetParameterFloat3x3(const PLCore::String &sName, float fValue[]) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	if (pParameter) {
		const float *pfTemp = pParameter->GetValueFloat3x3();
		if (pfTemp) {
			MemoryManager::Copy(fValue, pfTemp, sizeof(float)*3*3);

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

bool ParameterManager::SetParameterFloat3x3(const PLCore::String &sName, const float fValue[])
{
	Parameter *pParameter = CreateParameter(Float3x3, sName);
	return (pParameter && pParameter->SetValueFloat3x3(fValue));
}


// Float3x4
bool ParameterManager::GetParameterFloat3x4(const PLCore::String &sName, float fValue[]) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	if (pParameter) {
		const float *pfTemp = pParameter->GetValueFloat3x4();
		if (pfTemp) {
			MemoryManager::Copy(fValue, pfTemp, sizeof(float)*3*4);

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

bool ParameterManager::SetParameterFloat3x4(const PLCore::String &sName, const float fValue[])
{
	Parameter *pParameter = CreateParameter(Float3x4, sName);
	return (pParameter && pParameter->SetValueFloat3x4(fValue));
}


// Float4x4
bool ParameterManager::GetParameterMatrixfv(const PLCore::String &sName, float fValue[]) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	if (pParameter) {
		const float *pfTemp = pParameter->GetValueMatrixfv();
		if (pfTemp) {
			MemoryManager::Copy(fValue, pfTemp, sizeof(float)*4*4);

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

bool ParameterManager::SetParameterMatrixfv(const PLCore::String &sName, const float fValue[])
{
	Parameter *pParameter = CreateParameter(Float4x4, sName);
	return (pParameter && pParameter->SetValueMatrixfv(fValue));
}


// Double4x4
bool ParameterManager::GetParameterMatrixdv(const PLCore::String &sName, double fValue[]) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	if (pParameter) {
		const double *pfTemp = pParameter->GetValueMatrixdv();
		if (pfTemp) {
			MemoryManager::Copy(fValue, pfTemp, sizeof(double)*4*4);

			// Done
			return true;
		}
	}

	// Error!
	return false;
}

bool ParameterManager::SetParameterMatrixdv(const PLCore::String &sName, const double fValue[])
{
	Parameter *pParameter = CreateParameter(Double4x4, sName);
	return (pParameter && pParameter->SetValueMatrixdv(fValue));
}


// Texture buffer
TextureBuffer *ParameterManager::GetParameterTextureBuffer(const PLCore::String &sName) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	if (pParameter) {
		const Texture *pTexture = pParameter->GetValueTexture();
		return pTexture ? pTexture->GetTextureBuffer() : nullptr;
	} else {
		// Error!
		return nullptr;
	}
}

int ParameterManager::SetParameterTextureBuffer(const PLCore::String &sName, PLRenderer::TextureBuffer *pTextureBuffer)
{
	Parameter *pParameter = CreateParameter(TextureBuffer, sName);
	if (pParameter) {
		// Is this texture already set?
		Texture *pValueTexture = pParameter->GetValueTexture();
		if (!pValueTexture || pValueTexture->GetTextureBuffer() != pTextureBuffer) {
			// Texture buffer given?
			if (pTextureBuffer) {
				// Is the texture used by multiple handlers?
				if (!pValueTexture || pValueTexture->GetNumOfHandlers() > 1) {
					// We have to create a 'mesh texture' were we can 'embed' the given 'texture buffer'. Normally unused textures
					// are removed automatically, so, this shouldn't be a problem....
					Texture *pTexture = pTextureBuffer->GetRenderer().GetRendererContext().GetTextureManager().Create();
					pTexture->SetTextureBuffer(pTextureBuffer);
					return !pParameter->SetValueTexture(pTexture) ? -1 : 0;
				} else {
					// It's safe to just set the new texture buffer :)
					pValueTexture->SetTextureBuffer(pTextureBuffer);
				}
			} else {
				return !pParameter->SetValueTexture(nullptr) ? -1 : 0;
			}
		}

		// Done
		return 0;
	} else {
		// Error!
		return -1;
	}
}

Texture *ParameterManager::GetParameterTexture(const PLCore::String &sName) const
{
	const Parameter *pParameter = m_mapParameters.Get(sName);
	return pParameter ? pParameter->GetValueTexture() : nullptr;
}

int ParameterManager::SetParameterTexture(const PLCore::String &sName, const PLCore::String &sFilename)
{
	Parameter *pParameter = CreateParameter(TextureBuffer, sName);
	if (pParameter) {
		return !pParameter->SetValueTexture(GetRendererContext().GetTextureManager().LoadResource(sFilename)) ? -1 : 0;
	} else {
		// Error!
		return -1;
	}
}

int ParameterManager::SetParameterTexture(const PLCore::String &sName, Texture *pTexture)
{
	Parameter *pParameter = CreateParameter(TextureBuffer, sName);
	if (pParameter) {
		return !pParameter->SetValueTexture(pTexture) ? -1 : 0;
	} else {
		// Error!
		return -1;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
