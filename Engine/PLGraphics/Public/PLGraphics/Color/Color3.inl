/*********************************************************\
 *  File: Color3.inl                                     *
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
namespace PLGraphics {


//[-------------------------------------------------------]
//[ Special operators                                     ]
//[-------------------------------------------------------]
inline Color3 operator *(float fS, const Color3 &cC)
{
	return Color3(cC.r*fS, cC.g*fS, cC.b*fS);
}


//[-------------------------------------------------------]
//[ Public static functions                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    Maps a RGB floating point channels (0.0f to 1.0f range) to PLCore::uint32
*/
inline PLCore::uint32 Color3::ToUInt32(float fR, float fG, float fB)
{
	return static_cast<PLCore::uint32>(static_cast<PLCore::uint8>(fR*255.0f) |
		   static_cast<PLCore::uint16>(static_cast<PLCore::uint8>(fG*255.0f))<<8 |
		   static_cast<PLCore::uint32>(static_cast<PLCore::uint8>(fB*255.0f))<<16);
}

/**
*  @brief
*    Returns the red color component (float, 0.0-1.0) from a PLCore::uint32 RGB color
*/
inline float Color3::RedFromUInt32(PLCore::uint32 nColor)
{
	return (static_cast<PLCore::uint8>(nColor))/255.0f;
}

/**
*  @brief
*    Returns the green color component (float, 0.0-1.0) from a PLCore::uint32 RGB color
*/
inline float Color3::GreenFromUInt32(PLCore::uint32 nColor)
{
	return (static_cast<PLCore::uint8>((static_cast<PLCore::uint16>(nColor)) >> 8))/255.0f;
}

/**
*  @brief
*    Returns the blue color component (float, 0.0-1.0) from a PLCore::uint32 RGB color
*/
inline float Color3::BlueFromUInt32(PLCore::uint32 nColor)
{
	return (static_cast<PLCore::uint8>((nColor)>>16))/255.0f;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Constructor (all components are set to 0.0)
*/
inline Color3::Color3() :
	r(0.0f),
	g(0.0f),
	b(0.0f)
{
}

/**
*  @brief
*    Constructor
*/
inline Color3::Color3(float fR, float fG, float fB) :
	r(fR),
	g(fG),
	b(fB)
{
}

/**
*  @brief
*    Constructor
*/
inline Color3::Color3(PLCore::uint8 nR, PLCore::uint8 nG, PLCore::uint8 nB) :
	r(static_cast<float>(nR)/255.0f),
	g(static_cast<float>(nG)/255.0f),
	b(static_cast<float>(nB)/255.0f)
{
}

/**
*  @brief
*    Constructor
*/
inline Color3::Color3(const float fColor[]) :
	r(fColor[0]),
	g(fColor[1]),
	b(fColor[2])
{
}

/**
*  @brief
*    Constructor
*/
inline Color3::Color3(const PLCore::uint8 nColor[]) :
	r(nColor[0]/255.0f),
	g(nColor[1]/255.0f),
	b(nColor[2]/255.0f)
{
}

/**
*  @brief
*    Constructor
*/
inline Color3::Color3(float fColor) :
	r(fColor),
	g(fColor),
	b(fColor)
{
}

/**
*  @brief
*    Constructor
*/
inline Color3::Color3(PLCore::uint8 nColor) :
	r(static_cast<float>(nColor)/255.0f),
	g(static_cast<float>(nColor)/255.0f),
	b(static_cast<float>(nColor)/255.0f)
{
}

/**
*  @brief
*    Copy constructor
*/
inline Color3::Color3(const Color3 &cSource) :
	r(cSource.r),
	g(cSource.g),
	b(cSource.b)
{
}

/**
*  @brief
*    Constructor
*/
inline Color3::Color3(const PLCore::String &sString)
{
	// The following method sets all components, so we don't need the initializer list in here
	FromString(sString);
}

/**
*  @brief
*    Destructor
*/
inline Color3::~Color3()
{
}

/**
*  @brief
*    Maps this RGB floating point channels (0.0f to 1.0f range) to PLCore::uint32
*/
inline PLCore::uint32 Color3::ToUInt32() const
{
	return static_cast<PLCore::uint32>(static_cast<PLCore::uint8>(r*255.0f) |
		   static_cast<PLCore::uint16>(static_cast<PLCore::uint8>(g*255.0f))<<8 |
		   static_cast<PLCore::uint32>(static_cast<PLCore::uint8>(b*255.0f))<<16);
}

/**
*  @brief
*    Maps from a PLCore::uint32 to RGB floating point channels (0.0f to 1.0f range)
*/
inline void Color3::FromUInt32(PLCore::uint32 nColor)
{
	r = (static_cast<PLCore::uint8>(nColor))/255.0f;
	g = (static_cast<PLCore::uint8>((static_cast<PLCore::uint16>(nColor)) >> 8))/255.0f;
	b = (static_cast<PLCore::uint8>((nColor)>>16))/255.0f;
}

/**
*  @brief
*    Get the color component as float values
*/
inline void Color3::GetRGB(float &fR, float &fG, float &fB) const
{
	fR = r;
	fG = g;
	fB = b;
}

/**
*  @brief
*    Get the color component as integer values
*/
inline void Color3::GetRGB(PLCore::uint8 &nR, PLCore::uint8 &nG, PLCore::uint8 &nB) const
{
	nR = static_cast<PLCore::uint8>(r*255);
	nG = static_cast<PLCore::uint8>(g*255);
	nB = static_cast<PLCore::uint8>(b*255);
}

/**
*  @brief
*    Set the color components by using float values
*/
inline void Color3::SetRGB(float fR, float fG, float fB)
{
	r = fR;
	g = fG;
	b = fB;
}

/**
*  @brief
*    Set the color components by using integer values
*/
inline void Color3::SetRGB(PLCore::uint8 nR, PLCore::uint8 nG, PLCore::uint8 nB)
{
	r = static_cast<float>(nR)/255.0f;
	g = static_cast<float>(nG)/255.0f;
	b = static_cast<float>(nB)/255.0f;
}

/**
*  @brief
*    Get the color component as array of float values
*/
inline void Color3::GetRGB(float fColor[]) const
{
	fColor[0] = r;
	fColor[1] = g;
	fColor[2] = b;
}

/**
*  @brief
*    Get the color component as array of integer values
*/
inline void Color3::GetRGB(PLCore::uint8 nColor[]) const
{
	nColor[0] = static_cast<PLCore::uint8>(r*255);
	nColor[1] = static_cast<PLCore::uint8>(g*255);
	nColor[2] = static_cast<PLCore::uint8>(b*255);
}

/**
*  @brief
*    Set the color components by using a reference to float values
*/
inline void Color3::SetRGB(const float fColor[])
{
	r = fColor[0];
	g = fColor[1];
	b = fColor[2];
}

/**
*  @brief
*    Set the color components by using a reference to integer values
*/
inline void Color3::SetRGB(const PLCore::uint8 nColor[])
{
	r = static_cast<float>(nColor[0])/255.0f;
	g = static_cast<float>(nColor[1])/255.0f;
	b = static_cast<float>(nColor[2])/255.0f;
}

/**
*  @brief
*    Set all color component to the same float value
*/
inline void Color3::SetRGB(float fValue)
{
	r = fValue;
	g = fValue;
	b = fValue;
}

/**
*  @brief
*    Set all color component to the same integer value
*/
inline void Color3::SetRGB(PLCore::uint8 nColor)
{
	r = static_cast<float>(nColor)/255.0f;
	g = static_cast<float>(nColor)/255.0f;
	b = static_cast<float>(nColor)/255.0f;
}

/**
*  @brief
*    Get the red color value as float
*/
inline float Color3::GetR() const
{
	return r;
}

/**
*  @brief
*    Get the red color value as integer
*/
inline PLCore::uint8 Color3::GetRInt() const
{
	return static_cast<PLCore::uint8>(r*255);
}

/**
*  @brief
*    Set the red color component using a float value
*/
inline void Color3::SetR(float fR)
{
	r = fR;
}

/**
*  @brief
*    Set the red color component using an integer value
*/
inline void Color3::SetR(PLCore::uint8 nR)
{
	r = static_cast<float>(nR)/255.0f;
}

/**
*  @brief
*    Get the green color value as float
*/
inline float Color3::GetG() const
{
	return g;
}

/**
*  @brief
*    Get the green color value as integer
*/
inline PLCore::uint8 Color3::GetGInt() const
{
	return static_cast<PLCore::uint8>(g*255);
}

/**
*  @brief
*    Set the green color component using a float value
*/
inline void Color3::SetG(float fG)
{
	g = fG;
}

/**
*  @brief
*    Set the green color component using an integer value
*/
inline void Color3::SetG(PLCore::uint8 nG)
{
	g = static_cast<float>(nG)/255.0f;
}

/**
*  @brief
*    Get the blue color value as float
*/
inline float Color3::GetB() const
{
	return b;
}

/**
*  @brief
*    Get the blue color value as integer
*/
inline PLCore::uint8 Color3::GetBInt() const
{
	return static_cast<PLCore::uint8>(b*255);
}

/**
*  @brief
*    Set the blue color component using a float value
*/
inline void Color3::SetB(float fB)
{
	b = fB;
}

/**
*  @brief
*    Set the blue color component using an integer value
*/
inline void Color3::SetB(PLCore::uint8 nB)
{
	b = static_cast<float>(nB)/255.0f;
}

/**
*  @brief
*    Returns if the color is valid
*/
inline bool Color3::IsValid() const
{
	return (r >= 0.0f && r <= 1.0f && g >= 0.0f && g <= 1.0f && b >= 0.0f && b <= 1.0f);
}

/**
*  @brief
*    Clamps the color values between 0.0 and 1.0
*/
inline void Color3::Saturate()
{
	if (r < 0.0f)
		r = 0.0f;
	if (r > 1.0f)
		r = 1.0f;
	if (g < 0.0f)
		g = 0.0f;
	if (g > 1.0f)
		g = 1.0f;
	if (b < 0.0f)
		b = 0.0f;
	if (b > 1.0f)
		b = 1.0f;
}

/**
*  @brief
*    Get luminance of color as float
*/
inline float Color3::GetLuminance() const
{
	// "Human formula"... our eyes recognize color components not uniformly
	return r*0.299f + g*0.587f + b*0.114f;
}

/**
*  @brief
*    Get luminance of color as integer
*/
inline PLCore::uint8 Color3::GetLuminanceInt() const
{
	// "Human formula"... our eyes recognize color components not uniformly
	return static_cast<PLCore::uint8>(r*0.299f + g*0.587f + b*0.114f);
}

/**
*  @brief
*    Get luminance as color (results in a grayscale color)
*/
inline Color3 Color3::GetLuminanceColor() const
{
	// "Human formula"... our eyes recognize color components not uniformly
	const float fLuminance = r*0.299f + g*0.587f + b*0.114f;

	// Return grayscale color
	return Color3(fLuminance, fLuminance, fLuminance);
}


//[-------------------------------------------------------]
//[ Assignment operators                                  ]
//[-------------------------------------------------------]
inline Color3 &Color3::operator =(const Color3 &cC)
{
	r = cC.r;
	g = cC.g;
	b = cC.b;

	return *this;
}

inline Color3 &Color3::operator =(const float fC[])
{
	r = fC[0];
	g = fC[1];
	b = fC[2];

	return *this;
}

inline Color3 &Color3::operator =(float fD)
{
	r = g = b = fD;

	return *this;
}

inline Color3::operator float *()
{
	return &r;
}

inline Color3::operator const float *() const
{
	return &r;
}


//[-------------------------------------------------------]
//[ Comparison                                            ]
//[-------------------------------------------------------]
inline bool Color3::operator ==(const Color3 &cC) const
{
	return (r == cC.r && g == cC.g && b == cC.b);
}

inline bool Color3::operator !=(const Color3 &cC) const
{
	return (r != cC.r || g != cC.g || b != cC.b);
}

inline bool Color3::operator ==(float f) const
{
	return (r == f && g == f && b == f);
}

inline bool Color3::operator !=(float f) const
{
	return (r != f || g != f || b != f);
}


//[-------------------------------------------------------]
//[ Misc                                                  ]
//[-------------------------------------------------------]
inline Color3 Color3::operator +(const Color3 &cC) const
{
	return Color3(r+cC.r, g+cC.g, b+cC.b);
}

inline Color3 Color3::operator +(float fN) const
{
	return Color3(r+fN, g+fN, b+fN);
}

inline Color3 &Color3::operator +=(const Color3 &cC)
{
	r += cC.r;
	g += cC.g;
	b += cC.b;

	return *this;
}

inline Color3 &Color3::operator +=(float fN)
{
	r += fN;
	g += fN;
	b += fN;

	return *this;
}

inline Color3 Color3::operator -() const
{
	return Color3(-r, -g, -b);
}

inline Color3 Color3::operator -(const Color3 &cC) const
{
	return Color3(r-cC.r, g-cC.g, b-cC.b);
}

inline Color3 Color3::operator -(float fN) const
{
	return Color3(r-fN, g-fN, b-fN);
}

inline Color3 &Color3::operator -=(const Color3 &cC)
{
	r -= cC.r;
	g -= cC.g;
	b -= cC.b;

	return *this;
}

inline Color3 &Color3::operator -=(float fN)
{
	r -= fN;
	g -= fN;
	b -= fN;

	return *this;
}

inline Color3 Color3::operator *(const Color3 &cC) const 
{
	return Color3(r*cC.r, g*cC.g, b*cC.b);
}

inline Color3 Color3::operator *(float fS) const
{
	return Color3(r*fS, g*fS, b*fS);
}

inline Color3 &Color3::operator *=(const Color3 &cC)
{
	r *= cC.r;
	g *= cC.g;
	b *= cC.b;

	return *this;
}

inline Color3 &Color3::operator *=(float fS)
{
	r *= fS;
	g *= fS;
	b *= fS;

	return *this;
}

inline Color3 Color3::operator /(const Color3 &cC) const
{
	return Color3(r/cC.r, g/cC.g, b/cC.b);
}

inline Color3 Color3::operator /(float fS) const
{
	return Color3(r/fS, g/fS, b/fS);
}

inline Color3 &Color3::operator /=(const Color3 &cC)
{
	r /= cC.r;
	g /= cC.g;
	b /= cC.b;

	return *this;
}

inline Color3 &Color3::operator /=(float fS)
{
	r /= fS;
	g /= fS;
	b /= fS;

	return *this;
}

inline float &Color3::operator [](int nIndex)
{
	return fColor[nIndex];
}

/**
*  @brief
*    To string
*/
inline PLCore::String Color3::ToString() const
{
	return PLCore::String::Format("%g %g %g", r, g, b);
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLGraphics
