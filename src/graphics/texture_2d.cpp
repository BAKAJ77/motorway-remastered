#include <graphics/texture_2d.h>
#include <glad/glad.h>

Texture2D::Texture2D(const void* pixels, const glm::ivec2& size, uint32_t pixelDataType, uint32_t internalFormat, uint32_t format) :
	TextureBuffer(GL_TEXTURE_2D, size)
{
	// Generate the texture buffer and fill it with the pixel data given
	glGenTextures(1, &m_id);
	glBindTexture(m_target, m_id);
	glTexImage2D(m_target, 0, internalFormat, size.x, size.y, 0, format, pixelDataType, pixels);

	// Set the default texture filter and wrap modes
	this->SetFilter(GL_LINEAR, GL_LINEAR);
	this->SetWrap(GL_REPEAT, GL_REPEAT);

	glBindTexture(m_target, 0); // Unbind the texture buffer
}

Texture2D::Texture2D(Texture2D&& temp) noexcept
{
	m_id = temp.m_id;
	m_target = temp.m_target;
	m_size = temp.m_size;

	temp.m_id = 0;
}

Texture2D& Texture2D::operator=(Texture2D&& temp) noexcept
{
	m_id = temp.m_id;
	m_target = temp.m_target;
	m_size = temp.m_size;

	temp.m_id = 0;
	return *this;
}

void Texture2D::ModifyData(const void* pixels, const glm::ivec2& offset, const glm::ivec2& size, uint32_t pixelDataType, uint32_t format)
{
	glBindTexture(m_target, m_id);
	glTexSubImage2D(m_target, 0, offset.x, offset.y, size.x, size.y, format, pixelDataType, pixels);
	glBindTexture(m_target, 0);
}
