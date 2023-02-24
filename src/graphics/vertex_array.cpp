#include <graphics/vertex_array.h>
#include <glad/glad.h>

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_id);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_id);
}

void VertexArray::AttachBuffers(const VertexBuffer& vbo, const IndexBuffer* ibo)
{
    // Bind the VAO first then bind the buffer objects
    // This attaches the buffer objects to the VAO
    glBindVertexArray(m_id);
    vbo.Bind();

    if (ibo)
        ibo->Bind();

    // Configure the vertex buffer's layout attributes
    for (const VertexBuffer::Layout& vertexLayout : vbo.GetVertexLayouts())
    {
        glEnableVertexAttribArray(vertexLayout.m_index);
        glVertexAttribPointer(vertexLayout.m_index, vertexLayout.m_size, vertexLayout.m_type, vertexLayout.m_normalize, 
            (GLsizei)vertexLayout.m_strideBytes, (void*)vertexLayout.m_offsetBytes);

        glVertexAttribDivisor(vertexLayout.m_index, vertexLayout.m_divisor);
    }

    // Unbind the VAO and buffer objects
    glBindVertexArray(0);
    vbo.Unbind();

    if (ibo)
        ibo->Unbind();
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_id);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}

const uint32_t& VertexArray::GetID() const
{
    return m_id;
}
