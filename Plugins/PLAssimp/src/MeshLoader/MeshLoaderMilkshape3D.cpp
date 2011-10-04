/*********************************************************\
 *  File: MeshLoaderMilkshape3D.cpp                      *
 *
 *  Copyright (C) 2002-2011 The PixelLight Team (http://www.pixellight.org/)
 *
 *  This file is part of PixelLight.
 *
 *  PixelLight is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  PixelLight is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with PixelLight. If not, see <http://www.gnu.org/licenses/>.
\*********************************************************/


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "PLAssimp/AssimpMeshLoader.h"
#include "PLAssimp/MeshLoader/MeshLoaderMilkshape3D.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLMesh;
namespace PLAssimp {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(MeshLoaderMilkshape3D)


//[-------------------------------------------------------]
//[ Public virtual MeshLoader functions                   ]
//[-------------------------------------------------------]
bool MeshLoaderMilkshape3D::LoadParams(Mesh &cMesh, File &cFile, bool bStatic)
{
	// Just let Assimp do all the hard work for us
	return AssimpMeshLoader().Load(cMesh, cFile, bStatic, "ms3d");
}


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
MeshLoaderMilkshape3D::MeshLoaderMilkshape3D()
{
}

/**
*  @brief
*    Destructor
*/
MeshLoaderMilkshape3D::~MeshLoaderMilkshape3D()
{
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLAssimp
