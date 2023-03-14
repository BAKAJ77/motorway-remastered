#ifndef CAMERA_3D_H
#define CAMERA_3D_H

#include <graphics/camera_base.h>

class Camera3D : public CameraBase
{
protected:
	glm::vec3 m_position, m_direction;
	float m_fov;
public:
	Camera3D();
	Camera3D(const glm::vec3& pos, const glm::vec2& size, const glm::vec3& dir, float fov = 45.0f);

	virtual ~Camera3D() = default;

	// Sets the position of the camera.
	void SetPosition(const glm::vec3& pos);

	// Sets the direction the camera is facing.
	virtual void SetDirection(const glm::vec3& dir);

	// Sets the FOV angle of the camera.
	// The angle you specify should be in degrees.
	void SetFOV(float degrees);

	// Returns the computed camera view matrix.
	glm::mat4 ComputeViewMatrix() const override;

	// Returns the computed camera projection matrix.
	glm::mat4 ComputeProjectionMatrix() const override;

	// Returns the position of the camera.
	const glm::vec3& GetPosition() const;

	// Returns the direction the camera is facing.
	const glm::vec3& GetDirection() const;

	// Returns the FOV angle of the camera.
	// The angle returned is in degrees.
	float GetFOV() const;
};

#endif