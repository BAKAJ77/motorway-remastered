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

void Geometry::SetAmbient(const glm::vec3& color)
{
    m_materialData.m_ambientColor = color;
}

void Geometry::SetDiffuse(const glm::vec3& color)
{
    m_materialData.m_diffuseColor = color;
}

void Geometry::SetDiffuse(Texture2DPtr texture)
{
    m_materialData.m_diffuseTexture = texture;
}

void Geometry::SetSpecular(const glm::vec3& color)
{
    m_materialData.m_specularColor = color;
}

void Geometry::SetSpecular(Texture2DPtr texture)
{
    m_materialData.m_specularTexture = texture;
}

void Geometry::SetEmission(const glm::vec3& color)
{
    m_materialData.m_emissionColor = color;
}

void Geometry::SetEmission(Texture2DPtr texture)
{
    m_materialData.m_emissionTexture = texture;
}

void Geometry::SetShininess(float value)
{
    m_materialData.m_shininess = value;
}

void Geometry::SetOpacity(float value)
{
    m_materialData.m_opacity = value;
}

void Geometry::SetTexturesUsage(bool enable)
{
    m_materialData.m_enableTextures = enable;
}

glm::mat4 Geometry::ComputeModelMatrix() const
{
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, m_transformData.m_position);
    modelMatrix = glm::scale(modelMatrix, m_transformData.m_size);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(m_transformData.m_rotationAngle), m_transformData.m_rotationAxis);

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
        std::array<float, 32> vertices =
        {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
        };

        std::array<uint32_t, 6> indices = { 0, 1, 2, 0, 2, 3 };

        // Setup the vbo, ibo and vao
        VertexBufferPtr vertexBuffer = AssetSystem::CreateVertexBuffer(vertices.data(), sizeof(vertices), GL_STATIC_DRAW);
        vertexBuffer->PushLayout(0, GL_FLOAT, 3, 8 * sizeof(float));
        vertexBuffer->PushLayout(1, GL_FLOAT, 2, 8 * sizeof(float), 3 * sizeof(float));
        vertexBuffer->PushLayout(2, GL_FLOAT, 3, 8 * sizeof(float), 5 * sizeof(float));

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
        std::array<float, 24> vertices =
        {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
             0.0f,  0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f
        };

        // Setup the vbo and vao
        VertexBufferPtr vertexBuffer = AssetSystem::CreateVertexBuffer(vertices.data(), sizeof(vertices), GL_STATIC_DRAW);
        vertexBuffer->PushLayout(0, GL_FLOAT, 3, 8 * sizeof(float));
        vertexBuffer->PushLayout(1, GL_FLOAT, 2, 8 * sizeof(float), 3 * sizeof(float));
        vertexBuffer->PushLayout(2, GL_FLOAT, 3, 8 * sizeof(float), 5 * sizeof(float));

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
        std::vector<float> vertices({ 0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f });
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

            // These are the normal vector components
            vertices.emplace_back(0.0f);
            vertices.emplace_back(0.0f);
            vertices.emplace_back(1.0f);
        }

        // Setup the vbo and vao
        VertexBufferPtr vertexBuffer = AssetSystem::CreateVertexBuffer(vertices.data(), vertices.size() * sizeof(float), GL_STATIC_DRAW);
        vertexBuffer->PushLayout(0, GL_FLOAT, 3, 8 * sizeof(float));
        vertexBuffer->PushLayout(1, GL_FLOAT, 2, 8 * sizeof(float), 3 * sizeof(float));
        vertexBuffer->PushLayout(2, GL_FLOAT, 3, 8 * sizeof(float), 5 * sizeof(float));

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

Cube::Cube()
{
    this->InitGeometryData();
}

Cube::Cube(const Transform& transform, const Material& material) :
    Geometry(transform, material)
{
    this->InitGeometryData();
}

void Cube::InitGeometryData()
{
    // Attempt to fetch the geometry's buffer objects from the asset system
    if (AssetSystem::GeometryData* geometryData = AssetSystem::GetInstance().GetGeometryBuffers("Cube"))
    {
        // It has been found so simply attach them to the VAO
        m_vertexArray.AttachBuffers(*geometryData->m_vertexBuffer, geometryData->m_indexBuffer.get());
    }
    else
    {
        // Define the vertex and index data
        std::array<float, 192> vertices =
        {
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f,  0.0f,  1.0f, // Front Bottom Left 0
             0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  0.0f,  0.0f,  1.0f, // Front Bottom Right 1
             0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f, // Front Top Right 2
            -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0f, // Front Top Left 3

            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f, // Back Top Left 4
             0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f, // Back Top Right 5
             0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f, // Back Bottom Right 6
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f, // Back Bottom Left 7

             0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f, // Right Bottom Left 8
             0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  1.0f,  0.0f,  0.0f, // Right Bottom Right 9
             0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f, // Right Top Right 10
             0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f, // Right Top Left 11

            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f, // Left Bottom Left 12
            -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f, // Left Top Left 13
            -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f, // Left Top Right 14
            -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f, // Left Bottom Right 15

            -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  0.0f,  1.0f,  0.0f, // Top Bottom Left 16
             0.5f,  0.5f,  0.5f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f, // Top Bottom Right 17
             0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f, // Top Top Right 18
            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f, // Top Top Left 19

            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f, // Bottom Bottom Left 20
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f, // Bottom Top Left 21
             0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  0.0f, -1.0f,  0.0f, // Bottom Top Right 22
             0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  0.0f, -1.0f,  0.0f  // Bottom Bottom Right 23
        };

        std::array<uint32_t, 36> indices =
        {
            0, 1, 2, 0, 2, 3,       // Front Face
            7, 4, 5, 7, 5, 6,       // Back Face
            8, 9, 10, 8, 10, 11,    // Right Face
            12, 13, 14, 12, 14, 15, // Left Face
            16, 17, 18, 16, 18, 19, // Top Face
            20, 21, 22, 20, 22, 23  // Bottom Face
        };

        // Setup the vbo, ibo and vao
        VertexBufferPtr vertexBuffer = AssetSystem::CreateVertexBuffer(vertices.data(), sizeof(vertices), GL_STATIC_DRAW);
        vertexBuffer->PushLayout(0, GL_FLOAT, 3, 8 * sizeof(float));
        vertexBuffer->PushLayout(1, GL_FLOAT, 2, 8 * sizeof(float), 3 * sizeof(float));
        vertexBuffer->PushLayout(2, GL_FLOAT, 3, 8 * sizeof(float), 5 * sizeof(float));

        IndexBufferPtr indexBuffer = AssetSystem::CreateIndexBuffer(indices.data(), sizeof(indices), GL_STATIC_DRAW);
        m_vertexArray.AttachBuffers(*vertexBuffer, indexBuffer.get());

        // Store the buffer objects in the asset system
        AssetSystem::GetInstance().StoreGeometryBuffers("Cube", vertexBuffer, indexBuffer);
    }

    // Assign the rendering parameters
    m_renderFunc = RenderFunction::RENDER_ELEMENTS;
    m_primitiveType = PrimitiveType::TRIANGLES;
    m_count = 36;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////