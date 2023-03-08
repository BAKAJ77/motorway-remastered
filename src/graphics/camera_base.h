#ifndef CAMERA_BASE_H
#define CAMERA_BASE_H

#include <glm/glm.hpp>

class CameraBase
{
protected:
	glm::vec2 m_size;
public:
	CameraBase();
	CameraBase(const glm::vec2& size);

	virtual ~CameraBase() = default;

	// Sets the dimension size of the camera.
	void SetSize(const glm::vec2& size);

	virtual glm::mat4 ComputeViewMatrix() const = 0;
	virtual glm::mat4 ComputeProjectionMatrix() const = 0;

	// Returns the dimension size of the camera.
	const glm::vec2& GetSize() const;
};

#endif
