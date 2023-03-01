#include <graphics/camera_3d.h>
#include <core/input_system.h>

#include <glm/gtc/matrix_transform.hpp>

Camera3D::Camera3D() :
    m_position(glm::vec3(0.0f)), m_frontDir(glm::vec3(0.0f)), m_prevCursorPosition(glm::vec2(0.0f)), m_eulerAngles(glm::vec2(0.0f)), 
    m_sensitivity(0.0f), m_fov(0.0f), m_cursorSynced(false)
{}

Camera3D::Camera3D(const glm::vec3& pos, const glm::vec2& size, float fov, float sensitivity) :
    m_position(pos), CameraBase(size), m_frontDir({ 0.0f, 0.0f, -1.0f }), m_prevCursorPosition(glm::vec2(0.0f)),
    m_sensitivity(sensitivity), m_fov(glm::radians(fov)), m_cursorSynced(false)
{
    m_eulerAngles = { m_frontDir.z * 90.0f, 0.0f };
}

void Camera3D::SetPosition(const glm::vec3& pos)
{
    m_position = pos;
}

void Camera3D::SetSensitivity(float value)
{
    m_sensitivity = value;
}

void Camera3D::SetFOV(float degrees)
{
    m_fov = glm::radians(degrees);
}

void Camera3D::Update()
{
    const glm::vec2 currentCursorPosition = InputSystem::GetInstance().GetCursorPosition();
    
    if (!m_cursorSynced)
    {
        m_prevCursorPosition.x = currentCursorPosition.x;
        m_prevCursorPosition.y = currentCursorPosition.y;
        m_cursorSynced = true;
    }

    const glm::vec2 cursorOffset = { currentCursorPosition.x - m_prevCursorPosition.x, m_prevCursorPosition.y - currentCursorPosition.y };
    m_prevCursorPosition = currentCursorPosition;

    m_eulerAngles.x += (cursorOffset.x * m_sensitivity); // Yaw
    m_eulerAngles.y += (cursorOffset.y * m_sensitivity); // Pitch

    if (m_eulerAngles.y > 89.0) // Constrain pitch values
        m_eulerAngles.y = 89.0;
    else if (m_eulerAngles.y < -89.0)
        m_eulerAngles.y = -89.0;

    // Calculate new camera front direction vector
    glm::vec3 newFrontDir = glm::vec3(0.0f);
    newFrontDir.x = (float)(glm::cos(glm::radians(m_eulerAngles.y)) * glm::cos(glm::radians(m_eulerAngles.x)));
    newFrontDir.y = (float)glm::sin(glm::radians(m_eulerAngles.y));
    newFrontDir.z = (float)(glm::cos(glm::radians(m_eulerAngles.y)) * glm::sin(glm::radians(m_eulerAngles.x)));

    m_frontDir = glm::normalize(newFrontDir);
}

glm::mat4 Camera3D::GetViewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_frontDir, { 0.0f, 1.0f, 0.0f });
}

glm::mat4 Camera3D::GetProjectionMatrix() const
{
    return glm::perspective(m_fov, m_size.x / m_size.y, 0.1f, 1000.0f);
}

const glm::vec3& Camera3D::GetPosition() const
{
    return m_position;
}

const glm::vec3& Camera3D::GetDirection() const
{
    return m_frontDir;
}

const float& Camera3D::GetSensitivity() const
{
    return m_sensitivity;
}

float Camera3D::GetFOV() const
{
    return glm::degrees(m_fov);
}
