#include <graphics/camera_3d.h>
#include <glm/gtc/matrix_transform.hpp>

Camera3D::Camera3D() :
    m_position(glm::vec3(0.0f, 3.0f, 3.0f)), m_direction(glm::vec3(0.0f, -0.7071067811865475f, -0.7071067811865475f)), 
    m_fov(glm::radians(45.0f))
{}

Camera3D::Camera3D(const glm::vec3& pos, const glm::vec2& size, const glm::vec3& dir, float fov) :
    m_position(pos), CameraBase(size), m_direction(dir), m_fov(glm::radians(fov))
{}

void Camera3D::SetPosition(const glm::vec3& pos)
{
    m_position = pos;
}

void Camera3D::SetDirection(const glm::vec3& dir)
{
    m_direction = dir;
}

void Camera3D::SetFOV(float degrees)
{
    m_fov = glm::radians(degrees);
}

glm::mat4 Camera3D::ComputeViewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_direction, { 0.0f, 1.0f, 0.0f });
}

glm::mat4 Camera3D::ComputeProjectionMatrix() const
{
    return glm::perspective(m_fov, m_size.x / m_size.y, 0.1f, 1000.0f);
}

const glm::vec3& Camera3D::GetPosition() const
{
    return m_position;
}

const glm::vec3& Camera3D::GetDirection() const
{
    return m_direction;
}

float Camera3D::GetFOV() const
{
    return glm::degrees(m_fov);
}