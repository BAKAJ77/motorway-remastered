#include <graphics/camera_base.h>

CameraBase::CameraBase() :
	m_size(glm::vec2(0.0f))
{}

CameraBase::CameraBase(const glm::vec2& size) :
	m_size(size)
{}

void CameraBase::SetSize(const glm::vec2& size)
{
	m_size = size;
}

const glm::vec2& CameraBase::GetSize() const
{
	return m_size;
}
