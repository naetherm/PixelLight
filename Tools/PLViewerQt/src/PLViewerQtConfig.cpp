/*********************************************************\
 *  File: Config.cpp                                     *
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
#include "ApplicationQt.h"
#include "PLViewerQtConfig.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_class_metadata(PLViewerQtConfig, "", PLCore::ConfigGroup, "PLViewerQt general configuration classes")
	// Constructors
	pl_constructor_0_metadata(DefaultConstructor,	"Default constructor",	"")
	// Attributes
	pl_attribute_metadata(ShowColorGradientBackground,	bool,	true,	ReadWrite,	"Show color gradient background? (whether or not this has an effect depends on the used scene renderer)",	"")
	pl_attribute_metadata(ShowBackfacesAndSilhouettes,	bool,	true,	ReadWrite,	"Show backfaces and silhouettes? (whether or not this has an effect depends on the used scene renderer)",	"")
pl_class_metadata_end(PLViewerQtConfig)


//[-------------------------------------------------------]
//[ Public RTTI get/set functions                         ]
//[-------------------------------------------------------]
bool PLViewerQtConfig::GetShowColorGradientBackground() const
{
	return m_bShowColorGradientBackground;
}

void PLViewerQtConfig::SetShowColorGradientBackground(bool bValue)
{
	m_bShowColorGradientBackground = bValue;

	// Update color gradient background according to the configuration
	CoreApplication *pCoreApplication = CoreApplication::GetApplication();
	if (pCoreApplication && pCoreApplication->IsInstanceOf("ApplicationQt"))
		static_cast<ApplicationQt*>(pCoreApplication)->UpdateColorGradientBackground();
}

bool PLViewerQtConfig::GetShowBackfacesAndSilhouettes() const
{
	return m_bShowBackfacesAndSilhouettes;
}

void PLViewerQtConfig::SetShowBackfacesAndSilhouettes(bool bValue)
{
	m_bShowBackfacesAndSilhouettes = bValue;

	// Update backfaces and silhouettes visualization according to the configuration
	CoreApplication *pCoreApplication = CoreApplication::GetApplication();
	if (pCoreApplication && pCoreApplication->IsInstanceOf("ApplicationQt"))
		static_cast<ApplicationQt*>(pCoreApplication)->UpdateBackfacesAndSilhouettes();
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
PLViewerQtConfig::PLViewerQtConfig() :
	ShowColorGradientBackground(this),
	ShowBackfacesAndSilhouettes(this),
	m_bShowColorGradientBackground(true),
	m_bShowBackfacesAndSilhouettes(true)

{
}

/**
*  @brief
*    Destructor
*/
PLViewerQtConfig::~PLViewerQtConfig()
{
}
