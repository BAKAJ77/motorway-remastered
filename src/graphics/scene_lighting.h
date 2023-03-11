#ifndef SCENE_LIGHTING_H
#define SCENE_LIGHTING_H

#include <glm/glm.hpp>

struct DirectionalLight
{
	glm::vec3 m_direction = { -1.0f, -1.0f, 1.0f };
	glm::vec3 m_ambientIntensity = glm::vec3(0.1f), m_diffuseIntensity = glm::vec3(1.0f), m_specularIntensity = glm::vec3(1.0f);
};

struct SceneLighting
{
	DirectionalLight m_globalLight;
};

#endif
