/*********************************************************\
 *  File: CoreConfigGroup.h                              *
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


#ifndef __PLCORE_CONFIGGROUP_H__
#define __PLCORE_CONFIGGROUP_H__
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLCore/Config/Config.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace PLCore {


//[-------------------------------------------------------]
//[ Classes                                               ]
//[-------------------------------------------------------]
/**
*  @brief
*    PLCore general configuration 'collection' class
*/
class CoreConfigGroup : public ConfigGroup {


	//[-------------------------------------------------------]
	//[ Reflected class                                       ]
	//[-------------------------------------------------------]
	pl_rtti()


	//[-------------------------------------------------------]
	//[ Protected functions                                   ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		CoreConfigGroup();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~CoreConfigGroup();


};

/**
*  @brief
*    PLCore configuration classes
*/
class CoreConfig : public CoreConfigGroup {


	//[-------------------------------------------------------]
	//[ Reflected class                                       ]
	//[-------------------------------------------------------]
	pl_rtti()


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLCORE_API CoreConfig();

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~CoreConfig();


	//[-------------------------------------------------------]
	//[ Public properties                                     ]
	//[-------------------------------------------------------]
	public:
		pl_declare_property(FirstRun, bool);
		pl_declare_property(Language, String);

};

/**
*  @brief
*    PLCore frontend configuration classes
*/
class FrontendConfig : public CoreConfigGroup {


	//[-------------------------------------------------------]
	//[ Reflected class                                       ]
	//[-------------------------------------------------------]
	pl_rtti()


	//[-------------------------------------------------------]
	//[ Public functions                                      ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PLCORE_API FrontendConfig();

		/**
		*  @brief
		*    Destructor
		*/
		PLCORE_API virtual ~FrontendConfig();

	//[-------------------------------------------------------]
	//[ Public properties                                     ]
	//[-------------------------------------------------------]
	public:
		pl_declare_property(X, int);
		pl_declare_property(Y, int);
		pl_declare_property(Width, uint32);
		pl_declare_property(Height, uint32);

};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLCore


//[-------------------------------------------------------]
//[ Reflected class                                       ]
//[-------------------------------------------------------]
pl_declare_class(PLCore::CoreConfigGroup)
pl_declare_class(PLCore::CoreConfig)
pl_declare_class(PLCore::FrontendConfig)

#endif // __PLCORE_CONFIGGROUP_H__
