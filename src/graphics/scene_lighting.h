#ifndef SCENE_LIGHTING_H
#define SCENE_LIGHTING_H

#include <glm/glm.hpp>

struct LightSource
{
	glm::vec3 m_ambientIntensity = glm::vec3(0.1f), m_diffuseIntensity = glm::vec3(1.0f), m_specularIntensity = glm::vec3(1.0f);
	bool m_enabled = false;
};

struct DirectionalLight : public LightSource
{
	glm::vec3 m_direction = { -1.0f, -1.0f, 1.0f };
};

struct PointLight : public LightSource
{
	glm::vec3 m_position = glm::vec3(0.0f);
	float m_constantFactor = 1.0f, m_linearFactor = 0.14f, m_quadraticFactor = 0.07f;
};

struct SpotLight : public PointLight
{
	glm::vec3 m_direction = { 0.0f, -1.0f, 0.0f };
	float m_innerCutOffAngle = 12.5f, m_outerCutOffAngle = 17.5f;
};

struct SceneLighting
{
	DirectionalLight m_globalLight;
	PointLight m_pointLight;
	SpotLight m_spotLight;
};

#endif
