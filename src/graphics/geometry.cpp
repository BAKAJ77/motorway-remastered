#include <graphics/geometry.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <array>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Geometry::Geometry() :
    m_count(0), m_renderFunc(RenderFunction::RENDER_ARRAYS), m_primitiveType(PrimitiveType::TRIANGLES)
{}

Geometry::Geometry(const Transform& transform, const Material& material) :
    m_transformData(transform), m_materialData(material), m_count(0), m_renderFunc(RenderFunction::RENDER_ARRAYS),
    m_primitiveType(PrimitiveType::TRIANGLES)
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

Geometry::RenderFunction Geometry::GetRenderFunction() const
{
    return m_renderFunc;
}

Geometry::PrimitiveType Geometry::GetPrimitiveType() const
{
    return m_primitiveType;
}

uint32_t Geometry::GetCount() const
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
    // Attempt to fetch the geometry's buffer objects from the asset system
    if (AssetSystem::GeometryData* geometryData = AssetSystem::GetInstance().GetGeometryBuffers("Square"))
    {
        // It has been found so simply attach them to the VAO
        m_vertexArray.AttachBuffers(*geometryData->m_vertexBuffer, geometryData->m_indexBuffer.get());
    }
    else
    {
        // Define the vertex and index data
        std::array<float, 20> vertices =
        {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
        };

        std::array<uint32_t, 6> indices = { 0, 1, 2, 0, 2, 3 };

        // Setup the vbo, ibo and vao
        VertexBufferPtr vertexBuffer = AssetSystem::CreateVertexBuffer(vertices.data(), sizeof(vertices), GL_STATIC_DRAW);
        vertexBuffer->PushLayout(0, GL_FLOAT, 3, 5 * sizeof(float));
        vertexBuffer->PushLayout(1, GL_FLOAT, 2, 5 * sizeof(float), 3 * sizeof(float));

        IndexBufferPtr indexBuffer = AssetSystem::CreateIndexBuffer(indices.data(), sizeof(indices), GL_STATIC_DRAW);
        m_vertexArray.AttachBuffers(*vertexBuffer, indexBuffer.get());

        // Store the buffer objects in the asset system
        AssetSystem::GetInstance().StoreGeometryBuffers("Square", vertexBuffer, indexBuffer);
    }

    // Assign the rendering parameters
    m_renderFunc = RenderFunction::RENDER_ELEMENTS;
    m_primitiveType = PrimitiveType::TRIANGLES;
    m_count = 6;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Triangle::Triangle()
{
    this->InitGeometryData();
}

Triangle::Triangle(const Transform& transform, const Material& material) :
    Geometry(transform, material)
{
    this->InitGeometryData();
}

void Triangle::InitGeometryData()
{
    // Attempt to fetch the geometry's buffer objects from the asset system
    if (AssetSystem::GeometryData* geometryData = AssetSystem::GetInstance().GetGeometryBuffers("Triangle"))
    {
        // It has been found so simply attach them to the VAO
        m_vertexArray.AttachBuffers(*geometryData->m_vertexBuffer, geometryData->m_indexBuffer.get());
    }
    else
    {
        // Define the vertex data
        std::array<float, 15> vertices =
        {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.0f,  0.5f, 0.0f, 0.5f, 1.0f
        };

        // Setup the vbo and vao
        VertexBufferPtr vertexBuffer = AssetSystem::CreateVertexBuffer(vertices.data(), sizeof(vertices), GL_STATIC_DRAW);
        vertexBuffer->PushLayout(0, GL_FLOAT, 3, 5 * sizeof(float));
        vertexBuffer->PushLayout(1, GL_FLOAT, 2, 5 * sizeof(float), 3 * sizeof(float));

        m_vertexArray.AttachBuffers(*vertexBuffer);

        // Store the buffer objects in the asset system
        AssetSystem::GetInstance().StoreGeometryBuffers("Triangle", vertexBuffer);
    }

    // Assign the rendering parameters
    m_renderFunc = RenderFunction::RENDER_ARRAYS;
    m_primitiveType = PrimitiveType::TRIANGLES;
    m_count = 3;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Circle::Circle()
{
    this->InitGeometryData();
}

Circle::Circle(const Transform& transform, const Material& material) :
    Geometry(transform, material)
{
    this->InitGeometryData();
}

void Circle::InitGeometryData()
{
    // Attempt to fetch the geometry's buffer objects from the asset system
    if (AssetSystem::GeometryData* geometryData = AssetSystem::GetInstance().GetGeometryBuffers("Circle"))
    {
        // It has been found so simply attach them to the VAO
        m_vertexArray.AttachBuffers(*geometryData->m_vertexBuffer, geometryData->m_indexBuffer.get());
    }
    else
    {
        // Calculate the vertex and index data
        std::vector<float> vertices({ 0.0f, 0.0f, 0.0f, 0.5f, 0.5f });
        constexpr float angleDecrementStep = 10.0f;

        for (float angle = 360; angle >= 0; angle -= angleDecrementStep)
        {
            const glm::vec3 vertexCoord = { glm::sin(glm::radians(angle)) / 2.0f, glm::cos(glm::radians(angle)) / 2.0f, 0.0f };
            const glm::vec2 uvCoord = { vertexCoord.x + 0.5f, vertexCoord.y + 0.5f };

            vertices.emplace_back(vertexCoord.x);
            vertices.emplace_back(vertexCoord.y);
            vertices.emplace_back(vertexCoord.z);
            vertices.emplace_back(uvCoord.x);
            vertices.emplace_back(uvCoord.y);
        }

        // Setup the vbo and vao
        VertexBufferPtr vertexBuffer = AssetSystem::CreateVertexBuffer(vertices.data(), vertices.size() * sizeof(float), GL_STATIC_DRAW);
        vertexBuffer->PushLayout(0, GL_FLOAT, 3, 5 * sizeof(float));
        vertexBuffer->PushLayout(1, GL_FLOAT, 2, 5 * sizeof(float), 3 * sizeof(float));

        m_vertexArray.AttachBuffers(*vertexBuffer);

        // Store the buffer objects in the asset system
        AssetSystem::GetInstance().StoreGeometryBuffers("Circle", vertexBuffer);
    }

    // Assign the rendering parameters
    m_renderFunc = RenderFunction::RENDER_ARRAYS;
    m_primitiveType = PrimitiveType::TRIANGLE_FAN;
    m_count = 38;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TODO: Optimise vertex data storage and loading