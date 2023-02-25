#include <graphics/texture_buffer.h>
#include <glad/glad.h>

TextureBuffer::TextureBuffer() :
    m_id(0), m_target(0), m_size(glm::ivec2(0))
{}

TextureBuffer::TextureBuffer(uint32_t target, const glm::ivec2& size) :
    m_id(0), m_target(target), m_size(size)
{}

TextureBuffer::~TextureBuffer()
{
    glDeleteTextures(1, &m_id);
}

void TextureBuffer::SetFilter(uint32_t min, uint32_t mag) const
{
    glBindTexture(m_target, m_id);
    glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, min);
    glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, mag);
    glBindTexture(m_target, 0);
}

void TextureBuffer::SetWrap(uint32_t sAxis, uint32_t tAxis) const
{
    glBindTexture(m_target, m_id);
    glTexParameteri(m_target, GL_TEXTURE_WRAP_S, sAxis);
    glTexParameteri(m_target, GL_TEXTURE_WRAP_T, tAxis);
    glBindTexture(m_target, 0);
}

void TextureBuffer::Bind() const
{
    glBindTexture(m_target, m_id);
}

void TextureBuffer::Bind(int textureUnit) const
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(m_target, m_id);
}

void TextureBuffer::Unbind() const
{
    glBindTexture(m_target, 0);
}

const uint32_t& TextureBuffer::GetID() const
{
    return m_id;
}

const uint32_t& TextureBuffer::GetTarget() const
{
    return m_target;
}

const glm::ivec2& TextureBuffer::GetSize() const
{
    return m_size;
}
