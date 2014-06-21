/*********************************************************\
 *  File: Parameters.h                                   *
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


#ifndef __PLRENDERER_PARAMETERS_H__
#define __PLRENDERER_PARAMETERS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/String/String.h>
#include <PLCore/Container/Array.h>
#include <PLCore/Container/HashMap.h>
#include "PLRenderer/PLRenderer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLRenderer {


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class Texture;
class TextureBuffer;


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Abstract parameters interface
*
*  @note
*    - This interface offers no 'create parameter' functions because on some implementations it is not possible
*      to create new parameters 'from outside'
*/
class Parameters {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Parameter data types
		*/
		enum EDataType {
			String			= 0,	/**< String */
			Integer			= 1,	/**< Integer 1 */
			Integer2		= 2,	/**< Integer 2 */
			Integer3		= 3,	/**< Integer 3 */
			Integer4		= 4,	/**< Integer 4 */
			Float			= 5,	/**< Float 1 */
			Float2			= 6,	/**< Float 2 */
			Float3			= 7,	/**< Float 3 */
			Float4			= 8,	/**< Float 4 */
			Double			= 9,	/**< Double 1 */
			Double2			= 10,	/**< Double 2 */
			Double3			= 11,	/**< Double 3 */
			Double4			= 12,	/**< Double 4 */
			Float3x3		= 13,	/**< Float3x3 matrix */
			Float3x4		= 14,	/**< Float3x4 matrix */
			Float4x4		= 15,	/**< Float4x4 matrix */
			Double4x4		= 16,	/**< Double4x4 matrix */
			TextureBuffer	= 17,	/**< Texture buffer */
			UnknownDataType = 18	/**< Unknown data type */
		};


	//[-------------------------------------------------------]
	//[ Public static functions                               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns a data type as string
		*
		*  @param[in] nDataType
		*    Data type to return as string
		*
		*  @return
		*    The given data type as string
		*/
		static PLRENDERER_API PLCore::String DataTypeToString(EDataType nDataType);

		/**
		*  @brief
		*    Returns a data type as number
		*
		*  @param[in] sDataType
		*    Data type to return as number
		*
		*  @return
		*    The given data type as number
		*/
		static PLRENDERER_API EDataType DataTypeFromString(const PLCore::String &sDataType);


	//[-------------------------------------------------------]
	//[ Public virtual Parameters functions                   ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns whether there's a valid parameter with the given name or not
		*
		*  @param[in] sName
		*    Name of the parameter to check
		*
		*  @return
		*    'true' if there's a valid parameter with the given name, else 'false'
		*/
		virtual bool IsParameter(const PLCore::String &sName) const = 0;

		//[-------------------------------------------------------]
		//[ Get/set parameters                                    ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns a parameter value as string (e.g. "1.0 5.4 0.21")
		*
		*  param[in] sName
		*    Name of the parameter to return the value from
		*
		*  @return
		*    The value of the parameter as string, empty string on error
		*/
		virtual PLCore::String GetParameterString(const PLCore::String &sName) const = 0;

		/**
		*  @brief
		*    Sets a parameter value from a string (e.g. "1.0 5.4 0.21")
		*
		*  param[in] sName
		*    Name of the parameter to set
		*  param[in] sValue
		*    New parameter value
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe unknown parameter)
		*/
		virtual bool SetParameterString(const PLCore::String &sName, const PLCore::String &sValue) = 0;

		// Integer
		virtual bool GetParameter1i(const PLCore::String &sName, int &nX) const = 0;
		virtual bool SetParameter1i(const PLCore::String &sName, int nX) = 0;
		// Integer2
		virtual bool GetParameter2i(const PLCore::String &sName, int &nX, int &nY) const = 0;
		virtual bool GetParameter2iv(const PLCore::String &sName, int nValue[]) const = 0;
		virtual bool SetParameter2i(const PLCore::String &sName, int nX, int nY) = 0;
		virtual bool SetParameter2iv(const PLCore::String &sName, const int nValue[]) = 0;
		// Integer3
		virtual bool GetParameter3i(const PLCore::String &sName, int &nX, int &nY, int &nZ) const = 0;
		virtual bool GetParameter3iv(const PLCore::String &sName, int nValue[]) const = 0;
		virtual bool SetParameter3i(const PLCore::String &sName, int nX, int nY, int nZ) = 0;
		virtual bool SetParameter3iv(const PLCore::String &sName, const int nValue[]) = 0;
		// Integer4
		virtual bool GetParameter4i(const PLCore::String &sName, int &nX, int &nY, int &nZ, int &nW) const = 0;
		virtual bool GetParameter4iv(const PLCore::String &sName, int nValue[]) const = 0;
		virtual bool SetParameter4i(const PLCore::String &sName, int nX, int nY, int nZ, int nW) = 0;
		virtual bool SetParameter4iv(const PLCore::String &sName, const int nValue[]) = 0;
		// Float
		virtual bool GetParameter1f(const PLCore::String &sName, float &fX) const = 0;
		virtual bool SetParameter1f(const PLCore::String &sName, float fX) = 0;
		// Float2
		virtual bool GetParameter2f(const PLCore::String &sName, float &fX, float &fY) const = 0;
		virtual bool GetParameter2fv(const PLCore::String &sName, float fValue[]) const = 0;
		virtual bool SetParameter2f(const PLCore::String &sName, float fX, float fY) = 0;
		virtual bool SetParameter2fv(const PLCore::String &sName, const float fValue[]) = 0;
		// Float3
		virtual bool GetParameter3f(const PLCore::String &sName, float &fX, float &fY, float &fZ) const = 0;
		virtual bool GetParameter3fv(const PLCore::String &sName, float fValue[]) const = 0;
		virtual bool SetParameter3f(const PLCore::String &sName, float fX, float fY, float fZ) = 0;
		virtual bool SetParameter3fv(const PLCore::String &sName, const float fValue[]) = 0;
		// Float4
		virtual bool GetParameter4f(const PLCore::String &sName, float &fX, float &fY, float &fZ, float &fW) const = 0;
		virtual bool GetParameter4fv(const PLCore::String &sName, float fValue[]) const = 0;
		virtual bool SetParameter4f(const PLCore::String &sName, float fX, float fY, float fZ, float fW) = 0;
		virtual bool SetParameter4fv(const PLCore::String &sName, const float fValue[]) = 0;
		// Double
		virtual bool GetParameter1d(const PLCore::String &sName, double &fX) const = 0;
		virtual bool SetParameter1d(const PLCore::String &sName, double fX) = 0;
		// Double2
		virtual bool GetParameter2d(const PLCore::String &sName, double &fX, double &fY) const = 0;
		virtual bool GetParameter2dv(const PLCore::String &sName, double fValue[]) const = 0;
		virtual bool SetParameter2d(const PLCore::String &sName, double fX, double fY) = 0;
		virtual bool SetParameter2dv(const PLCore::String &sName, const double fValue[]) = 0;
		// Double3
		virtual bool GetParameter3d(const PLCore::String &sName, double &fX, double &fY, double &fZ) const = 0;
		virtual bool GetParameter3dv(const PLCore::String &sName, double fValue[]) const = 0;
		virtual bool SetParameter3d(const PLCore::String &sName, double fX, double fY, double fZ) = 0;
		virtual bool SetParameter3dv(const PLCore::String &sName, const double fValue[]) = 0;
		// Double4
		virtual bool GetParameter4d(const PLCore::String &sName, double &fX, double &fY, double &fZ, double &fW) const = 0;
		virtual bool GetParameter4dv(const PLCore::String &sName, double fValue[]) const = 0;
		virtual bool SetParameter4d(const PLCore::String &sName, double fX, double fY, double fZ, double fW) = 0;
		virtual bool SetParameter4dv(const PLCore::String &sName, const double fValue[]) = 0;
		// Float3x3
		virtual bool GetParameterFloat3x3(const PLCore::String &sName, float fValue[]) const = 0;
		virtual bool SetParameterFloat3x3(const PLCore::String &sName, const float fValue[]) = 0;
		// Float3x4
		virtual bool GetParameterFloat3x4(const PLCore::String &sName, float fValue[]) const = 0;
		virtual bool SetParameterFloat3x4(const PLCore::String &sName, const float fValue[]) = 0;
		// Float4x4
		virtual bool GetParameterMatrixfv(const PLCore::String &sName, float fValue[]) const = 0;
		virtual bool SetParameterMatrixfv(const PLCore::String &sName, const float fValue[]) = 0;
		// Double4x4
		virtual bool GetParameterMatrixdv(const PLCore::String &sName, double fValue[]) const = 0;
		virtual bool SetParameterMatrixdv(const PLCore::String &sName, const double fValue[]) = 0;
		// Texture buffer
		/**
		*  @brief
		*    Returns the texture buffer of the parameter
		*
		*  @return
		*    The texture buffer of the parameter, can be a null pointer
		*/
		virtual PLRenderer::TextureBuffer *GetParameterTextureBuffer(const PLCore::String &sName) const = 0;

		/**
		*  @brief
		*    Sets the texture buffer of the parameter
		*
		*  @param[in] sName
		*    Name of the parameter where to set the texture buffer
		*  @param[in] pTextureBuffer
		*    Texture buffer to set, can be a null pointer
		*
		*  @return
		*    Texture stage the texture buffer was set to, < 0 on error
		*/
		virtual int SetParameterTextureBuffer(const PLCore::String &sName, PLRenderer::TextureBuffer *pTextureBuffer) = 0;

		/**
		*  @brief
		*    Returns the texture of the parameter
		*
		*  @return
		*    The texture of the parameter, can be a null pointer
		*/
		virtual PLRenderer::Texture *GetParameterTexture(const PLCore::String &sName) const = 0;

		/**
		*  @brief
		*    Sets the texture of the parameter
		*
		*  @param[in] sName
		*    Name of the parameter where to set the texture
		*  @param[in] sFilename
		*    Texture filename
		*
		*  @return
		*    Texture stage the texture was set to, < 0 on error
		*/
		virtual int SetParameterTexture(const PLCore::String &sName, const PLCore::String &sFilename) = 0;

		/**
		*  @brief
		*    Sets the texture of the parameter
		*
		*  @param[in] sName
		*    Name of the parameter where to set the texture
		*  @param[in] pTexture
		*    Texture to set, can be a null pointer
		*
		*  @return
		*    Texture stage the texture was set to, < 0 on error
		*/
		virtual int SetParameterTexture(const PLCore::String &sName, PLRenderer::Texture *pTexture) = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		PLRENDERER_API Parameters();

		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~Parameters();


	//[-------------------------------------------------------]
	//[ Private functions                                     ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] cSource
		*    Source to copy from
		*/
		Parameters(const Parameters &cSource);

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] cSource
		*    Source to copy from
		*
		*  @return
		*    Reference to this instance
		*/
		Parameters &operator =(const Parameters &cSource);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


#endif // __PLRENDERER_PARAMETERS_H__
