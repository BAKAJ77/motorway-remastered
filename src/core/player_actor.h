#ifndef PLAYER_ACTOR_H
#define PLAYER_ACTOR_H

#include <core/input_system.h>
#include <graphics/camera_3d.h>

class PlayerActor : public Camera3D
{
public:
	struct KeyBindings
	{
		InputSystem::KeyCode m_forwardKey = InputSystem::KeyCode::KEY_W, m_backwardKey = InputSystem::KeyCode::KEY_S,
			m_leftKey = InputSystem::KeyCode::KEY_A, m_rightKey = InputSystem::KeyCode::KEY_D,
			m_upKey = InputSystem::KeyCode::KEY_SPACE, m_downKey = InputSystem::KeyCode::KEY_LEFT_SHIFT;
	};
private:
	KeyBindings m_keyBindings;
	glm::vec2 m_prevCursorPosition;
	float m_pitch, m_yaw, m_speed, m_sensitivity;
	bool m_cursorSynced, m_inputEnabled;

	// Computes the euler angles of the current camera direction vector.
	void ComputeEulerAngles();

	// Updates the position of the player actor based on keyboard input.
	void UpdatePosition(float deltaTime);

	// Updates the direction the player actor is facing based on mouse movement.
	void UpdateDirection();
public:
	PlayerActor();
	PlayerActor(const glm::vec3& pos, const glm::vec2& size, const glm::vec3& dir, bool inputEnabled = false, float speed = 5.0f, 
		float sensitivity = 0.14f, float fov = 45.0f);

	~PlayerActor() = default;

	// Sets whether keyboard and mouse input is enabled for the player actor.
	void SetInputState(bool enable);

	// Sets the key bindings for player movement.
	void SetKeyBindings(KeyBindings movementKeys);

	// Sets the direction which the player actor is facing.
	void SetDirection(const glm::vec3& dir) override;

	// Sets the speed of the player.
	void SetSpeed(float value);

	// Sets the mouse sensitivity for looking around with the player camera.
	void SetSensitivity(float value);

	// Updates the position of the player when a movement key is pressed.
	// The direction the player's camera is facing is also updated if the mouse is moved.
	void Update(float deltaTime);

	// Returns TRUE if keyboard and mouse input is enabled for the player actor.
	bool GetInputState() const;

	// Returns the key bindings for player movement.
	const KeyBindings& GetKeyBindings() const;

	// Returns the mouse sensitivity for looking around with the player camera.
	float GetSensitivity() const;
};

#endif
