#ifndef CAMERA_3D_H
#define CAMERA_3D_H

#include <graphics/camera_base.h>

class Camera3D : public CameraBase
{
private:
	glm::vec3 m_position, m_frontDir;
	glm::vec2 m_prevCursorPosition, m_eulerAngles;
	float m_fov, m_sensitivity;
	bool m_cursorSynced = false;
public:
	Camera3D();
	Camera3D(const glm::vec3& pos, const glm::vec2& size, float fov = 45.0f, float sensitivity = 0.14f);

	~Camera3D() = default;

	// Sets the position of the camera.
	void SetPosition(const glm::vec3& pos);

	// Sets the sensitivity of the camera.
	void SetSensitivity(float value);

	// Sets the FOV angle of the camera.
	// The angle you specify should be in degrees.
	void SetFOV(float degrees);

	// Updates the direction the camera is facing based on the movement of the cursor.
	void Update();

	// Returns the computed camera view matrix.
	glm::mat4 ComputeViewMatrix() const override;

	// Returns the computed camera projection matrix.
	glm::mat4 ComputeProjectionMatrix() const override;

	// Returns the position of the camera.
	const glm::vec3& GetPosition() const;

	// Returns the direction the camera is facing.
	const glm::vec3& GetDirection() const;

	// Returns the sensitivity of the camera.
	float GetSensitivity() const;

	// Returns the FOV angle of the camera.
	// The angle returned is in degrees.
	float GetFOV() const;
};

#endif
