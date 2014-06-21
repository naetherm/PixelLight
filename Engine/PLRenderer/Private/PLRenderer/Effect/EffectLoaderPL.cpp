/*********************************************************\
 *  File: EffectLoaderPL.cpp                             *
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
#include <PLCore/Log/Log.h>
#include <PLCore/Xml/Xml.h>
#include <PLCore/Tools/Tools.h>
#include <PLCore/String/ParseTools.h>
#include "PLRenderer/Texture/TextureHandler.h"
#include "PLRenderer/Material/ParameterManager.h"
#include "PLRenderer/Effect/EffectPass.h"
#include "PLRenderer/Effect/EffectManager.h"
#include "PLRenderer/Effect/EffectTechnique.h"
#include "PLRenderer/Effect/EffectPassLayer.h"
#include "PLRenderer/Effect/EffectLoaderPL.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLGraphics;
namespace PLRenderer {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(EffectLoaderPL)


//[-------------------------------------------------------]
//[ Public RTTI methods                                   ]
//[-------------------------------------------------------]
bool EffectLoaderPL::Load(Effect &Effect, File &cFile)
{
	// Load XML document
	XmlDocument cDocument;
	if (cDocument.Load(cFile)) {
		// Get effect element
		const XmlElement *pFXElement = cDocument.GetFirstChildElement("Effect");
		if (pFXElement) {
			// Get the format version
			const int nVersion = pFXElement->GetAttribute("Version").GetInt();

			// Unknown
			if (nVersion > 1) {
				PL_LOG(Error, cDocument.GetValue() + ": " + UnknownFormatVersion)

			// 1 (current)
			} else if (nVersion == 1) {
				return LoadV1(Effect, *pFXElement);

			// ""/0 (same format as 1)
			} else if (nVersion == 0) {
				// [DEPRECATED]
				PL_LOG(Warning, cDocument.GetValue() + ": " + DeprecatedFormatVersion)
				return LoadV1(Effect, *pFXElement);

			// No longer supported format version
			} else if (nVersion >= 0) {
				PL_LOG(Warning, cDocument.GetValue() + ": " + NoLongerSupportedFormatVersion)

			// Invalid format version (negative!)
			} else {
				PL_LOG(Error, cDocument.GetValue() + ": " + InvalidFormatVersion)
			}
		} else {
			PL_LOG(Error, "Can't find 'Effect' element")
		}
	} else {
		PL_LOG(Error, cDocument.GetValue() + ": " + cDocument.GetErrorDesc())
	}

	// Error!
	return false;
}

bool EffectLoaderPL::Save(const Effect &cEffect, File &cFile)
{
	// Create XML document
	XmlDocument cDocument;

	// Add declaration
	XmlDeclaration *pDeclaration = new XmlDeclaration("1.0", "", "");
	cDocument.LinkEndChild(*pDeclaration);

	// Add effect
	XmlElement *pFXElement = new XmlElement("Effect");
	pFXElement->SetAttribute("Version", "1");

	// General
	if (cEffect.GetFlags() || cEffect.GetBlend()) {
		XmlElement *pGeneralElement = new XmlElement("General");

		// Flags
		if (cEffect.GetFlags())
			pGeneralElement->SetAttribute("Flags", ParseTools::GetStringFromFlags(cEffect.GetFlags()));

		// Blend
		if (cEffect.GetBlend())
			pGeneralElement->SetAttribute("Blend", "1");

		// Link general element
		pFXElement->LinkEndChild(*pGeneralElement);
	}

	// Save parameters
	if (cEffect.GetParameterManager().GetNumOfParameters()) {
		XmlElement *pParametersElement = new XmlElement("Parameters");

		// Populate the XML parameters element
		cEffect.GetParameterManager().AddParametersToXMLElement(*pParametersElement);

		// Link parameters element
		pFXElement->LinkEndChild(*pParametersElement);
	}

	// Save techniques
	for (uint32 nTech=0; nTech<cEffect.GetNumOfTechniques(); nTech++) {
		const EffectTechnique *pTechnique = cEffect.GetTechnique(nTech);
		if (pTechnique) {
			XmlElement *pTechniqueElement = new XmlElement("Technique");

			// Name
			pTechniqueElement->SetAttribute("Name", pTechnique->GetName());

			// Write down passes
			for (uint32 nPass=0; nPass<pTechnique->GetNumOfPasses(); nPass++) {
				const EffectPass *pFXPass = pTechnique->GetPass(nPass);
				if (pFXPass) {
					XmlElement *pFXPassElement = new XmlElement("Pass");

					// Name
					pFXPassElement->SetAttribute("Name", pFXPass->GetName());

					{ // Render states
						XmlElement *pRenderStatesElement = new XmlElement("RenderStates");
						pFXPass->GetRenderStates().GetValuesXml(*pRenderStatesElement);
						if (pRenderStatesElement->GetFirstAttribute())
							pFXPassElement->LinkEndChild(*pRenderStatesElement);
						else
							delete pRenderStatesElement;
					}

					{ // Fixed functions render states
						XmlElement *pFixedFunctionsRenderStatesElement = new XmlElement("FixedFunctionsRenderStates");
						pFXPass->GetFixedFunctionsRenderStates().GetValuesXml(*pFixedFunctionsRenderStatesElement);
						if (pFixedFunctionsRenderStatesElement->GetFirstAttribute())
							pFXPassElement->LinkEndChild(*pFixedFunctionsRenderStatesElement);
						else
							delete pFixedFunctionsRenderStatesElement;
					}

					{ // Effect
						XmlElement *pEffectElement = new XmlElement("Effect");

						// Shininess
						const float fShininess = Tools::UInt32ToFloat(pFXPass->GetMaterialState(FixedFunctions::MaterialState::Shininess));
						if (fShininess)
							pEffectElement->SetAttribute("Shininess", String(fShininess));

						// Color
						Color4 cColor = pFXPass->GetColor();
						if (cColor.r != 1.0f || cColor.g != 1.0f || cColor.b != 1.0f || cColor.a != 1.0f)
							pEffectElement->SetAttribute("Color", cColor.ToString());

						// AmbientColor
						uint32 nColor = pFXPass->GetMaterialState(FixedFunctions::MaterialState::Ambient);
						cColor.FromUInt32(nColor);
						if (cColor.r != 0.2f || cColor.g != 0.2f || cColor.b != 0.2f || cColor.a != 1.0f)
							pEffectElement->SetAttribute("AmbientColor", cColor.ToString());

						// DiffuseColor
						nColor = pFXPass->GetMaterialState(FixedFunctions::MaterialState::Diffuse);
						cColor.FromUInt32(nColor);
						if (cColor.r != 0.8f || cColor.g != 0.8f || cColor.b != 0.8f || cColor.a != 1.0f)
							pEffectElement->SetAttribute("DiffuseColor", cColor.ToString());

						// SpecularColor
						nColor = pFXPass->GetMaterialState(FixedFunctions::MaterialState::Specular);
						cColor.FromUInt32(nColor);
						if (cColor.r || cColor.g || cColor.b || cColor.a != 1.0f)
							pEffectElement->SetAttribute("SpecularColor", cColor.ToString());

						// EmissionColor
						nColor = pFXPass->GetMaterialState(FixedFunctions::MaterialState::Emission);
						cColor.FromUInt32(nColor);
						if (cColor.r || cColor.g || cColor.b || cColor.a != 1.0f)
							pEffectElement->SetAttribute("EmissionColor", cColor.ToString());

						// Link effect element
						if (pEffectElement->GetFirstAttribute())
							pFXPassElement->LinkEndChild(*pEffectElement);
						else
							delete pEffectElement;
					}

					// Layers
					for (uint32 i=0; i<pFXPass->GetNumOfLayers(); i++) {
						const EffectPassLayer *pFXPassLayer = pFXPass->GetLayer(i);
						if (pFXPassLayer) {
							XmlElement *pLayerElement = new XmlElement("Layer");

							// Texture
							if (pFXPassLayer->GetTexture().GetLength()) {
								XmlElement *pTextureElement = new XmlElement("Texture");
								XmlText *pValue = new XmlText(pFXPassLayer->GetTexture());
								pTextureElement->LinkEndChild(*pValue);
								pLayerElement->LinkEndChild(*pTextureElement);
							}

							{ // Sampler states
								XmlElement *pSamplerStatesElement = new XmlElement("SamplerStates");
								pFXPassLayer->GetSamplerStates().GetValuesXml(*pSamplerStatesElement);
								if (pSamplerStatesElement->GetFirstAttribute())
									pLayerElement->LinkEndChild(*pSamplerStatesElement);
								else
									delete pSamplerStatesElement;
							}

							{ // Fixed functions texture stage states
								XmlElement *pFixedFunctionsTextureStageStatesElement = new XmlElement("FixedFunctionsTextureStageStates");
								pFXPassLayer->GetFixedFunctionsTextureStageStates().GetValuesXml(*pFixedFunctionsTextureStageStatesElement);
								if (pFixedFunctionsTextureStageStatesElement->GetFirstAttribute())
									pLayerElement->LinkEndChild(*pFixedFunctionsTextureStageStatesElement);
								else
									delete pFixedFunctionsTextureStageStatesElement;
							}

							// Link layer element
							pFXPassElement->LinkEndChild(*pLayerElement);
						}
					}

					// [TODO] New shader interface
					/*
					// Vertex shader
					const ShaderHandler *pShader = pFXPass->GetVertexShader();
					if (pShader) {
						XmlElement *pVertexShaderElement = new XmlElement("VertexShader");
						// Profile
						if (pShader->GetProfile().GetLength())
							pVertexShaderElement->SetAttribute("Profile", pShader->GetProfile());
						// Filename
						pVertexShaderElement->SetAttribute("Filename", pShader->GetResource()->GetName());
						// Link vertex shader element
						pFXPassElement->LinkEndChild(*pVertexShaderElement);
					}

					// Fragment shader
					pShader = pFXPass->GetFragmentShader();
					if (pShader) {
						XmlElement *pFragmentShaderElement = new XmlElement("FragmentShader");
						// Profile
						if (pShader->GetProfile().GetLength())
							pFragmentShaderElement->SetAttribute("Profile", pShader->GetProfile());
						// Filename
						pFragmentShaderElement->SetAttribute("Filename", pShader->GetResource()->GetName());
						// Link fragment shader element
						pFXPassElement->LinkEndChild(*pFragmentShaderElement);
					}
					*/

					pTechniqueElement->LinkEndChild(*pFXPassElement);
				}
			}

			// Link technique element
			pFXElement->LinkEndChild(*pTechniqueElement);
		}
	}

	// Link effect element
	cDocument.LinkEndChild(*pFXElement);

	// Save settings
	cDocument.Save(cFile);

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
EffectLoaderPL::EffectLoaderPL()
{
}

/**
*  @brief
*    Destructor
*/
EffectLoaderPL::~EffectLoaderPL()
{
}


//[-------------------------------------------------------]
//[ Private functions                                     ]
//[-------------------------------------------------------]
/**
*  @brief
*    Loader implementation for format version 1
*/
bool EffectLoaderPL::LoadV1(Effect &cFX, const XmlElement &cFXElement) const
{
	// Iterate through all elements
	int nSelectedTechnique = -1;
	const XmlElement *pElement = cFXElement.GetFirstChildElement();
	while (pElement) {
		String sValue = pElement->GetValue();
		if (sValue.GetLength()) {
			// General
			if (sValue == "General") {
				// Flags
				int nFlags = 0;
				pElement->QueryIntAttribute("Flags", &nFlags);
				cFX.SetFlags(nFlags);

				// Blend
				sValue = pElement->GetAttribute("Blend");
				cFX.SetBlend(sValue.GetLength() && sValue.GetBool());

			// Parameters
			} else if (sValue == "Parameters") {
				const XmlNode *pParameterNode = pElement->GetFirstChild();
				while (pParameterNode) {
					// Is this an XML element?
					if (pParameterNode->GetType() == XmlNode::Element)
						cFX.GetParameterManager().AddParameterFromXMLElement(*static_cast<const XmlElement*>(pParameterNode));

					// Next node, please
					pParameterNode = pElement->IterateChildren(pParameterNode);
				}

			// Technique
			} else if (sValue == "Technique") {
				// Add technique
				EffectTechnique *pTechnique = cFX.AddTechnique();
				cFX.SelectTechnique(cFX.GetNumOfTechniques()-1);

				// Set name
				pTechnique->SetName(pElement->GetAttribute("Name"));

				// Iterate through all passes
				bool bTechniqueValid = true;
				const XmlElement *pTechniqueElement = pElement->GetFirstChildElement("Pass");
				while (pTechniqueElement) {
					// Add pass
					EffectPass *pFXPass = pTechnique->AddPass();

					// Set pass name
					sValue = pTechniqueElement->GetAttribute("Name");
					pFXPass->SetName(sValue.GetLength() ? sValue : (String("Pass ") + (pTechnique->GetNumOfPasses()-1)));

					// Iterate through all pass elements
					const XmlElement *pFXPassElement = pTechniqueElement->GetFirstChildElement();
					while (pFXPassElement) {
						sValue = pFXPassElement->GetValue();
						if (sValue.GetLength()) {
							//[-------------------------------------------------------]
							//[ RenderStates                                          ]
							//[-------------------------------------------------------]
							if (sValue == "RenderStates") {
								pFXPass->GetRenderStates().SetValuesXml(*pFXPassElement);

							//[-------------------------------------------------------]
							//[ FixedFunctionsRenderStates                            ]
							//[-------------------------------------------------------]
							} else if (sValue == "FixedFunctionsRenderStates") {
								pFXPass->GetFixedFunctionsRenderStates().SetValuesXml(*pFXPassElement);

							//[-------------------------------------------------------]
							//[ Material                                              ]
							//[-------------------------------------------------------]
							} else if (sValue == "Material") {
								// Shininess
								sValue = pFXPassElement->GetAttribute("Shininess");
								if (sValue.GetLength())
									pFXPass->SetMaterialState(FixedFunctions::MaterialState::Shininess, Tools::FloatToUInt32(sValue.GetFloat()));
								// Color
								sValue = pFXPassElement->GetAttribute("Color");
								if (sValue.GetLength()) {
									Color4 cColor = Color4::White;
									ParseTools::ParseFloatArray(sValue, cColor, 4);
									pFXPass->SetColor(cColor);
								}
								// Ambient
								sValue = pFXPassElement->GetAttribute("Ambient");
								if (sValue.GetLength()) {
									Color4 cColor(0.2f, 0.2f, 0.2f, 1.0f);
									ParseTools::ParseFloatArray(sValue, cColor, 4);
									pFXPass->SetMaterialState(FixedFunctions::MaterialState::Ambient, cColor.ToUInt32());
								}
								// Diffuse
								sValue = pFXPassElement->GetAttribute("Diffuse");
								if (sValue.GetLength()) {
									Color4 cColor(0.8f, 0.8f, 0.8f, 1.0f);
									ParseTools::ParseFloatArray(sValue, cColor, 4);
									pFXPass->SetMaterialState(FixedFunctions::MaterialState::Diffuse, cColor.ToUInt32());
								}
								// Specular
								sValue = pFXPassElement->GetAttribute("Specular");
								if (sValue.GetLength()) {
									Color4 cColor(0.0f, 0.0f, 0.0f, 1.0f);
									ParseTools::ParseFloatArray(sValue, cColor, 4);
									pFXPass->SetMaterialState(FixedFunctions::MaterialState::Specular, cColor.ToUInt32());
								}
								// Emission
								sValue = pFXPassElement->GetAttribute("Emission");
								if (sValue.GetLength()) {
									Color4 cColor(0.0f, 0.0f, 0.0f, 1.0f);
									ParseTools::ParseFloatArray(sValue, cColor, 4);
									pFXPass->SetMaterialState(FixedFunctions::MaterialState::Emission, cColor.ToUInt32());
								}

							//[-------------------------------------------------------]
							//[ Layer                                                 ]
							//[-------------------------------------------------------]
							} else if (sValue == "Layer") {
								// [TODO]
								// Check if we still can add texture layers
					//			if (m_lstLayers.GetNumOfElements() >= (signed)pRenderer->GetCapabilities().nMaxTextureUnits)
					//				continue; // Sorry, texture layer limit reached!

								// Add new effect pass layer
								EffectPassLayer *pFXPassLayer = pFXPass->AddLayer();

								// Iterate through all layer elements
								const XmlElement *pLayerElement = pFXPassElement->GetFirstChildElement();
								while (pLayerElement) {
									sValue = pLayerElement->GetValue();
									if (sValue.GetLength()) {
										// Texture
										if (sValue == "Texture") {
											// Get value
											const XmlNode *pNode = pLayerElement->GetFirstChild();
											if (pNode && pNode->GetType() == XmlNode::Text)
												pFXPassLayer->SetTexture(pNode->GetValue());

										// SamplerStates
										} else if (sValue == "SamplerStates") {
											pFXPassLayer->GetSamplerStates().SetValuesXml(*pLayerElement);

										// FixedFunctionsTextureStageStates
										} else if (sValue == "FixedFunctionsTextureStageStates") {
											pFXPassLayer->GetFixedFunctionsTextureStageStates().SetValuesXml(*pLayerElement);
										}
									}

									// Next element, please
									pLayerElement = pLayerElement->GetNextSiblingElement();
								}

							//[-------------------------------------------------------]
							//[ VertexShader                                          ]
							//[-------------------------------------------------------]
							} else if (sValue == "VertexShader") {
								// Get shader language
								const String sLanguage = pFXPassElement->GetAttribute("Language");

								// Get optional profile
								sValue = pFXPassElement->GetAttribute("Profile");
								String sProfile;
								if (sValue.GetLength())
									sProfile = sValue;

								// Get filename and load the shader
								sValue = pFXPassElement->GetAttribute("Filename");
								if (sValue.GetLength()) {
									if (!pFXPass->LoadVertexShader(sValue, sLanguage, sProfile))
										bTechniqueValid = false; // Error - shader loading/initialization failed!
								}

							//[-------------------------------------------------------]
							//[ FragmentShader                                        ]
							//[-------------------------------------------------------]
							} else if (sValue == "FragmentShader") {
								// Get shader language
								const String sLanguage = pFXPassElement->GetAttribute("Language");

								// Get optional profile
								sValue = pFXPassElement->GetAttribute("Profile");
								String sProfile;
								if (sValue.GetLength())
									sProfile = sValue;

								// Get filename and load the shader
								sValue = pFXPassElement->GetAttribute("Filename");
								if (sValue.GetLength()) {
									if (!pFXPass->LoadFragmentShader(sValue, sLanguage, sProfile))
										bTechniqueValid = false; // Error - shader loading/initialization failed!
								}
							}
						}

						// Next element, please
						pFXPassElement = pFXPassElement->GetNextSiblingElement();
					}

					// Next element, please
					pTechniqueElement =pTechniqueElement->GetNextSiblingElement("Pass");
				}

				// Is this technique is valid and at the moment no technique is selected
				// in the effect choose this technique by default
				pTechnique->SetValid(bTechniqueValid);
				if (bTechniqueValid && nSelectedTechnique < 0)
					nSelectedTechnique = cFX.GetNumOfTechniques()-1;
			}
		}

		// Next element, please
		pElement = pElement->GetNextSiblingElement();
	}
	cFX.SelectTechnique(nSelectedTechnique);

	// Done
	return true;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLRenderer
