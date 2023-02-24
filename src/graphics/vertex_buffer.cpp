#include <graphics/vertex_buffer.h>
#include <glad/glad.h>

VertexBuffer::VertexBuffer() :
    m_id(0)
{}

VertexBuffer::VertexBuffer(const void* data, size_t size, uint32_t usage)
{
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBuffer::VertexBuffer(VertexBuffer&& temp) noexcept :
    m_id(temp.m_id)
{
    temp.m_id = 0;
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_id);
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& temp) noexcept
{
    m_id = temp.m_id;
    temp.m_id = 0;

    return *this;
}

void VertexBuffer::PushLayout(uint32_t index, uint32_t type, uint32_t size, size_t strideBytes, size_t offsetBytes, uint32_t divisor,
    bool normalize)
{
    m_vertexLayouts.push_back({ index, type, size, divisor, strideBytes, offsetBytes, normalize });
}

void VertexBuffer::ModifyData(const void* data, size_t offset, size_t size)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

const std::vector<VertexBuffer::Layout>& VertexBuffer::GetVertexLayouts() const
{
    return m_vertexLayouts;
}

const uint32_t& VertexBuffer::GetID() const
{
    return m_id;
}