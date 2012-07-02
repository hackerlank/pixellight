/*********************************************************\
 *  File: ShaderFunctionShadingLighting.cpp              *
 *
 *  Copyright (C) 2002-2012 The PixelLight Team (http://www.pixellight.org/)
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
#include <PLCore/Base/ClassManager.h>
#include <PLRenderer/Renderer/ProgramWrapper.h>
#include <PLScene/Scene/SNLight.h>
#include <PLScene/Visibility/VisNode.h>
#include <PLVolume/Scene/SNVolume.h>
#include "PLVolumeRenderer/SRPVolume.h"
#include "PLVolumeRenderer/Shading/ShaderFunctionShadingLighting.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
using namespace PLCore;
using namespace PLGraphics;
using namespace PLMath;
using namespace PLScene;
using namespace PLRenderer;
namespace PLVolumeRenderer {


//[-------------------------------------------------------]
//[ RTTI interface                                        ]
//[-------------------------------------------------------]
pl_implement_class(ShaderFunctionShadingLighting)


//[-------------------------------------------------------]
//[ Public functions                                      ]
//[-------------------------------------------------------]
/**
*  @brief
*    Default constructor
*/
ShaderFunctionShadingLighting::ShaderFunctionShadingLighting()
{
}

/**
*  @brief
*    Destructor
*/
ShaderFunctionShadingLighting::~ShaderFunctionShadingLighting()
{
}


//[-------------------------------------------------------]
//[ Public virtual ShaderFunction functions               ]
//[-------------------------------------------------------]
String ShaderFunctionShadingLighting::GetSourceCode(const String &sShaderLanguage, ESourceCodeType nSourceCodeType)
{
	// Check requested shader language
	if (sShaderLanguage == GLSL) {
		#include "Lighting_GLSL.h"

		// Return the requested source code
		if (nSourceCodeType == FragmentShaderBody)
			return sSourceCode_Fragment;
	} else if (sShaderLanguage == Cg) {
		#include "Lighting_Cg.h"

		// Return the requested source code
		if (nSourceCodeType == FragmentShaderBody)
			return sSourceCode_Fragment;
	}

	// Error!
	return "";
}

void ShaderFunctionShadingLighting::SetProgram(Program &cProgram, const PLScene::SQCull &cCullQuery, const PLScene::VisNode &cVisNode, SRPVolume &cSRPVolume)
{
	// Get simplified GPU program wrapper interface
	ProgramWrapper &cProgramWrapper = static_cast<ProgramWrapper&>(cProgram);

	// Get the volume scene node
	PLVolume::SNVolume *pSNVolume = static_cast<PLVolume::SNVolume*>(cVisNode.GetSceneNode());
	if (!pSNVolume)
		return;	// Early escape, there's no sense in continuing

	// Light
	Vector3	vLightDirection = Vector3::UnitZ;	// Normalized view space light direction vector (any default value)
	Color3	cLightColor		= Color3::Black;	// Light color
	{ // Get a list of all visible scene node instances intersecting with the given scene node
		const Class *pClass = ClassManager::GetInstance()->GetClass("PLScene::SNDirectionalLight");
		if (pClass) {
			Array<const VisNode*> lstIntersecting;
			cSRPVolume.GetIntersectingInstancesOf(cCullQuery, *pSNVolume, *pClass, lstIntersecting);
			if (lstIntersecting.GetNumOfElements()) {
				// Get the view space light direction vector and the light color
				const VisNode &cLightVisNode = *lstIntersecting[0];
				vLightDirection = cLightVisNode.GetWorldViewMatrix().RotateVector(Vector3::UnitZ);	// Vector from surface point to light source
				cLightColor     = static_cast<SNLight*>(cLightVisNode.GetSceneNode())->Color.Get();
			}
		}
	}

	// Get the view space to object space matrix
	Matrix4x4 mViewSpaceToObjectSpace = cVisNode.GetWorldViewMatrix();
	mViewSpaceToObjectSpace.Invert();

	{ // Set viewing direction
		// Bring the normalized view space viewing direction vector into scene node space
		Vector3 vObjectSpaceViewingDirection = mViewSpaceToObjectSpace.RotateVector(Vector3::NegativeUnitZ);	// Vector from surface point to camera (in view space, the camera looks along the negative z-axis)
		vObjectSpaceViewingDirection.Normalize();

		// Set the normalized viewing direction in scene node space
		cProgramWrapper.Set("ViewingDirection", vObjectSpaceViewingDirection);
	}

	{ // Set light direction
		// Bring the normalized view space light direction vector into scene node space
		Vector3 vObjectSpaceLightDirection = mViewSpaceToObjectSpace.RotateVector(vLightDirection);
		vObjectSpaceLightDirection.Normalize();

		// Set the normalized light direction in scene node space
		cProgramWrapper.Set("LightDirection", vObjectSpaceLightDirection);
	}

	// Set ambient color
	cProgramWrapper.Set("AmbientColor", cSRPVolume.AmbientColor.Get());
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // PLVolumeRenderer