#include "Lighting.h"

#include "RayTrace.h"

// Calculate the ambient factor.
glm::vec3 ambient(FragmentShadingParameters params) {
	return params.sceneAmbient * params.material.color;
}

// Calculate the diffuse factor.
glm::vec3 diffuse(FragmentShadingParameters params) {
	float tmpDiffuse = std::max(0.0f, dot_normalized(params.pointNormal, params.lightPosition-params.point));
	return params.sceneDiffuse * tmpDiffuse * params.material.color;
}

// Calculate the specular factor.
float specular(FragmentShadingParameters params) {
	glm::vec3 viewDirection = params.point - params.rayOrigin;
	return std::pow(dot_normalized(params.lightPosition-params.point-viewDirection, params.pointNormal), params.material.specularStrength);
}

// Put it all together into a phone shaded equation
glm::vec3 phongShading(FragmentShadingParameters params) {
	if(params.inShadow) {
		return ambient(params);
	}

	if(params.material.specularStrength <= 0.0 && params.material.reflectionStrength <= 0.0) {
		return ambient(params) + diffuse(params);
	}

	if (params.material.reflectionStrength <= 0) {
		if(params.material.specularStrength > 0.0) {
			return (0.7f * (ambient(params) + diffuse(params))) + 0.3f * specular(params);
		} else {
			return ambient(params) + diffuse(params) + specular(params);
		}
	} else {
		glm::vec3 color;
		if(params.material.specularStrength > 0.0) {
			color = ambient(params) + diffuse(params) + specular(params);
		} else {
			color = ambient(params) + diffuse(params);
		}
		float r = params.material.reflectionStrength;
		return (1-r) * color + r * params.reflectedColor;
	}


}

