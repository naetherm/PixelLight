/*********************************************************\
 *  File: PLDatabaseMySQL.cpp                            *
 *
 *  Copyright (C) 2002-2013 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  PixelLight is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  PixelLight is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with PixelLight. If not, see <http://www.gnu.org/licenses/>.
\*********************************************************/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <PLCore/ModuleMain.h>


//[-------------------------------------------------------]
//[ Module definition                                     ]
//[-------------------------------------------------------]
pl_module_plugin("PLDatabaseMySQL")
	pl_module_vendor("Copyright (C) 2002-2013 by The PixelLight Team")
	pl_module_license("GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version")
	pl_module_description("MySQL 5.0.67 (http://www.mysql.com/) database implementation")
	pl_module_dependencies_windows_32_release("libmySQL.dll")
	pl_module_dependencies_windows_32_debug("libmySQL.dll")
	pl_module_dependencies_windows_64_release("libmySQL64.dll")
	pl_module_dependencies_windows_64_debug("libmySQL64.dll")
pl_module_end
