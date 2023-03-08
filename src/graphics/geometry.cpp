#include <graphics/geometry.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <array>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Geometry::Geometry() :
    m_count(0), m_renderFunc(RenderFunction::RENDER_ARRAYS)
{}

Geometry::Geometry(const Transform& transform, const Material& material) :
    m_transformData(transform), m_materialData(material), m_count(0), m_renderFunc(RenderFunction::RENDER_ARRAYS)
{}

void Geometry::SetPosition(const glm::vec3& pos)
{
    m_transformData.m_position = pos;
}

void Geometry::SetSize(const glm::vec3& size)
{
    m_transformData.m_size = size;
}

void Geometry::SetRotation(const glm::vec3& axis, float angle)
{
    m_transformData.m_rotationAxis = axis;
    m_transformData.m_rotationAngle = angle;
}

void Geometry::SetDiffuse(const glm::vec4& color)
{
    m_materialData.m_diffuseColor = color;
}

void Geometry::SetDiffuse(Texture2DPtr texture)
{
    m_materialData.m_diffuseTexture = texture;
}

void Geometry::SetTexturesUsage(bool enable)
{
    m_materialData.m_enableTextures = enable;
}

glm::mat4 Geometry::ComputeModelMatrix() const
{
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::scale(modelMatrix, m_transformData.m_size);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(m_transformData.m_rotationAngle), m_transformData.m_rotationAxis);
    modelMatrix = glm::translate(modelMatrix, m_transformData.m_position);

    return modelMatrix;
}

const VertexArray& Geometry::GetVertexArray() const
{
    return m_vertexArray;
}

const Geometry::Transform& Geometry::GetTransformData() const
{
    return m_transformData;
}

const Geometry::Material& Geometry::GetMaterialData() const
{
    return m_materialData;
}

const Geometry::RenderFunction& Geometry::GetRenderFunction() const
{
    return m_renderFunc;
}

const uint32_t& Geometry::GetCount() const
{
    return m_count;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Square::Square()
{
    this->InitGeometryData();
}

Square::Square(const Transform& transform, const Material& material) :
    Geometry(transform, material)
{
    this->InitGeometryData();
}

void Square::InitGeometryData()
{
    // Setup the buffer objects
    std::array<float, 20> vertices =
    {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
    };

    std::array<uint32_t, 6> indices = { 0, 1, 2, 0, 2, 3 };

    m_vertexBuffer = VertexBuffer(vertices.data(), sizeof(vertices), GL_STATIC_DRAW);
    m_vertexBuffer.PushLayout(0, GL_FLOAT, 3, 5 * sizeof(float));
    m_vertexBuffer.PushLayout(1, GL_FLOAT, 2, 5 * sizeof(float), 3 * sizeof(float));

    m_indexBuffer = IndexBuffer(indices.data(), sizeof(indices), GL_STATIC_DRAW);
    m_vertexArray.AttachBuffers(m_vertexBuffer, &m_indexBuffer);

    // Specify the rendering parameters
    m_renderFunc = RenderFunction::RENDER_ELEMENTS;
    m_count = (uint32_t)indices.size();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////