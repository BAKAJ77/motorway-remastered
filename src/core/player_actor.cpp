#include <core/player_actor.h>

PlayerActor::PlayerActor() :
    m_prevCursorPosition(glm::vec2(0.0f)), m_speed(1.0f), m_sensitivity(0.14f), m_cursorSynced(false), m_inputEnabled(false)
{
    this->ComputeEulerAngles();
}

PlayerActor::PlayerActor(const glm::vec3& pos, const glm::vec2& size, const glm::vec3& dir, bool inputEnabled, float speed,
    float sensitivity, float fov) :
    Camera3D(pos, size, dir, fov), m_speed(speed), m_sensitivity(sensitivity), m_inputEnabled(inputEnabled), 
    m_prevCursorPosition(glm::vec2(0.0f)), m_cursorSynced(false)
{
    this->ComputeEulerAngles();
}

void PlayerActor::ComputeEulerAngles()
{
    // Calculate the pitch angle
    glm::vec3 direction = m_direction;
    m_pitch = glm::degrees(glm::asin(glm::dot(direction, { 0.0f, 1.0f, 0.0f })));

    // Calculate the yaw angle
    direction.y = 0.0f;
    direction = glm::normalize(direction);
    m_yaw = -glm::degrees(glm::acos(glm::dot(direction, { 1.0f, 0.0f, 0.0f })));
    if (dot(direction, { 0.0f, 0.0f, 1.0f }) > 0)
        m_yaw = -360.0f - m_yaw;
}

void PlayerActor::UpdatePosition(float deltaTime)
{
    const glm::vec3 perpendicularDirection = glm::cross(m_direction, { 0.0f, 1.0f, 0.0f });

    if (InputSystem::GetInstance().WasKeyPressed(InputSystem::KeyCode::KEY_W))
        m_position += glm::vec3(m_direction.x, 0.0f, m_direction.z) * m_speed * deltaTime;
    else if (InputSystem::GetInstance().WasKeyPressed(InputSystem::KeyCode::KEY_S))
        m_position -= glm::vec3(m_direction.x, 0.0f, m_direction.z) * m_speed * deltaTime;

    if (InputSystem::GetInstance().WasKeyPressed(InputSystem::KeyCode::KEY_A))
        m_position -= perpendicularDirection * m_speed * deltaTime;
    else if (InputSystem::GetInstance().WasKeyPressed(InputSystem::KeyCode::KEY_D))
        m_position += perpendicularDirection * m_speed * deltaTime;

    if (InputSystem::GetInstance().WasKeyPressed(InputSystem::KeyCode::KEY_SPACE))
        m_position += glm::vec3(0.0f, 1.0f, 0.0f) * m_speed * deltaTime;
    else if (InputSystem::GetInstance().WasKeyPressed(InputSystem::KeyCode::KEY_LEFT_SHIFT))
        m_position -= glm::vec3(0.0f, 1.0f, 0.0f) * m_speed * deltaTime;
}

void PlayerActor::UpdateDirection()
{
    const glm::vec2 currentCursorPosition = InputSystem::GetInstance().GetCursorPosition(); // Get the current cursor position

    // Make sure the cursor is synced, this is to prevent an initial camera rotation spike
    if (!m_cursorSynced)
    {
        m_prevCursorPosition.x = currentCursorPosition.x;
        m_prevCursorPosition.y = currentCursorPosition.y;
        m_cursorSynced = true;
    }

    // Calculate the cursor offset
    const glm::vec2 cursorOffset = { currentCursorPosition.x - m_prevCursorPosition.x,
        m_prevCursorPosition.y - currentCursorPosition.y };

    m_prevCursorPosition = currentCursorPosition;

    // Update the yaw and pitch angle values using the calculated cursor offset values
    // The glm::mod function prevents the yaw value from getting too large
    // Large values can result in floating precision being lost
    m_yaw = glm::mod(m_yaw + (cursorOffset.x * m_sensitivity), -360.0f);
    m_pitch += cursorOffset.y * m_sensitivity;

    // Constrain the pitch values to prevent camera flipping
    if (m_pitch > 89.0f)
        m_pitch = 89.0;
    else if (m_pitch < -89.0)
        m_pitch = -89.0;

    // Calculate new camera direction vector
    glm::vec3 newFrontDir = glm::vec3(0.0f);
    newFrontDir.x = (float)(glm::cos(glm::radians(m_pitch)) * glm::cos(glm::radians(m_yaw)));
    newFrontDir.y = (float)glm::sin(glm::radians(m_pitch));
    newFrontDir.z = (float)(glm::cos(glm::radians(m_pitch)) * glm::sin(glm::radians(m_yaw)));

    m_direction = glm::normalize(newFrontDir);
}

void PlayerActor::SetInputState(bool enable)
{
    m_inputEnabled = enable;
}

void PlayerActor::SetKeyBindings(KeyBindings movementKeys)
{
    m_keyBindings = movementKeys;
}

void PlayerActor::SetDirection(const glm::vec3& dir)
{
    m_direction = dir;
    this->ComputeEulerAngles();
}

void PlayerActor::SetSpeed(float value)
{
    m_speed = value;
}

void PlayerActor::SetSensitivity(float value)
{
    m_sensitivity = value;
}

void PlayerActor::Update(float deltaTime)
{
    if (m_inputEnabled)
    {
        this->UpdatePosition(deltaTime);
        this->UpdateDirection();
    }
}

bool PlayerActor::GetInputState() const
{
    return m_inputEnabled;
}

const PlayerActor::KeyBindings& PlayerActor::GetKeyBindings() const
{
    return m_keyBindings;
}

float PlayerActor::GetSensitivity() const
{
    return m_sensitivity;
}
