/*********************************************************\
 *  File: AssimpLoader.cpp                                *
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
#include <assimp/material.h>
#include <assimp/postprocess.h>
#include <PLRenderer/RendererContext.h>
#include <PLRenderer/Texture/TextureManager.h>
#include <PLRenderer/Material/Material.h>
#include <PLRenderer/Material/Parameter.h>
#include <PLRenderer/Material/ParameterManager.h>
#include "PLAssimp/AssimpLoader.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLRenderer;
namespace PLAssimp {


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
AssimpLoader::AssimpLoader()
{
}

/**
*  @brief
*    Constructor
*/
AssimpLoader::AssimpLoader(const String &sDefaultTextureFileExtension) :
	m_sDefaultTextureFileExtension(sDefaultTextureFileExtension)
{
}

/**
*  @brief
*    Destructor
*/
AssimpLoader::~AssimpLoader()
{
}

/**
*  @brief
*    Returns the default texture file extension
*/
String AssimpLoader::GetDefaultTextureFileExtension() const
{
	return m_sDefaultTextureFileExtension;
}

/**
*  @brief
*    Converts a given Assimp string into a PixelLight string
*/
String AssimpLoader::AssimpStringToPL(const aiString &sAssimpString) const
{
	return String::FromUTF8(sAssimpString.data, -1, static_cast<uint32>(sAssimpString.length));
}

/**
*  @brief
*    Returns the Assimp post processing flags for the given quality step
*/
unsigned int AssimpLoader::QualityToAssimpFlags(uint8 nQuality) const
{
	switch (nQuality) {
		case 0:
			return 0;

		case 1:
			return aiProcessPreset_TargetRealtime_Fast;

		case 2:
			return aiProcessPreset_TargetRealtime_Quality;

		default:
			return aiProcessPreset_TargetRealtime_MaxQuality;
	}
}


/**
*  @brief
*    Converts a given Assimp material into a PixelLight material
*/
void AssimpLoader::AssimpMaterialToPL(const aiMaterial &cAssimpMaterial, Material &cPLMaterial) const
{
	// Get the parameter manager of the material
	ParameterManager &cParameterManager = cPLMaterial.GetParameterManager();

	{ // AI_MATKEY_TWOSIDED - default: 0
		int nTwoSided = 0;
		aiGetMaterialInteger(&cAssimpMaterial, AI_MATKEY_TWOSIDED, &nTwoSided);
		if (nTwoSided)
			cParameterManager.SetParameter1i("TwoSided", 1);
	}

	// AI_MATKEY_SHADING_MODEL - not supported
	// AI_MATKEY_ENABLE_WIREFRAME - not supported
	// AI_MATKEY_BLEND_FUNC - not supported

	{ // AI_MATKEY_OPACITY - default: 1.0
		float fOpacity = 1.0f;
		aiGetMaterialFloat(&cAssimpMaterial, AI_MATKEY_OPACITY, &fOpacity);
		if (fOpacity < 1.0f)
			cParameterManager.SetParameter1f("Opacity", fOpacity);
	}

	{ // AI_MATKEY_BUMPSCALING - default: 1.0
		float fBumpScaling = 1.0f;
		aiGetMaterialFloat(&cAssimpMaterial, AI_MATKEY_BUMPSCALING, &fBumpScaling);
		if (fBumpScaling != 1.0f)
			cParameterManager.SetParameter1f("NormalMapBumpiness", fBumpScaling);
	}

	{ // AI_MATKEY_SHININESS - default: 45.0
		float fShininess = 45.0f;
		aiGetMaterialFloat(&cAssimpMaterial, AI_MATKEY_SHININESS, &fShininess);
		if (fShininess != 45.0f)
			cParameterManager.SetParameter1f("SpecularExponent", fShininess);
	}

	// AI_MATKEY_REFLECTIVITY - not supported
	// AI_MATKEY_SHININESS_STRENGTH - not supported
	// AI_MATKEY_REFRACTI - not supported

	{ // AI_MATKEY_COLOR_DIFFUSE - default: 1.0 1.0 1.0
		aiColor3D cColor(1.0f, 1.0f, 1.0f);
		if (cAssimpMaterial.Get(AI_MATKEY_COLOR_DIFFUSE, cColor) == AI_SUCCESS)
			cParameterManager.SetParameter3f("DiffuseColor", cColor.r, cColor.g, cColor.b);
	}

	// AI_MATKEY_COLOR_AMBIENT - not supported

	{ // AI_MATKEY_COLOR_SPECULAR - default: 1.0 1.0 1.0
		aiColor3D cColor(1.0f, 1.0f, 1.0f);
		if (cAssimpMaterial.Get(AI_MATKEY_COLOR_SPECULAR, cColor) == AI_SUCCESS)
			cParameterManager.SetParameter3f("SpecularColor", cColor.r, cColor.g, cColor.b);
	}

	{ // AI_MATKEY_COLOR_EMISSIVE - default: 1.0 1.0 1.0
		aiColor3D cColor(1.0f, 1.0f, 1.0f);
		if (cAssimpMaterial.Get(AI_MATKEY_COLOR_EMISSIVE, cColor) == AI_SUCCESS)
			cParameterManager.SetParameter3f("EmissiveMapColor", cColor.r, cColor.g, cColor.b);
	}

	// AI_MATKEY_COLOR_TRANSPARENT - not supported
	// AI_MATKEY_COLOR_REFLECTIVE - not supported
	// AI_MATKEY_GLOBAL_BACKGROUND_IMAGE - not supported

	{ // aiTextureType_DIFFUSE
		aiString sAssimpFilename;
		if (cAssimpMaterial.GetTexture(aiTextureType_DIFFUSE, 0, &sAssimpFilename) == AI_SUCCESS)
			SetParameterTexture(cParameterManager, "DiffuseMap", AssimpStringToPL(sAssimpFilename));
	}

	{ // aiTextureType_SPECULAR
		aiString sAssimpFilename;
		if (cAssimpMaterial.GetTexture(aiTextureType_SPECULAR, 0, &sAssimpFilename) == AI_SUCCESS)
			SetParameterTexture(cParameterManager, "SpecularMap", AssimpStringToPL(sAssimpFilename));
	}

	{ // aiTextureType_SPECULAR
		aiString sAssimpFilename;
		if (cAssimpMaterial.GetTexture(aiTextureType_SPECULAR, 0, &sAssimpFilename) == AI_SUCCESS)
			SetParameterTexture(cParameterManager, "SpecularMap", AssimpStringToPL(sAssimpFilename));
	}

	// aiTextureType_AMBIENT - not supported

	{ // aiTextureType_EMISSIVE
		aiString sAssimpFilename;
		if (cAssimpMaterial.GetTexture(aiTextureType_EMISSIVE, 0, &sAssimpFilename) == AI_SUCCESS)
			SetParameterTexture(cParameterManager, "EmissiveMap", AssimpStringToPL(sAssimpFilename));
	}

	{ // aiTextureType_HEIGHT
		aiString sAssimpFilename;
		if (cAssimpMaterial.GetTexture(aiTextureType_HEIGHT, 0, &sAssimpFilename) == AI_SUCCESS)
			SetParameterTexture(cParameterManager, "HeightMap", AssimpStringToPL(sAssimpFilename));
	}

	{ // aiTextureType_NORMALS
		aiString sAssimpFilename;
		if (cAssimpMaterial.GetTexture(aiTextureType_NORMALS, 0, &sAssimpFilename) == AI_SUCCESS)
			SetParameterTexture(cParameterManager, "NormalMap", AssimpStringToPL(sAssimpFilename));
	}

	//  aiTextureType_SHININESS - not supported
	//  aiTextureType_OPACITY - not supported
	//  aiTextureType_DISPLACEMENT - not supported

	{ // aiTextureType_LIGHTMAP
		aiString sAssimpFilename;
		if (cAssimpMaterial.GetTexture(aiTextureType_LIGHTMAP, 0, &sAssimpFilename) == AI_SUCCESS)
			SetParameterTexture(cParameterManager, "LightMap", AssimpStringToPL(sAssimpFilename));
	}

	{ // aiTextureType_REFLECTION
		aiString sAssimpFilename;
		if (cAssimpMaterial.GetTexture(aiTextureType_REFLECTION, 0, &sAssimpFilename) == AI_SUCCESS)
			SetParameterTexture(cParameterManager, "ReflectionColor", AssimpStringToPL(sAssimpFilename));
	}
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Copy constructor
*/
AssimpLoader::AssimpLoader(const AssimpLoader &cSource)
{
	// No implementation because the copy constructor is never used
}

/**
*  @brief
*    Copy operator
*/
AssimpLoader &AssimpLoader::operator =(const AssimpLoader &cSource)
{
	// No implementation because the copy operator is never used
	return *this;
}

/**
*  @brief
*    Sets the texture of a material parameter
*/
void AssimpLoader::SetParameterTexture(ParameterManager &cParameterManager, const String &sName, const String &sFilename) const
{
	// Get/create the material parameter
	Parameter *pParameter = cParameterManager.CreateParameter(ParameterManager::TextureBuffer, sName);
	if (pParameter) {
		// Get the texture manager instance
		TextureManager &cTextureManager = cParameterManager.GetRendererContext().GetTextureManager();

		// First, try to use the provided filename directly
		Texture *pTexture = cTextureManager.LoadResource(sFilename);
		if (!pTexture) {
			// Hm, we were not able to load in the given texture, so, append the given default texture file extension and try it again
			pTexture = cTextureManager.LoadResource(sFilename + '.' + m_sDefaultTextureFileExtension);
			if (!pTexture) {
				// It failed, again...

				#ifndef WIN32
					// If we're not on MS Windows, there might be a chance that there's an issue with upper/lower case within the used default texture file extension... our last chance...
					String sUpperDefaultTextureFileExtension = m_sDefaultTextureFileExtension;
					sUpperDefaultTextureFileExtension.ToUpper();
					pTexture = cTextureManager.LoadResource(sFilename + '.' + sUpperDefaultTextureFileExtension);
				#endif
			}
		}

		// Texture successfully loaded?
		if (pTexture)
			pParameter->SetValueTexture(pTexture);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLAssimp
