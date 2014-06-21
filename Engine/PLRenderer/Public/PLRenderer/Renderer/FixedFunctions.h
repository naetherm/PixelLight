/*********************************************************\
 *  File: FixedFunctions.h                               *
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


#ifndef __PLRENDERER_FIXEDFUNCTIONS_H__
#define __PLRENDERER_FIXEDFUNCTIONS_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/Base/Rtti.h>
#include <PLMath/Vector3.h>
#include <PLGraphics/Color/Color4.h>
#include "PLRenderer/PLRenderer.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLMath {
	class Matrix3x4;
	class Matrix4x4;
}
namespace PLRenderer {
	class VertexBuffer;
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
*    Abstract fixed functions renderer interface
*
*  @note
*    - A legacy renderer interface for previously build in features in old graphics APIs and GPUs
*/
class FixedFunctions {


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Holds all hardware capabilities
		*/
		struct Capabilities {
			PLCore::uint8 nMaxActiveLights;			/**< Maximum number of active lights */
			PLCore::uint8 nMaxClipPlanes;			/**< Maximum number of clip planes */
			bool          bVertexBufferFogCoord;	/**< Vertex buffer fog coordinates supported? (for VertexBuffer::FogCoord) */
			PLCore::uint8 nMaxVertexBufferStreams;	/**< Maximum number of vertex buffer streams */
		};

		/**
		*  @brief
		*    Fixed functions render states
		*/
		class RenderState {
			public:
			enum Enum {
				// Fog
				FogEnable           = 0,	/**< Enable/disable fog (false/true, default: false) */
				FogColor            = 1,	/**< RGBA fog color (PLCore::uint32, default: 0/0/0/0) */
				FogDensity          = 2,	/**< Fog density (float, default: 1.0) */
				FogStart            = 3,	/**< Fog start (float, default: 0.0) */
				FogEnd              = 4,	/**< Fog start (float, default: 1.0) */
				FogMode             = 5,	/**< Fog mode (Fog, default: Fog::Exp) */
				// Alpha test
				AlphaTestEnable     = 6,	/**< Enable/disable alpha test (false/true, default: false) */
				AlphaTestFunction   = 7,	/**< Alpha test comparison function (see 'Compare', default: Compare::GreaterEqual) */
				AlphaTestReference  = 8,	/**< Alpha test reference value (0-1, default: 0.5) */
				// Misc
				Lighting            = 9,	/**< Enable/disable lighting (false/true, default: false) */
				Ambient             = 10,	/**< General RGBA ambient color (PLCore::uint32, default: 0.0/0.0/0.0/0.0) */
				NormalizeNormals    = 11,	/**< Enable/disable normalize normals (false/true, default: true) */
				ShadeMode           = 12,	/**< Shade mode (see Shade, default: Shade::Smooth) */
				// End
				Number              = 13,	/**< Number of render states */
				Unknown             = 14	/**< Unknown render state */
			};
		};

		/**
		*  @brief
		*    Transform states
		*
		*  @note
		*    - The default value for each matrix is the identity matrix
		*/
		class Transform {
			public:
			enum Enum {
				// View and projection
				Projection =  0,	/**< Projection transformation matrix */
				View       =  1,	/**< View transformation matrix */
				World      =  2,	/**< World transformation matrix */
				// Texture
				Texture0   =  3,	/**< Texture matrix for stage 0 */
				Texture1   =  4,	/**< Texture matrix for stage 1 */
				Texture2   =  5,	/**< Texture matrix for stage 2 */
				Texture3   =  6,	/**< Texture matrix for stage 3 */
				Texture4   =  7,	/**< Texture matrix for stage 4 */
				Texture5   =  8,	/**< Texture matrix for stage 5 */
				Texture6   =  9,	/**< Texture matrix for stage 6 */
				Texture7   = 10,	/**< Texture matrix for stage 7 */
				// End
				Number     = 11,	/**< Number of transform states */
				Unknown    = 12		/**< Unknown transform state */
			};
		};

		/**
		*  @brief
		*    Texture environment modes
		*/
		class TextureEnvironment {
			public:
			enum Enum {
				Add                 = 0,	/**< Add */
				Replace             = 1,	/**< Replace */
				Modulate            = 2,	/**< Modulate */
				PassThru            = 3,	/**< Pass thru */
				Dot3                = 4,	/**< Dot 3*/
				Interpolate         = 5,	/**< Interpolate */
				InterpolatePrimary  = 6,	/**< Interpolate primary */
				InterpolateTexAlpha = 7,	/**< Interpolate texture alpha */
				// End
				Number              = 8,	/**< Number of texture environment modes */
				Unknown             = 9		/**< Unknown texture environment mode */
			};
			pl_enum(Enum)
				pl_enum_value(Add,					"Add")
				pl_enum_value(Replace,				"Replace")
				pl_enum_value(Modulate,				"Modulate")
				pl_enum_value(PassThru,				"Pass thru")
				pl_enum_value(Dot3,					"Dot 3")
				pl_enum_value(Interpolate,			"Interpolate")
				pl_enum_value(InterpolatePrimary,	"Interpolate primary")
				pl_enum_value(InterpolateTexAlpha,	"Interpolate texture alpha")
			pl_enum_end
		};

		/**
		*  @brief
		*    Texture coordinate generation modes
		*/
		class TexCoordGen {
			public:
			enum Enum {
				None          = 0,	/**< No texture coordinate generation (passthru) */
				ObjectLinear  = 1,	/**< Object linear */
				EyeLinear     = 2,	/**< Eye linear */
				ReflectionMap = 3,	/**< Reflection map. Internally the texture matrix is multiplied with the inversed view matrix
										 automatically for correct reflections before the matrix is send to the API. If you request
										 the current texture matrix, the original matrix set by SetTransformState() is returned. */
				NormalMap     = 4,	/**< Normal map */
				SphereMap     = 5,	/**< Sphere map */
				// End
				Number        = 6,	/**< Number of texture coordinate generation modes */
				Unknown       = 7	/**< Unknown texture coordinate generation mode */
			};
			pl_enum(Enum)
				pl_enum_value(None,				"No texture coordinate generation (passthru)")
				pl_enum_value(ObjectLinear,		"Object linear")
				pl_enum_value(EyeLinear,		"Eye linear")
				pl_enum_value(ReflectionMap,	"Reflection map")
				pl_enum_value(NormalMap,		"Normal map")
				pl_enum_value(SphereMap,		"Sphere map")
			pl_enum_end
		};

		/**
		*  @brief
		*    Texture stage states
		*/
		class TextureStage {
			public:
			enum Enum {
				ColorTexEnv = 0,	/**< Color texture environment mode (TextureEnvironment, default: TextureEnvironment::Modulate) */
				AlphaTexEnv = 1,	/**< Alpha texture environment mode (TextureEnvironment, default: TextureEnvironment::Modulate) */
				TexGen      = 2,	/**< Texture coordinate generation mode (TexCoordGen, default: TexCoordGen::None) */
				// End
				Number      = 3,	/**< Number of texture stage states */
				Unknown     = 4		/**< Unknown texture stage state */
			};
		};

		/**
		*  @brief
		*    Fog modes
		*/
		class Fog {
			public:
			enum Enum {
				Exp     = 0,	/**< Fog effect intensifies exponentially (f=1/((e^(d*density)))) */
				Exp2    = 1,	/**< Fog effect intensifies exponentially with the square of the distance (f=1/((e^((d*density)^2)))) */
				Linear  = 2,	/**< Fog effect intensifies linearly between the start and end points (f=(end-d)/(end-start)) */
				// End
				Number  = 3,	/**< Number of fog modes */
				Unknown = 4		/**< Unknown fog mode */
			};
			pl_enum(Enum)
				pl_enum_value(Exp,		"Fog effect intensifies exponentially (f=1/((e^(d*density))))")
				pl_enum_value(Exp2,		"Fog effect intensifies exponentially with the square of the distance (f=1/((e^((d*density)^2))))")
				pl_enum_value(Linear,	"Fog effect intensifies linearly between the start and end points (f=(end-d)/(end-start))")
			pl_enum_end
		};

		/**
		*  @brief
		*    Material states
		*/
		class MaterialState {
			public:
			enum Enum {
				Ambient   = 0,	/**< RGBA ambient color (PLCore::uint32, default: 0.2, 0.2, 0.2, 1.0) */
				Diffuse   = 1,	/**< RGBA diffuse color (PLCore::uint32, default: 0.8, 0.8, 0.8, 1.0) */
				Specular  = 2,	/**< RGBA specular color (PLCore::uint32, default: 0.0, 0.0, 0.0, 1.0) */
				Emission  = 3,	/**< RGBA emission color (PLCore::uint32, default: 0.0, 0.0, 0.0, 1.0) */
				Shininess = 4,	/**< Shininess (float, default: 0.0) */
				// End
				Number    = 5,	/**< Number of material states */
				Unknown   = 6	/**< Unknown material state */
			};
		};

		/**
		*  @brief
		*    Light types
		*/
		class LightType {
			public:
			enum Enum {
				Point       = 0,	/**< Point light */
				Spot        = 1,	/**< Spot light */
				Directional = 2,	/**< Directional light */
				// End
				Number      = 3,	/**< Number of light types */
				Unknown     = 4		/**< Unknown light type */
			};
		};

		/**
		*  @brief
		*    Light data
		*
		*  @remarks
		*    attenuation = 1/(fConstantAttenuation      +
		*                     fLinearAttenuation    * d +
		*                     fQuadraticAttenuation * d2)
		*    Where: d  = Distance from vertex position to light position \n
		*           d2 = d squared
		*/
		struct Light {
			LightType::Enum    nType;					/**< Light source type (LightType, default: LightType::Point) */
			PLGraphics::Color4 cAmbient;				/**< RGBA ambient color of light (default: 0.0/0.0/0.0/0.0) */
			PLGraphics::Color4 cDiffuse;				/**< RGBA diffuse color of light (default: 0.0/0.0/0.0/0.0) */
			PLGraphics::Color4 cSpecular;				/**< RGBA specular color of light (default: 0.0/0.0/0.0/0.0) */
			PLMath::Vector3    vPosition;				/**< x/y/z position of light in world space (default: 0.0/0.0/0.0) */
			PLMath::Vector3    vDirection;				/**< x/y/z direction of light in world space (default: 0.0/0.0/0.0) */
			float              fCutOff;					/**< Cutoff range (default: 0.0) */
			float              fConstantAttenuation;	/**< Constant attenuation (default: 1.0) */
			float              fLinearAttenuation;		/**< Linear attenuation (default: 0.0) */
			float              fQuadraticAttenuation;	/**< Quadratic attenuation (default: 0.0) */
		};

		/**
		*  @brief
		*    Shade modes
		*/
		class Shade {
			public:
			enum Enum {
				Flat    = 0,	/**< No interpolated during rasterizing */
				Smooth  = 1,	/**< Interpolated during rasterizing */
				// End
				Number  = 2,	/**< Number of shade modes */
				Unknown = 3		/**< Unknown shade mode */
			};
			pl_enum(Enum)
				pl_enum_value(Flat,		"No interpolated during rasterizing")
				pl_enum_value(Smooth,	"Interpolated during rasterizing")
			pl_enum_end
		};


	//[-------------------------------------------------------]
	//[ Public virtual FixedFunctions functions               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the renderer fixed functions capabilities
		*
		*  @return
		*    The renderer fixed functions capabilities
		*/
		virtual const Capabilities &GetCapabilities() const = 0;

		/**
		*  @brief
		*    Resets all fixed functions render states, texture units etc. to default
		*
		*  @note
		*    - Avoid calling this function frequently because this can be a performance hit
		*    - Internally this functions are called:
		*      ResetRenderStates(), SetColor(), ResetTransformStates(), ResetTextureStageStates(),
		*      ResetMaterialStates(), ResetLights(), SetClipPlaneEnabled(), SetClipPlane(), SetVertexBuffer()
		*/
		virtual void Reset() = 0;

		//[-------------------------------------------------------]
		//[ Render states                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns a default render state
		*
		*  @param[in] nState
		*    Render state to return the default value from
		*
		*  @return
		*    The default state
		*/
		virtual PLCore::uint32 GetDefaultRenderState(RenderState::Enum nState) const = 0;

		/**
		*  @brief
		*    Resets all render states to default
		*
		*  @see
		*    - GetDefaultRenderState()
		*/
		virtual void ResetRenderStates() = 0;

		/**
		*  @brief
		*    Retrieves a render-state value
		*
		*  @param[in] nState
		*    State variable that is being queried. This parameter can
		*    be any member of the render state enumerated type.
		*
		*  @return
		*    The value of the queried render state variable, < 0 on error
		*
		*  @see
		*    - GetDefaultRenderState()
		*/
		virtual int GetRenderState(RenderState::Enum nState) const = 0;

		/**
		*  @brief
		*    Sets a single render-state parameter
		*
		*  @param[in] nState
		*    State variable that is being modified. This parameter
		*    can be any member of the render state enumerated type.
		*  @param[in] nValue
		*    New value for the render state to be set. The meaning of
		*    this parameter is dependent on the value specified for nState.
		*    For example, if nState is FogMode, the second parameter
		*    must be one member of the Shade enumerated type. (e.g. Fog::Exp)
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @see
		*    - GetDefaultRenderState()
		*/
		virtual bool SetRenderState(RenderState::Enum nState, PLCore::uint32 nValue) = 0;

		/**
		*  @brief
		*    Gets the current color
		*
		*  @return
		*    Current RGBA color
		*/
		virtual PLGraphics::Color4 GetColor() const = 0;

		/**
		*  @brief
		*    Sets the current color
		*
		*  @param[in] cColor
		*    RGBA color
		*/
		virtual void SetColor(const PLGraphics::Color4 &cColor = PLGraphics::Color4::White) = 0;

		//[-------------------------------------------------------]
		//[ Transform states                                      ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Resets all transform states to default (identity matrix)
		*/
		virtual void ResetTransformStates() = 0;

		/**
		*  @brief
		*    Retrieves a transform-state value
		*
		*  @param[in] nState
		*    State variable that is being queried. This parameter can
		*    be any member of the transform enumerated type.
		*  @param[in] bOriginal
		*    Return the original set state? The returned transform state may differ from the given
		*    original state. OpenGL backend example: If a rectangle texture buffer is used the used
		*    transform matrix is scaled because in this case OpenGL requires not normalized texture
		*    coordinates. If you give this matrix to a shader program, don't use the original matrix! This has
		*    only an influence on the texture matrices, view etc. matrices are always original.
		*
		*  @return
		*    The requested matrix, on error the projection matrix is returned
		*
		*  @note
		*    - A transform state is a 4x4 matrix. Matrices are assumed to be stored in
		*      column major order like OpenGL does.
		*/
		virtual const PLMath::Matrix4x4 &GetTransformState(Transform::Enum nState, bool bOriginal = true) const = 0;

		/**
		*  @brief
		*    Sets a single transform-state parameter
		*
		*  @param[in] nState
		*    State variable that is being modified. This parameter
		*    can be any member of the transform enumerated type.
		*  @param[in] mTrans
		*    New value for the transform state to be set
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @see
		*    - See GetTransformState()
		*/
		virtual bool SetTransformState(Transform::Enum nState, const PLMath::Matrix3x4 &mTrans) = 0;
		virtual bool SetTransformState(Transform::Enum nState, const PLMath::Matrix4x4 &mTrans) = 0;

		//[-------------------------------------------------------]
		//[ Texture stage states                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns a default texture stage state
		*
		*  @param[in] nState
		*    Texture stage state to return the default value from
		*
		*  @return
		*    The default state
		*/
		virtual PLCore::uint32 GetDefaultTextureStageState(TextureStage::Enum nState) const = 0;

		/**
		*  @brief
		*    Resets all texture stage states to default
		*
		*  @see
		*    - GetDefaultTextureStageState()
		*/
		virtual void ResetTextureStageStates() = 0;

		/**
		*  @brief
		*    Retrieves a texture stage state value
		*
		*  @param[in] nStage
		*    Texture stage to get the value from
		*  @param[in] nState
		*    State variable that is being queried. This parameter can
		*    be any member of the texture stage enumerated type.
		*
		*  @return
		*    The value of the queried texture stage state variable, < 0 on error
		*
		*  @see
		*    - GetDefaultTextureStageState()
		*/
		virtual int GetTextureStageState(PLCore::uint32 nStage, TextureStage::Enum nState) const = 0;

		/**
		*  @brief
		*    Sets a single texture stage state parameter
		*
		*  @param[in] nStage
		*    Texture stage to set the value
		*  @param[in] nState
		*    State variable that is being modified. This parameter
		*    can be any member of the texture stage enumerated type.
		*  @param[in] nValue
		*    New value for the texture stage state to be set. The meaning of
		*    this parameter is dependent on the value specified for nState.
		*    For example, if nState is TextureStage::ColorTexEnv, the second parameter
		*    must be one member of the texture environment enumerated type. (e.g. TextureEnvironment::Modulate)
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @see
		*    - GetDefaultTextureStageState()
		*/
		virtual bool SetTextureStageState(PLCore::uint32 nStage, TextureStage::Enum nState, PLCore::uint32 nValue) = 0;

		//[-------------------------------------------------------]
		//[ Material states                                       ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns a default material state
		*
		*  @param[in] nState
		*    Material state to return the default value from
		*
		*  @return
		*    The default state
		*
		*  @return
		*    The default state
		*/
		virtual PLCore::uint32 GetDefaultMaterialState(MaterialState::Enum nState) const = 0;

		/**
		*  @brief
		*    Resets all material states to default
		*
		*  @see
		*    - GetDefaultMaterialState()
		*/
		virtual void ResetMaterialStates() = 0;

		/**
		*  @brief
		*    Retrieves a material-state value
		*
		*  @param[in] nState
		*    State variable that is being queried. This parameter can
		*    be any member of the material enumerated type.
		*
		*  @return
		*    The value of the queried material state variable, < 0 on error
		*
		*  @see
		*    - GetDefaultMaterialState()
		*/
		virtual int GetMaterialState(MaterialState::Enum nState) const = 0;

		/**
		*  @brief
		*    Sets a single material-state parameter
		*
		*  @param[in] nState
		*    State variable that is being modified. This parameter
		*    can be any member of the material enumerated type.
		*  @param[in] nValue
		*    New value for the material state to be set
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @see
		*    - GetDefaultMaterialState()
		*/
		virtual bool SetMaterialState(MaterialState::Enum nState, PLCore::uint32 nValue) = 0;

		//[-------------------------------------------------------]
		//[ Light states                                          ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Gets the default light settings
		*
		*  @param[out] sLight
		*    Light structure which will receive the default settings
		*/
		virtual void GetDefaultLightSettings(Light &sLight) const = 0;

		/**
		*  @brief
		*    Resets the lights
		*
		*  @see
		*    - GetDefaultLightSettings()
		*/
		virtual void ResetLights() = 0;

		/**
		*  @brief
		*    Returns whether a light is enabled or not
		*
		*  @param[in] nLightID
		*    ID of the light
		*
		*  @return
		*    'true' if the light is enabled, else 'false'
		*
		*  @see
		*    - GetDefaultLightSettings()
		*/
		virtual bool IsLightEnabled(PLCore::uint32 nLightID) const = 0;

		/**
		*  @brief
		*    Sets whether a light is enabled or not
		*
		*  @param[in] nLightID
		*    ID of the light
		*  @param[in] bEnabled
		*    Is this light enabled?
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @see
		*    - GetDefaultLightSettings()
		*/
		virtual bool SetLightEnabled(PLCore::uint32 nLightID, bool bEnabled) = 0;

		/**
		*  @brief
		*    Returns light settings
		*
		*  @param[in]  nLightID
		*    ID of the light
		*  @param[out] sLight
		*    Will receive the light settings
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @see
		*    - GetDefaultLightSettings()
		*/
		virtual bool GetLight(PLCore::uint32 nLightID, Light &sLight) const = 0;

		/**
		*  @brief
		*    Sets light settings
		*
		*  @param[in] nLightID
		*    ID of the light
		*  @param[in] sLight
		*    The light settings
		*
		*  @return
		*    'true' if all went fine, else 'false'
		*
		*  @see
		*    - GetDefaultLightSettings()
		*/
		virtual bool SetLight(PLCore::uint32 nLightID, const Light &sLight) = 0;

		//[-------------------------------------------------------]
		//[ Clip plane states                                     ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Returns whether a clip plane is enabled/disabled
		*
		*  @param[in] nIndex
		*    Index (0 - Capabilities::nMaxClipPlanes-1) of the clip plane which should be checked
		*
		*  @return
		*    'true' if the requested clip plane is enabled, else 'false'
		*
		*  @note
		*    - A clip plane specifies a plane against which all geometry is clipped
		*    - By default, all clipping planes are defined as (0,0,0,0) in eye coordinates and are disabled
		*    - The coefficients which the clip plane methods take have the form of the general plane equation
		*    - Clip planes will only work in fix pass and often they are not well supported by hardware. So
		*      try to avoid using clip planes whenever you can!
		*/
		virtual bool IsClipPlaneEnabled(PLCore::uint8 nIndex) const = 0;

		/**
		*  @brief
		*    Enables/disables a clip plane
		*
		*  @param[in] nIndex
		*    Index (0 - Capabilities::nMaxClipPlanes-1) of the clip plane which should be enabled/disabled,
		*    < 0 = Set all clip planes to this value
		*  @param[in] bEnable
		*    Should the defined clip clip plane be enabled?
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe invalid plane index)
		*
		*  @see
		*    - IsClipPlaneEnabled()
		*/
		virtual bool SetClipPlaneEnabled(char nIndex = -1, bool bEnable = false) = 0;

		/**
		*  @brief
		*    Returns a clip plane
		*
		*  @param[in]  nIndex
		*    Index (0 - Capabilities::nMaxClipPlanes-1) of the clip plane which should be returned
		*  @param[out] fA
		*    Will receive the A coefficient of the requested clip plane
		*  @param[out] fB
		*    Will receive the B coefficient of the requested clip plane
		*  @param[out] fC
		*    Will receive the C coefficient of the requested clip plane
		*  @param[out] fD
		*    Will receive the D coefficient of the requested clip plane
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe invalid plane index)
		*
		*  @see
		*    - IsClipPlaneEnabled()
		*/
		virtual bool GetClipPlane(PLCore::uint8 nIndex, float &fA, float &fB, float &fC, float &fD) const = 0;

		/**
		*  @brief
		*    Sets a clip plane
		*
		*  @param[in] nIndex
		*    Index (0 - Capabilities::nMaxClipPlanes-1) of the clip plane which should be returned
		*    < 0 = Set all clip planes to this value
		*  @param[in] fA
		*    The new A coefficient of the given clip plane
		*  @param[in] fB
		*    The new B coefficient of the given clip plane
		*  @param[in] fC
		*    The new C coefficient of the given clip plane
		*  @param[in] fD
		*    The new D coefficient of the given clip plane
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe invalid plane index)
		*
		*  @see
		*    - IsClipPlaneEnabled()
		*/
		virtual bool SetClipPlane(char nIndex = -1, float fA = 0.0f, float fB = 0.0f, float fC = 0.0f, float fD = 0.0f) = 0;

		//[-------------------------------------------------------]
		//[ Vertex buffer states                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Gets the current vertex buffer
		*
		*  @param[in]  nStreamNumber
		*    Specifies the data stream number (0 - Capabilities::nMaxVertexBufferStreams()-1)
		*  @param[out] pnOffset
		*    If not a null pointer, this variable will receive the vertex offset in bytes
		*
		*  @return
		*    The current vertex buffer, a null pointer if there's no one
		*
		*  @note
		*    - By assigning different vertex buffers to different stream numbers its for instance possible
		*      to get the vertex positions from vertex buffer 0, the vertex normals from vertex buffer 1
		*      and so on.
		*    - Try to avoid conflicts in which different vertex buffers want to provide for instance the
		*      vertex positions!
		*    - For better performance, try to avoid using more than one vertex buffer at the same time
		*      whenever possible.
		*/
		virtual VertexBuffer *GetVertexBuffer(PLCore::uint32 nStreamNumber = 0, PLCore::uint32 *pnOffset = nullptr) const = 0;

		/**
		*  @brief
		*   Sets the current vertex buffer
		*
		*  @param[in] pVertexBuffer
		*    The vertex buffer which should be set, a null pointer if no vertex buffer should be set
		*  @param[in] nOffset
		*    Vertex offset (vertex index, NOT in bytes!)
		*  @param[in] nStreamNumber
		*    Specifies the data stream number (0 - Capabilities::nMaxVertexBufferStreams()-1)
		*
		*  @return
		*    'true' if all went fine, else 'false' (maybe this is already the current vertex buffer)
		*
		*  @note
		*    - If nStreamNumber is 0 and pVertexBuffer is a null pointer no vertex buffer at any stream number is set
		*    - If the first stream (the base stream) is set all other stream settings are cleared
		*    - See GetVertexBuffer()
		*/
		virtual bool SetVertexBuffer(VertexBuffer *pVertexBuffer = nullptr, PLCore::uint32 nOffset = 0, PLCore::uint32 nStreamNumber = 0) = 0;


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		PLRENDERER_API FixedFunctions();

		/**
		*  @brief
		*    Destructor
		*/
		PLRENDERER_API virtual ~FixedFunctions();


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
		FixedFunctions(const FixedFunctions &cSource);

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
		FixedFunctions &operator =(const FixedFunctions &cSource);


};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer


#endif // __PLRENDERER_FIXEDFUNCTIONS_H__
