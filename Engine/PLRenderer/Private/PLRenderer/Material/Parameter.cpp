/*********************************************************\
 *  File: Parameter.cpp                                  *
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
#include <PLCore/String/Tokenizer.h>
#include <PLMath/Matrix3x3.h>
#include <PLMath/Matrix3x4.h>
#include "PLRenderer/RendererContext.h"
#include "PLRenderer/Renderer/Program.h"
#include "PLRenderer/Renderer/ProgramUniform.h"
#include "PLRenderer/Texture/Texture.h"
#include "PLRenderer/Texture/TextureHandler.h"
#include "PLRenderer/Material/ParameterManager.h"
#include "PLRenderer/Material/Parameter.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLMath;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Sets the value of this parameter to a parameter within the given manager
*/
bool Parameter::SetManagerParameterValue(Parameters &cManager, const PLCore::String &sName) const
{
	// Set parameter
	switch (m_nType) {
		case Parameters::String:
			cManager.SetParameterString(sName, *static_cast<const PLCore::String*>(m_pValue));
			break;

		case Parameters::Integer:
			cManager.SetParameter1i(sName, *static_cast<const int*>(m_pValue));
			break;

		case Parameters::Integer2:
			cManager.SetParameter2iv(sName, static_cast<const int*>(m_pValue));
			break;

		case Parameters::Integer3:
			cManager.SetParameter3iv(sName, static_cast<const int*>(m_pValue));
			break;

		case Parameters::Integer4:
			cManager.SetParameter4iv(sName, static_cast<const int*>(m_pValue));
			break;

		case Parameters::Float:
			cManager.SetParameter1f(sName, *static_cast<const float*>(m_pValue));
			break;

		case Parameters::Float2:
			cManager.SetParameter2fv(sName, static_cast<const float*>(m_pValue));
			break;

		case Parameters::Float3:
			cManager.SetParameter3fv(sName, static_cast<const float*>(m_pValue));
			break;

		case Parameters::Float4:
			cManager.SetParameter4fv(sName, static_cast<const float*>(m_pValue));
			break;

		case Parameters::Double:
			cManager.SetParameter1d(sName, *static_cast<const double*>(m_pValue));
			break;

		case Parameters::Double2:
			cManager.SetParameter2dv(sName, static_cast<const double*>(m_pValue));
			break;

		case Parameters::Double3:
			cManager.SetParameter3dv(sName, static_cast<const double*>(m_pValue));
			break;

		case Parameters::Double4:
			cManager.SetParameter4dv(sName, static_cast<const double*>(m_pValue));
			break;

		case Parameters::Float3x3:
			cManager.SetParameterFloat3x3(sName, static_cast<const float*>(m_pValue));
			break;

		case Parameters::Float3x4:
			cManager.SetParameterFloat3x4(sName, static_cast<const float*>(m_pValue));
			break;

		case Parameters::Float4x4:
			cManager.SetParameterMatrixfv(sName, static_cast<const float*>(m_pValue));
			break;

		case Parameters::Double4x4:
			cManager.SetParameterMatrixdv(sName, static_cast<const double*>(m_pValue));
			break;

		case Parameters::TextureBuffer:
			return false; // Error, not supported

		case Parameters::UnknownDataType:
		default:
			return false; // Error, not supported
	}

	// Done
	return true;
}

/**
*  @brief
*    Sets the value of this parameter to a parameter within the given manager GPU program
*/
bool Parameter::SetManagerParameterValue(Program &cProgram, const PLCore::String &sName) const
{
	// Get the GPU program uniform
	ProgramUniform *pProgramUniform = cProgram.GetUniform(sName);
	if (pProgramUniform) {
		// Set parameter
		switch (m_nType) {
			case Parameters::String:
				// GPU programs don't have string parameters
				break;

			case Parameters::Integer:
				pProgramUniform->Set(*static_cast<const int*>(m_pValue));
				break;

			case Parameters::Integer2:
				pProgramUniform->Set2(static_cast<const int*>(m_pValue));
				break;

			case Parameters::Integer3:
				pProgramUniform->Set3(static_cast<const int*>(m_pValue));
				break;

			case Parameters::Integer4:
				pProgramUniform->Set4(static_cast<const int*>(m_pValue));
				break;

			case Parameters::Float:
				pProgramUniform->Set(*static_cast<const float*>(m_pValue));
				break;

			case Parameters::Float2:
				pProgramUniform->Set2(static_cast<const float*>(m_pValue));
				break;

			case Parameters::Float3:
				pProgramUniform->Set3(static_cast<const float*>(m_pValue));
				break;

			case Parameters::Float4:
				pProgramUniform->Set4(static_cast<const float*>(m_pValue));
				break;

			case Parameters::Double:
				pProgramUniform->Set(*static_cast<const double*>(m_pValue));
				break;

			case Parameters::Double2:
				pProgramUniform->Set2(static_cast<const double*>(m_pValue));
				break;

			case Parameters::Double3:
				pProgramUniform->Set3(static_cast<const double*>(m_pValue));
				break;

			case Parameters::Double4:
				pProgramUniform->Set4(static_cast<const double*>(m_pValue));
				break;

			case Parameters::Float3x3:
				// [TODO] New shader interface: Add Set3x3 method?
				//pProgramUniform->Set(static_cast<const float*>(m_pValue));
				pProgramUniform->Set(Matrix3x3(static_cast<const float*>(m_pValue)));
				break;

			case Parameters::Float3x4:
				// [TODO] New shader interface: Add Set3x4 method?
				//pProgramUniform->Set(static_cast<const float*>(m_pValue));
				break;

			case Parameters::Float4x4:
				// [TODO] New shader interface: Add Set4x4 method?
				pProgramUniform->Set(Matrix4x4(static_cast<const float*>(m_pValue)));
				break;

			case Parameters::Double4x4:
				// [TODO] New shader interface: Add Set4x4 method?
				//pProgramUniform->Set(static_cast<const double*>(m_pValue));
				break;

			case Parameters::TextureBuffer:
				return false; // Error, not supported

			case Parameters::UnknownDataType:
			default:
				return false; // Error, not supported
		}

		// Done
		return true;
	}

	// Error!
	return false;
}

/**
*  @brief
*    Set the parameter name
*/
bool Parameter::SetName(const PLCore::String &sName)
{
	// Check parameter
	if (sName.GetLength()) {
		// Same name?
		if (m_sName == sName)
			return true; // Nothing to do

		// Is there already a parameter with the given name inside the parameter manager this parameter is in?
		if (!m_pManager->GetParameter(sName)) {
			// Set the new name
			m_sName = sName;

			// Inform the parameter manager about the change
			m_pManager->OnParameterChange(*this);

			// Done
			return true;
		}
	}

	// Error!
	return false;
}


//[-------------------------------------------------------]
//[ Get/set value                                         ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns a parameter value as string (e.g. "1.0 5.4 0.21")
*/
PLCore::String Parameter::GetParameterString() const
{
	switch (m_nType) {
		case Parameters::String:
			return *static_cast<const PLCore::String*>(m_pValue);

		case Parameters::Integer:
			return PLCore::String(*static_cast<const int*>(m_pValue));

		case Parameters::Integer2:
			return PLCore::String::Format("%d %d", static_cast<const int*>(m_pValue)[0], static_cast<const int*>(m_pValue)[1]);

		case Parameters::Integer3:
			return PLCore::String::Format("%d %d %d", static_cast<const int*>(m_pValue)[0], static_cast<const int*>(m_pValue)[1], static_cast<const int*>(m_pValue)[2]);

		case Parameters::Integer4:
			return PLCore::String::Format("%d %d %d %d", static_cast<const int*>(m_pValue)[0], static_cast<const int*>(m_pValue)[1], static_cast<const int*>(m_pValue)[2], static_cast<const int*>(m_pValue)[3]);

		case Parameters::Float:
			return PLCore::String::Format("%g", *static_cast<const float*>(m_pValue));

		case Parameters::Float2:
			return PLCore::String::Format("%g %g", static_cast<const float*>(m_pValue)[0], static_cast<const float*>(m_pValue)[1]);

		case Parameters::Float3:
			return PLCore::String::Format("%g %g %g", static_cast<const float*>(m_pValue)[0], static_cast<const float*>(m_pValue)[1], static_cast<const float*>(m_pValue)[2]);

		case Parameters::Float4:
			return PLCore::String::Format("%g %g %g %g", static_cast<const float*>(m_pValue)[0], static_cast<const float*>(m_pValue)[1], static_cast<const float*>(m_pValue)[2], static_cast<const float*>(m_pValue)[3]);

		case Parameters::Double:
			return PLCore::String::Format("%g", *static_cast<const double*>(m_pValue));

		case Parameters::Double2:
			return PLCore::String::Format("%g %g", static_cast<const double*>(m_pValue)[0], static_cast<const double*>(m_pValue)[1]);

		case Parameters::Double3:
			return PLCore::String::Format("%g %g %g", static_cast<const double*>(m_pValue)[0], static_cast<const double*>(m_pValue)[1], static_cast<const double*>(m_pValue)[2]);

		case Parameters::Double4:
			return PLCore::String::Format("%g %g %g %g", static_cast<const double*>(m_pValue)[0], static_cast<const double*>(m_pValue)[1], static_cast<const double*>(m_pValue)[2], static_cast<const double*>(m_pValue)[3]);

		case Parameters::Float3x3:
		{
			const float *pfValue = static_cast<const float*>(m_pValue);
			return PLCore::String::Format("%g %g %g %g %g %g %g %g %g",
										 pfValue[0], pfValue[1], pfValue[2],
										 pfValue[3], pfValue[4], pfValue[5],
										 pfValue[6], pfValue[7], pfValue[8]);
		}

		case Parameters::Float3x4:
		{
			const float *pfValue = static_cast<const float*>(m_pValue);
			return PLCore::String::Format("%g %g %g %g %g %g %g %g %g %g %g %g",
										 pfValue[0], pfValue[1], pfValue[2], pfValue[3],
										 pfValue[4], pfValue[5], pfValue[6], pfValue[7],
										 pfValue[8], pfValue[9], pfValue[10], pfValue[11]);
		}

		case Parameters::Float4x4:
		{
			const float *pfValue = static_cast<const float*>(m_pValue);
			return PLCore::String::Format("%g %g %g %g %g %g %g %g %g %g %g %g %g %g %g %g",
										 pfValue[0], pfValue[1], pfValue[2], pfValue[3],
										 pfValue[4], pfValue[5], pfValue[6], pfValue[7],
										 pfValue[8], pfValue[9], pfValue[10], pfValue[11],
										 pfValue[12], pfValue[13], pfValue[14], pfValue[15]);
		}

		case Parameters::Double4x4:
		{
			const double *pfValue = static_cast<const double*>(m_pValue);
			return PLCore::String::Format("%g %g %g %g %g %g %g %g %g %g %g %g %g %g %g %g",
										 pfValue[0], pfValue[1], pfValue[2], pfValue[3],
										 pfValue[4], pfValue[5], pfValue[6], pfValue[7],
										 pfValue[8], pfValue[9], pfValue[10], pfValue[11],
										 pfValue[12], pfValue[13], pfValue[14], pfValue[15]);
		}

		case Parameters::TextureBuffer:
		{
			const Texture *pTexture = GetValueTexture();
			return pTexture ? pTexture->GetName() : "";
		}

		case Parameters::UnknownDataType:
		default:
			return ""; // Error!
	}
}

/**
*  @brief
*    Sets a parameter value from a string (e.g. "1.0 5.4 0.21")
*/
bool Parameter::SetParameterString(const PLCore::String &sValue)
{
	// Empty value string?
	if (!sValue.GetLength())
		return false; // Error!

	// Set parameter value
	if (m_nType == Parameters::TextureBuffer) {
		// We can use the value directly
		SetValueTexture(sValue);
	} else {
		// Setup tokenizer
		PLCore::Tokenizer cTokenizer;
		cTokenizer.Start(sValue);

		// Parse
		PLCore::uint32 nIndex = 0;
		switch (m_nType) {
			case Parameters::String:
				*static_cast<PLCore::String*>(m_pValue) = sValue;
				m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
				break;

			case Parameters::Integer:
				while (cTokenizer.GetNextToken().GetLength()) {
					*static_cast<int*>(m_pValue) = cTokenizer.GetToken().GetInt();
					nIndex++;
					if (nIndex >= 1)
						break;
				}
				m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
				break;

			case Parameters::Integer2:
				while (cTokenizer.GetNextToken().GetLength()) {
					static_cast<int*>(m_pValue)[nIndex] = cTokenizer.GetToken().GetInt();
					nIndex++;
					if (nIndex >= 2)
						break;
				}
				m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
				break;

			case Parameters::Integer3:
				while (cTokenizer.GetNextToken().GetLength()) {
					static_cast<int*>(m_pValue)[nIndex] = cTokenizer.GetToken().GetInt();
					nIndex++;
					if (nIndex >= 3)
						break;
				}
				m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
				break;

			case Parameters::Integer4:
				while (cTokenizer.GetNextToken().GetLength()) {
					static_cast<int*>(m_pValue)[nIndex] = cTokenizer.GetToken().GetInt();
					nIndex++;
					if (nIndex >= 4)
						break;
				}
				m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
				break;

			case Parameters::Float:
				while (cTokenizer.GetNextToken().GetLength()) {
					*static_cast<float*>(m_pValue) = cTokenizer.GetToken().GetFloat();
					nIndex++;
					if (nIndex >= 1)
						break;
				}
				m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
				break;

			case Parameters::Float2:
				while (cTokenizer.GetNextToken().GetLength()) {
					static_cast<float*>(m_pValue)[nIndex] = cTokenizer.GetToken().GetFloat();
					nIndex++;
					if (nIndex >= 2)
						break;
				}
				m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
				break;

			case Parameters::Float3:
				while (cTokenizer.GetNextToken().GetLength()) {
					static_cast<float*>(m_pValue)[nIndex] = cTokenizer.GetToken().GetFloat();
					nIndex++;
					if (nIndex >= 3)
						break;
				}
				m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
				break;

			case Parameters::Float4:
				while (cTokenizer.GetNextToken().GetLength()) {
					static_cast<float*>(m_pValue)[nIndex] = cTokenizer.GetToken().GetFloat();
					nIndex++;
					if (nIndex >= 4)
						break;
				}
				m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
				break;

			case Parameters::Double:
				while (cTokenizer.GetNextToken().GetLength()) {
					*static_cast<double*>(m_pValue) = cTokenizer.GetToken().GetDouble();
					nIndex++;
					if (nIndex >= 1)
						break;
				}
				m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
				break;

			case Parameters::Double2:
				while (cTokenizer.GetNextToken().GetLength()) {
					static_cast<double*>(m_pValue)[nIndex] = cTokenizer.GetToken().GetDouble();
					nIndex++;
					if (nIndex >= 2)
						break;
				}
				m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
				break;

			case Parameters::Double3:
				while (cTokenizer.GetNextToken().GetLength()) {
					static_cast<double*>(m_pValue)[nIndex] = cTokenizer.GetToken().GetDouble();
					nIndex++;
					if (nIndex >= 3)
						break;
				}
				m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
				break;

			case Parameters::Double4:
				while (cTokenizer.GetNextToken().GetLength()) {
					static_cast<double*>(m_pValue)[nIndex] = cTokenizer.GetToken().GetDouble();
					nIndex++;
					if (nIndex >= 4)
						break;
				}
				m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
				break;

			case Parameters::Float3x3:
				while (cTokenizer.GetNextToken().GetLength()) {
					static_cast<float*>(m_pValue)[nIndex] = cTokenizer.GetToken().GetFloat();
					nIndex++;
					if (nIndex >= 3*3)
						break;
				}
				m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
				break;

			case Parameters::Float3x4:
				while (cTokenizer.GetNextToken().GetLength()) {
					static_cast<float*>(m_pValue)[nIndex] = cTokenizer.GetToken().GetFloat();
					nIndex++;
					if (nIndex >= 3*4)
						break;
				}
				m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
				break;

			case Parameters::Float4x4:
				while (cTokenizer.GetNextToken().GetLength()) {
					static_cast<float*>(m_pValue)[nIndex] = cTokenizer.GetToken().GetFloat();
					nIndex++;
					if (nIndex >= 4*4)
						break;
				}
				m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
				break;

			case Parameters::Double4x4:
				while (cTokenizer.GetNextToken().GetLength()) {
					static_cast<double*>(m_pValue)[nIndex] = cTokenizer.GetToken().GetDouble();
					nIndex++;
					if (nIndex >= 4*4)
						break;
				}
				m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
				break;

			case Parameters::TextureBuffer:
			case Parameters::UnknownDataType:
			default:
				// Nothing to do in here
				break;
		}

		// Stop tokenizer
		cTokenizer.Stop();
	}

	// Done
	return true;
}

// Integer
bool Parameter::SetValue1i(int nX)
{
	if (m_nType != Parameters::Integer)
		return false; // Error!
	*static_cast<int*>(m_pValue) = nX;
	m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
	return true; // Done
}

// Integer2
bool Parameter::SetValue2i(int nX, int nY)
{
	if (m_nType != Parameters::Integer2)
		return false; // Error!
	static_cast<int*>(m_pValue)[0] = nX;
	static_cast<int*>(m_pValue)[1] = nY;
	m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
	return true; // Done
}

bool Parameter::SetValue2iv(const int nValue[])
{
	if (m_nType != Parameters::Integer2)
		return false; // Error!
	static_cast<int*>(m_pValue)[0] = nValue[0];
	static_cast<int*>(m_pValue)[1] = nValue[1];
	m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
	return true; // Done
}

// Integer3
bool Parameter::SetValue3i(int nX, int nY, int nZ)
{
	if (m_nType != Parameters::Integer3)
		return false; // Error!
	static_cast<int*>(m_pValue)[0] = nX;
	static_cast<int*>(m_pValue)[1] = nY;
	static_cast<int*>(m_pValue)[2] = nZ;
	m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
	return true; // Done
}

bool Parameter::SetValue3iv(const int nValue[])
{
	if (m_nType != Parameters::Integer3)
		return false; // Error!
	static_cast<int*>(m_pValue)[0] = nValue[0];
	static_cast<int*>(m_pValue)[1] = nValue[1];
	static_cast<int*>(m_pValue)[2] = nValue[2];
	m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
	return true; // Done
}

// Integer4
bool Parameter::SetValue4i(int nX, int nY, int nZ, int nW)
{
	if (m_nType != Parameters::Integer4)
		return false; // Error!
	static_cast<int*>(m_pValue)[0] = nX;
	static_cast<int*>(m_pValue)[1] = nY;
	static_cast<int*>(m_pValue)[2] = nZ;
	static_cast<int*>(m_pValue)[3] = nW;
	m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
	return true; // Done
}

bool Parameter::SetValue4iv(const int nValue[])
{
	if (m_nType != Parameters::Integer4)
		return false; // Error!
	static_cast<int*>(m_pValue)[0] = nValue[0];
	static_cast<int*>(m_pValue)[1] = nValue[1];
	static_cast<int*>(m_pValue)[2] = nValue[2];
	static_cast<int*>(m_pValue)[3] = nValue[3];
	m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
	return true; // Done
}


// Float
bool Parameter::SetValue1f(float fX)
{
	if (m_nType != Parameters::Float)
		return false; // Error!
	*static_cast<float*>(m_pValue) = fX;
	m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
	return true; // Done
}

// Float2
bool Parameter::SetValue2f(float fX, float fY)
{
	if (m_nType != Parameters::Float2)
		return false; // Error!
	static_cast<float*>(m_pValue)[0] = fX;
	static_cast<float*>(m_pValue)[1] = fY;
	m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
	return true; // Done
}

bool Parameter::SetValue2fv(const float fValue[])
{
	if (m_nType != Parameters::Float2)
		return false; // Error!
	static_cast<float*>(m_pValue)[0] = fValue[0];
	static_cast<float*>(m_pValue)[1] = fValue[1];
	m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
	return true; // Done
}

// Float3
bool Parameter::SetValue3f(float fX, float fY, float fZ)
{
	if (m_nType != Parameters::Float3)
		return false; // Error!
	static_cast<float*>(m_pValue)[0] = fX;
	static_cast<float*>(m_pValue)[1] = fY;
	static_cast<float*>(m_pValue)[2] = fZ;
	m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
	return true; // Done
}

bool Parameter::SetValue3fv(const float fValue[])
{
	if (m_nType != Parameters::Float3)
		return false; // Error!
	static_cast<float*>(m_pValue)[0] = fValue[0];
	static_cast<float*>(m_pValue)[1] = fValue[1];
	static_cast<float*>(m_pValue)[2] = fValue[2];
	m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
	return true; // Done
}

// Float4
bool Parameter::SetValue4f(float fX, float fY, float fZ, float fW)
{
	if (m_nType != Parameters::Float4)
		return false; // Error!
	static_cast<float*>(m_pValue)[0] = fX;
	static_cast<float*>(m_pValue)[1] = fY;
	static_cast<float*>(m_pValue)[2] = fZ;
	static_cast<float*>(m_pValue)[3] = fW;
	m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
	return true; // Done
}

bool Parameter::SetValue4fv(const float fValue[])
{
	if (m_nType != Parameters::Float4)
		return false; // Error!
	static_cast<float*>(m_pValue)[0] = fValue[0];
	static_cast<float*>(m_pValue)[1] = fValue[1];
	static_cast<float*>(m_pValue)[2] = fValue[2];
	static_cast<float*>(m_pValue)[3] = fValue[3];
	m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
	return true; // Done
}


// Double
bool Parameter::SetValue1d(double fX)
{
	if (m_nType != Parameters::Double)
		return false; // Error!
	*static_cast<double*>(m_pValue) = fX;
	m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
	return true; // Done
}

// Double2
bool Parameter::SetValue2d(double fX, double fY)
{
	if (m_nType != Parameters::Double2)
		return false; // Error!
	static_cast<double*>(m_pValue)[0] = fX;
	static_cast<double*>(m_pValue)[1] = fY;
	m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
	return true; // Done
}

bool Parameter::SetValue2dv(const double fValue[])
{
	if (m_nType != Parameters::Double2)
		return false; // Error!
	static_cast<double*>(m_pValue)[0] = fValue[0];
	static_cast<double*>(m_pValue)[1] = fValue[1];
	m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
	return true; // Done
}

// Double3
bool Parameter::SetValue3d(double fX, double fY, double fZ)
{
	if (m_nType != Parameters::Double3)
		return false; // Error!
	static_cast<double*>(m_pValue)[0] = fX;
	static_cast<double*>(m_pValue)[1] = fY;
	static_cast<double*>(m_pValue)[2] = fZ;
	m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
	return true; // Done
}

bool Parameter::SetValue3dv(const double fValue[])
{
	if (m_nType != Parameters::Double3)
		return false; // Error!
	static_cast<double*>(m_pValue)[0] = fValue[0];
	static_cast<double*>(m_pValue)[1] = fValue[1];
	static_cast<double*>(m_pValue)[2] = fValue[2];
	m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
	return true; // Done
}

// Double4
bool Parameter::SetValue4d(double fX, double fY, double fZ, double fW)
{
	if (m_nType != Parameters::Double4)
		return false; // Error!
	static_cast<double*>(m_pValue)[0] = fX;
	static_cast<double*>(m_pValue)[1] = fY;
	static_cast<double*>(m_pValue)[2] = fZ;
	static_cast<double*>(m_pValue)[3] = fW;
	m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
	return true; // Done
}

bool Parameter::SetValue4dv(const double fValue[])
{
	if (m_nType != Parameters::Double4)
		return false; // Error!
	static_cast<double*>(m_pValue)[0] = fValue[0];
	static_cast<double*>(m_pValue)[1] = fValue[1];
	static_cast<double*>(m_pValue)[2] = fValue[2];
	static_cast<double*>(m_pValue)[3] = fValue[3];
	m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
	return true; // Done
}


// Float3x3
bool Parameter::SetValueFloat3x3(const float fValue[])
{
	if (m_nType != Parameters::Float3x3)
		return false; // Error!
	PLCore::MemoryManager::Copy(m_pValue, fValue, sizeof(float)*3*3);
	m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
	return true; // Done
}


// Float3x4
bool Parameter::SetValueFloat3x4(const float fValue[])
{
	if (m_nType != Parameters::Float3x4)
		return false; // Error!
	PLCore::MemoryManager::Copy(m_pValue, fValue, sizeof(float)*3*4);
	m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
	return true; // Done
}


// Float4x4
bool Parameter::SetValueMatrixfv(const float fValue[])
{
	if (m_nType != Parameters::Float4x4)
		return false; // Error!
	PLCore::MemoryManager::Copy(m_pValue, fValue, sizeof(float)*4*4);
	m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
	return true; // Done
}


// Double4x4
bool Parameter::SetValueMatrixdv(const double fValue[])
{
	if (m_nType != Parameters::Double4x4)
		return false; // Error!
	PLCore::MemoryManager::Copy(m_pValue, fValue, sizeof(double)*4*4);
	m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
	return true; // Done
}


// Texture
Texture *Parameter::GetValueTexture() const
{
	return (m_nType == Parameters::TextureBuffer) ? static_cast<const TextureHandler*>(m_pValue)->GetTexture() : nullptr;
}

TextureBuffer *Parameter::GetValueTextureBuffer() const
{
	if (m_nType == Parameters::TextureBuffer) {
		Texture *pTexture = static_cast<const TextureHandler*>(m_pValue)->GetTexture();
		return pTexture ? pTexture->GetTextureBuffer() : nullptr;
	} else {
		return nullptr;
	}
}

bool Parameter::SetValueTexture(Texture *pTexture)
{
	if (m_nType != Parameters::TextureBuffer)
		return false; // Error!
	static_cast<TextureHandler*>(m_pValue)->SetResource(pTexture);
	m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
	return true; // Done
}

bool Parameter::SetValueTexture(const PLCore::String &sFilename)
{
	if (m_nType != Parameters::TextureBuffer)
		return false; // Error!
	TextureHandler *pTextureHandler = static_cast<TextureHandler*>(m_pValue);
	if (!pTextureHandler->GetResource() || pTextureHandler->GetResource()->GetFilename() != sFilename) {
		pTextureHandler->Load(m_pManager->GetRendererContext().GetTextureManager(), sFilename);
		m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
		return (pTextureHandler->GetResource() != nullptr); // Done
	} else {
		// Done
		m_pManager->OnParameterChange(*this);	// Inform the parameter manager about the change
		return true;
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor
*/
Parameter::Parameter(ParameterManager &cManager, Parameters::EDataType nType, const PLCore::String &sName) :
	m_pManager(&cManager),
	m_nType(nType),
	m_sName(sName)
{
	switch (m_nType) {
		case Parameters::String:
			m_pValue = new PLCore::String();
			break;

		case Parameters::Integer:
			m_pValue = new int;
			*static_cast<int*>(m_pValue) = 0;
			break;

		case Parameters::Integer2:
			m_pValue = new int[2];
			static_cast<int*>(m_pValue)[0] = 0;
			static_cast<int*>(m_pValue)[1] = 0;
			break;

		case Parameters::Integer3:
			m_pValue = new int[3];
			static_cast<int*>(m_pValue)[0] = 0;
			static_cast<int*>(m_pValue)[1] = 0;
			static_cast<int*>(m_pValue)[3] = 0;
			break;

		case Parameters::Integer4:
			m_pValue = new int[4];
			static_cast<int*>(m_pValue)[0] = 0;
			static_cast<int*>(m_pValue)[1] = 0;
			static_cast<int*>(m_pValue)[2] = 0;
			static_cast<int*>(m_pValue)[3] = 0;
			break;

		case Parameters::Float:
			m_pValue = new float;
			*static_cast<float*>(m_pValue) = 0.0f;
			break;

		case Parameters::Float2:
			m_pValue = new float[2];
			static_cast<float*>(m_pValue)[0] = 0.0f;
			static_cast<float*>(m_pValue)[1] = 0.0f;
			break;

		case Parameters::Float3:
			m_pValue = new float[3];
			static_cast<float*>(m_pValue)[0] = 0.0f;
			static_cast<float*>(m_pValue)[1] = 0.0f;
			static_cast<float*>(m_pValue)[2] = 0.0f;
			break;

		case Parameters::Float4:
			m_pValue = new float[4];
			static_cast<float*>(m_pValue)[0] = 0.0f;
			static_cast<float*>(m_pValue)[1] = 0.0f;
			static_cast<float*>(m_pValue)[2] = 0.0f;
			static_cast<float*>(m_pValue)[3] = 0.0f;
			break;

		case Parameters::Double:
			m_pValue = new double;
			*static_cast<double*>(m_pValue) = 0.0;
			break;

		case Parameters::Double2:
			m_pValue = new double[2];
			static_cast<double*>(m_pValue)[0] = 0.0;
			static_cast<double*>(m_pValue)[1] = 0.0;
			break;

		case Parameters::Double3:
			m_pValue = new double[3];
			static_cast<double*>(m_pValue)[0] = 0.0;
			static_cast<double*>(m_pValue)[1] = 0.0;
			static_cast<double*>(m_pValue)[2] = 0.0;
			break;

		case Parameters::Double4:
			m_pValue = new double[4];
			static_cast<double*>(m_pValue)[0] = 0.0;
			static_cast<double*>(m_pValue)[1] = 0.0;
			static_cast<double*>(m_pValue)[2] = 0.0;
			static_cast<double*>(m_pValue)[3] = 0.0;
			break;

		case Parameters::Float3x3:
			m_pValue = new float[3*3];
			PLCore::MemoryManager::Set(m_pValue, 0, sizeof(float)*3*3);
			break;

		case Parameters::Float3x4:
			m_pValue = new float[3*4];
			PLCore::MemoryManager::Set(m_pValue, 0, sizeof(float)*3*4);
			break;

		case Parameters::Float4x4:
			m_pValue = new float[4*4];
			PLCore::MemoryManager::Set(m_pValue, 0, sizeof(float)*4*4);
			break;

		case Parameters::Double4x4:
			m_pValue = new double[4*4];
			PLCore::MemoryManager::Set(m_pValue, 0, sizeof(double)*4*4);
			break;

		case Parameters::TextureBuffer:
			m_pValue = new TextureHandler();
			break;

		case Parameters::UnknownDataType:
		default:
			m_pValue = nullptr;
			break;
	}

	// A new parameter was added - inform the parameter manager about the change
	m_pManager->OnParameterChange(*this);
}

/**
*  @brief
*    Destructor
*/
Parameter::~Parameter()
{
	// The parameter is in the process of getting killed - inform the parameter manager about the change
	m_pManager->OnParameterChange(*this);

	// Cleanup
	if (m_pValue) {
		// Cast pointer if required (MUST be one of this types!)
		switch (m_nType) {
			case Parameters::String:
				delete static_cast<PLCore::String*>(m_pValue);
				break;

			case Parameters::Integer:
				delete static_cast<int*>(m_pValue);
				break;

			case Parameters::Integer2:
				delete [] static_cast<int*>(m_pValue);
				break;

			case Parameters::Integer3:
				delete [] static_cast<int*>(m_pValue);
				break;

			case Parameters::Integer4:
				delete [] static_cast<int*>(m_pValue);
				break;

			case Parameters::Float:
				delete static_cast<float*>(m_pValue);
				break;

			case Parameters::Float2:
				delete [] static_cast<float*>(m_pValue);
				break;

			case Parameters::Float3:
				delete [] static_cast<float*>(m_pValue);
				break;

			case Parameters::Float4:
				delete [] static_cast<float*>(m_pValue);
				break;

			case Parameters::Double:
				delete static_cast<double*>(m_pValue);
				break;

			case Parameters::Double2:
				delete [] static_cast<double*>(m_pValue);
				break;

			case Parameters::Double3:
				delete [] static_cast<double*>(m_pValue);
				break;

			case Parameters::Double4:
				delete [] static_cast<double*>(m_pValue);
				break;

			case Parameters::Float3x3:
				delete [] static_cast<float*>(m_pValue);
				break;

			case Parameters::Float3x4:
				delete [] static_cast<float*>(m_pValue);
				break;

			case Parameters::Float4x4:
				delete [] static_cast<float*>(m_pValue);
				break;

			case Parameters::Double4x4:
				delete [] static_cast<double*>(m_pValue);
				break;

			case Parameters::TextureBuffer:
				delete static_cast<TextureHandler*>(m_pValue);
				break;

			case Parameters::UnknownDataType:
			default:
				// Nothing to do in here
				break;
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
