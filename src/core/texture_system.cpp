#include <core/texture_system.h>
#include <util/logging_system.h>
#include <util/formatted_exception.h>

#include <glad/glad.h>
#include <stb_image.h>

void TextureSystem::Load(std::string_view id, std::string_view filePath, bool flipOnLoad, bool srgb)
{
    if (m_storedTextures.find(id.data()) == m_storedTextures.end()) // Check to make sure ID isn't already taken
    {
        // Load the pixel data from the image file
        int width = 0, height = 0, channels = 0;
        stbi_set_flip_vertically_on_load(flipOnLoad);
        const uint8_t* pixelData = stbi_load(filePath.data(), &width, &height, &channels, 0);
        
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
                throw FormattedException("The channel format for the texture image at path: %s, is not supported.", filePath.data());
                break;
            }

            // Setup and store the texture buffer
            m_storedTextures[id.data()] = std::make_shared<Texture2D>(pixelData, glm::ivec2(width, height), GL_UNSIGNED_BYTE, internalFormat, 
                format);
        }
        else
            throw FormattedException("Failed to load the texture image at path: %s.", filePath.data());
    }
    else
    {
        LoggingSystem::GetInstance().Output("Skipped texture image load operation, the ID \"%s\" has already been used.",
            LoggingSystem::Severity::WARNING, id.data());
    }
}

void TextureSystem::Remove(std::string_view id)
{
    m_storedTextures.erase(id.data());
}

Texture2DPtr TextureSystem::GetTexture(std::string_view id)
{
    auto textureIterator = m_storedTextures.find(id.data());
    if (textureIterator == m_storedTextures.end())
    {
        LoggingSystem::GetInstance().Output("No texture image exists with the ID \"%s\".", LoggingSystem::Severity::WARNING, id.data());
        return nullptr;
    }

    return textureIterator->second;
}

TextureSystem& TextureSystem::GetInstance()
{
    static TextureSystem instance;
    return instance;
}
