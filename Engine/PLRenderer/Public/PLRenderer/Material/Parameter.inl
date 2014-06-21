/*********************************************************\
 *  File: Parameter.inl                                  *
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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Returns the parameter manager this parameter is in
*/
inline ParameterManager &Parameter::GetManager() const
{
	return *m_pManager;
}

/**
*  @brief
*    Get the parameter type
*/
inline Parameters::EDataType Parameter::GetType() const
{
	return m_nType;
}

/**
*  @brief
*    Get the parameter name
*/
inline PLCore::String Parameter::GetName() const
{
	return m_sName;
}


//[-------------------------------------------------------]
//[ Get/set value                                         ]
//[-------------------------------------------------------]
// Integer
inline bool Parameter::GetValue1i(int &nX) const
{
	if (m_nType != Parameters::Integer)
		return false; // Error!
	nX = *static_cast<const int*>(m_pValue);
	return true; // Done
}

inline int Parameter::GetValue1i() const
{
	return (m_nType == Parameters::Integer) ? *static_cast<const int*>(m_pValue) : 0;
}

// Integer2
inline bool Parameter::GetValue2i(int &nX, int &nY) const
{
	if (m_nType != Parameters::Integer2)
		return false; // Error!
	nX = static_cast<const int*>(m_pValue)[0];
	nY = static_cast<const int*>(m_pValue)[1];
	return true; // Done
}

inline const int *Parameter::GetValue2iv() const
{
	return (m_nType == Parameters::Integer2) ? static_cast<const int*>(m_pValue) : nullptr;
}

// Integer3
inline bool Parameter::GetValue3i(int &nX, int &nY, int &nZ) const
{
	if (m_nType != Parameters::Integer3)
		return false; // Error!
	nX = static_cast<const int*>(m_pValue)[0];
	nY = static_cast<const int*>(m_pValue)[1];
	nZ = static_cast<const int*>(m_pValue)[2];
	return true; // Done
}

inline const int *Parameter::GetValue3iv() const
{
	return (m_nType == Parameters::Integer3) ? static_cast<const int*>(m_pValue) : nullptr;
}

// Integer4
inline bool Parameter::GetValue4i(int &nX, int &nY, int &nZ, int &nW) const
{
	if (m_nType != Parameters::Integer4)
		return false; // Error!
	nX = static_cast<const int*>(m_pValue)[0];
	nY = static_cast<const int*>(m_pValue)[1];
	nZ = static_cast<const int*>(m_pValue)[2];
	nW = static_cast<const int*>(m_pValue)[3];
	return true; // Done
}

inline const int *Parameter::GetValue4iv() const
{
	return (m_nType == Parameters::Integer4) ? static_cast<const int*>(m_pValue) : nullptr;
}


// Float
inline bool Parameter::GetValue1f(float &fX) const
{
	if (m_nType != Parameters::Float)
		return false; // Error!
	fX = *static_cast<const float*>(m_pValue);
	return true; // Done
}

inline float Parameter::GetValue1f() const
{
	return (m_nType == Parameters::Float) ? *static_cast<const float*>(m_pValue) : 0.0f;
}

// Float2
inline bool Parameter::GetValue2f(float &fX, float &fY) const
{
	if (m_nType != Parameters::Float2)
		return false; // Error!
	fX = static_cast<const float*>(m_pValue)[0];
	fY = static_cast<const float*>(m_pValue)[1];
	return true; // Done
}

inline const float *Parameter::GetValue2fv() const
{
	return (m_nType == Parameters::Float2) ? static_cast<const float*>(m_pValue) : nullptr;
}

// Float3
inline bool Parameter::GetValue3f(float &fX, float &fY, float &fZ) const
{
	if (m_nType != Parameters::Float3)
		return false; // Error!
	fX = static_cast<const float*>(m_pValue)[0];
	fY = static_cast<const float*>(m_pValue)[1];
	fZ = static_cast<const float*>(m_pValue)[2];
	return true; // Done
}

inline const float *Parameter::GetValue3fv() const
{
	return (m_nType == Parameters::Float3) ? static_cast<const float*>(m_pValue) : nullptr;
}

// Float4
inline bool Parameter::GetValue4f(float &fX, float &fY, float &fZ, float &fW) const
{
	if (m_nType != Parameters::Float4)
		return false; // Error!
	fX = static_cast<const float*>(m_pValue)[0];
	fY = static_cast<const float*>(m_pValue)[1];
	fZ = static_cast<const float*>(m_pValue)[2];
	fW = static_cast<const float*>(m_pValue)[3];
	return true; // Done
}

inline const float *Parameter::GetValue4fv() const
{
	return (m_nType == Parameters::Float4) ? static_cast<const float*>(m_pValue) : nullptr;
}


// Double
inline bool Parameter::GetValue1d(double &fX) const
{
	if (m_nType != Parameters::Double)
		return false; // Error!
	fX = *static_cast<const double*>(m_pValue);
	return true; // Done
}

// Double2
inline bool Parameter::GetValue2d(double &fX, double &fY) const
{
	if (m_nType != Parameters::Double2)
		return false; // Error!
	fX = static_cast<const double*>(m_pValue)[0];
	fY = static_cast<const double*>(m_pValue)[1];
	return true; // Done
}

inline const double *Parameter::GetValue2dv() const
{
	return (m_nType == Parameters::Double2) ? static_cast<const double*>(m_pValue) : nullptr;
}

// Double3
inline bool Parameter::GetValue3d(double &fX, double &fY, double &fZ) const
{
	if (m_nType != Parameters::Double3)
		return false; // Error!
	fX = static_cast<const double*>(m_pValue)[0];
	fY = static_cast<const double*>(m_pValue)[1];
	fZ = static_cast<const double*>(m_pValue)[2];
	return true; // Done
}

inline const double *Parameter::GetValue3dv() const
{
	return (m_nType == Parameters::Double3) ? static_cast<const double*>(m_pValue) : nullptr;
}

// Double4
inline bool Parameter::GetValue4d(double &fX, double &fY, double &fZ, double &fW) const
{
	if (m_nType != Parameters::Double4)
		return false; // Error!
	fX = static_cast<const double*>(m_pValue)[0];
	fY = static_cast<const double*>(m_pValue)[1];
	fZ = static_cast<const double*>(m_pValue)[2];
	fW = static_cast<const double*>(m_pValue)[3];
	return true; // Done
}

inline const double *Parameter::GetValue4dv() const
{
	return (m_nType == Parameters::Double4) ? static_cast<const double*>(m_pValue) : nullptr;
}


// Float3x3
inline const float *Parameter::GetValueFloat3x3() const
{
	return (m_nType == Parameters::Float3x3) ? static_cast<const float*>(m_pValue) : nullptr;
}


// Float3x4
inline const float *Parameter::GetValueFloat3x4() const
{
	return (m_nType == Parameters::Float3x4) ? static_cast<const float*>(m_pValue) : nullptr;
}


// Float4x4
inline const float *Parameter::GetValueMatrixfv() const
{
	return (m_nType == Parameters::Float4x4) ? static_cast<const float*>(m_pValue) : nullptr;
}


// Double4x4
inline const double *Parameter::GetValueMatrixdv() const
{
	return (m_nType == Parameters::Double4x4) ? static_cast<const double*>(m_pValue) : nullptr;
}


// Texture
inline TextureHandler *Parameter::GetValueTextureHandler() const
{
	return (m_nType == Parameters::TextureBuffer) ? static_cast<TextureHandler*>(m_pValue) : nullptr;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
