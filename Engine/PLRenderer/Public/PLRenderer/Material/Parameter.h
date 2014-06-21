/*********************************************************\
 *  File: Parameter.h                                    *
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


#ifndef __PLRENDERER_MATERIAL_PARAMETER_H__
#define __PLRENDERER_MATERIAL_PARAMETER_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLRenderer/Renderer/Parameters.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Texture;
class Program;
class TextureHandler;
class ParameterManager;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Parameters are used for material/effect parameters
*
*  @note
*    - You can only use the functions of the given parameter type!
*/
class Parameter {


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class ParameterManager;


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the parameter manager this parameter is in
		*
		*  @return
		*    The parameter manager this parameter is in
		*/
		inline ParameterManager &GetManager() const;

		/**
		*  @brief
		*    Sets the value of this parameter to a parameter within the given manager
		*
		*  @param[in] cManager
		*    Parameter manager to use
		*  @param[in] sName
		*    Name of the parameter within the given manager
		* 
		*  @return
		*    'true' if all went fine, else 'false' (maybe incompatible parameter types?)
		*/
		PLRENDERER_API bool SetManagerParameterValue(Parameters &cManager, const PLCore::String &sName) const;

		/**
		*  @brief
		*    Sets the value of this parameter to a parameter within the given GPU program
		*
		*  @param[in] cProgram
		*    GPU program to use
		*  @param[in] sName
		*    Name of the parameter within the given manager
		* 
		*  @return
		*    'true' if all went fine, else 'false' (maybe incompatible parameter types?)
		*/
		PLRENDERER_API bool SetManagerParameterValue(Program &cProgram, const PLCore::String &sName) const;

		/**
		*  @brief
		*    Get the parameter type
		*
		*  @return
		*    Parameter type, NEVER Parameters::UnknownDataType!
		*/
		inline Parameters::EDataType GetType() const;

		/**
		*  @brief
		*    Get the parameter name
		*
		*  @return
		*    Parameter name
		*/
		inline PLCore::String GetName() const;

		/**
		*  @brief
		*    Set the parameter name
		*
		*  @param[in] sName
		*    New parameter name
		*
		*  @return
		*    'true' if all went fine and the parameter name was changed, else 'false'
		*    (maybe the given name is empty or there's already a parameter with this name
		*    within the parameter manager this parameter is in)
		*/
		PLRENDERER_API bool SetName(const PLCore::String &sName);

		//[-------------------------------------------------------]
		//[ Get/set value                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns a parameter value as string (e.g. "1.0 5.4 0.21")
		*
		*  @return
		*    The value of the parameter as string, empty string on error
		*
		*  @note
		*    - Not that performant, try to avoid using this multiple times within each frame
		*/
		PLRENDERER_API PLCore::String GetParameterString() const;

		/**
		*  @brief
		*    Sets a parameter value from a string (e.g. "1.0 5.4 0.21")
		*
		*  param[in] sValue
		*    New parameter value
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe unknown parameter)
		*
		*  @see
		*    - GetParameterString()
		*/
		PLRENDERER_API bool SetParameterString(const PLCore::String &sValue);

		// Integer
		inline bool GetValue1i(int &nX) const;
		inline int GetValue1i() const;
		PLRENDERER_API bool SetValue1i(int nX);

		// Integer2
		inline bool GetValue2i(int &nX, int &nY) const;
		inline const int *GetValue2iv() const;
		PLRENDERER_API bool SetValue2i(int nX, int nY);
		PLRENDERER_API bool SetValue2iv(const int nValue[]);

		// Integer3
		inline bool GetValue3i(int &nX, int &nY, int &nZ) const;
		inline const int *GetValue3iv() const;
		PLRENDERER_API bool SetValue3i(int nX, int nY, int nZ);
		PLRENDERER_API bool SetValue3iv(const int nValue[]);

		// Integer4
		inline bool GetValue4i(int &nX, int &nY, int &nZ, int &nW) const;
		inline const int *GetValue4iv() const;
		PLRENDERER_API bool SetValue4i(int nX, int nY, int nZ, int nW);
		PLRENDERER_API bool SetValue4iv(const int nValue[]);

		// Float
		inline bool GetValue1f(float &fX) const;
		inline float GetValue1f() const;
		PLRENDERER_API bool SetValue1f(float fX);

		// Float2
		inline bool GetValue2f(float &fX, float &fY) const;
		inline const float *GetValue2fv() const;
		PLRENDERER_API bool SetValue2f(float fX, float fY);
		PLRENDERER_API bool SetValue2fv(const float fValue[]);

		// Float3
		inline bool GetValue3f(float &fX, float &fY, float &fZ) const;
		inline const float *GetValue3fv() const;
		PLRENDERER_API bool SetValue3f(float fX, float fY, float fZ);
		PLRENDERER_API bool SetValue3fv(const float fValue[]);

		// Float4
		inline bool GetValue4f(float &fX, float &fY, float &fZ, float &fW) const;
		inline const float *GetValue4fv() const;
		PLRENDERER_API bool SetValue4f(float fX, float fY, float fZ, float fW);
		PLRENDERER_API bool SetValue4fv(const float fValue[]);

		// Double
		inline bool GetValue1d(double &fW) const;
		PLRENDERER_API bool SetValue1d(double fW);

		// Double2
		inline bool GetValue2d(double &fX, double &fY) const;
		inline const double *GetValue2dv() const;
		PLRENDERER_API bool SetValue2d(double fX, double fY);
		PLRENDERER_API bool SetValue2dv(const double fValue[]);

		// Double3
		inline bool GetValue3d(double &fX, double &fY, double &fZ) const;
		inline const double *GetValue3dv() const;
		PLRENDERER_API bool SetValue3d(double fX, double fY, double fZ);
		PLRENDERER_API bool SetValue3dv(const double fValue[]);

		// Double4
		inline bool GetValue4d(double &fX, double &fY, double &fZ, double &fW) const;
		inline const double *GetValue4dv() const;
		PLRENDERER_API bool SetValue4d(double fX, double fY, double fZ, double fW);
		PLRENDERER_API bool SetValue4dv(const double fValue[]);

		// Float3x3
		inline const float *GetValueFloat3x3() const;
		PLRENDERER_API bool SetValueFloat3x3(const float fValue[]);

		// Float3x4
		inline const float *GetValueFloat3x4() const;
		PLRENDERER_API bool SetValueFloat3x4(const float fValue[]);

		// Float4x4
		inline const float *GetValueMatrixfv() const;
		PLRENDERER_API bool SetValueMatrixfv(const float fValue[]);

		// Double4x4
		inline const double *GetValueMatrixdv() const;
		PLRENDERER_API bool SetValueMatrixdv(const double fValue[]);

		// Texture
		PLRENDERER_API Texture *GetValueTexture() const;
		PLRENDERER_API TextureBuffer *GetValueTextureBuffer() const;
		inline TextureHandler *GetValueTextureHandler() const;
		PLRENDERER_API bool SetValueTexture(Texture *pTexture);
		PLRENDERER_API bool SetValueTexture(const PLCore::String &sFilename);


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] cManager
		*    The parameter manager this parameter is in
		*  @param[in] nType
		*    Type of the parameter, NEVER Parameters::UnknownDataType!
		*  @param[in] sName
		*    Name of the parameter
		*/
		Parameter(ParameterManager &cManager, Parameters::EDataType nType, const PLCore::String &sName);

		/**
		*  @brief
		*    Destructor
		*/
		~Parameter();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ParameterManager	  *m_pManager;	/**< The parameter manager this parameter is in (NEVER a null pointer!) */
		Parameters::EDataType  m_nType;		/**< Parameter type */
		PLCore::String		   m_sName;		/**< Parameter name */
		void				  *m_pValue;	/**< Parameter value, can be a null pointer */


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "PLRenderer/Material/Parameter.inl"


#endif // __PLRENDERER_MATERIAL_PARAMETER_H__
