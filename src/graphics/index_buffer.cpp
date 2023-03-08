#include <graphics/index_buffer.h>
#include <glad/glad.h>

IndexBuffer::IndexBuffer() :
    m_id(0)
{}

IndexBuffer::IndexBuffer(const void* data, size_t size, uint32_t usage)
{
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexBuffer::IndexBuffer(IndexBuffer&& temp) noexcept :
    m_id(temp.m_id)
{
    temp.m_id = 0;
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_id);
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& temp) noexcept
{
    m_id = temp.m_id;
    temp.m_id = 0;

    return *this;
}

void IndexBuffer::ModifyData(const void* data, size_t offset, size_t size)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint32_t IndexBuffer::GetID() const
{
    return m_id;
}
