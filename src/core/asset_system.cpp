#include <core/asset_system.h>
#include <util/logging_system.h>
#include <util/formatted_exception.h>

#include <glad/glad.h>
#include <stb_image.h>

void AssetSystem::LoadShader(std::string_view nameID, std::string_view vshFilePath, std::string_view fshFilePath)
{
    if (m_storedShaders.find(nameID.data()) == m_storedShaders.end()) // Make sure the ID given isn't already taken
        m_storedShaders[nameID.data()] = std::make_shared<ShaderProgram>(vshFilePath, fshFilePath);
    else
    {
        LoggingSystem::GetInstance().Output("Skipped shader load operation, the ID \"%s\" has already been used.",
            LoggingSystem::Severity::WARNING, nameID.data());
    }
}

void AssetSystem::LoadTexture(std::string_view nameID, std::string_view imageFilePath, bool flipOnLoad, bool srgb)
{
    if (m_storedTextures.find(nameID.data()) == m_storedTextures.end()) // Check to make sure ID isn't already taken
    {
        // Load the pixel data from the image file
        int width = 0, height = 0, channels = 0;
        stbi_set_flip_vertically_on_load(flipOnLoad);
        const uint8_t* pixelData = stbi_load(imageFilePath.data(), &width, &height, &channels, 0);

        if (pixelData) // Check that the image was loaded successfully
        {
            // Figure out how to store the pixel data (the internal format) and how the pixel data given is formatted (the format)
            uint32_t internalFormat = 0, format = 0;
            switch (channels)
            {
            case 4:
                internalFormat = srgb ? GL_SRGB_ALPHA : GL_RGBA;
                format = GL_RGBA;
                break;
            case 3:
                internalFormat = srgb ? GL_SRGB : GL_RGB;
                format = GL_RGB;
                break;
            default:
                throw FormattedException("The channel format for the texture image at path: %s, is not supported.", 
                    imageFilePath.data());
                break;
            }

            // Setup and store the texture buffer
            m_storedTextures[nameID.data()] = std::make_shared<Texture2D>(pixelData, glm::ivec2(width, height), GL_UNSIGNED_BYTE, 
                internalFormat, format);
        }
        else
            throw FormattedException("Failed to load the texture image at path: %s.", imageFilePath.data());
    }
    else
    {
        LoggingSystem::GetInstance().Output("Skipped texture image load operation, the ID \"%s\" has already been used.",
            LoggingSystem::Severity::WARNING, nameID.data());
    }
}

void AssetSystem::StoreGeometryBuffers(std::string_view nameID, VertexBufferPtr vertexBuffer, IndexBufferPtr indexBuffer)
{
    if (!vertexBuffer)
        throw FormattedException("No vertex buffer was given for geometry assigned with ID \"%s\".", nameID.data());

    if (m_storedGeometryBuffers.find(nameID.data()) == m_storedGeometryBuffers.end()) // Make sure the ID given isn't already taken
        m_storedGeometryBuffers[nameID.data()] = { vertexBuffer, indexBuffer };
    else
    {
        LoggingSystem::GetInstance().Output("Skipped geometry buffer objects storage operation, the ID \"%s\" has already been used.",
            LoggingSystem::Severity::WARNING, nameID.data());
    }
}

void AssetSystem::RemoveShader(std::string_view nameID)
{
    m_storedShaders.erase(nameID.data());
}

void AssetSystem::RemoveTexture(std::string_view nameID)
{
    m_storedTextures.erase(nameID.data());
}

void AssetSystem::RemoveGeometryBuffers(std::string_view nameID)
{
    m_storedGeometryBuffers.erase(nameID.data());
}

ShaderProgramPtr AssetSystem::GetShader(std::string_view nameID) const
{
    auto shaderIterator = m_storedShaders.find(nameID.data());
    if (shaderIterator == m_storedShaders.end())
    {
        LoggingSystem::GetInstance().Output("No shader exists with the assigned ID \"%s\".", LoggingSystem::Severity::WARNING, 
            nameID.data());

        return nullptr;
    }

    return shaderIterator->second;
}

Texture2DPtr AssetSystem::GetTexture(std::string_view nameID) const
{
    auto textureIterator = m_storedTextures.find(nameID.data());
    if (textureIterator == m_storedTextures.end())
    {
        LoggingSystem::GetInstance().Output("No texture image exists with the assigned ID \"%s\".", LoggingSystem::Severity::WARNING, 
            nameID.data());

        return nullptr;
    }

    return textureIterator->second;
}

AssetSystem::GeometryData* AssetSystem::GetGeometryBuffers(std::string_view nameID)
{
    auto geometryDataIterator = m_storedGeometryBuffers.find(nameID.data());
    if (geometryDataIterator == m_storedGeometryBuffers.end())
        return nullptr;

    return &geometryDataIterator->second;
}

VertexBufferPtr AssetSystem::CreateVertexBuffer(const void* data, size_t size, uint32_t usage)
{
    return std::make_shared<VertexBuffer>(data, size, usage);
}

IndexBufferPtr AssetSystem::CreateIndexBuffer(const void* data, size_t size, uint32_t usage)
{
    return std::make_shared<IndexBuffer>(data, size, usage);
}

AssetSystem& AssetSystem::GetInstance()
{
    static AssetSystem instance;
    return instance;
}
